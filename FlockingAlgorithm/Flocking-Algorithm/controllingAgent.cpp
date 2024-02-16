//#include <windows.h>
//#pragma comment(lib, "user32.lib") 

#include <GL/glut.h>  // (or others, depending on the system in use)
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
using namespace std;

#include "Color.h"
#include "Vector.h"
using namespace mathtool;
#include "Simulator.h"
#include "Environment.h"

double theta = 0; //Initial angle for some rotational calculations.
double speedTheta = 0.1; //Speed at which the angle theta changes.
int drawMode=1; //Current mode for drawing objects.

int numDraws=0; //Number of times the drawing function is called.
int windowW = 800; //Width of the display window.
int windowH = 800; //Height of the display window.
static int window_id; //Height of the display window.
static int menu_id; //Height of the display window.
vector<MyColor> colors; //Vector to store colors.
MyColor bColor(1,1,1); //Background color of the display window.
bool isConvex=true; //Flag indicating whether a shape is convex.

Simulator gSim; //Instance of the Simulator class for simulation.
Environment* gEnv=NULL; //Pointer to an instance of the Environment class, initially set to NULL.
bool isSimulating=false; //Flag indicating whether simulation is running.
Vector3d followPt; //Represents a point that the camera may follow.
bool isFollowing=false; //Flag indicating whether the camera is following a point.

bool addIndividual=true; //Flag indicating whether to add individual elements.
bool addGroup=false; //Flag indicating whether to add a group of elements.
bool addAttractionPt=false; //Flag indicating whether to add an attraction point.
bool addAdversary=false; //Flag indicating whether to add an adversary element.

// Initializes the OpenGL environment and sets up the projection parameters
void init()
{
	// Set the background color for the display window
	glClearColor(bColor.r, bColor.g, bColor.b, 0.0);

	// Select the projection matrix and reset it to the default state
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set up an orthographic projection for a top-down view
	gluOrtho2D(-windowW / 2, windowW / 2, -windowH / 2, windowH / 2);

	// Create an environment if not already initialized
	if (gEnv == NULL)
		gEnv = new Environment("env.txt", -windowW / 2, windowW / 2, -windowH / 2, windowH / 2);
}


// Adjusts the projection matrix to follow a specific point
void setFollowCamera()
{
	// Output the follow point for debugging purposes
	cout << "setFollowCamera followPt: " << followPt << endl;

	// Set the projection matrix and reset it to the default state
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Calculate the new boundaries for the orthographic projection based on the follow point
	double newXMin = followPt[0] - windowW / 10;
	double newXMax = followPt[0] + windowW / 10;
	double newYMin = followPt[1] - windowH / 10;
	double newYMax = followPt[1] + windowH / 10;

	// Set up an orthographic projection with the new boundaries
	gluOrtho2D(newXMin, newXMax, newYMin, newYMax);
}

void WritePointXYONLYsToFile(string filename) 
{
	//TODO
}

void LoadPointXYONLYsFromFile(string filename) 
{
	//TODO
}

// Menu Functionality: Handles menu selections
void menu(int num)
{
	string fileToLoad = "";
	bool fileSpecified = false;

	// Switch statement to handle different menu options
	switch (num)
	{
	case -1:
		// Clear points and edges, then trigger a redisplay
		glutPostRedisplay();
		break;
	case 0:
		// Destroy the window and exit the program
		glutDestroyWindow(window_id);
		exit(0);
		break;
	case 1:
		// Set mode to add individual
		addIndividual = true;
		addGroup = false;
		addAttractionPt = false;
		break;
	case 2:
		// Set mode to add group
		addIndividual = false;
		addGroup = true;
		addAttractionPt = false;
		break;
	case 3:
		// Set mode to add attraction point
		addAttractionPt = true;
		addIndividual = false;
		addGroup = false;
		break;
	case 4:
		// Toggle the addition of adversaries
		addAdversary = !addAdversary;
		break;
	}
}

// Creates a menu for user interaction
void createMenu()
{
	// Create a new menu and attach it to the right mouse button
	menu_id = glutCreateMenu(menu);

	// Add menu entries for different options
	glutAddMenuEntry("Clear PointXYONLYs", -1);          // Option to clear certain points
	glutAddMenuEntry("Quit", 0);                         // Option to quit the program
	glutAddMenuEntry("Add Indiv.", 1);                   // Option to add individual elements
	glutAddMenuEntry("Add Group", 2);                    // Option to add group elements
	glutAddMenuEntry("Add Attraction Point", 3);         // Option to add an attraction point
	glutAddMenuEntry("Toggle Adversary", 4);             // Option to toggle the adversary

	// Attach the menu to the right mouse button
	glutAttachMenu(GLUT_RIGHT_BUTTON);
} // End of createMenu function


