#include "Simulator.h" // Include the header file for the Simulator class

#include <stdlib.h> // Include standard library for utility functions
#include <GL/glut.h> // Include OpenGL Utility Toolkit for graphics rendering
#include "Basic.h" // Include Basic.h header for additional functionalities

// Constructor for the Simulator class
Simulator::Simulator() {
    // Initialize the simulation environment with 8 agents
    for (int i = 0; i < 8; i++) {
        // Generate random initial positions for agents
        double x = 50 + 200 * (myRand() - 0.5);
        double y = 50 + 200 * (myRand() - 0.5);
        double z = 0;
        // Add a member (agent) to the simulation environment
        AddMember(i, x, y, false); // The last argument indicates if the agent is an adversary or not
    }
    // Initialize the controlling agent index
    controlAgentIndex = -1;
}

// Method to add a member (agent) to the simulation environment
void Simulator::AddMember(int index, double x, double y, bool _adv) {
    // Generate a new agent with random properties
    double z = 0;
    Vector3d pos(x, y, z); // Set position vector
    double vx = 20 * (myRand() - 0.5); // Random x-component velocity
    double vy = 20 * (myRand() - 0.5); // Random y-component velocity
    double vz = 0; // z-component velocity (constant for 2D simulation)
    Vector3d vel(vx, vy, vz); // Set velocity vector
    double mass = 4 + myRand() * 10; // Random mass between 4 and 14
    double maxVel = 8 + myRand() * 5; // Random maximum velocity between 8 and 28
    double maxAccel = 20 + myRand() * 10; // Random maximum acceleration between 20 and 30
    double viewRadius = 40 + myRand() * 10; // Random view radius between 40 and 50
    Agent a_i; // Create a new Agent instance
    // Initialize the agent with the generated parameters
    a_i.Init(index, pos, vel, mass, maxVel, maxAccel, viewRadius);
    a_i.setIsAdversary(_adv); // Set whether the agent is an adversary or not
    agents.push_back(a_i); // Add the agent to the simulation environment
}

// Method to draw the simulation environment
void Simulator::Draw() {
    // Iterate through each agent and draw it
    for (int i = 0; i < (int)agents.size(); i++) {
        agents[i].Draw();
    }
}

// Method to update the simulation state
void Simulator::Update() {
    double dt = 0.025; // Time step for simulation update

    // Update each agent's state
    for (int i = 0; i < (int)agents.size(); i++) {
        agents[i].Update(agents, dt);
    }

    int Del_Index = -1; // Index of the agent to be deleted (if any)

    // Check if any agent needs to be removed from the simulation
    for (int i = 0; i < (int)agents.size(); i++) {
        if (agents[i].IsAdversary()) { // Check if the agent is an adversary
            if (agents[i].GetAdvLife() <= 0.f) {
                Del_Index = i; // Mark the agent for deletion if its life is zero or below
                break;
            }
        }
        else { // For non-adversary agents
            if (agents[i].GetBaseLife() <= 0.f) {
                Del_Index = i; // Mark the agent for deletion if its life is zero or below
                break;
            }
        }
    }

    // Remove the marked agent from the simulation
    if (Del_Index >= 0) {
        agents.erase(agents.begin() + Del_Index);
    }
}

// Method to increment the controlling agent
void Simulator::IncrementControllingAgent() {
    // Set the currently controlling agent to uncontrolled
    if (controlAgentIndex >= 0 && controlAgentIndex < agents.size()) {
        agents[controlAgentIndex].SetControlled(false);
    }
    // Increment the controlling agent index and handle overflow
    controlAgentIndex++;
    if (controlAgentIndex >= agents.size()) {
        controlAgentIndex = controlAgentIndex % agents.size();
    }
    // Set the new controlling agent to controlled
    agents[controlAgentIndex].SetControlled(true);
}

// Method to send control signals to the controlling agent
void Simulator::SendControl(string control) {
    // Send control signal to the currently controlling agent
    if (controlAgentIndex >= 0 && controlAgentIndex < agents.size()) {
        agents[controlAgentIndex].SetControl(control);
    }
}

// Method to toggle control of the currently controlling agent
void Simulator::ToggleControlledAgent() {
    // Toggle control of the currently controlling agent
    if (controlAgentIndex >= 0 && controlAgentIndex < agents.size()) {
        agents[controlAgentIndex].ToggleControlled();
    }
}

// Method to toggle the adversary status of the currently controlling agent
void Simulator::ToggleControlledAdversary() {
    // Toggle adversary status of the currently controlling agent
    if (controlAgentIndex >= 0 && controlAgentIndex < agents.size()) {
        agents[controlAgentIndex].ToggleAdversary();
    }
}

// Method to get the point to follow in the simulation (used for camera tracking)
Vector3d Simulator::GetPointToFollow() {
    // Get the position of the currently controlling agent
    if (controlAgentIndex >= 0 && controlAgentIndex < agents.size()) {
        return agents[controlAgentIndex].GetPos();
    }
    else {
        // Return the position of the first agent if no controlling agent is set
        return agents[0].GetPos();
    }
}
