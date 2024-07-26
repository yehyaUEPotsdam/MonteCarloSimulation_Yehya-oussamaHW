// MonteCarloSimulation.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <random>
#include <omp.h>
#include <chrono>

// Function to estimate Pi using Monte Carlo simulation
// This function takes the number of random points to generate as input
double monteCarloPiEstimate(int numPoints) {
    int insideCircle = 0; // Counter for points within the unit circle

    // Set up a high-quality random number generator
    std::random_device rd;  // Seed with a real random value, if available
    std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<> dis(0.0, 1.0); // Uniform distribution between 0 and 1

    // Parallelize the loop with OpenMP
#pragma omp parallel for reduction(+:insideCircle)
    for (int i = 0; i < numPoints; ++i) {
        double x = dis(gen); // Generate random x coordinate
        double y = dis(gen); // Generate random y coordinate
        if (x * x + y * y <= 1) {
            insideCircle++; // Increment if inside the unit circle
        }
    }

    // Pi is approximately 4 times the ratio of points inside the circle to total points
    return 4.0 * insideCircle / numPoints;
}

int main() {
    while (true) {
        int numPoints; // Number of points to generate
        std::cout << "Enter the number of points for the simulation (or enter 0 to exit): ";
        std::cin >> numPoints; // User input for the number of points

        if (numPoints == 0) {
            std::cout << "Exiting program." << std::endl;
            break; // Exit the loop if the user enters 0
        }

        if (numPoints < 0) {
            std::cerr << "Error: Number of points must be greater than zero." << std::endl;
            continue; // Request input again if the input is invalid
        }

        std::cout << "Estimating Pi using " << numPoints << " random points." << std::endl;

        // Record the start time
        auto start = std::chrono::high_resolution_clock::now();
        // Call function to estimate Pi
        double pi = monteCarloPiEstimate(numPoints);
        // Record the end time
        auto end = std::chrono::high_resolution_clock::now();

        // Calculate and print the duration of the computation
        std::chrono::duration<double> elapsed = end - start;
        std::cout << "Estimated value of Pi: " << pi << std::endl;
        std::cout << "Calculation took " << elapsed.count() << " seconds." << std::endl;
    }

    return 0;
}
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
