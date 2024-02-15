#include <windows.h> // Include the Windows header file
#pragma comment(lib, "user32.lib") // Link user32.lib library

#include "Environment.h" // Include the header file for the Environment class
#include <fstream> // Include the file stream functionality

#include <GL/glut.h> // Include the OpenGL Utility Toolkit header file

///////////////////////////////////////////////////////////
// Implementation of methods for the Cell class
Cell::Cell() { // Default constructor for Cell class
    blocked = false; // Initialize blocked flag to false
    isCol = false; // Initialize collision flag to false
}

void Cell::Init(int _i, int _j, Point _center, double _dx, double _dy, bool _blocked) { // Method to initialize cell properties
    xi = _i; // Set x index of the cell
    xj = _j; // Set y index of the cell
    dx = _dx; // Set width of the cell
    dy = _dy; // Set height of the cell
    center = _center; // Set center point of the cell
    minPt.x = center.x - dx / 2.0; // Calculate minimum x-coordinate of the cell
    minPt.y = center.y - dy / 2.0; // Calculate minimum y-coordinate of the cell
    maxPt.x = center.x + dx / 2.0; // Calculate maximum x-coordinate of the cell
    maxPt.y = center.y + dy / 2.0; // Calculate maximum y-coordinate of the cell
    blocked = _blocked; // Set blocked flag of the cell
    isCol = false; // Reset collision flag to false
}

void Cell::Draw() { // Method to draw the cell
    glLineWidth(1); // Set the line width
    if (blocked) { // If the cell is blocked
        if (isCol) { // If there is a collision
            glColor3f(0.6, 0.9, 0.6); // Set color to indicate collision
        }
        else {
            glColor3f(0.6, 0.6, 0.6); // Set default color for blocked cell
        }
        glBegin(GL_POLYGON); // Begin drawing filled polygon for blocked cell
    }
    else {
        glColor3f(0.6, 0.6, 0.6); // Set default color for unblocked cell
        glBegin(GL_LINE_LOOP); // Begin drawing outline for unblocked cell
    }
    // Define vertices of the cell
    glVertex2f(minPt.x, minPt.y);
    glVertex2f(maxPt.x, minPt.y);
    glVertex2f(maxPt.x, maxPt.y);
    glVertex2f(minPt.x, maxPt.y);
    glEnd(); // End drawing the cell
}

bool Cell::IsCollision(Vector3d pNew, double radius) { // Method to check collision with a point
    bool collision = (pNew[0] < (center.GetX() + dx / 2 + radius)) &&
        (pNew[0] > (center.GetX() - dx / 2 - radius)) &&
        (pNew[1] < (center.GetY() + dy / 2 + radius)) &&
        (pNew[1] > (center.GetY() - dy / 2 - radius));
    if (collision) { // If there is a collision
        cout << "pNew=" << pNew << " radius=" << radius << " dx= " << dx << " dy=" << dy << endl; // Print collision details
        cout << "condition1=" << (center.GetX() + dx + radius) << endl;
        cout << "condition2=" << (center.GetX() - dx - radius) << endl;
        cout << "condition3=" << (center.GetY() + dy + radius) << endl;
        cout << "condition4=" << (center.GetY() - dy - radius) << endl;
        isCol = true; // Set collision flag to true
        return true; // Return true to indicate collision
    }
    return false; // Return false if no collision
}

void Cell::EdgeCheck(Vector3d p, double radius, bool& flipX, bool& flipY, double& txmin, double& txmax,
    double& tymin, double& tymax, int& xcondition, int& ycondition) { // Method to check edge conditions
    // Calculate minimum and maximum x, y coordinates for edge conditions
    txmin = center.GetX() - dx / 2 - radius;
    txmax = center.GetX() + dx / 2 + radius;
    tymax = center.GetY() + dy / 2 + radius;
    tymin = center.GetY() - dy / 2 - radius;
    xcondition = -1; // Initialize x condition
    ycondition = -1; // Initialize y condition
    flipX = false; // Reset flipX flag to false
    flipY = false; // Reset flipY flag to false

    if (p[0] > txmin && p[0] < txmax) { // Check x edge conditions
        flipX = true; // Set flipX flag to true
        if (fabs(p[0] - txmax) < fabs(p[0] - txmin))
            xcondition = 1; // Set x condition to 1
        else
            xcondition = 0; // Set x condition to 0
    }

    if (p[1] > tymin && p[1] < tymax) { // Check y edge conditions
        flipY = true; // Set flipY flag to true
        if (fabs(p[1] - tymax) < fabs(p[1] - tymin))
            ycondition = 1; // Set y condition to 1
        else
            ycondition = 0; // Set y condition to 0
    }
}

