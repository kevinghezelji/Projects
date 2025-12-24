//
// Created by Kevin Ghezelji on 2025-11-22.
//
#include <stdio.h>
#include <math.h>

// Constant Definitions:
#define N 1000000 // N = City Population
#define I0 200 // I0 = Initial Infected, 200 on Day 0
#define S0 999800 // S0 = Total Susceptible, 1,000,000 - 200 Infected on Day 0 = 999,800 Susceptible
#define R0 0 // R0 = Total Recovered, 0 on Day 0
#define HOSPITAL_BEDS 2500 // Given Estimate for Hospital Beds

// Constants for Checks:
#define STABILITY_CHECKS 0.1 // Test value given from PDF to test the stability of one day and the previous day
#define TRUE_BETA_RANGE 0.01 // Stop Value to see if True Beta is within 0.005, works if the range is 0.01
#define MAX_DAYS 10000 // Value to stop infinite loops from happening

// Functions For MODE 1:
void getBetaGammaFromUser(double *beta, double *gamma, char *filename) //Gets the Values of Beta and Gamma from the User
{
    printf("Please enter the Filename you wish to save to: \n");
    scanf("%s", filename); // Obtains Filename for later export

    printf("Please enter the value for Beta: \n");
    scanf("%lf", beta); // Obtains value for Beta

    printf("Please enter the value for Gamma: \n");
    scanf("%lf", gamma); // Obtains value for Gamma

}

void simulateSIR(double beta, double gamma, double S[], double I[], double R[], int *dayCounter) //Uses Euler Method to simulate the values of S, I and R
{
    // Given the Differential Equations in the PDF File, we will use the following inside loops:
    // S[t+1] = S[t] - (beta * I[t] * S[t]) / N
    // I[t+1] = I[t] + (beta * I[t] * S[t]) / N - gamma * I[t]
    // R[t+1] = R[t] + gamma * I[t]

    S[0] = S0; // Sets day 0 to 999,800
    I[0] = I0; // Sets day 0 to 200
    R[0] = R0; // Sets day 0 to 0

    for (int day = 0; day < MAX_DAYS; day++) // Start the simulation loop, day by day
    {
        // Loop calculations using the Differential Equations
        S[day+1] = S[day] - (beta * I[day] * S[day]) / N;                   // Calculations for S
        I[day+1] = I[day] + (beta * I[day] * S[day]) / N - gamma * I[day];  // Calculations for I
        R[day+1] = R[day] + gamma * I[day];                                 // Calculations for R

        if (S[day+1] < 0) // Check to prevent S from going negative
        {
            S[day + 1] = 0;
        }

        // Group all the statements into one If statement as all 3 S, I and R need to pass the check, not just one
        if (
            fabs(S[day + 1] - S[day]) < STABILITY_CHECKS        // Checking to see if the absolute difference > 0.1
            && fabs(I[day + 1] - I[day]) < STABILITY_CHECKS     // Same Function for I
            && fabs(R[day + 1] - R[day]) < STABILITY_CHECKS     // Same Function for R
            ){

            *dayCounter = day + 1;
            return;
            }
    }
    *dayCounter = MAX_DAYS;
}

void printSIRtoFile(char *filename, double S[], double I[], double R[], int dayCounter) // Prints the Calculated data from previous function to an ASCII file
{
    FILE *file = fopen(filename, "w"); // Open a new file with the inputted file name, and set to write
    if (file == NULL) // Resets the Program if the file opening fails
    {
        printf("File Error\n");
        return;
    }
    fprintf(file, "Day,S,I,R\n"); // Setup Headers for the file

    for (int day = 0; day <= dayCounter; day++) // Print Loop for the file
    {
        fprintf(file, "%d,%.0lf,%.0lf,%.0lf\n", day, S[day], I[day], R[day]);
    }
    fclose(file); // Close the file after writing
}

void ModeOne() // Use all 3 helper functions to Finalize Mode 1 Deliverable
{
    // Variables
    double beta, gamma;
    char filename[100];
    double S[MAX_DAYS], I[MAX_DAYS], R[MAX_DAYS];
    int dayCounter = 0;

    getBetaGammaFromUser(&beta, &gamma, filename); // Use First Helper Function
    simulateSIR(beta, gamma, S, I, R, &dayCounter); // Use Second Helper Function
    printSIRtoFile(filename, S, I, R, dayCounter); // Use Final Helper Function
}

// Functions for Mode 2
void getBetaGammaThetaFromUser(double *beta, double *gamma, double *theta) // Need different Variables for input than Mode 1
{
    // We can reuse the start of getBetaGammaFromUser
    printf("Please enter the value for Beta: \n");
    scanf("%lf", beta); // Obtains value for Beta

    printf("Please enter the value for Gamma: \n");
    scanf("%lf", gamma); // Obtains value for Gamma

    printf("Please enter the value for theta: \n");
    scanf("%lf", theta); // Obtains value for Theta
}

