#include <windows.h>
#pragma comment(lib, "user32.lib") 

#include "Agent.h"
#include "Environment.h"
#include <tuple>

#include <GL/glut.h>  // (or others, depending on the system in use)

bool agDebug = false;//true;

extern int drawMode;

float theta = -180;
bool rotOtherWay = false;

extern Environment* gEnv;

// Constructor: Initializes the agent with default values
Agent::Agent() {
    initialized = false; // Flag indicating if the agent is initialized
    drawForce = true; // Flag for drawing force vectors
    drawVelocity = true; // Flag for drawing velocity vectors
    drawVR = false; // Flag for drawing view radius
    isControlled = false; // Flag indicating if the agent is controlled
    ori = 0; // Orientation angle
    status = 1000; // Current status of the agent
    maxStatus = status; // Maximum status value
    isAdversary = false; // Flag indicating if the agent is an adversary
    tailLength = 40; // Length of the agent's tail
} // End of empty constructor


// Initialize the agent with specified parameters
void Agent::Init(int _id, Vector3d _pos, Vector3d _vel, double _mass,
    double _maxVel, double _maxAccel, double _viewRadius) {
    initialized = true; // Set initialized flag to true
    id = _id; // Set agent ID
    pos = _pos; // Set agent position
    vel = _vel; // Set agent velocity
    mass = _mass; // Set agent mass
    maxVel = _maxVel; // Set maximum velocity
    maxAccel = _maxAccel; // Set maximum acceleration
    viewRadius = _viewRadius; // Set view radius
    radius = 12; // Set agent radius
    tailLength = 40; // Set tail length

    drawForce = true; // Enable drawing force vectors
    drawVelocity = true; // Enable drawing velocity vectors
    drawVR = false; // Disable drawing view radius
    isControlled = false; // Set controlled flag to false

    separationComponent = 30; // Set separation component
    cohesionComponent = 20; // Set cohesion component
    alignmentComponent = 20; // Set alignment component
    ori = 0; // Set orientation angle
    status = 1000; // Set agent status
    maxStatus = status; // Set maximum status
    isAdversary = false; // Set adversary flag to false

    lifespan = 0.f; // Set lifespan
    AdvR = 1.f, AdvG = 0.f, AdvB = 0.f; // Set adversary color
    BaseR = 0.f, BaseG = 0.f, BaseB = 1.f; // Set base color
    FinR = 112, FinG = 128, FinB = 144; // Set fin color
}

// Copy constructor to create a new agent from another agent
Agent::Agent(const Agent& other) {
    initialized = other.initialized; // Copy initialization flag
    id = other.id; // Copy agent ID
    pos = other.pos; // Copy agent position
    vel = other.vel; // Copy agent velocity
    separationComponent = other.separationComponent; // Copy separation component
    cohesionComponent = other.cohesionComponent; // Copy cohesion component
    alignmentComponent = other.alignmentComponent; // Copy alignment component
    radius = other.radius; // Copy agent radius
    mass = other.mass; // Copy agent mass
    maxVel = other.maxVel; // Copy maximum velocity
    maxAccel = other.maxAccel; // Copy maximum acceleration
    viewRadius = other.viewRadius; // Copy view radius
    gridCell = other.gridCell; // Copy grid cell
    drawForce = other.drawForce; // Copy draw force flag
    drawVelocity = other.drawVelocity; // Copy draw velocity flag
    drawVR = other.drawVR; // Copy draw view radius flag
    tailLength = other.tailLength; // Copy tail length
    isControlled = other.isControlled; // Copy controlled flag
    ori = other.ori; // Copy orientation angle
    status = other.status; // Copy agent status
    maxStatus = other.maxStatus; // Copy maximum status
    isAdversary = other.isAdversary; // Copy adversary flag
    lifespan = other.lifespan; // Copy lifespan
    AdvR = other.AdvR; // Copy adversary red color component
    AdvG = other.AdvG; // Copy adversary green color component
    AdvB = other.AdvB; // Copy adversary blue color component
    BaseR = other.BaseR; // Copy base red color component
    BaseG = other.BaseG; // Copy base green color component
    BaseB = other.BaseB; // Copy base blue color component
    FinR = other.FinR; // Copy fin red color component
    FinG = other.FinG; // Copy fin green color component
    FinB = other.FinB; // Copy fin blue color component
}

