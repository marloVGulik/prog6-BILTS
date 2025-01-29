#ifndef ECGSIMULATOR_H
#define ECGSIMULATOR_H

#include <cmath> // For exp and pow

class ECGsimulator
{
public:
    // Constructor: Optionally accepts heart rate if needed
    ECGsimulator(int heartrate = 70);

    // Function to generate ECG value at a specific time
    double GenerateWave(double time);
    // Heart rate in beats per minute (bpm)
    int heartrate;

private:
    // Private helper functions to calculate individual waves
    double P_wave(double t);
    double QRS_wave(double t);
    double T_wave(double t);

    // Constants for the PQRST waveforms (amplitude and time)
    double A_P, t_P;
    double A_Q, t_Q;
    double A_R, t_R;
    double A_S, t_S;
    double A_T, t_T;
    double sigma_P, sigma_Q, sigma_R, sigma_S, sigma_T;
};

#endif // ECGSIMULATOR_H
