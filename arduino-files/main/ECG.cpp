#include "ECG.h"

ECGSensor::ECGSensor(TwoWire &wire, uint8_t address) : _wire(wire), _address(address) {}


void ECGSensor::readData(uint8_t *buffer) {
    this->_wire.requestFrom(_address, (uint8_t)6);
    
    for (int i = 0; i < 6; i++) {
        if (_wire.available()) {
            buffer[i] = _wire.read();
        }
    }
}
