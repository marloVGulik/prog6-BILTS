#include <Wire.h>
#include "wiring_private.h" // Required for SERCOM configuration
#include "ECG.h"
#include "MCP3426.h"

// Define custom pins for SERCOM3 and SERCOM4
#define CUSTOM_WIRE_SDA_SERCOM3 (11) // SERCOM3.0
#define CUSTOM_WIRE_SCL_SERCOM3 (13) // SERCOM3.1
#define CUSTOM_WIRE_SDA_SERCOM4 (28) // SERCOM4.0
#define CUSTOM_WIRE_SCL_SERCOM4 (39) // SERCOM4.1
// #define CUSTOM_WIRE_SDA_SERCOM5 (26) // SERCOM5.0
// #define CUSTOM_WIRE_SCL_SERCOM5 (27) // SERCOM5.1

#define th_w 100
#define th_r 500
#define th_z 890

#define th_temp 500

// Declare SERCOM instances for I2C
SERCOM mySERCOM3(SERCOM3);
SERCOM mySERCOM4(SERCOM4);
// SERCOM mySERCOM5(SERCOM5);

// Create TwoWire instances for custom SERCOMs
TwoWire myWire3(&mySERCOM3, CUSTOM_WIRE_SDA_SERCOM3, CUSTOM_WIRE_SCL_SERCOM3);
TwoWire myWire4(&mySERCOM4, CUSTOM_WIRE_SDA_SERCOM4, CUSTOM_WIRE_SCL_SERCOM4);
// TwoWire myWire5(&mySERCOM5, CUSTOM_WIRE_SDA_SERCOM5, CUSTOM_WIRE_SCL_SERCOM5);

ECGSensor ecgSensor(myWire3, 0x2A);  // ECG sensor on SERCOM3
MCP3426Sensor tempSensor1(myWire3, 0x68);  // Temperature sensor 1 on SERCOM3
MCP3426Sensor tempSensor2(myWire4, 0x68);  // Temperature sensor 2 on SERCOM4

void setup() {
    Serial.begin(115200);
    while (!Serial);

    // Initialize I2C on both SERCOMs
    myWire3.begin();
    pinPeripheral(CUSTOM_WIRE_SDA_SERCOM3, PIO_SERCOM_ALT);
    pinPeripheral(CUSTOM_WIRE_SCL_SERCOM3, PIO_SERCOM_ALT);

    myWire4.begin();
    pinPeripheral(CUSTOM_WIRE_SDA_SERCOM4, PIO_SERCOM_ALT);
    pinPeripheral(CUSTOM_WIRE_SCL_SERCOM4, PIO_SERCOM_ALT);

    // myWire5.begin();
    // pinPeripheral(CUSTOM_WIRE_SDA_SERCOM5, PIO_SERCOM_ALT);
    // pinPeripheral(CUSTOM_WIRE_SCL_SERCOM5, PIO_SERCOM_ALT);

    // Serial.println("Reading ECG and temperature data...");
}
void readECGData(uint8_t address, uint8_t *buffer) {
    myWire3.requestFrom(address, (uint8_t)6);
    
    for (int i = 0; i < 6; i++) {
        if (myWire3.available()) {
            buffer[i] = myWire3.read();
        }
    }
}

void loop() {;
    uint8_t ecgData[6];
    ecgSensor.readData(ecgData); // Reading ECG data from the sensor

    uint16_t rood = (ecgData[0] << 8) | ecgData[1];
    uint16_t nigga = (ecgData[2] << 8) | ecgData[3];
    uint16_t wit = (ecgData[4] << 8) | ecgData[5];

    // Serial.print("rood: "); Serial.print(rood);
    // Serial.print(" | zwart: "); Serial.print(nigga);
    // Serial.print(" | wit: "); Serial.println(wit);

    bool drood = (rood < th_r);
    bool dnigga = (nigga > th_z);
    bool dwit = (wit < th_w);

    //Read temperature from both MCP3426 sensors
    int16_t temp1 = tempSensor1.readTemperature(); // Sensor 1 on SERCOM3
    int16_t temp2 = tempSensor2.readTemperature(); // Sensor 2 on SERCOM4

    // Serial.print("Temp Sensor 1: "); Serial.println(temp1);
    // Serial.print("Temp Sensor 2: "); Serial.println(temp2);

    bool dtemp1 = (temp1 > th_temp);
    bool dtemp2 = (temp2 > th_temp);
    // Serial.print(drood);Serial.print(dnigga);Serial.print(dwit);Serial.print(dtemp1);Serial.println(dtemp2);
    uint8_t bits = (drood << 4) | (dnigga << 3) | (dwit << 2) | (dtemp1 << 1) | dtemp2;
    send5BitData(bits);
    delay(500); // Adjust delay as needed
}
void send5BitData(uint8_t data) {
    // // Ensure the data is 5 bits
    // data &= 0x1F; // Mask the higher bits to keep only the lower 5 bits

    // // Send the data over I2C
    // myWire5.beginTransmission(0x69);
    // myWire5.write(data);  // Send the 5-bit data (packed into a byte)
    // myWire5.endTransmission();

    // // Serial.print("Sent 5-bit data: ");
    Serial.println(data, BIN);  // Print the 5-bit data in binary format
}
