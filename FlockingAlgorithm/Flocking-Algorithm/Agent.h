#ifndef AGENT // Header guard to prevent multiple inclusion of the same header file
#define AGENT

#include "Vector.h" // Include the Vector header file
using namespace mathtool; // Namespace for mathematical tools
#include <vector> // Include the vector library
using namespace std; // Namespace for standard library

class Agent { // Declaration of the Agent class
public:
    Agent(); // Default constructor
    Agent(const Agent& other); // Copy constructor

    // Method to initialize agent properties
    void Init(int _id, Vector3d _pos, Vector3d _vel, double _mass, double _maxVel, double _maxAccel, double _viewRadius);

    // Methods to get agent position, velocity, and radius
    Vector3d& GetPos() { return pos; }
    Vector3d& GetVel() { return vel; }
    double GetRadius() { return radius; }

    // Method to get agent ID
    int GetID() { return id; }

    // Method to update agent state based on the environment and other agents
    void Update(vector<Agent>& agents, double dt);

    // Method to draw the agent
    void Draw();

    // Methods to control the agent
    void SetControlled(bool ic) { isControlled = ic; }
    void ToggleControlled() { isControlled = !isControlled; }
    void SetControl(string control);

    // Methods to handle adversary behavior
    bool IsAdversary() { return isAdversary; }
    void ToggleAdversary() { isAdversary = !isAdversary; }
    void setIsAdversary(bool _adv) { isAdversary = _adv; }

    // Method to calculate force exerted by control
    Vector3d GetForceFromControl();

    // Method to resolve collisions with other agents
    void ResolveCollisionWithOtherAgents(vector<Agent>& agents);

    // Method to calculate environmental force
    Vector3d GetEnvironmentalForce(double mag);

    // Methods to get adversary and base life values
    float GetAdvLife() { return AdvR; }
    float GetBaseLife() { return BaseB; }

private:
    // Member variables
    int id; // Agent ID
    Vector3d pos; // Position vector
    Vector3d oldPos; // Old position vector
    Vector3d vel; // Velocity vector
    Vector3d separationForce; // Separation force vector
    Vector3d cohesionForce; // Cohesion force vector
    Vector3d alignmentForce; // Alignment force vector
    Vector3d obstacleForce; // Obstacle avoidance force vector
    vector<Vector3d> pastPos; // Vector to store past positions
    int tailLength; // Length of the tail

    double separationComponent; // Separation component
    double cohesionComponent; // Cohesion component
    double alignmentComponent; // Alignment component

    double radius; // Radius of the agent
    double mass; // Mass of the agent
    double maxVel; // Maximum velocity of the agent
    double maxAccel; // Maximum acceleration of the agent
    double viewRadius; // View radius of the agent

    float dmg; // Damage
    float lifespan; // Lifespan
    float AdvR, AdvG, AdvB; // Adversary color components
    float BaseR, BaseG, BaseB; // Base color components
    int FinR, FinG, FinB; // Final color components

    bool initialized; // Flag indicating if the agent is initialized

    pair<int, int> gridCell; // Grid cell indices

    bool drawForce; // Flag to indicate if force should be drawn
    bool drawVelocity; // Flag to indicate if velocity should be drawn
    bool drawVR; // Flag to indicate if view radius should be drawn

    double ori; // Orientation
    double status; // Status
    double maxStatus; // Maximum status

    bool isControlled; // Flag indicating if the agent is controlled
    string lastControl; // Last control applied to the agent
    int timeInControl; // Time spent in control

    bool isAdversary; // Flag indicating if the agent is an adversary
};

#endif // End of header guard
