#version 150

//uniform parameters are all set in the program by calling program.setUniform()
uniform mat4 modelViewMatrix;
uniform mat4 normalMatrix;
uniform mat4 projectionMatrix;
uniform vec4 lightInEyeSpace;

// Lighting
uniform vec4 Ia;
uniform vec4 Id;
uniform vec4 Is;

// Material parameters
uniform vec4 ka;
uniform vec4 kd;
uniform vec4 ks;
uniform float s;

//textures
uniform sampler2D diffuseRamp;
uniform sampler2D specularRamp;

out vec4 color;
in vec3 position;				//vertex transformed into eye space
in vec3 norm;					//normal from vert shader

void main() {
    // We'll start with black, then add various lighting terms to it
    // as we calculate them.
    vec3 finalColor = vec3(0.0, 0.0, 0.0);

    // TODO: Calculate ambient, diffuse, and specular lighting for this pixel
    // based on its position, normal, etc.

	vec3 n = normalize(norm);
	vec3 l = normalize(lightInEyeSpace.xyz - position);
	vec3 e = normalize(-position);
	vec3 h = normalize(l + e);									//halfway vector

	finalColor = vec3(0.0, 0.0, 0.0);
	finalColor += (ka*Ia).xyz;									//ambient

	//not using textures
	//finalColor += (kd*Id).xyz*max(dot(n,l), 0.0);				//diffuse
	//finalColor += (ks*Is).xyz*pow(max(dot(n, h), 0.0), s);	//specular

	vec3 diffTex = (texture(diffuseRamp, vec2(0.5*dot(n,l) + 0.5, 0))).xyz;
	vec3 specTex = (texture(specularRamp, vec2(pow(max(dot(n, h), 0.0), s), 0))).xyz;

	//using textures
	finalColor += kd.xyz*Id.xyz*diffTex;
	finalColor += ks.xyz*Is.xyz*specTex;

    // Use the R,G,B components of finalColor for the output color of this
    // fragment, and set the alpha component to 1.0 (completely opaque).
    color.rgb = finalColor;
    color.a = 1.0;

}
