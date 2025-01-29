#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"
#include "MQTTClient.h"

#define RENDER_DEBUG

#define MQTT_SERVER "192.168.137.133"
#define MQTT_PORT 1883
#define MQTT_CLIENT_ID "HeartRateMonitorClient"
#define MQTT_TOPIC "BILTS_broker"

#include "RenderingEngine.h"
#include "MonitorWidget.h"

#include "sp02.h"

#include <chrono>
#include <thread>
#include <mutex>
#include <queue>
#include <iostream>
#include <sstream>
#include <string>

std::queue<std::string> messageQueue;
std::mutex queueMutex;
bool running = true;

MQTTClient client;
MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;

enum State {
	NSR,         // Normal Sinus Rhythm
	Tachycardia, // Fast heart rate
	Bradycardia  // Slow heart rate
};

int BPM = 145;   // Beats Per Minute
int BrPM = 45;  // Breaths per Minute
int CT = 36;    // Core Temperature
int ST = 38;    // Surface Temperature
int NIBP = 65;  // Non-invasive Blood Pressure
int SpO2 = 100;  // Blood Oxygenation

// Array of states
State currentState = NSR;

uint64_t timeSinceEpochMillisec() {
  using namespace std::chrono;
  return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

void processMessage(const std::string& message) {
	std::istringstream iss(message);
	std::string key;
	std::string valueStr;
	int value = 0;

	// Split message into key and value
	size_t delimiterPos = message.find(": ");
	if (delimiterPos != std::string::npos) {
		key = message.substr(0, delimiterPos);
		valueStr = message.substr(delimiterPos + 2);
		value = std::stoi(valueStr); // Convert value string to integer
	} else {
		key = message;
	}

	if (key == "heartrate") {
		BPM = value;
		currentState = NSR;
	}
	else if (key.find("heartrate/") == 0) {
		std::string stateStr = key.substr(10); // Extract state from "heartrate/STATE"
		BPM = value;

		if (stateStr == "NSR") {
			currentState = NSR;
		} else if (stateStr == "tachycardia") {
			currentState = Tachycardia;
		} else if (stateStr == "bradycardia") {
			currentState = Bradycardia;
		}
	}
	else if (key == "NIBP") {
		NIBP = value;
	}
	else if (key == "SpO2") {
		SpO2 = value;
	}
	else if (key == "breathing") {
		BrPM = value;
	}
	else if (key == "temperature/core") {
		CT = value;
	}
	else if (key == "temperature/surface") {
		ST = value;
	}

	// Print for debugging
	std::cout << "Updated values -> BPM: " << BPM
			  << ", NIBP: " << NIBP
			  << ", SpO2: " << SpO2
			  << ", BrPM: " << BrPM
			  << ", CT: " << CT
			  << ", ST: " << ST
			  << ", State: " << currentState << std::endl;
}

void mqttPollingThread() {
	while (running) {
		MQTTClient_message* message = NULL;
		char* topic = NULL;
		int topicLen = 0;

		int rc = MQTTClient_receive(client, &topic, &topicLen, &message, 100); // Wait 100ms (10Hz polling)
		if (rc == MQTTCLIENT_SUCCESS && message != NULL) {
			std::string payload(static_cast<char*>(message->payload), message->payloadlen);

			std::lock_guard<std::mutex> lock(queueMutex);
			messageQueue.push(payload);

			MQTTClient_freeMessage(&message);
			MQTTClient_free(topic);
		}
	}
}

// Main code
int main(int, char**)
{
	RenderingEngine::setup();

	int rc = MQTTClient_create(&client, MQTT_SERVER, MQTT_CLIENT_ID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
	if (rc != MQTTCLIENT_SUCCESS) {
		std::cerr << "Failed to create MQTT client. Error code: " << rc << std::endl;
		return -1;
	}

	conn_opts.keepAliveInterval = 20;
	conn_opts.cleansession = 1;


	// Connect to the MQTT broker
	rc = MQTTClient_connect(client, &conn_opts);
	if (rc != MQTTCLIENT_SUCCESS) {
		std::cerr << "Failed to connect to MQTT broker. Error code: " << rc << std::endl;
		return -1;
	}
	std::cout << "Connected to MQTT broker at " << MQTT_SERVER << ":" << MQTT_PORT << std::endl;

	// Subscribe to the 'BILTS' topic
	rc = MQTTClient_subscribe(client, MQTT_TOPIC, 0);
	if (rc != MQTTCLIENT_SUCCESS) {
		std::cerr << "Failed to subscribe to topic. Error code: " << rc << std::endl;
		return -1;
	}
	std::cout << "Subscribed to topic: " << MQTT_TOPIC << std::endl;

	std::thread mqttThread(mqttPollingThread);

	ImGuiIO* io = RenderingEngine::getIO();

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4& clear_color = RenderingEngine::getBackgroundColor();

	uint64_t previousTime = timeSinceEpochMillisec();

	// Setup MonitorWidgets
	MonitorWidget w1 = MonitorWidget(
		ImVec4(100, 100, 2000, 200), // Graph position
		ImVec4(2100, 100, 200, 200), // Info position
		ImVec4(0.0f, 1.0f, 0.0f, 0.0f), // Graph color
		"Pulse", // Name
		100.0, // Max
		0.0, // Min
		2, // Line width
		20, // Deadzone size
		3000.0 // Time in ms
	);
	MonitorWidget w2 = MonitorWidget(
		ImVec4(100, 300, 2000, 200), // Graph position
		ImVec4(2100, 300, 200, 200), // Info position
		ImVec4(0.0f, 1.0f, 1.0f, 0.0f), // Graph color
		"SVP",
		100.0,
		0.0,
		2,
		20,
		5000.0
	);
	MonitorWidget w3 = MonitorWidget(
		ImVec4(100, 500, 2000, 200), // Graph position
		ImVec4(2100, 500, 200, 200), // Info position
		ImVec4(0.0f, 0.0f, 1.0f, 0.0f), // Graph color
		"Oxygen",
		100.0,
		-100.0,
		2,
		20,
		10000.0
	);

	setSpO2Value(SpO2);
	setPrbpmValue(BPM);
	updateWaveform();
	for(int i = 0; i < 2000; i++) {
		w3.setValue(i, calculateWaveform(i, 1000) / 200 + 0.5);
	}

    while (RenderingEngine::isRunning())
    {
		// Deltatime calculation
		uint64_t currentTime = timeSinceEpochMillisec();
		uint64_t dt = currentTime - previousTime;
		previousTime = currentTime;

		RenderingEngine::preRender();

    	std::string latestMessage;
	    {
			std::lock_guard<std::mutex> lock(queueMutex);
			if (!messageQueue.empty()) {
				latestMessage = messageQueue.front();
				messageQueue.pop();
				std::cout << "Processed message: " << latestMessage << std::endl;

				processMessage(latestMessage); // Call function to store values

				setSpO2Value(SpO2);
				setPrbpmValue(BPM);
				updateWaveform();
				for(int i = 0; i < 2000; i++) {
					w3.setValue(i, calculateWaveform(i, 1000) / 200 + 0.5);
				}


			}
	    }

		w1.render(currentTime, dt);
		w2.render(currentTime, dt);
		w3.render(currentTime, dt);

		// if(w1.needsUpdate()) {
		// 	updateWaveform();
		// 	for(int i = 0; i < 2000; i++) {
		// 		w3.setValue(i, calculateWaveform(i, 1000) / 200 + 0.5);
		// 	}
		// }
		// if(w2.needsUpdate()) {
  //
		// }
		// if(w3.needsUpdate()) {
		// }

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io->Framerate, io->Framerate);
            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }

        RenderingEngine::render();
    }
	// Disconnect and clean up MQTT client
	MQTTClient_disconnect(client, 10000);
	MQTTClient_destroy(&client);
	std::cout << "MQTT client disconnected and destroyed." << std::endl;

	running = false;
	mqttThread.join(); // Wait for thread to finish

    RenderingEngine::stop();

    return 0;
}
