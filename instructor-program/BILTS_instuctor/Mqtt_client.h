// Mqtt_client.h
#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#include <string>
#include <windows.h>
#include <iostream>

class MqttClient {
public:
    // Constructor
    MqttClient(const std::string& brokerAddress, const std::string& topic);

    // Destructor
    ~MqttClient();

    // Method to publish a message
    bool publishMessage(const std::string& message);

private:
    std::string brokerAddress;
    std::string topic;

    // Helper method to convert std::string to wchar_t*
    wchar_t* convertToWideString(const std::string& str);
};

#endif // MQTT_CLIENT_H
