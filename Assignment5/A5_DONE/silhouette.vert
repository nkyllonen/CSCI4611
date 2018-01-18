#version 150

uniform mat4 modelViewMatrix;
uniform mat4 normalMatrix;
uniform mat4 projectionMatrix;
uniform float thickness;

in vec3 vertex;
in vec3 direction;
in vec3 leftNormal;
in vec3 rightNormal;

void main() {

    vec3 displacedVertex = vertex;

    // TODO: Compute the eye vector pointing towards the camera, and check
    // if dot(leftNormal, eye) and dot(rightNormal, eye) have opposite signs.
    // If so, displace the vertex by thickness*direction.
	vec3 vertexInEyeSpace = (modelViewMatrix*vec4(vertex,1)).xyz;
	vec3 eye = normalize(-vertexInEyeSpace);

	vec3 leftInEyeSpace = (normalMatrix*vec4(leftNormal,0)).xyz;
	vec3 rightInEyeSpace = (normalMatrix*vec4(rightNormal,0)).xyz;

	float left_dot = dot(leftInEyeSpace, eye);
	float right_dot = dot(rightInEyeSpace, eye);

	//float left_dot = dot(leftNormal, eye);
	//float right_dot = dot(rightNormal, eye);

	if ((left_dot*right_dot) < 0) //product of opposite signs is always negative!
	{
		displacedVertex += (thickness*direction);
	}

    gl_Position = projectionMatrix * modelViewMatrix * vec4(displacedVertex,1);

}
