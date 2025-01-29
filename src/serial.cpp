#include "serial.h"
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

Serial::Serial(const std::string &portName, int baudRate) {
    fd = open(portName.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1) {
        std::cerr << "Error: Unable to open port " << portName << std::endl;
        return;
    }

    struct termios options;
    tcgetattr(fd, &options);

    cfsetispeed(&options, baudRate);
    cfsetospeed(&options, baudRate);

    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;

    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    options.c_iflag &= ~(IXON | IXOFF | IXANY);

    tcsetattr(fd, TCSANOW, &options);
}

Serial::~Serial() {
    if (fd != -1) {
        close(fd);
    }
}

bool Serial::isOpen() const {
    return fd != -1;
}

std::string Serial::serialPoll() {
    if (fd == -1) return "";

    std::string result;
    char c;
    while (read(fd, &c, 1) > 0) {
        if (c == '\n') break;
        result += c;
    }

    return padToFiveDigits(result);
}

std::string Serial::padToFiveDigits(const std::string &input) {
    std::string padded = input;
    while (padded.length() < 5) {
        padded = "0" + padded;
    }
    return padded;
}