///////////////////////////////////////////////////////////
// Implementation of methods for the Environment class
Environment::Environment() { /*Constructor*/ } // Default constructor for Environment class

Environment::Environment(string _inputFile, double _xmin, double _xmax, double _ymin, double _ymax) { // Constructor with parameters
    inputFile = _inputFile; // Set input file name
    xmin = _xmin; // Set minimum x-coordinate of the environment
    xmax = _xmax; // Set maximum x-coordinate of the environment
    ymin = _ymin; // Set minimum y-coordinate of the environment
    ymax = _ymax; // Set maximum y-coordinate of the environment
    cout << " Environment boundaries: [" << xmin << ", " << xmax << " | " << ymin << ", " << ymax << "]" << endl; // Print environment boundaries
    cout << "Attempting to load environment file: " << inputFile << endl; // Print attempt to load environment file
    ifstream infile; // Declare input file stream object
    infile.open(inputFile.c_str()); // Open input file

    if (!infile) { // If input file cannot be opened
        cout << "Could NOT open infile: " << inputFile << endl; // Print error message
        //exit(-1); // Exit the program with error code
    }
    if (infile >> rows) { // Read number of rows from input file
        cout << "There will be " << rows << " rows "; // Print number of rows
    }
    if (infile >> columns) { // Read number of columns from input file
        cout << "There will be " << columns << " rows "; // Print number of columns
    }
    cells = new Cell * [rows]; // Allocate memory for array of cell pointers
    for (int i = 0; i < rows; i++) { // Iterate over rows
        cells[i] = new Cell[columns]; // Allocate memory for cells in each row
    }
    dx = (_xmax - _xmin) / columns; // Calculate width of each cell
    dy = (_ymax - _ymin) / rows; // Calculate height of each cell
    string colData; // Declare variable to store column data
    string tmpStr; // Declare temporary string variable
    getline(infile, tmpStr); // Read and discard first line (header)
    int rowIndex = 0; // Initialize row index
    while (getline(infile, colData)) { // Read column data from input file
        for (int i = 0; i < colData.length(); i++) { // Iterate over characters in column data
            Cell& cell = cells[rowIndex][i]; // Get reference to cell at current position
            bool isBlocked = (colData.at(i) == 'x' || colData.at(i) == 'X') ? true : false; // Check if cell is blocked
            Point center(xmin + dx / 2 + dx * i, ymin + dy / 2 + dy * rowIndex); // Calculate center point of the cell
            cell.Init(rowIndex, i, center, dx, dy, isBlocked); // Initialize cell properties
        }
        rowIndex++; // Increment row index
    }
    infile.close(); // Close input file
}

void drawAttractionPt(double radius, int divisions, bool filled) { // Function to draw attraction point
    double deltaAng = TWOPI / divisions; // Calculate angle increment
    double curAng = 0; // Initialize current angle
    if (!filled) { // If not filled
        glBegin(GL_LINE_LOOP); // Begin drawing outline of attraction point
    }
    else {
        glBegin(GL_POLYGON); // Begin drawing filled attraction point
    }
    for (int i = 0; i < divisions; i++) { // Iterate over divisions
        double x = radius * cos(curAng); // Calculate x-coordinate of vertex
        double y = radius * sin(curAng); // Calculate y-coordinate of vertex
        glVertex2f(x, y); // Specify vertex
        curAng += deltaAng; // Increment current angle
    }
    glEnd(); // End drawing attraction point
}

