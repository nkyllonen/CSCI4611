#version 150

//uniform parameters are all set in the program by calling program.setUniform()
uniform mat4 modelViewMatrix;
uniform mat4 normalMatrix;
uniform mat4 projectionMatrix;

in vec3 vertex;
in vec3 normal;

out vec3 position;	//position to be give to the frag shader
out vec3 norm;

void main() {

    // TODO: Transform the vertex position and normal to eye space
    // and pass them on to the fragment shader so that it can
    // calculate the lighting correctly.
	
	position = (modelViewMatrix * vec4(vertex,1)).xyz;
	norm = normalize((normalMatrix*vec4(normal,0)).xyz);

    gl_Position = projectionMatrix * modelViewMatrix * vec4(vertex,1);
}
