#ifndef SIMULATOR
#define SIMULATOR

#include "Vector.h" // Include necessary dependencies for vector manipulation
using namespace mathtool; // Use the mathtool namespace for Vector3d

#include "Agent.h" // Include the Agent class for simulation

#include <vector> // Include the vector container for managing Agent instances
using namespace std; // Use the standard namespace

// Class representing a simulation environment
class Simulator {
public:
    // Constructor
    Simulator();

    // Method to add a member to the simulation
    void AddMember(int index, double x, double y, bool _adv);

    // Method to draw the simulation
    void Draw();

    // Method to update the simulation state
    void Update();

    // Method to increment the controlling agent
    void IncrementControllingAgent();

    // Method to send control signals to the controlling agent
    void SendControl(string control);

    // Method to toggle control between adversary and agent
    void ToggleControlledAdversary();
    void ToggleControlledAgent();

    // Method to get the point to follow in the simulation
    Vector3d GetPointToFollow();

private:
    vector<Agent> agents; // Vector to store instances of Agent
    int controlAgentIndex; // Index of the currently controlling agent
};

#endif
