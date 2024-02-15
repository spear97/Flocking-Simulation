#ifndef _MYCOLOR_ // Header guard to prevent multiple inclusion of the same header file
#define _MYCOLOR_

class MyColor // Declaration of the MyColor class
{
public:
    // Constructor to initialize MyColor object with RGB values and an optional name
    MyColor(GLfloat _r, GLfloat _g, GLfloat _b, string _name = "")
    {
        r = _r; // Assign red component
        g = _g; // Assign green component
        b = _b; // Assign blue component
        name = _name; // Assign name if provided
    }

    // Method to print the color components and name
    void Print()
    {
        cout << "Color " << name << "(" << r << "," << g << "," << b << ")" << endl; // Print color information
    }

    // Member variables to store RGB components and name of the color
    GLfloat r, g, b;
    string name;
};

#endif // End of header guard
