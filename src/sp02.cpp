#include <cmath>   // for sin()

double spO2Value = 98.0;  // Default SpO2 level
int prbpmValue = 60;      // Default PRbpm
double waveformPhase = 0.0;

void setSpO2Value(double value1) {
    if (value1 < 70.0) value1 = 70.0;
    if (value1 > 100.0) value1 = 100.0;
    spO2Value = value1;
}

void setPrbpmValue(int value2) {
    if (value2 < 30) value2 = 30;
    if (value2 > 180) value2 = 180;
    prbpmValue = value2;
}

void updateWaveform() {
    double offsetFactor = prbpmValue / 60.0;
    waveformPhase += offsetFactor * 0.03;
    if (waveformPhase > 1.0) {
        waveformPhase -= 1.0;
    }
}

double calculateWaveform(double x, double width) {
    double t = (waveformPhase + x / (width * 0.5));
    if (t > 1.0) t -= 1.0;

    double amplitudeScale = (spO2Value - 70.0) / 30.0;
    int amplitude = 30 + (amplitudeScale * 70);

    double y = amplitude * (
        0.8 * std::sin(2 * M_PI * t) +
        0.3 * std::sin(4 * M_PI * t) +
        0.1 * std::sin(8 * M_PI * t)
    );

    y += amplitude * 0.1 * std::sin(2 * M_PI * t * 0.1);

    if (spO2Value < 90.0) {
        double noiseAmount = (90.0 - spO2Value) / 50.0;
        y += amplitude * noiseAmount * ((rand() % 100) / 100.0 - 0.5);
    }

    return y;
}

