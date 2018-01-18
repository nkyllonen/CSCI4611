# Assignment 3 : Earthquake Visualization
Nikki Kyllonen | kyllo089 | CSCI4611 | 3/20/2017

## Overview
3D visualization of earthquake data gathered by USGS between 1905 and 2007.

## Controls
- `LEFT` and `RIGHT` arrow keys : control speed of chronological iteration through data
- `m` : turns mesh display on or off
- `s` : turns spherical view on or off

## Implementation
- `void Earth::populateVNTArrays()` : fill vertices, normals, and texCoords arrays
	- starts at upper left corner, moving left to right then down the mesh, ending at the bottom right vertex
- `void Earth::populateIArray()` : fill indices array
	- steps down each row/stack, adding the indices for the bottom triangle and then the top triangle of each cell of the mesh
- `void Earth::setSpherical(float s)` : update V and N buffers to display mesh as a sphere
	- empties then repopulates vertices, normals, and texCoords arrays
	- repopulates with new values (only V and N actually change)
	- copies new values into corresponding buffers
- `vec3 Earth::getPosition(float latitude, float longitude)` : returns vec3 representing 3D	position at	corresponding lat and long
	- uses `tWidth` and `tHeight` values (width and height of displayed mesh)
	- linearly maps `latitude` and `longitude` values to 3D positions
- `vec3 Earth::getNormal(float latitude, float longitude)` : returns normal vec3 corresponding to given latitude and longitude
	- determines `sphericalNormal` by computing a normalized radial vector
- `vec2 Earth::getTCoord(float latitude, float longitude)` : returns texCoord corresponding to given latitude and longitude
	- linearly maps `longitude` and `latitude` to `tCoord.x` and `tCoord.y` respectively
- `QuakeVis::drawGraphics()` : displays all graphical elements of visualization
	- uses the range of magnitude values (`diff`) to determine an alpha value to color each data point and the size of the sphere to be displayed
	- determines alpha and radius using a linear interpolation
	- transparency is enabled using:
		```
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		```
- `Util::lerp(float x, float y, float a)` : returns linear interpolation between x and y by an amount a
- `Util::float getV3Magnitude(vec3 v)` : returns magnitude of vec3

## Included Files
`camera.hpp` | `config.h` | `draw.hpp` | `earth.hpp` | `engine.hpp` | `grahics.hpp` | `main.cpp` | `quake.hpp` | `README.md` | `README.pdf` | `text.hpp` | `util.h`