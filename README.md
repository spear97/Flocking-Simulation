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

- Header Includes: Includes necessary header files such as <windows.h>, <GL/glut.h>, and custom headers "Agent.h" and "Environment.h".
- Global Variables: Declares global variables like agDebug, theta, rotOtherWay, and gEnv.
- Agent Class: Defines an Agent class with member variables and functions to represent and manipulate agents. The class includes constructors, initialization functions, methods for updating agent state, drawing agents, handling controls, calculating forces, resolving collisions, and more.
- Drawing Functions: Defines functions to draw various shapes like circles, triangles, fish bodies, tails, and fins using OpenGL commands.
- Collision Resolution: Implements a method ResolveCollisionWithOtherAgents to handle collisions between agents.

### Cell

### Environment

### MyColor

### Point

### Simulator

### Vecot3d