// Calculate environmental force based on the agent's position and the environment
Vector3d Agent::GetEnvironmentalForce(double mag) {
    double closeEdgeDist = 1e6; // Initialize closest edge distance
    Vector3d pushDir; // Initialize push direction vector

    // Check if the agent is close to the left edge of the environment
    if (fabs(pos[0] - gEnv->xmin) < closeEdgeDist) {
        pushDir = Vector3d(1, 0, 0); // Set push direction towards the right
        closeEdgeDist = fabs(pos[0] - gEnv->xmin); // Update closest edge distance
    }

    // Check if the agent is close to the right edge of the environment
    if (fabs(pos[0] - gEnv->xmax) < closeEdgeDist) {
        pushDir = Vector3d(-1, 0, 0); // Set push direction towards the left
        closeEdgeDist = fabs(pos[0] - gEnv->xmax); // Update closest edge distance
    }

    // Check if the agent is close to the bottom edge of the environment
    if (fabs(pos[1] - gEnv->ymin) < closeEdgeDist) {
        pushDir = Vector3d(0, 1, 0); // Set push direction upwards
        closeEdgeDist = fabs(pos[1] - gEnv->ymin); // Update closest edge distance
    }

    // Check if the agent is close to the top edge of the environment
    if (fabs(pos[1] - gEnv->ymax) < closeEdgeDist) {
        pushDir = Vector3d(0, -1, 0); // Set push direction downwards
        closeEdgeDist = fabs(pos[1] - gEnv->ymax); // Update closest edge distance
    }

    // If the closest edge distance is greater than the agent's view radius, no force is applied
    if (closeEdgeDist > viewRadius) {
        mag = 0; // Set magnitude to zero
    }

    pushDir *= mag; // Scale push direction by magnitude
    return pushDir; // Return the environmental force vector
}

