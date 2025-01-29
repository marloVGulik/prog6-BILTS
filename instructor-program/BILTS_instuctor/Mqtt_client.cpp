#include "Mqtt_client.h"

MqttClient::MqttClient(const std::string& brokerAddress, const std::string& topic)
    : brokerAddress(brokerAddress), topic(topic) {}

MqttClient::~MqttClient() {}

bool MqttClient::publishMessage(const std::string& message) {
    // Construct the full command to send via MQTT
    std::string command = "mosquitto_pub -h " + brokerAddress + " -t " + topic + " -m \"" + message + "\"";

    // Convert the command to a wide string
    wchar_t* wideCommand = convertToWideString(command);

    // Set up the startup information
    STARTUPINFO si = { sizeof(STARTUPINFO) };
    si.dwFlags = STARTF_USESHOWWINDOW;  // Prevent the terminal window from appearing
    si.wShowWindow = SW_HIDE;  // Hide the window

    PROCESS_INFORMATION pi;

    // Create the process
    if (!CreateProcess(NULL, wideCommand, NULL, NULL, FALSE, DETACHED_PROCESS, NULL, NULL, &si, &pi)) {
        // Handle the error if process creation failed
        DWORD dwError = GetLastError();
        std::cerr << "CreateProcess failed with error code: " << dwError << std::endl;
        delete[] wideCommand;
        return false;
    }

    // Clean up and close process and thread handles
    delete[] wideCommand;
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return true;
}

wchar_t* MqttClient::convertToWideString(const std::string& str) {
    int sizeNeeded = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
    wchar_t* wideStr = new wchar_t[sizeNeeded];
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wideStr, sizeNeeded);
    return wideStr;
}