int findHospitalExceeded(double I[], double theta, int dayCounter) // Function to find what day the hospital beds will be exceeded
{
    for (int day = 0; day <= dayCounter; day++) // Loop to pass through each day
    {
        double hospitalizations = I[day] * theta; // Uses Theta value to Calculate the amount of hospitalizations on any given day

        if (hospitalizations > HOSPITAL_BEDS) // Checks if hospitalizations exceed hospital beds
        {
            return day; // True, They Exceed
        }
    }
    return -1; // False, Never Exceeded
}

void printHospitalCapacity(double I[], double theta, int dayCounter) // Print Function for the Hospital Capacity
{
    int dayCapacity = findHospitalExceeded(I, theta, dayCounter); // Sets Variable Capacity to the value of findHospitalExceeded
    if (dayCapacity >= 0) // Checks if the Hospital is Exceeded
    {
        printf("Hospital Capacity Exceeded on Day %d\n", dayCapacity);
    }
    else if (dayCapacity < 0) // Checks if Hospitals never get Exceeded
    {
        printf("Hospital Capacity never Exceeded\n\n");
    }
}

void ModeTwo() // Use 4 Helper Functions
{
    // Variables
    double beta, gamma, theta;
    double S[MAX_DAYS], I[MAX_DAYS], R[MAX_DAYS];
    int dayCounter = 0;

    getBetaGammaThetaFromUser(&beta, &gamma, &theta); // Get Inputs for Beta, Gamma and Theta
    simulateSIR(beta, gamma, S, I, R, &dayCounter); // Reuse Helper Function
    printHospitalCapacity(I, theta, dayCounter); // Use Both Helper Functions to print Outcome
}

// Functions for Mode 3
void getGammaThetaFromUser(double *gamma, double *theta) // Function to get only Gamma and Theta from the user
{
    printf("Please enter the value for Gamma: \n");
    scanf("%lf", gamma); // Obtains value for gamma

    printf("Please enter the value for Theta: \n");
    scanf("%lf", theta); // Obtains value for Theta
}

double findTrueBeta(double gamma, double theta) // Function to find the largest value for B within true Beta range
{
    // Create a range of Beta
    double betaMin = 0.0; // Floor of our range
    double betaMax = 5.0; // Ceiling of our range

    for (int i = 0; i < MAX_DAYS; i++) // Creates loop with MAX_DAYS as our simulation ceiling
    {
        if ((betaMax - betaMin) <= TRUE_BETA_RANGE) // Checks to see if the difference in values is within the accepted 0.005 range
        {
            break;
        }
        double betaMid = (betaMax + betaMin) / 2; // Identify the Middle Point
        double S[MAX_DAYS], I[MAX_DAYS], R[MAX_DAYS]; // Declare our Array's
        int dayCounter = 0; // Initialize the counter
        simulateSIR(betaMid, gamma, S, I, R, &dayCounter); // Run the simulation using the TrueBeta found

        int exceededDay = findHospitalExceeded(I, theta, dayCounter); // Find the day Hospitals get exceeded using TrueBeta

        if (exceededDay >= 0) // Checks the value returned by the function
        {
            betaMax = betaMid; // Hospitals are Exceeded, lower the Beta Range by half, bring down the ceiling
        }
        else {
            betaMin = betaMid; // Hospitals aren't Exceeded, lower the Beta Range by half, raise up the floor
        }
    }
    return betaMin;
}

void printTrueBeta(double trueBeta) // Prints the results of the previous function
{
    printf("True Beta value to not Exceed Hospitals is %.3lf\n\n", trueBeta);
}

void ModeThree() // Use the helper functions
{
    double gamma, theta; // Variables
    getGammaThetaFromUser(&gamma, &theta); // Obtain values using user function
    double trueBeta = findTrueBeta(gamma, theta); // Obtain trueBeta using the bisection search for betaMin
    printTrueBeta(trueBeta); // Print Result
}

int main() {
    int option; // Initialize Variable for Menu
    int run = 1; // For While loop

    while (run) // Loop for menu and operations
    {
        // Main Menu Options
        printf("\nWelcome to the SIR Model Simulator\n");
        printf("Please select one of the Options Below\n");
        printf("(1) Mode 1: Generate SIR Data\n");
        printf("(2) Mode 2: Check Hospital Capacities\n");
        printf("(3) Mode 3: Find Social Distancing Plan\n");
        printf("(4) Exit Program\n");

        // User Input
        printf("Please Enter Your Choice: ");
        scanf("%d", &option);

        switch (option) // Switch case for the user's choice
        {
            case 1: // Runs Mode 1
                ModeOne();
                break;

            case 2: // Runs Mode 2
                ModeTwo();
                break;

            case 3: // Runs Mode 3
                ModeThree();
                break;

            case 4: // Exits Program
                printf("Thank You for using the SIR Simulator");
                run = 0;
                break;

            default:
                printf("Invalid Option, Enter any option from (1) - (4)");
                break;
        }
    }
    return 0;
}