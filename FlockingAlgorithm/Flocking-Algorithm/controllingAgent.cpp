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
	// Create the menu and attach it to the right mouse button
	menu_id = glutCreateMenu(menu);
	glutAddMenuEntry("Clear PointXYONLYs", -1);
	glutAddMenuEntry("Quit", 0);
	glutAddMenuEntry("Add Indiv.", 1);
	glutAddMenuEntry("Add Group", 2);
	glutAddMenuEntry("Add Attraction Point", 3);
	glutAddMenuEntry("Toggle Adversary", 4);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int homeX, homeY;
bool mousePressed=false;
int timeSincePress=0;
int changeY=0;
int changeX=0;
double alpha = 0.5;

void customDraw();

void update() 
{
	if (isSimulating)
	{
		gSim.Update();
	} 
  //cout << "update mousePressed=" << mousePressed << endl;
	if( mousePressed ) 
	{
		speedTheta = changeY*0.2;
		timeSincePress += 1;
	}

	if( isFollowing ) 
	{
    followPt = gSim.GetPointToFollow();
    cout << "update::isFollowing " << isFollowing << " followPt: " << followPt << endl;
    setFollowCamera();
	}

	customDraw();
}

void customDraw() 
{
  if(0 && (++numDraws % 100 == 0)) 
  {
    cout << " customDraw numDraws = " << numDraws << " drawMode: " << drawMode << endl;
  }


  glClearColor(bColor.r, bColor.g, bColor.b, 0.0); //Set display-window color to white.
  glClear(GL_COLOR_BUFFER_BIT);     //Clear display window.

  gEnv->Draw();
  gSim.Draw();

  glFlush();                        //Process all OpenGL routines as quickly as possible.
}

void mousebutton(int button, int state, int x, int y)
{
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) 
  {
    if( !mousePressed ) 
	{
      homeX = x;
      homeY = y;
      timeSincePress = 0;
    }

    double tx = -windowW/2 + (1.0*x/windowW)*(windowW);
    double ty = -windowH/2 + ((1.0*windowH-y)/windowH)*(windowH);

	if (addIndividual)
	{
		gSim.AddMember(-1, tx, ty, addAdversary);
	}
     
	else if (addGroup)
	{
		for (int i = 0; i < 5; i++)
		{
			gSim.AddMember(-1, tx, ty, addAdversary);
		}
	}
     
    else if( addAttractionPt ) 
	{
      gEnv->AddAttractionPoint(tx,ty);
    }
    mousePressed = true;
    glutPostRedisplay();
  }

  if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) 
  {
    cout << " release button " << endl;
    theta = theta+timeSincePress*speedTheta;
    speedTheta = 1;
    changeY = 0;
    //alpha = 1;
    mousePressed = false;
    timeSincePress = 0;
  }

  if( button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN ) 
  {
    cout << "glut_middle_button == down" << endl;
  }
}

void setBackgroundColor(int colorIndex, double colorDif, double colorRange) 
{
	cout << "setBackgroundColor colorIndex = " << colorIndex;
	cout << " colorDif = " << colorDif;
	cout << " colorRange = " << colorRange << endl;

	if (colorIndex >= colors.size())
	{
		colorIndex = colors.size() - 1;
	}

	MyColor& c_i = colors[colorIndex];

	int j = colorIndex + 1;
	if (j >= colors.size())
	{
		j = colors.size() - 1;
	}

	MyColor& c_j = colors[j];

	c_i.Print();
	c_j.Print();
	double s = colorDif / colorRange;	
	double tr = (1-s)*c_i.r + s*c_j.r;
	double tg = (1-s)*c_i.g + s*c_j.g;
	double tb = (1-s)*c_i.b + s*c_j.b;

	bColor.r = tr;	
	bColor.g = tg;	
	bColor.b = tb;	
	
	cout << "Setting color (" << tr << "," << tg << "," << tb << ")" << endl;
	
}
/*
void mouseMove(int mx, int my) {
	cout << "mouseMove mx: " << mx << " my: " << my << endl;
	//int changeY = my - homeY;
	changeY = my - homeY;
	//speedTheta *=1.05;
	//theta += changeY*speedTheta;
	cout << "theta " << theta << " speedTheta " << speedTheta << " changeY " << changeY << endl;
	changeX = mx - homeX;
	alpha = 0.5+ (1.0*changeX)/windowW;
	double ratioInScreen = double(mx)/windowW;
	if(ratioInScreen<0) ratioInScreen=0; //bound the allowable values
	if(ratioInScreen>1) ratioInScreen=1;
	int colorIndex = int(ratioInScreen*colors.size());
	double colorRange = 1.0/colors.size();
	double colorDif = ((ratioInScreen*colors.size())-colorIndex)/colors.size();
	cout << "alpha " << alpha << " mx " << mx << " homeX " << homeX << endl;
	cout <<" ratioInScreen " << ratioInScreen << " colorIndex " << colorIndex << " colorRange " << colorRange << " colorDif " << colorDif << endl;
 	setBackgroundColor(colorIndex, colorDif, colorRange);	

}
*/

void keyboard(unsigned char key, int x, int y)
{
  switch (key)
  {
  case 'q':
	exit(0);
	break;
  case 27:             // ESCAPE key
	exit(0);
	break;
  case ' ':
	isSimulating = !isSimulating; 
	break;
  case 'f':
  	isSimulating = true;
	update();
	isSimulating = false;
  	break;
  case 'c':
  	isFollowing = !isFollowing; 
	if (!isFollowing)
	{
		init();
	}
	break;
  case 'e':
  	gEnv->MakeEmptyEnv();
  case 'a':
  	gSim.ToggleControlledAdversary(); 
	break;
  case 'b':
  	gSim.ToggleControlledAgent(); 
	break;
  case '1':
	drawMode = 1; 
	break;
  case '2':
	drawMode = 2; 
	break;
  case '3':
	drawMode = 3; 
	break;
  case 9: //tab key
  	gSim.IncrementControllingAgent(); 
	break;

  }
}

void otherKeyInput(int key, int x, int y) 
{
  switch(key) 
  {
    case GLUT_KEY_UP:
      cout << "GLUT_KEY_UP" << endl;
      gSim.SendControl("forward");
      break;	
    case GLUT_KEY_DOWN:
      //do something here
      cout << "GLUT_KEY_DOWN" << endl;
      gSim.SendControl("back");
      break;
    case GLUT_KEY_LEFT:
      //do something here
      cout << "GLUT_KEY_LEFT" << endl;
      gSim.SendControl("left");
      break;
    case GLUT_KEY_RIGHT:
      //do something here
      cout << "GLUT_KEY_RIGHT" << endl;
      gSim.SendControl("right");
      break;
  }
  glutPostRedisplay();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);                         //Initialize GLUT.
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);   //Set display mode.
	//glutInitWindowPosition(50, 100);               //Set top-left display-window position.
	glutInitWindowSize(windowW, windowH);                  //Set display-window width and height.
	window_id = glutCreateWindow("Make Polygon");  //Create display window.

	init();                                        //Execute initialization procedure.
	//string fileToLoad = "ConvexConcaveInputs/obj6.txt";
	///LoadPointXYONLYsFromFile(fileToLoad);


	createMenu();
	//glutIdleFunc(drawPointXYONLYs);
	glutIdleFunc(update);
	glutDisplayFunc(customDraw);                   //Send graphics to display window.
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mousebutton);                    //How to handle mouse press events. 
	//glutMotionFunc(mouseMove);
	glutSpecialFunc(otherKeyInput);
	glutMainLoop();                                //Display everything and wait.
}

