#version 150

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;

in vec2 vertex;

void main() {

    gl_Position = projectionMatrix * modelViewMatrix * vec4(vertex,0,1);

}
