#ifndef BASIC_H_
#define BASIC_H_

#include <cmath> // Header for mathematical functions
#include <cstdlib> // Header for standard library functions
#include <iostream> // Header for input and output streams
#include <iomanip> // Header for stream manipulators
#include <limits> // Header for numeric limits

// Declaration of a function to generate a random number between 0 and 1
double myRand();

// Namespace for mathematical tools
namespace mathtool {

    // Constants

    // Definition of PI if not already defined
#ifndef PI
#define PI 3.1415926535897
#endif

    // Definition of TWOPI if not already defined
#ifndef TWOPI
#define TWOPI 6.2831853071794
#endif

    // Function to calculate the square of a value
    template<typename T>
    inline T sqr(const T& a) {
        return a * a;
    }

    // Function to determine the sign of a number
    inline int sign(double x) {
        return x >= 0 ? 1 : -1;
    }

    // Angle conversion functions

    // Function to convert degrees to radians
    inline double degToRad(double x) {
        return x * PI / 180;
    }

    // Function to convert radians to degrees
    inline double radToDeg(double x) {
        return x * 180 / PI;
    }

    // Trigonometric functions for degrees

    // Function to calculate the sine of an angle in degrees
    inline double sind(double x) {
        return std::sin(degToRad(x));
    }

    // Function to calculate the cosine of an angle in degrees
    inline double cosd(double x) {
        return std::cos(degToRad(x));
    }

    // Function to calculate the tangent of an angle in degrees
    inline double tand(double x) {
        return std::tan(degToRad(x));
    }

    // Function to calculate the arcsine in degrees
    inline double asind(double x) {
        return radToDeg(std::asin(x));
    }

    // Function to calculate the arccosine in degrees
    inline double acosd(double x) {
        return radToDeg(std::acos(x));
    }

    // Function to calculate the arctangent in degrees
    inline double atand(double x) {
        return radToDeg(std::atan(x));
    }

    // Function to calculate the arctangent (y/x) in degrees
    inline double atan2d(double x, double y) {
        return radToDeg(std::atan2(x, y));
    }

}

#endif
