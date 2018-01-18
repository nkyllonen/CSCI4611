# Assignment 6 : Pencil Physics
Nikki Kyllonen | kyllo089 | CSCI4611 | 5/1/2017

## Overview
Using the Box2D physics engine to implement an interactive, physically accurate 2D game.

## Implementation
- `shapes.hpp` : header file containing definitions for the three shapes available to be drawn in the game
	- Each constructor defines the shape's `b2Body` with the appropriate parameters
	- `bool contains(vec2 worldPoint)` : determines if `worldPoint` lies within the bounds of the shape by converting `worldPoint` to local coordinates using `GetLocalPoint()`
- `draw.hpp` : header file in charge of displaying meshes to the window
	- I adapted the `circle(...)` and `box(...)` methods to take in and `angle` measurement in place of a `mat4` transformation matrix. Both functions now apply the appropriate rotations and translations as obtained by the `b2Body` objects attached to each `Circle` and `Box` object.

## Included Files
`camera.hpp` | `config.hpp` | `constant2d.frag` | `constant2d.vert` | `draw.hpp` | `engine.hpp` | `grahics.hpp` | `main.cpp` | `mesh.hpp` | `README.md` | `README.pdf` | `shader.hpp` | `shapes.hpp` | `uihelper.hpp`