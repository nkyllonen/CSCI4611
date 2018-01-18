# Assignment 5 : Artistic Rendering
Nikki Kyllonen | kyllo089 | CSCI4611 | 4/5/2017

## Overview
Using vertex and fragment shaders to implement cartoon-like artistic shading.

## Implementation
- `phong.vert` : vertex shader that implements Blinn-Phong shading
	- transforms vertex `position` and `normal` into eye space using the `modelViewMatrix` and `normalMatrix` respectively
- `phong.frag` : fragment shader that implements Blinn-Phong shading
	- performs per-fragment/pixel shading
	- uses half-vector `h` to calculate the specular component
	- uses temporary `vec3` variables, `diffTex` and `specTex` for clarity and readability while using the textures to determine the diffuse and specular components
- `silhouette.vert` : vertex shader that implements artistic silhouette outline
	- transforms `vertex` and normals, `leftNormal` and `rightNormal`, into eye space using the `modelViewMatrix` and `normalMatrix` respectively
	- determines if `vertex` is on the edge by calculating the product of the individual dot products of `leftInEyeSpace` and `rightInEyeSpace` with the `eye` vector
- `silhouette.frag` : fragment shader that implements artistic silhouette outline
	- sets the silhouette/outline to be black

## Included Files
`camera.hpp` | `config.hpp` | `draw.hpp` | `engine.hpp` | `grahics.hpp` | `main.cpp` | `mesh.hpp` | `phong.frag` | `phong.vert` | `README.md` | `README.pdf` | `shader.hpp` | `silhouette.frag` | `silhouette.vert`