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

- `Header Includes`: Includes necessary header files such as <windows.h>, <GL/glut.h>, and custom headers "Agent.h" and "Environment.h".
- `Global Variables`: Declares global variables like agDebug, theta, rotOtherWay, and gEnv.
- `Agent Class`: Defines an Agent class with member variables and functions to represent and manipulate agents. The class includes constructors, initialization functions, methods for updating agent state, drawing agents, handling controls, calculating forces, resolving collisions, and more.
- `Drawing Functions`: Defines functions to draw various shapes like circles, triangles, fish bodies, tails, and fins using OpenGL commands.
- `Collision Resolution`: Implements a method ResolveCollisionWithOtherAgents to handle collisions between agents. 

### Cell

The `Cell` is designed to represent cells in an environment grid. The Cell class provides functionality for initializing cell properties, drawing cells, checking for collisions with points, and handling collisions with cell edges.

#### Features

- Constructor for initializing cell properties such as indices, dimensions, center point, and blocking status.
- Method to draw the cell visually in a graphical simulation environment.
- Method to retrieve the center of the cell as a Vector3d object.
- Method to check for collisions between a point and the cell.
- Method to handle collisions with the edges of the cell, including flipping coordinates and determining collision conditions.

#### Usage 

- Integrate the Cell class into your project structure.
- Include the necessary dependencies, including the Point and Vector3d classes.
- Customize the Cell class behavior according to your project requirements, adjusting methods and properties as needed.
- Implement functionality to interact with cells, such as checking collisions and updating cell properties based on the environment.
- Test the integrated code thoroughly to ensure it behaves as expected, verifying cell drawing, collision detection, and edge handling.
- Document the code and its usage for future reference and maintenance.

### Environment

### MyColor

### Point

### Simulator

### Vecot3d
