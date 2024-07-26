// MonteCarloSimulation.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <random>
#include <omp.h>
#include <chrono>

// Function to perform Monte Carlo simulation in serial
double monteCarloPiEstimateSerial(int numPoints) {
    int insideCircle = 0;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    for (int i = 0; i < numPoints; ++i) {
        double x = dis(gen);
        double y = dis(gen);
        if (x * x + y * y <= 1) {
            insideCircle++;
        }
    }
    return 4.0 * insideCircle / static_cast<double>(numPoints);
}

// Function to perform Monte Carlo simulation in parallel using OpenMP
double monteCarloPiEstimateParallel(int numPoints, int numThreads) {
    int insideCircle = 0;

#pragma omp parallel num_threads(numThreads) reduction(+:insideCircle)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0.0, 1.0);

#pragma omp for
        for (int i = 0; i < numPoints; ++i) {
            double x = dis(gen);
            double y = dis(gen);
            if (x * x + y * y <= 1) {
                insideCircle++;
            }
        }
    }
    return 4.0 * insideCircle / static_cast<double>(numPoints);
}

int main() {
    while (true) {
        int numPoints, numThreads;

        std::cout << "Enter the number of points for the simulation (or enter 0 to exit): ";
        std::cin >> numPoints;
        if (numPoints == 0) {
            std::cout << "Exiting program." << std::endl;
            break;
        }

        std::cout << "Enter the number of threads for parallel simulation: ";
        std::cin >> numThreads;

        auto startSerial = std::chrono::high_resolution_clock::now();
        double piSerial = monteCarloPiEstimateSerial(numPoints);
        auto endSerial = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsedSerial = endSerial - startSerial;

        auto startParallel = std::chrono::high_resolution_clock::now();
        double piParallel = monteCarloPiEstimateParallel(numPoints, numThreads);
        auto endParallel = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsedParallel = endParallel - startParallel;

        std::cout << "Serial Pi estimation: " << piSerial << std::endl;
        std::cout << "Serial computation took: " << elapsedSerial.count() << " seconds." << std::endl;
        std::cout << "Parallel Pi estimation: " << piParallel << std::endl;
        std::cout << "Parallel computation took: " << elapsedParallel.count() << " seconds." << std::endl;
    }

    return 0;
}
