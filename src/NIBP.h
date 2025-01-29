//
// Created by Samuel_v_G on 29/01/2025.
//

#ifndef NIBP_H
#define NIBP_H


class BloodPressureSimulation
{
private:
    int systolicPressureBase;
    int diastolicPressureBase;
    int currentSystolicPressure;
    int currentDiastolicPressure;

public:
    // Constructor to initialize with default or user-defined values
    BloodPressureSimulation(int systolic = 120, int diastolic = 80);

    // Method to start the simulation (request user input and generate readings)
    void startSimulation();

    // Method to generate random readings based on the base values
    void generateReading();
};

#endif //NIBP_H