void Environment::Draw() { // Method to draw the environment
    for (int i = 0; i < rows; i++) { // Iterate over rows
        for (int j = 0; j < columns; j++) { // Iterate over columns
            cells[i][j].Draw(); // Draw each cell
        }
    }
    for (int i = 0; i < attractionPts.size(); i++) { // Iterate over attraction points
        Vector3d& tv = attractionPts[i]; // Get reference to attraction point
        glPushMatrix(); // Push current matrix onto the stack
        glTranslatef(tv[0], tv[1], 0); // Translate to attraction point position
        drawAttractionPt(10, 4, true); // Draw filled attraction point
        glPopMatrix(); // Pop matrix from the stack
    }
}

pair<int, int> Environment::GetCellIndices(Vector3d p) { // Method to get cell indices for a given point
    int i = int((p[0] - xmin) / dx); // Calculate column index
    int j = int((p[1] - ymin) / dy); // Calculate row index

    if (i < 0) { i = 0; } // Ensure column index is non-negative
    if (j < 0) { j = 0; } // Ensure row index is non-negative
    if (i >= columns) { i = columns - 1; } // Ensure column index is within bounds
    if (j >= rows) { j = rows - 1; } // Ensure row index is within bounds

    return make_pair(j, i); // Return row and column indices
}

pair<int, int> Environment::GetClosestBlocked(pair<int, int> curCellInd, Vector3d pos, double radius) {
    // Method to get the closest blocked cell
    int numXToOffset = int(dx / radius + 0.5) + 1; // Calculate number of cells to offset in x-direction
    int numYToOffset = int(dy / radius + 0.5) + 1; // Calculate number of cells to offset in y-direction
    int xIndexStart = curCellInd.second - numXToOffset; // Calculate starting column index
    int yIndexStart = curCellInd.first - numYToOffset; // Calculate starting row index

    if (xIndexStart < 0) { xIndexStart = 0; } // Ensure starting column index is non-negative
    if (yIndexStart < 0) { yIndexStart = 0; } // Ensure starting row index is non-negative
    int xIndexEnd = curCellInd.second + numXToOffset; // Calculate ending column index
    int yIndexEnd = curCellInd.first + numYToOffset; // Calculate ending row index

    if (xIndexEnd >= columns) { xIndexEnd = columns - 1; } // Ensure ending column index is within bounds
    if (yIndexEnd >= rows) { yIndexEnd = rows - 1; } // Ensure ending row index is within bounds

    pair<int, int> closestCellIndex(-1, -1); // Initialize indices of closest blocked cell
    double closeDist = 1e6; // Initialize closest distance
    for (int j = yIndexStart; j <= yIndexEnd; j++) { // Iterate over rows
        for (int i = xIndexStart; i <= xIndexEnd; i++) { // Iterate over columns
            if (cells[j][i].blocked) { // If cell is blocked
                double dist = (cells[j][i].GetCenter() - pos).norm(); // Calculate distance to cell center
                if (dist < closeDist) { // If distance is smaller than current closest distance
                    closeDist = dist; // Update closest distance
                    closestCellIndex = make_pair(j, i); // Update indices of closest blocked cell
                }
            }
        }
    }
    return closestCellIndex; // Return indices of closest blocked cell
}

Vector3d Environment::GetWrappedPosition(Vector3d p, bool& updated) { // Method to get wrapped position
    Vector3d pNew(p); // Create copy of input position vector
    if (pNew.GetX() > xmax) { // If x-coordinate exceeds maximum
        pNew.SetX(pNew.GetX() - (xmax - xmin)); // Wrap around to minimum
        updated = true; // Set updated flag
    }
    else if (pNew.GetX() < xmin) { // If x-coordinate is below minimum
        pNew.SetX(pNew.GetX() + (xmax - xmin)); // Wrap around to maximum
        updated = true; // Set updated flag
    }

    if (pNew.GetY() > ymax) { // If y-coordinate exceeds maximum
        pNew.SetY(pNew.GetY() - (ymax - ymin)); // Wrap around to minimum
        updated = true; // Set updated flag
    }
    else if (pNew.GetY() < ymin) { // If y-coordinate is below minimum
        pNew.SetY(pNew.GetY() + (ymax - ymin)); // Wrap around to maximum
        updated = true; // Set updated flag
    }
    return pNew; // Return wrapped position
}

