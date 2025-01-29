#ifndef MCP3426_H
#define MCP3426_H

#include <Wire.h>

class MCP3426Sensor {
public:
    MCP3426Sensor(TwoWire &wire, uint8_t address);
    int readTemperature();
    
private:
    TwoWire &_wire;
    uint8_t _address;
    
    unsigned long _conversionStartTime;  // Store the start time of conversion
    bool _isConversionComplete;          // Flag to track conversion status
};

#endif