int homeX, homeY;            // Stores the initial mouse position
bool mousePressed = false;   // Indicates whether the mouse button is pressed
int timeSincePress = 0;      // Tracks the time since the mouse button was pressed
int changeY = 0;             // Stores the change in mouse y-coordinate
int changeX = 0;             // Stores the change in mouse x-coordinate
double alpha = 0.5;          // Alpha value used for some calculations

void customDraw();  // Function prototype for the custom drawing function

// Updates the simulation and display
void update()
{
	// Update the simulation if it's running
	if (isSimulating)
	{
		gSim.Update(); // Update the simulation
	}

	// Adjust speedTheta if the mouse is pressed
	if (mousePressed)
	{
		speedTheta = changeY * 0.2; // Adjust speedTheta based on mouse movement
		timeSincePress += 1; // Increment time since mouse press
	}

	// Update the camera follow point if following is enabled
	if (isFollowing)
	{
		followPt = gSim.GetPointToFollow(); // Update camera follow point
		cout << "update::isFollowing " << isFollowing << " followPt: " << followPt << endl; // Print follow status and point
		setFollowCamera(); // Update camera to follow the point
	}

	// Call the custom draw function to update the display
	customDraw(); // Update the display
} // End of update function

// Draws the environment and simulation
void customDraw()
{
	// If condition is false and every 100 draws, print debug information
	if (0 && (++numDraws % 100 == 0))
	{
		cout << " customDraw numDraws = " << numDraws << " drawMode: " << drawMode << endl; // Print debug information including draw mode and number of draws
	}

	glClearColor(bColor.r, bColor.g, bColor.b, 0.0); // Set display-window color to white
	glClear(GL_COLOR_BUFFER_BIT); // Clear the color buffer to refresh the display

	gEnv->Draw(); // Draw the environment using its Draw method
	gSim.Draw(); // Draw the simulation using its Draw method

	glFlush(); // Process all OpenGL routines as quickly as possible to update the display
} // End of customDraw function

// Handles mouse button events
void mousebutton(int button, int state, int x, int y)
{
	// If left button is pressed down
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		// If mouse is not already pressed
		if (!mousePressed)
		{
			homeX = x; // Store initial x-coordinate of mouse
			homeY = y; // Store initial y-coordinate of mouse
			timeSincePress = 0; // Reset time since press counter
		}

		// Convert mouse coordinates to world coordinates
		double tx = -windowW / 2 + (1.0 * x / windowW) * (windowW);
		double ty = -windowH / 2 + ((1.0 * windowH - y) / windowH) * (windowH);

		// If adding individual members
		if (addIndividual)
		{
			gSim.AddMember(-1, tx, ty, addAdversary); // Add a member at the mouse position
		}
		// If adding a group of members
		else if (addGroup)
		{
			for (int i = 0; i < 5; i++)
			{
				gSim.AddMember(-1, tx, ty, addAdversary); // Add multiple members at the mouse position
			}
		}
		// If adding an attraction point
		else if (addAttractionPt)
		{
			gEnv->AddAttractionPoint(tx, ty); // Add an attraction point at the mouse position in the environment
		}
		mousePressed = true; // Set mousePressed flag to true
		glutPostRedisplay(); // Request a redraw of the window
	}

	// If left button is released
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		cout << " release button " << endl; // Print message indicating left button release
		theta = theta + timeSincePress * speedTheta; // Update theta based on time since press and speedTheta
		speedTheta = 1; // Reset speedTheta
		changeY = 0; // Reset changeY
		mousePressed = false; // Set mousePressed flag to false
		timeSincePress = 0; // Reset time since press
	}

	// If middle button is pressed down
	if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
	{
		cout << "glut_middle_button == down" << endl; // Print message indicating middle button press
	}
}

