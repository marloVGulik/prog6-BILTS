#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"

#define RENDER_DEBUG

#include "RenderingEngine.h"
#include "MonitorWidget.h"

#include "sp02.h"

#include <chrono>
uint64_t timeSinceEpochMillisec() {
  using namespace std::chrono;
  return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

// class TestTask : public BGTask {
// public:
// 	TestTask() { std::cout << "Test task" << std::endl; }
//
// private:
// }

// Main code
int main(int, char**)
{
	RenderingEngine::setup();

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

    while (RenderingEngine::isRunning())
    {
		// Deltatime calculation
		uint64_t currentTime = timeSinceEpochMillisec();
		uint64_t dt = currentTime - previousTime;
		previousTime = currentTime;

		RenderingEngine::preRender();

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
		// 	updateWaveform();
		// 	for(int i = 0; i < 2000; i++) {
		// 		w3.setValue(i, calculateWaveform(i, 1000) / 200 + 0.5);
		// 	}
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

    RenderingEngine::stop();


    return 0;
}
