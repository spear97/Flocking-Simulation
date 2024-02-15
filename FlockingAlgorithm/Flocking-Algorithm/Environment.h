#ifndef ENVIRONMENT
#define ENVIRONMENT

#include "Point.h" // Include the header file for the Point class
#include "Vector.h" // Include the header file for the Vector3d class
using namespace mathtool; // Use the mathtool namespace for Point and Vector3d

#include <iostream> // Include input/output stream functionality
#include <string> // Include string handling functionality
#include <vector> // Include vector container functionality
using namespace std; // Use the standard namespace

// Class representing a cell in the environment grid
class Cell {
public:
    Cell(); // Constructor
    void Init(int _i, int _j, Point _center, double _dx, double _dy, bool _blocked); // Method to initialize cell properties
    void Draw(); // Method to draw the cell
    Vector3d GetCenter() { // Method to get the center of the cell
        return Vector3d(center.GetX(), center.GetY(), 0); // Return the center point as a Vector3d
    }
    bool IsCollision(Vector3d pNew, double radius); // Method to check collision with a point
    void EdgeCheck(Vector3d pNew, double radius, bool& flipX, bool& flipY, double& txmin, double& txmax, double& tymin, double& tymax, int& xcondition, int& ycondition); // Method to check collision with cell edges

    int xi; // Cell index in x-direction
    int xj; // Cell index in y-direction
    double dx; // Cell width
    double dy; // Cell height
    Point center; // Center point of the cell
    Point minPt; // Minimum point of the cell
    Point maxPt; // Maximum point of the cell
    bool blocked; // Flag indicating if the cell is blocked
    bool isCol; // Flag indicating if there is a collision with the cell
};

// Class representing the environment grid
class Environment {
public:
    Environment(); // Default constructor
    Environment(string _inputFile, double _xmin, double _xmax, double _ymin, double _ymax); // Constructor with parameters
    void Draw(); // Method to draw the environment

    pair<int, int> GetCellIndices(Vector3d p); // Method to get the cell indices for a given point
    pair<int, int> GetClosestBlocked(pair<int, int> curCellInd, Vector3d pos, double radius); // Method to get the closest blocked cell
    Vector3d GetWrappedPosition(Vector3d p, bool& updated); // Method to get wrapped position if the point is outside the environment
    Vector3d GetValidPosition(Vector3d p, Vector3d oldP, double radius, Vector3d& vel, bool& updated); // Method to get a valid position for a point considering collisions and wrapping
    void MakeEmptyEnv(); // Method to make the environment empty

    void AddAttractionPoint(double tx, double ty); // Method to add an attraction point to the environment

    vector<Vector3d>& GetAttractionPoints() { // Method to get attraction points
        return attractionPts; // Return the vector of attraction points
    }

    Cell** cells; // 2D array representing the grid of cells

    double xmin, xmax, ymin, ymax; // Minimum and maximum coordinates of the environment
    double dx, dy; // Cell width and height
    int rows, columns; // Number of rows and columns in the grid
    string inputFile; // File containing environment data

    vector<Vector3d> attractionPts; // Vector storing attraction points in the environment
};

#endif