// Update the agent's state based on the environment and other agents
void Agent::Update(vector<Agent>& agents, double dt) {
    // Check if the agent has been initialized
    if (!initialized) {
        cout << "Agent::Update id: " << id << " HAS NOT BEEN INITIALIZED> " << endl;
        exit(-1); // Exit the program with an error if not initialized
    }

    // Initialize force components
    Vector3d separation(0, 0, 0);
    Vector3d cohesion(0, 0, 0);
    Vector3d alignment(0, 0, 0);
    Vector3d center(0, 0, 0);
    Vector3d force(0, 0, 0);
    Vector3d attractionPtForce(0, 0, 0);
    Vector3d obstacleForce(0, 0, 0);

    // Reset force components
    separationForce.reset();
    cohesionForce.reset();
    alignmentForce.reset();

    // If the agent is controlled by user input
    if (isControlled) {
        force = GetForceFromControl(); // Get force from user control
    }
    else {
        int numSeen = 0;

        // Generate forces based on nearby agents
        if (!isAdversary) { // If the agent is not an adversary
            double closestDistToAdv = 1e6;
            int numAdv = 0;

            // Loop through all agents
            for (int i = 0; i < (int)agents.size(); i++) {
                if (id == i) continue; // Skip self

                // Check if the agent is within view radius
                double dist = (agents[i].GetPos() - pos).norm();
                if (dist < viewRadius) {
                    if (agents[i].IsAdversary() && (dist < (radius + agents[i].GetRadius()))) {
                        numAdv++;
                        if (dist < closestDistToAdv) closestDistToAdv = dist;
                    }
                    // Update separation, center, and alignment forces
                    separation += (pos - agents[i].GetPos()).normalize() * (1.0 - dist / viewRadius);
                    center += agents[i].GetPos();
                    alignment += agents[i].GetVel();
                    numSeen++;
                }
            }

            // Get attraction point force from the environment
            vector<Vector3d>& attractionPoints = gEnv->GetAttractionPoints();
            int closestAPIndex = -1;
            double closestAPDist = 1e6;
            for (int i = 0; i < (int)attractionPoints.size(); i++) {
                double thisDist = (attractionPoints[i] - pos).norm();
                if (thisDist < closestAPDist) {
                    closestAPDist = thisDist;
                    closestAPIndex = i;
                }
            }

            if (closestAPIndex != -1) {
                Vector3d& apt = attractionPoints[closestAPIndex];
                attractionPtForce = (apt - pos).normalize() * 100.0;
            }

            // Reduce status if adversaries are nearby
            if (numAdv > 0) {
                status -= 5.0 * numAdv;
                if (status < 0) status = 0;
            }
        }
        else { // If the agent is an adversary
            int closestIndex = 0;
            double closeDist = 1e6;
            // Loop through all agents to find the closest one
            for (int i = 0; i < (int)agents.size(); i++) {
                if (id == i) continue; // Skip self
                double dist = (agents[i].GetPos() - pos).norm();
                if (dist < viewRadius && dist < closeDist) {
                    closestIndex = i;
                    closeDist = dist;
                    numSeen++;
                }
            }
            // Set force to chase the closest agent
            force = maxAccel * (agents[closestIndex].GetPos() - pos).normalize();
        }

        // Get obstacle avoidance force from the environment
        obstacleForce = GetEnvironmentalForce(20);

        if (numSeen > 0 && !isAdversary) { // If other agents are visible and the agent is not an adversary
            // Calculate cohesion, separation, and alignment forces
            center /= 1.0 * numSeen;
            double distToCenter = (center - pos).norm();
            cohesion = (center - pos).normalize() * cohesionComponent * (distToCenter / viewRadius);
            separation = separation * separationComponent;
            alignment = alignment.normalize() * alignmentComponent;

            // Sum up all forces
            force = separation + cohesion + alignment + attractionPtForce + obstacleForce;

            // Limit force magnitude
            if (force.norm() > maxAccel) {
                force.selfScale(maxAccel);
            }

            // Store individual force components
            separationForce = separation;
            cohesionForce = cohesion;
            alignmentForce = alignment;

            // Normalize force by mass
            force /= mass;
        }
        else if (numSeen > 0 && isAdversary) { // If other agents are visible and the agent is an adversary
            force /= mass; // Normalize force by mass
        }
    }

    // Update position and velocity
    oldPos = pos;
    pastPos.push_back(pos);
    while (pastPos.size() > tailLength) {
        pastPos.erase(pastPos.begin());
    }
    pos = pos + vel * dt;
    vel = vel + force * dt;

    // Truncate velocity if it exceeds maximum velocity
    if (vel.norm() > maxVel) {
        vel.selfScale(maxVel);
    }

    // Update orientation if velocity is significant
    if (vel.norm() > 0.1) {
        ori = atan2(vel[1], vel[0]);
    }

    // Update lifetime
    lifespan += 0.000000005;

    // Handle world wrapping
    bool wrapWorld = false; // Set to true to enable world wrapping
    if (wrapWorld) {
        bool updated = false;
        Vector3d pNew = gEnv->GetWrappedPosition(pos, updated);
        if (updated) {
            pos = pNew;
            pastPos.clear();
        }
    }

    // Handle collision with other agents
    bool cdWithAgents = true; // Set to true to enable collision detection with other agents
    if (cdWithAgents) {
        ResolveCollisionWithOtherAgents(agents);
    }

    // Handle collision with the world
    bool cdWorld = true; // Set to true to enable collision detection with the world
    if (cdWorld) {
        bool updated = false;
        Vector3d pNew = gEnv->GetValidPosition(pos, oldPos, radius, vel, updated);
        if (updated) {
            pos = pNew;
        }
    }
}