Vector3d Environment::GetValidPosition(Vector3d p, Vector3d oldP, double radius, Vector3d& vel, bool& updated) { // Method to get valid position
    Vector3d pNew(p); // Create copy of input position vector
    if (pNew.GetX() + radius > xmax) { // If x-coordinate exceeds maximum
        pNew = oldP; // Restore old position
        pNew[0] = xmax - radius; // Set x-coordinate to maximum
        vel[0] *= -1; // Reverse velocity direction
        updated = true; // Set updated flag
    }
    else if (pNew.GetX() - radius < xmin) { // If x-coordinate is below minimum
        pNew = oldP; // Restore old position
        pNew[0] = xmin + radius; // Set x-coordinate to minimum
        vel[0] *= -1; // Reverse velocity direction
        updated = true; // Set updated flag
    }

    if (pNew.GetY() + radius > ymax) { // If y-coordinate exceeds maximum
        pNew[1] = ymax - radius; // Set y-coordinate to maximum
        vel[1] *= -1; // Reverse velocity direction
        updated = true; // Set updated flag
    }
    else if (pNew.GetY() - radius < ymin) { // If y-coordinate is below minimum
        pNew[1] = ymin + radius; // Set y-coordinate to minimum
        vel[1] *= -1; // Reverse velocity direction
        updated = true; // Set updated flag
    }

    pair<int, int> curCellInd = GetCellIndices(pNew); // Get indices of current cell
    pair<int, int> closestBlocked = GetClosestBlocked(curCellInd, pNew, radius); // Get indices of closest blocked cell

    if (closestBlocked.first != -1 && closestBlocked.second != -1) { // If a blocked cell is found
        int j = closestBlocked.first; // Get row index of blocked cell
        int i = closestBlocked.second; // Get column index of blocked cell
        Cell& cell = cells[j][i]; // Get reference to blocked cell
        if (cell.IsCollision(pNew, radius)) { // If collision with blocked cell occurs
            pNew = oldP; // Restore old position
            updated = true; // Set updated flag
            bool flipX = false; // Initialize flipX flag
            bool flipY = false; // Initialize flipY flag
            double txmin, txmax, tymin, tymax; // Declare variables for edge conditions
            int tcondX, tcondY; // Declare variables for edge condition indices
            cell.EdgeCheck(pNew, radius, flipX, flipY, txmin, txmax, tymin, tymax, tcondX, tcondY); // Check edge conditions
            if (flipX) { // If flipX is true
                vel[0] *= -1.0; // Reverse x-direction velocity
                if (tcondX == 1) {
                    pNew[0] = txmax; // Set x-coordinate to maximum edge
                }
                else {
                    pNew[0] = txmin; // Set x-coordinate to minimum edge
                }
            }
            if (flipY) { // If flipY is true
                vel[1] *= -1.0; // Reverse y-direction velocity
                if (tcondY == 1) {
                    pNew[1] = tymax; // Set y-coordinate to maximum edge
                }
                else {
                    pNew[1] = tymin; // Set y-coordinate to minimum edge
                }
            }
        }
    }
    return pNew; // Return valid position
}

void Environment::MakeEmptyEnv() { // Method to make the environment empty
    for (int j = 0; j < rows; j++) { // Iterate over rows
        for (int i = 0; i < columns; i++) { // Iterate over columns
            cells[j][i].blocked = false; // Set cell as unblocked
        }
    }
}

void Environment::AddAttractionPoint(double tx, double ty) { // Method to add an attraction point
    Vector3d aP(tx, ty, 0); // Create attraction point vector
    attractionPts.push_back(aP); // Add attraction point to list
}
