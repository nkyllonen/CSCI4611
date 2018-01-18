# Assignment 4 : Spline Walking with MoCap Data
Nikki Kyllonen | kyllo089 | CSCI4611 | 4/5/2017

## Overview
3D animation of Motion Capture data, using a stick-like character.

## Implementation
- `void Bone::draw()` : displays specific Bone object
	- in order to align the local z-axis with the vector along the length of the bone:
		1. normalize the bone vector
		2. determine the arbitrary axis of rotation using `glm::cross(z, b)`
		3. determine the amount of rotation by taking the dot product of `b` and `z`
- `vec3 Spline3::getValue(float t)` : returns interpolated spline value at time `t`
	- uses utility functions written in `util.hpp` for clarity and organization
- `vec3 Spline3::getDerivative(float t)` : returns interpolated spline derivative value at time `t`
	- uses utility functions written in `util.hpp` for clarity and organization
- `void drawGraphics()` : displays all graphical items to the window
	- uses same algorithm as specified in `Bone::draw()` to align the `character` with the current velocity vector by aligning the `character`'s z-axis with the velocity

## Included Files
`amcutil.hpp` | `camera.hpp` | `character.hpp` | `character_impl.hpp` | `config.hpp` | `draw.hpp` | `engine.hpp` | `grahics.hpp` | `main.cpp` | `reader.hpp` | `README.md` | `README.pdf` | `spline.hpp` | `util.hpp`