// Set the control for the agent
void Agent::SetControl(string control) {
    lastControl = control; // Set the last control input
    timeInControl = 0; // Reset the time in control
}

// Get the force applied based on the last control input
Vector3d Agent::GetForceFromControl() {
  timeInControl++; // Increment the time spent in control

  Vector3d force(0,0,0); // Initialize the force vector

  // Check if the agent has been in control for less than 2 steps
  if( timeInControl < 2 ) {
    // Apply force based on the last control input
    if( lastControl == "forward" ) {
      Vector3d dir = vel.normalize(); // Get the normalized velocity direction
      force = dir * 10.0 * maxAccel; // Apply forward force
    }
    else if( lastControl == "back" ) {
      Vector3d dir = vel.normalize(); // Get the normalized velocity direction
      force = dir * (-10.0 * maxAccel); // Apply backward force
    }
    else if( lastControl == "left" ) {
      Vector3d dir = vel.normalize(); // Get the normalized velocity direction
      force[0] = dir[1]; // Set x-component of force to y-component of velocity
      force[1] = -1 * dir[0]; // Set y-component of force to negative x-component of velocity
      force *= -10.0 * maxAccel; // Apply leftward force
    }
    else if( lastControl == "right" ) {
      Vector3d dir = vel.normalize(); // Get the normalized velocity direction
      force[0] = dir[1]; // Set x-component of force to y-component of velocity
      force[1] = -1 * dir[0]; // Set y-component of force to negative x-component of velocity
      force *= 10 * maxAccel; // Apply rightward force
    }
  }
  
  return force; // Return the computed force
}


//const double TWOPI = 6.28318;
void drawCircle(double radius, int divisions, bool filled) 
{
  double deltaAng = TWOPI / divisions; // Calculate the angle increment
  double curAng = 0; // Initialize the current angle

  // Begin drawing either a line loop or a filled polygon based on the 'filled' parameter
  if (!filled)
  {
      glBegin(GL_LINE_LOOP); // Draw an outline of the circle
  }
  else
  {
      glBegin(GL_POLYGON); // Draw a filled circle
  }

  // Iterate over the specified number of divisions
  for(int i=0; i<divisions; i++) 
  {
    // Calculate the x and y coordinates of the current point on the circle
    double x = radius * cos(curAng);
    double y = radius * sin(curAng);

    // Output debug information if debugging is enabled
    if (agDebug)
    {
        cout << "Circle " << i << " x= " << x << " y= " << y << endl;
    }

    // Draw the vertex corresponding to the current point
    glVertex2f( x, y );

    curAng += deltaAng; // Increment the current angle

    // Output debug information if debugging is enabled
    if (agDebug)
    {
        cout << "curAng= " << curAng << " deltaAng= " << deltaAng << endl;
    }
  }

  glEnd(); // End drawing the circle
}

// Draw an agent as a circle with the specified radius, number of divisions, fill mode, and percentage status
void drawAgentAsCircle(double radius, int divisions, bool filled, double percStatus) {
    // Draw the outline of the agent
    drawCircle(radius, divisions, false);

    // Draw a filled circle representing the status of the agent, if the percentage status is less than 0.99
    if (percStatus < 0.99) {
        drawCircle(percStatus * radius, divisions, filled);
    }
}

// Draw a triangle with the specified length and fill mode
void drawTriangle(double length, bool filled) {
    if (!filled) {
        glBegin(GL_LINE_LOOP); // Draw an outline of the triangle
    }
    else {
        glBegin(GL_TRIANGLES); // Draw a filled triangle
    }

    // Define the vertices of the triangle
    glVertex2f(length / 2.0, 0);          // Top vertex
    glVertex2f(-length / 2.0, length / 3.0); // Bottom-left vertex
    glVertex2f(-length / 2.0, -length / 3.0); // Bottom-right vertex

    glEnd(); // End drawing the triangle
}