// Sets the background color based on a given color index, color difference, and color range
void setBackgroundColor(int colorIndex, double colorDif, double colorRange)
{
	// Print information about the background color setting
	cout << "setBackgroundColor colorIndex = " << colorIndex;
	cout << " colorDif = " << colorDif;
	cout << " colorRange = " << colorRange << endl;

	// Ensure that the color index does not exceed the available colors
	if (colorIndex >= colors.size())
	{
		colorIndex = colors.size() - 1; // Set color index to the last available color index
	}

	// Get references to the current color and the next color in the color list
	MyColor& c_i = colors[colorIndex]; // Current color
	int j = colorIndex + 1;
	if (j >= colors.size())
	{
		j = colors.size() - 1;
	}
	MyColor& c_j = colors[j]; // Next color

	// Print information about the current and next colors
	c_i.Print(); // Print details of current color
	c_j.Print(); // Print details of next color

	// Calculate the interpolation factor based on the color difference and color range
	double s = colorDif / colorRange; // Interpolation factor

	// Interpolate the RGB components of the background color between the current and next colors
	double tr = (1 - s) * c_i.r + s * c_j.r; // Interpolated red component
	double tg = (1 - s) * c_i.g + s * c_j.g; // Interpolated green component
	double tb = (1 - s) * c_i.b + s * c_j.b; // Interpolated blue component

	// Set the background color based on the interpolated RGB components
	bColor.r = tr; // Set red component of background color
	bColor.g = tg; // Set green component of background color
	bColor.b = tb; // Set blue component of background color

	// Print the newly set color
	cout << "Setting color (" << tr << "," << tg << "," << tb << ")" << endl;
}

// Handles keyboard input events
void keyboard(unsigned char key, int x, int y)
{
	// Switch statement to handle different key presses
	switch (key)
	{
	case 'q':
	case 27: // ESCAPE key
		exit(0); // Exit the program
		break;
	case ' ': // Space key
		isSimulating = !isSimulating; // Toggle simulation state
		break;
	case 'f': // 'f' key
		isSimulating = true; // Set simulation state to true
		update(); // Update simulation
		isSimulating = false; // Reset simulation state
		break;
	case 'c': // 'c' key
		isFollowing = !isFollowing; // Toggle following mode
		if (!isFollowing)
		{
			init(); // Initialize following mode
		}
		break;
	case 'e': // 'e' key
		gEnv->MakeEmptyEnv(); // Make the environment empty
		break;
	case 'a': // 'a' key
		gSim.ToggleControlledAdversary(); // Toggle controlled adversary
		break;
	case 'b': // 'b' key
		gSim.ToggleControlledAgent(); // Toggle controlled agent
		break;
	case '1': // '1' key
		drawMode = 1; // Set draw mode to 1
		break;
	case '2': // '2' key
		drawMode = 2; // Set draw mode to 2
		break;
	case '3': // '3' key
		drawMode = 3; // Set draw mode to 3
		break;
	case 9: // Tab key
		gSim.IncrementControllingAgent(); // Increment controlling agent
		break;
	}
}

// Handles special key input events
void otherKeyInput(int key, int x, int y)
{
	// Switch statement to handle different special key presses
	switch (key)
	{
	case GLUT_KEY_UP:
		cout << "GLUT_KEY_UP" << endl; // Print message for UP arrow key press
		gSim.SendControl("forward"); // Send control command for moving forward
		break;
	case GLUT_KEY_DOWN:
		// Do something for DOWN arrow key press
		cout << "GLUT_KEY_DOWN" << endl; // Print message for DOWN arrow key press
		gSim.SendControl("back"); // Send control command for moving backward
		break;
	case GLUT_KEY_LEFT:
		// Do something for LEFT arrow key press
		cout << "GLUT_KEY_LEFT" << endl; // Print message for LEFT arrow key press
		gSim.SendControl("left"); // Send control command for moving left
		break;
	case GLUT_KEY_RIGHT:
		// Do something for RIGHT arrow key press
		cout << "GLUT_KEY_RIGHT" << endl; // Print message for RIGHT arrow key press
		gSim.SendControl("right"); // Send control command for moving right
		break;
	}
	glutPostRedisplay(); // Request a redraw of the window
}

// Entry point of the program
int main(int argc, char** argv) {
	glutInit(&argc, argv);                                 // Initialize GLUT.
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);          // Set display mode.
	glutInitWindowSize(windowW, windowH);                  // Set display-window width and height.
	window_id = glutCreateWindow("Make Polygon");           // Create display window.

	init();                                                // Execute initialization procedure.

	createMenu();                                          // Create menu for user interaction.
	glutIdleFunc(update);                                  // Set update function for idle state.
	glutDisplayFunc(customDraw);                           // Send graphics to display window.
	glutKeyboardFunc(keyboard);                            // Handle keyboard input events.
	glutMouseFunc(mousebutton);                            // Handle mouse button events.
	glutSpecialFunc(otherKeyInput);                        // Handle special key input events.
	glutMainLoop();                                        // Display everything and wait.
	return 0;                                              // Return success status.
}