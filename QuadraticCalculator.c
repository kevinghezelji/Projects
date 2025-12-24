#include <stdio.h>
#include <math.h>

// Quadratic Simulator, feed Quadratic in form ax^2 + bx + c, outputs the solved numbers for x
void solveQuadratic(double a, double b, double c, double *pos1, double *pos2)
    {
    double discriminant = b*b - 4*a*c; // Setup for the formula of the discriminant
    if (discriminant < 0 || a == 0) // Checking to make sure we aren't dividing by 0 or if there's a negative sqrt
        {
        printf("There are no solutions for this Quadratic Polynomial"); // Print Statement
        return; // Return, to end function if the parameters are not met
    }
    else
        {
        *pos1 = (-b + sqrt(discriminant)) / (2*a);
        *pos2 = (-b - sqrt(discriminant)) / (2*a);

        printf("For Quadratic %.2lfx^2 + %.2lfx + %.2lf are\n", a, b, c);
        printf("x = %.2lf and x = %.2lf", *pos1, *pos2);
    }
}

int main()
{
    double a, b, c;
    printf("Welcome to the Quadratic Formula Calculator, for the form ax^2 + bx + c, please enter a, b and c:\n");
    scanf("%lf %lf %lf", &a, &b, &c);

    double solutionOne, solutionTwo;
    solveQuadratic(a, b, c, &solutionOne, &solutionTwo);

    return 0;
}