// Draw a triangle with status indication based on percentage status
void drawTriangleStatus(double length, double percStatus) {
    // Draw the outline of the triangle
    glBegin(GL_LINE_LOOP);
    glVertex2f(-length / 2.0, length / 3.0);        // Top-left vertex
    glVertex2f(-length / 2.0, -length / 3.0);       // Bottom-left vertex
    glVertex2f(-length / 2.0 - 5, -length / 3.0);   // Bottom-right vertex offset
    glVertex2f(-length / 2.0 - 5, length / 3.0);    // Top-right vertex offset
    glEnd();

    // Calculate the y-coordinate difference based on percentage status
    double deltaY = percStatus * 2 * length / 3;

    // Draw the filled part of the triangle representing status
    glBegin(GL_POLYGON);
    glVertex2f(-length / 2.0 - 5, length / 3.0);          // Top-left vertex of filled part
    glVertex2f(-length / 2.0, length / 3.0);              // Top-right vertex of filled part
    glVertex2f(-length / 2.0, length / 3.0 - deltaY);     // Bottom-right vertex of filled part
    glVertex2f(-length / 2.0 - 5, length / 3.0 - deltaY); // Bottom-left vertex of filled part
    glEnd();
}

// Draw the body of a fish with the specified x and y radii
void drawBodyFish(double xradius, double yradius)
{
    const float DEG2RAD = 3.14159 / 180.0; // Define the conversion factor from degrees to radians
    float degInRad; // Variable to store angle in radians
    float x, y; // Variables to store x and y coordinates

    // Begin drawing the body of the fish
    glBegin(GL_POLYGON);
    // Iterate over 360 degrees to draw a complete circle
    for (int i = 0; i < 360; i++)
    {
        /// Convert degrees to radians
        degInRad = i * DEG2RAD;
        // Calculate x and y coordinates on the ellipse based on the angle
        x = cos(degInRad) * xradius, y = sin(degInRad) * yradius;
        // Draw the vertex corresponding to the calculated coordinates
        glVertex2f(x, y);
    }
    glEnd(); // End drawing the body of the fish
}

// Draw the tail of a fish
void drawFishTail() {
    // Begin drawing the fish tail
    glBegin(GL_POLYGON);
    // Define the vertices of the tail polygon
    glVertex2f(-10.0, 4.0);
    glVertex2f(-10.0, -4.0);
    glVertex2f(-30.0, -8.0);
    glVertex2f(-15.0, -2.0);
    glVertex2f(-20.0, 9.0);
    glEnd(); // End drawing the fish tail
}

// Draw the fins of a fish
void drawFishFins() {
    // Begin drawing the fish fins
    glBegin(GL_TRIANGLES);
    // Define the vertices of the fins triangle
    glVertex2f(10.0, -2.0);
    glVertex2f(25.0, -2.0);
    glVertex2f(0.0, -12.0);
    glEnd(); // End drawing the fish fins
}

