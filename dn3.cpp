#include <iostream>
#include <cmath>

double calcAtan(double* x, int* N_steps);
double f(double x, int* N_atan_steps);
double trapIntegral(double a, double b, int* N_trap, int* N_atan_steps);

int main()
{
    double a = 0.0;
    double b = M_PI / 4.0;

    int N_trap = 100000;      
    int N_atan_steps = 50;    

    double I = trapIntegral(a, b, &N_trap, &N_atan_steps);

    std::cout << "Integral (trapezna metoda) = " << I << std::endl;

    return 0;
}

double calcAtan(double* x, int* N_steps)
{
    double s = 0.0;
    double xx = *x;

    for (int n = 0; n < *N_steps; n++)
    {
        double num = std::pow(xx, 2*n + 1);
        double den = 2*n + 1;
        double term = num / den;

        if (n % 2 == 1) term = -term;
        s += term;
    }

    return s;
}

double f(double x, int* N_atan_steps)
{
    double t = x / 2.0;
    double atan_t = calcAtan(&t, N_atan_steps);
    return std::exp(3.0 * x) * atan_t;
}

double trapIntegral(double a, double b, int* N_trap, int* N_atan_steps)
{
    int n = *N_trap;
    double dx = (b - a) / n;

    double sum = f(a, N_atan_steps) + f(b, N_atan_steps);

    for (int i = 1; i < n; i++)
    {
        double x = a + i * dx;
        sum += 2.0 * f(x, N_atan_steps);
    }

    return (dx / 2.0) * sum;
}
