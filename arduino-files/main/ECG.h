#ifndef ECG_H
#define ECG_H

#include <Wire.h>
#include "Arduino.h"
class ECGSensor {
public:
    ECGSensor(TwoWire &wire, uint8_t address);
    void readData(uint8_t *buffer);
    
private:
    TwoWire &_wire;
    uint8_t _address;
};

#endif
