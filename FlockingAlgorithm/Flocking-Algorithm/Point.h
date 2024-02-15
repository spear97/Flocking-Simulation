#ifndef POINT // Header guard to prevent multiple inclusion of the same header file
#define POINT

class Point { // Declaration of the Point class
public:
    // Constructor to initialize a Point object with optional x, y, and z coordinates
    Point(double _x = 0, double _y = 0, double _z = 0) {
        x = _x; // Assign x-coordinate
        y = _y; // Assign y-coordinate
        z = _z; // Assign z-coordinate
    }

    // Method to get the x-coordinate of the point
    double GetX() {
        return x; // Return the x-coordinate
    }

    // Method to get the y-coordinate of the point
    double GetY() {
        return y; // Return the y-coordinate
    }

    // Method to get the z-coordinate of the point
    double GetZ() {
        return z; // Return the z-coordinate
    }

    // Member variables to store the x, y, and z coordinates of the point
    double x, y, z;
};

#endif // End of header guard
