#include "MCP3426.h"
#include "arduino.h"
MCP3426Sensor::MCP3426Sensor(TwoWire &wire, uint8_t address) : _wire(wire), _address(address) {}

int MCP3426Sensor::readTemperature() {
    uint8_t config = 0x10; // Example configuration: 12-bit resolution, single-ended input
    uint8_t msb, lsb;
    
    // Send configuration command to MCP3426
    _wire.beginTransmission(_address);
    _wire.write(config);  // Send the config byte
    _wire.endTransmission();
    
    // Wait for conversion to complete (e.g., 6ms for 12-bit resolution)
    delay(10); 
    
    // Request 2 bytes of data from the MCP3426
    _wire.requestFrom(_address, 2);
    
    if (_wire.available() == 2) {
        msb = _wire.read(); // Most significant byte
        lsb = _wire.read(); // Least significant byte
        
        // Combine the two bytes into a 16-bit result
        return (msb << 8) | lsb;
    } else {
        return -1; // Error reading data
    }
}
