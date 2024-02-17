# Flocking-Simulation

## Description

This project serves to illustrate how the natural phenomenon of flocking can be simulated with a 2D Environment, and utlizes C++ and OpenGL to aid in achieving this. 

## Screenshots

## Table of Contents
1. Dependencies
2. What is Flocking?
3. Classes
    - Agent
    - Cell
    - Environment
    - MyColor
    - Point
    - Simulator
    - Vector3d

## Dependencies

## What is Flocking?

## Classes

### Agent

The `Agent` class represents an entity in a simulation environment with properties such as position, velocity, and behavior. This document outlines the functionalities and usage of the `Agent` class.

#### Features

- Initialization with specified parameters.
- Update of agent state based on the environment and other agents.
- Drawing the agent's graphical representation.
- Control methods for agent behavior.
- Collision resolution with other agents.
- Calculation of environmental forces.
- Access to adversary and base life values.

#### Usage

- Includes necessary header files such as <windows.h>, <GL/glut.h>, and custom headers "Agent.h" and "Environment.h".
- Declares global variables like agDebug, theta, rotOtherWay, and gEnv.
- Defines an Agent class with member variables and functions to represent and manipulate agents. The class includes constructors, initialization functions, methods for updating agent state, drawing agents, handling controls, calculating forces, resolving collisions, and more.
- Defines functions to draw various shapes like circles, triangles, fish bodies, tails, and fins using OpenGL commands.
- Implements a method ResolveCollisionWithOtherAgents to handle collisions between agents. 

### Cell

The `Cell` is designed to represent cells in an environment grid. The Cell class provides functionality for initializing cell properties, drawing cells, checking for collisions with points, and handling collisions with cell edges.

#### Features

- Constructor for initializing cell properties such as indices, dimensions, center point, and blocking status.
- Method to draw the cell visually in a graphical simulation environment.
- Method to retrieve the center of the cell as a `Vector3d` object.
- Method to check for collisions between a `point` and the `cell`.
- Method to handle collisions with the edges of the `cell`, including flipping coordinates and determining collision conditions.

#### Usage 

- Integrate the `Cell` class into your project structure.
- Include the necessary dependencies, including the `Point` and `Vector3d` classes.
- Customize the `Cell` class behavior according to your project requirements, adjusting methods and properties as needed.
- Implement functionality to interact with cells, such as checking collisions and updating cell properties based on the `environment`.

### Environment

The `Environment` class provided in this repository represents an environment grid. It offers functionality for creating, visualizing, and interacting with the grid. This class includes methods for drawing the environment, obtaining cell indices for given points, finding the closest blocked `cell`, managing wrapped positions, ensuring valid positions considering collisions and wrapping, making the environment empty, and adding attraction points.

#### Features

- Default constructor for creating an environment.
- Parameterized constructor allowing customization of environment properties such as input file, minimum and maximum coordinates, and grid dimensions.
- Method to draw the environment graphically.
- Methods for retrieving cell indices, finding the closest blocked cell, and managing wrapped positions.
- Functionality for obtaining valid positions for points, considering collisions and wrapping.
- Ability to create an empty environment or add attraction points.
- Accessor method for retrieving attraction points stored in the environment.
- Utilizes a 2D array to represent the grid of cells.
- Stores information about the environment's dimensions, grid cell dimensions, and input file.

#### Usage

- Integrate the `Environment` class into your project structure.
- Include the necessary dependencies, such as the Point and `Vector3d` classes.
- Customize the `Environment` class behavior according to your project requirements, adjusting methods and properties as needed.
- Utilize the provided methods to interact with the environment, including drawing, obtaining cell indices, managing positions, and adding attraction points.

### MyColor

The `MyColor` class provided in this repository represents a color using RGB (Red, Green, Blue) components. It offers functionality for initializing color objects with RGB values and an optional name, printing color information, and storing RGB components along with a name.

#### Features 

- Constructor to initialize `MyColor` objects with RGB values and an optional name.
- Method to print the color components along with the name.
- Member variables to store RGB components and the name of the color.
- Utilizes a header guard to prevent multiple inclusions of the same header file.

#### Usage

- Integrate the `MyColor` class header file into your project structure.
- Use the `MyColor` class to represent colors in your application, initializing objects with RGB values and optional names.
- Utilize the provided methods, such as printing color information, to interact with `MyColor` objects as needed.

### Point

The `Point` class provided in this repository represents a point in three-dimensional space. It offers functionality for initializing point objects with optional x, y, and z coordinates, as well as methods for retrieving the individual coordinates.

#### Features 

- Constructor to initialize Point objects with optional x, y, and z coordinates.
- Methods to retrieve the x, y, and z coordinates of the point.
- Member variables to store the x, y, and z coordinates of the point.
- Utilizes a header guard to prevent multiple inclusions of the same header file.

#### Usage
- Integrate the `Point` class header file into your project structure.
- Use the `Point` class to represent points in three-dimensional space, initializing objects with optional x, y, and z coordinates.
- Utilize the provided methods, such as retrieving individual coordinates, to interact with `Point` objects as needed.

### Simulator

The Simulator class provided in this repository manages a simulation environment, including the addition and manipulation of agents, drawing the simulation, updating the simulation state, and controlling agents' behaviors.

#### Features

- Constructor to initialize a `Simulator` object.
- Methods to add agents to the simulation, draw the simulation environment, and update the simulation state.
- Functionality to increment the controlling agent, send control signals to agents, and toggle control between adversaries and agents.
- Ability to retrieve the point to follow in the simulation.
- Utilizes header guards to prevent multiple inclusions of the same header file.

#### Usage

- Include the necessary dependencies, such as `Vector.h` for vector manipulation and Agent.h for the `Agent` class.
- Instantiate a `Simulator` object to manage the simulation environment.
- Add agents to the simulation using the `AddMember` method, specifying their attributes.
- Draw the simulation environment using the `Draw` method to visualize agent positions and interactions.
- Update the simulation state using the `Update` method to reflect changes over time.
- Control agents' behaviors by incrementing the controlling agent, sending control signals, or toggling control between adversaries and agents.
- Retrieve relevant information, such as the point to follow, using appropriate methods.

### Vector3d

The `Vector3d` class provided in this repository represents a 3-dimensional vector and offers various operations for vector manipulation, including arithmetic operations, comparison operators, and utility methods.

#### Features

- Constructors to initialize a vector with optional x, y, and z components, or from another vector or an array.
- Assignment operators to set new values for the vector's components.
- Comparison operators for equality and inequality checks between vectors.
- Compound assignment operators for addition, subtraction, scalar multiplication, and scalar division.
- Unary operators for negation.
- Binary arithmetic operators for addition, subtraction, scalar multiplication, and scalar division.
- Utility methods for dot product, magnitude, normalization, scaling, and component access.
- Additional utility functions for scalar multiplication with vectors and output operator for vectors.
- Utilizes header guards to prevent multiple inclusions of the same header file.

#### Usage

- Include the `Vector.h` header file in your project to access the `Vector3d` class.
- Instantiate `Vector3d` objects to represent 3-dimensional vectors, optionally specifying their x, y, and z components.
- Utilize the provided methods and operators to perform various vector operations, such as addition, subtraction, scalar multiplication, normalization, and scaling.
- Integrate the `Vector3d` class into your codebase to handle 3-dimensional vector manipulations efficiently and accurately.
