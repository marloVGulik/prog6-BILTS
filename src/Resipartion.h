//
// Created by Samuel_v_G on 29/01/2025.
//

#ifndef RESIPARTION_H
#define RESIPARTION_H

#include <vector>

class Respiration {
public:
    Respiration();

    // Update waveform data points
    void updateWaveform();

    // Change amplitude
    void changeAmplitude(int value);

    // Change frequency
    void changeFrequency(int value);

    // Print waveform (to replace painting with console output)
    void printWaveform() const;

private:
    double amplitude;
    double frequency;
    double time;
    std::vector<double> x;
    std::vector<double> y;
};

#endif //RESIPARTION_H
