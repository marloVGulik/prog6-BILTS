//
// Created by Samuel_v_G on 29/01/2025.
//

#ifndef SP02_H
#define SP02_H

#include <cmath>   // for sin()
#include <cstdlib> // for rand()

// Default SpO2 and Prbpm values
extern double spO2Value;   // SpO2 level (default: 98.0)
extern int prbpmValue;     // Pulse rate in beats per minute (default: 60)
extern double waveformPhase;  // Phase for waveform calculation

// Function declarations

void setSpO2Value(double value1);

void setPrbpmValue(int value2);

void updateWaveform();

double calculateWaveform(double x, double width);

#endif //SP02_H