// Draw the agent's graphical representation
void Agent::Draw() {
    // Adjust color based on whether the agent is an adversary or not
    if (!isAdversary) {
        BaseB -= lifespan; // Update base blue color over time
        glColor3f(BaseR, BaseG, BaseB); // Set color for non-adversary agents
    }
    else {
        AdvR -= lifespan; // Update adversary red color over time
        glColor3f(AdvR, AdvG, AdvB); // Set color for adversary agents
    }

    // Draw the agent based on the selected draw mode
    if (drawMode == 1) {
        // Draw fish-like body with tail and fins
        glPushMatrix();
        glTranslatef(pos.GetX(), pos.GetY(), 0);
        glRotated(radToDeg(ori), 0, 0, 1); // Rotate based on orientation
        drawBodyFish(radius, radius / 2); // Draw body
        glTranslatef(0.0, -1.0, 0.0); // Move to tail position
        glColor3ub(FinR, FinG, FinB);
        drawFishTail(); // Draw tail
        glTranslatef(-11.0, 2.0, 0.0); // Move to fin position
        glColor3ub(FinR, FinG, FinB);
        drawFishFins(); // Draw fins
        glPopMatrix();
    }
    else if (drawMode == 2) {
        // Draw triangular representation with status indicator
        glPushMatrix();
        glTranslatef(pos.GetX(), pos.GetY(), 0);
        glRotated(radToDeg(ori), 0, 0, 1); // Rotate based on orientation
        drawTriangle(2 * radius, isControlled); // Draw triangle
        drawTriangleStatus(2 * radius, 1.0 * status / maxStatus); // Draw status indicator
        glPopMatrix();
    }
    else if (drawMode == 3) {
        // Draw circular representation with status indicator
        glPushMatrix();
        glTranslatef(pos.GetX(), pos.GetY(), 0);
        drawAgentAsCircle(radius, 10, isControlled, 1.0 * status / maxStatus); // Draw circle
        glPopMatrix();
    }
    else {
        // Draw simple circle with trajectory path
        glPushMatrix();
        glTranslatef(pos.GetX(), pos.GetY(), 0);
        drawCircle(radius, 10, isControlled); // Draw circle
        glPopMatrix();
        glColor3f(0.8, 0.8, 0.8);
        glLineWidth(2);
        glBegin(GL_LINE_STRIP);
        for (int i = 0; i < (int)pastPos.size(); i++) {
            glVertex2f(pastPos[i][0], pastPos[i][1]); // Draw trajectory path
        }
        glEnd();
    }

    // Draw force vectors if enabled
    if (drawForce) {
        // Separation force (orange)
        glColor3f(1.0, 0.48, 0.0);
        glBegin(GL_LINES);
        glVertex2f(pos.GetX(), pos.GetY());
        glVertex2f(pos.GetX() + separationForce.GetX(), pos.GetY() + separationForce.GetY());
        glEnd();

        // Cohesion force (blue)
        glColor3f(0.0, 0.0, 1.0);
        glBegin(GL_LINES);
        glVertex2f(pos.GetX(), pos.GetY());
        glVertex2f(pos.GetX() + cohesionForce.GetX(), pos.GetY() + cohesionForce.GetY());
        glEnd();

        // Alignment force (green)
        glColor3f(0.0, 1.0, 0.0);
        glBegin(GL_LINES);
        glVertex2f(pos.GetX(), pos.GetY());
        glVertex2f(pos.GetX() + alignmentForce.GetX(), pos.GetY() + alignmentForce.GetY());
        glEnd();
    }

    // Draw velocity vector if enabled
    if (drawVelocity) {
        glColor3f(1.0, 1.0, 0.0); // Yellow color
        glBegin(GL_LINES);
        glVertex2f(pos.GetX(), pos.GetY());
        glVertex2f(pos.GetX() + vel.GetX(), pos.GetY() + vel.GetY());
        glEnd();
    }
}

// Resolve collision with other agents
void Agent::ResolveCollisionWithOtherAgents(vector<Agent>& agents) {
    // Iterate over all agents
    for (int i = 0; i < agents.size(); i++) {

        // Skip self
        if (id == agents[i].GetID()) {
            continue;
        }
        else {

            // Calculate the distance between this agent and the current other agent
            double dist = (pos - agents[i].GetPos()).norm();

            // Check if a collision occurs
            if (dist < (radius + agents[i].GetRadius())) {
                
                // Calculate the overlap distance
                double overlap = fabs(dist - (radius + agents[i].GetRadius()));

                // Calculate the direction to resolve the collision
                Vector3d resolveDir = (pos - agents[i].GetPos()).normalize();

                // Move this agent away from the collision
                pos += (overlap / 2) * resolveDir;

                // Move the other agent away from the collision
                Vector3d& pos_i = agents[i].GetPos();
                pos_i += (-overlap / 2) * resolveDir;
            }
        }
    }
}
