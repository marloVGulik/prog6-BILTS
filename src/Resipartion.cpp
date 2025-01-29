//
// Created by Samuel_v_G on 29/01/2025.
//

#include "Resipartion.h"

#include <cmath>
#include <iostream>

Respiration::Respiration()
    : amplitude(1.0), frequency(1.0), time(0.0) {}

void Respiration::updateWaveform() {
    x.clear();
    y.clear();

    // Generate data points for the sine wave (respiration waveform)
    for (int i = 0; i < 1001; ++i) {
        x.push_back(time + i * 0.01); // Time progression
        y.push_back(amplitude * std::sin(2 * M_PI * frequency * x[i])); // Sinusoidal waveform
    }
    time += 0.01; // Increment time
}

void Respiration::changeAmplitude(int value) {
    amplitude = value / 100.0; // Scale slider value from 0-1000 to 0-10
}

void Respiration::changeFrequency(int value) {
    frequency = value / 100.0; // Scale slider value from 0-1000 to 0-10
}

void Respiration::printWaveform() const {
    // Console output for visualization (simplified)
    for (size_t i = 0; i < x.size(); ++i) {
        int yPosition = static_cast<int>((y[i] * 20) + 20); // Scale y to fit console output
        for (int j = 0; j < yPosition; ++j) {
            std::cout << " "; // Move to the appropriate y-position
        }
        std::cout << "*" << std::endl; // Draw the point
    }
}
