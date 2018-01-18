#include "engine.hpp"
#include "camera.hpp"
#include "config.hpp"
#include "draw.hpp"
#include "earth.hpp"
#include "quake.hpp"
#include "text.hpp"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iomanip>
#include "util.h"
using namespace std;
using glm::vec3;
using glm::vec4;

const int slices = 12;
const int stacks = 6;

class QuakeVis: public Engine {
public:

    SDL_Window *window;
    OrbitCamera camera;

    Earth earth;
    bool visualizeMesh;
    EarthquakeDatabase qdb;

    float currentTime;
    bool playing;
    float playSpeed;

    Text text;

    QuakeVis() {
        window = createWindow("Earthquake Visualization", 1280, 720);
        camera = OrbitCamera(5, 0, 0, Perspective(40, 16/9., 0.1, 10));
        float isSpherical = 1;
		float targetSph = 1; //for interpolating
        earth.initialize(this, slices, stacks, isSpherical);
        visualizeMesh = false;
        qdb = EarthquakeDatabase(Config::quakeFile);
		if (!qdb.fileFound){
			errorMessage(("Failed to open earthquake file " + Config::quakeFile).c_str());
			exit(EXIT_FAILURE);
		}
        playSpeed = 30*24*3600;
        currentTime = qdb.getByIndex(qdb.getMinIndex()).getDate().asSeconds();
        playing = true;
        text.initialize();
    }

    ~QuakeVis() {
        SDL_DestroyWindow(window);
    }

    void run() {
        float fps = 60, dt = 1/fps;
        while (!shouldQuit()) {
            handleInput();
            advanceState(dt);
            drawGraphics();
            waitForNextFrame(dt);
        }
    }

    void advanceState(float dt) {
        if (playing) {
            currentTime += playSpeed * dt;
            float minTime = qdb.getByIndex(qdb.getMinIndex()).getDate().asSeconds(),
                  maxTime = qdb.getByIndex(qdb.getMaxIndex()).getDate().asSeconds();
            if (currentTime > maxTime)
                currentTime = minTime;
            if (currentTime < minTime)
                currentTime = maxTime;
        }

        // TODO: Adjust the Earth's isSpherical value if necessary.

    }

    void addLight(GLenum light, vec4 position, vec3 color) {
        glEnable(light);
        glLightfv(light, GL_POSITION, &position[0]);
        glLightfv(light, GL_DIFFUSE, &color[0]);
    }

    void drawGraphics() {
        // Black background
        glClearColor(0,0,0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Allow lines to show up on top of filled polygons
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(1,1);
        // Light at camera origin
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glEnable(GL_LIGHTING);
        addLight(GL_LIGHT0, vec4(0,0,0,1), vec3(0.8,0.8,0.8));
        // Apply camera transformation
        camera.apply();
        // Draw earth
        if (visualizeMesh) {
            glColor3f(1,1,1);
            earth.draw(false);
			glEnable(GL_POLYGON_OFFSET_FILL);
			glPolygonOffset(0, 0);
            glColor3f(0.5,0.5,1);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            earth.draw(false);
			glEnable(GL_POLYGON_OFFSET_FILL);
			glPolygonOffset(1, 1);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        } else {
            glColor3f(1,1,1);
            earth.draw(true);
        }
        // Draw quakes
        int start = qdb.getIndexByDate(Date(currentTime - Config::timeWindow));
        int end = qdb.getIndexByDate(Date(currentTime));
		float mag = 1, a = 0, r = 0;
		float diff = qdb.getMaxMag() - qdb.getMinMag();
		vec3 qPos;

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        for (int i = start; i <= end; i++) {
            Earthquake e = qdb.getByIndex(i);

            // TODO: Draw an earthquake
			qPos = earth.getPosition(e.getLatitude(), e.getLongitude());
			mag = e.getMagnitude();

			//determine alpha by lerp-ing between 0-0.2
			a = Util::lerp(0, 0.7, (mag / diff));
			glColor4f(1, 0, 0, a);
			
			//determine radius by lerp-ing between 0-0.06
			r = Util::lerp(0, 0.04, (mag / diff));
			Draw::sphere(qPos, r);
        }

        // Draw current date
        Date d(currentTime);
        stringstream s;
        s << std::right << std::setfill('0')
          << "Current date: " << std::setw(2) << d.getMonth()
          << "/" << std::setw(2) << d.getDay()
          << "/" << d.getYear()
          << "  " << std::setw(2) << d.getHour()
          << ":" << std::setw(2) << d.getMinute();
        glDisable(GL_LIGHTING);
        glColor3f(1,1,1);
        text.draw(s.str(), -0.9,0.9);
        SDL_GL_SwapWindow(window);
    }

    void onMouseMotion(SDL_MouseMotionEvent &e) {
        camera.onMouseMotion(e);
    }

    void onKeyDown(SDL_KeyboardEvent &e) {
        if (e.keysym.scancode == SDL_SCANCODE_LEFT)
            playSpeed /= 1.4;
        if (e.keysym.scancode == SDL_SCANCODE_RIGHT)
            playSpeed *= 1.4;
        if (e.keysym.scancode == SDL_SCANCODE_SPACE)
            playing = !playing;
        if (e.keysym.scancode == SDL_SCANCODE_M)
            visualizeMesh = !visualizeMesh;

        // TODO: Switch between rectangle and sphere on pressing S
		if (e.keysym.scancode == SDL_SCANCODE_S)
			if (earth.isSpherical() == 1) {
				earth.setSpherical(0);
			}
			else {
				earth.setSpherical(1);
			}
    }
};

int main(int argc, char **argv) {
    QuakeVis app;
    app.run();
    return EXIT_SUCCESS;
}
