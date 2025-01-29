#ifndef SERIAL_H
#define SERIAL_H

#include <string>

class Serial {
public:
    Serial(const std::string &portName, int baudRate = 9600);
    ~Serial();
    
    bool isOpen() const;
    std::string serialPoll();  // Reads a line and pads to 5 digits

private:
    int fd;  // File descriptor for serial port
    std::string padToFiveDigits(const std::string &input);
};

#endif // SERIAL_H