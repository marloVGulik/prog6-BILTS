#include "NIBP.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

BloodPressureSimulation::BloodPressureSimulation(int systolic, int diastolic)
    : systolicPressureBase(systolic),
      diastolicPressureBase(diastolic),
      currentSystolicPressure(systolic),
      currentDiastolicPressure(diastolic)
{
    // Initialize random number generator
    std::srand(std::time(0));
}

void BloodPressureSimulation::startSimulation()
{
    // Fetch base values from user input
    std::cout << "Enter Systolic Pressure (default 120): ";
    std::cin >> systolicPressureBase;
    std::cout << "Enter Diastolic Pressure (default 80): ";
    std::cin >> diastolicPressureBase;

    currentSystolicPressure = systolicPressureBase;
    currentDiastolicPressure = diastolicPressureBase;

    generateReading();  // Generate random pressure variations

    // Output the generated readings
    std::cout << "Systolic Pressure: " << currentSystolicPressure << " mmHg\n";
    std::cout << "Diastolic Pressure: " << currentDiastolicPressure << " mmHg\n";
}

void BloodPressureSimulation::generateReading()
{
    // Generate random variation for systolic pressure within ±3
    int systolicVariation = std::rand() % 7 - 3; // -3 to +3
    currentSystolicPressure = systolicPressureBase + systolicVariation;

    // Generate random variation for diastolic pressure within ±3
    int diastolicVariation = std::rand() % 7 - 3; // -3 to +3
    currentDiastolicPressure = diastolicPressureBase + diastolicVariation;
}
