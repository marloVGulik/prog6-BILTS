#include "ECGsimulator.h"

// Constructor to initialize ECG parameters and heart rate
ECGsimulator::ECGsimulator(int heartrate)
    : heartrate(heartrate)
{
    // Initialize amplitude and time constants
    A_P = 0.3; t_P = 0.12;
    A_Q = -0.2; t_Q = 0.5;
    A_R = 1.0; t_R = 0.55;
    A_S = -0.4; t_S = 0.6;
    A_T = 0.3; t_T = 0.9;

    // Initialize the standard deviation values (sigma) for Gaussian distributions
    sigma_P = 0.02; sigma_Q = 0.02; sigma_R = 0.02; sigma_S = 0.02; sigma_T = 0.02;
}

// Function to generate ECG value at a specific time (in seconds)
double ECGsimulator::GenerateWave(double time)

{
    // Convert time to the phase in the current ECG cycle based on the heart rate
    double period = 60.0 / heartrate;  // Calculate period of the heart rate
    double cycle_time = fmod(time, period); // Get time within the current heart cycle

    // Compute the ECG value as a sum of P, QRS, and T waves
    double ecg_value = P_wave(cycle_time) + QRS_wave(cycle_time) + T_wave(cycle_time);
    return ecg_value;
}

// Private method to calculate P wave at time t
double ECGsimulator::P_wave(double t)
{
    return A_P * std::exp(-std::pow((t - t_P) / sigma_P, 2) / 2);
}

// Private method to calculate the QRS complex (sum of Q, R, and S waves) at time t
double ECGsimulator::QRS_wave(double t)
{
    double Q = A_Q * std::exp(-std::pow((t - t_Q) / sigma_Q, 2) / 2);
    double R = A_R * std::exp(-std::pow((t - t_R) / sigma_R, 2) / 2);
    double S = A_S * std::exp(-std::pow((t - t_S) / sigma_S, 2) / 2);
    return Q + R + S;
}

// Private method to calculate T wave at time t
double ECGsimulator::T_wave(double t)
{
    return A_T * std::exp(-std::pow((t - t_T) / sigma_T, 2) / 2);
}
