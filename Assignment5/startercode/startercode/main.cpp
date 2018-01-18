#include "engine.hpp"
#include "camera.hpp"
#include "config.hpp"
#include "draw.hpp"
#include "mesh.hpp"
#include "shader.hpp"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <vector>
using namespace std;
using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;

class MyApp : public Engine {
public:

	SDL_Window *window;
	OrbitCamera camera;
	vec3 lightPosition;

	Mesh mesh;
	EdgeMesh edgeMesh;
	ShaderProgram phongProgram;
	Texture diffuseRamp, specularRamp;
	ShaderProgram silhouetteProgram;

	MyApp() {
		window = createWindow("4611", 1280, 720);
		camera = OrbitCamera(2.5, 0, 0, Perspective(30, 16 / 9., 1, 20));
		// Put the light in a nice position in camera space.
		// Press L to reset it to the camera position. This way you can
		// control the light source position semi-interactively.
		lightPosition = 3.0*glm::normalize(vec3(1, 1, 1));
		// Load the triangle mesh, and create an EdgeMesh for rendering
		// its silhouette edges.
		mesh.loadOBJ(Config::mesh);
		mesh.createGPUData(this);
		edgeMesh.fromMesh(mesh);
		edgeMesh.createGPUData(this);
		// Load the diffuse and specular ramps. We set the texture wrap mode
		// to "clamp" to prevent texels from the leftmost column from being
		// blended with those from the rightmost column.
		diffuseRamp = loadTexture(Config::diffuseRamp);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		specularRamp = loadTexture(Config::specularRamp);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		// Load the vertex and fragment shaders. Since you don't need to
		// recompile the C++ program to reload shaders, you can even do this
		// interactively as you debug your shaders! Press 'R' to reload the
		// shaders while the program is running.
		reloadShaders();
	}

	~MyApp() {
		SDL_DestroyWindow(window);
	}

	void reloadShaders() {
		phongProgram = ShaderProgram(Config::phongVert, Config::phongFrag);
		silhouetteProgram = ShaderProgram(Config::silhouetteVert, Config::silhouetteFrag);
	}

	void run() {
		float fps = 60, dt = 1 / fps;
		while (!shouldQuit()) {
			handleInput();
			drawGraphics();
			waitForNextFrame(dt);
		}
	}

	void onMouseMotion(SDL_MouseMotionEvent &e) {
		camera.onMouseMotion(e);
	}

	void onKeyDown(SDL_KeyboardEvent &e) {
		if (e.keysym.scancode == SDL_SCANCODE_L)
			lightPosition = camera.getEye();
		if (e.keysym.scancode == SDL_SCANCODE_R)
			reloadShaders();
	}

	void drawGraphics() {
		// Black background
		glClearColor(1, 1, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Line offset
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1, 1);
		// Apply camera transformation
		camera.apply(this);

		// Light position in eye space. We should computed this here because
		// the modelview matrix may be different when we're applying the shader.
		const vec4 lightInViewSpace = getMatrix(GL_MODELVIEW)*vec4(lightPosition, 1);

		// Draw mesh
		phongProgram.enable();
		phongProgram.setUniform("modelViewMatrix", getMatrix(GL_MODELVIEW));
		phongProgram.setUniform("normalMatrix", glm::inverse(glm::transpose(getMatrix(GL_MODELVIEW))));
		phongProgram.setUniform("projectionMatrix", getMatrix(GL_PROJECTION));
		// TODO: Pass the relevant parameters from Config into your shader
		// using uniform variables.
		phongProgram.setUniform("Ia", Config::Ia);
		phongProgram.setUniform("Id", Config::Id);
		phongProgram.setUniform("Is", Config::Is);
		phongProgram.setUniform("ka", Config::ka);
		phongProgram.setUniform("kd", Config::kd);
		phongProgram.setUniform("ks", Config::ks);
		phongProgram.setUniform("s", Config::s);

		phongProgram.setUniform("lightInEyeSpace", lightInViewSpace);

		//load in textures
		phongProgram.setTexture("diffuseRamp", diffuseRamp, 0);
		phongProgram.setTexture("specularRamp", specularRamp, 1);

		phongProgram.setAttribute("vertex", mesh.vertexBuffer, 3, GL_FLOAT);
		phongProgram.setAttribute("normal", mesh.normalBuffer, 3, GL_FLOAT);
		drawElements(GL_TRIANGLES, mesh.indexBuffer, mesh.triangles.size() * 3);
		phongProgram.disable();

		// Draw edge mesh
		silhouetteProgram.enable();
		silhouetteProgram.setUniform("modelViewMatrix", getMatrix(GL_MODELVIEW));
		silhouetteProgram.setUniform("normalMatrix", glm::inverse(glm::transpose(getMatrix(GL_MODELVIEW))));
		silhouetteProgram.setUniform("projectionMatrix", getMatrix(GL_PROJECTION));
		silhouetteProgram.setUniform("thickness", Config::thickness);
		silhouetteProgram.setAttribute("vertex", edgeMesh.vertexBuffer, 3, GL_FLOAT);
		silhouetteProgram.setAttribute("direction", edgeMesh.directionBuffer, 3, GL_FLOAT);
		silhouetteProgram.setAttribute("leftNormal", edgeMesh.leftNormalBuffer, 3, GL_FLOAT);
		silhouetteProgram.setAttribute("rightNormal", edgeMesh.rightNormalBuffer, 3, GL_FLOAT);
		drawElements(GL_TRIANGLES, edgeMesh.indexBuffer, edgeMesh.triangles.size() * 3);
		silhouetteProgram.disable();

		// Finish
		SDL_GL_SwapWindow(window);
	}

};

int main(int argc, char **argv) {
	MyApp app;
	app.run();
	return EXIT_SUCCESS;
}
