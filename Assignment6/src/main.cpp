#include "engine.hpp"
#include "camera.hpp"
#include "config.hpp"
#include "draw.hpp"
#include "mesh.hpp"
#include "shapes.hpp"
#include "uihelper.hpp"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <vector>

#include <Box2D/Box2D.h>

using namespace std;
using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;

class PencilPhysics: public Engine, UIMain {
public:

    SDL_Window *window;
    Camera2D camera;
    UIHelper uiHelper;
    Draw draw;

    Polyline walls;
    Circle redCircle;
    Box whiteBox;
    vector<Circle> circles;
    vector<Box> boxes;
    vector<Polyline> polylines;

    vec2 worldMin, worldMax;

	//Box2D variables
	b2World *world;
	b2MouseJoint* mouseJoint;

    PencilPhysics() {
        worldMin = vec2(-8, 0);
        worldMax = vec2(8, 9);
        window = createWindow("4611", 1280, 720);
        camera = Camera2D(worldMin, worldMax);
        uiHelper = UIHelper(this, worldMin, worldMax, 1280, 720);
        draw = Draw(this);
        // Initialize world
        initWorld();
    }

    ~PencilPhysics() {
        SDL_DestroyWindow(window);
    }

    void initWorld() {

        // TODO: Create a Box2D world and make these shapes static
        // bodies in it.
		world = new b2World(b2Vec2(0, -9.8f));

        // Create walls
        vector<vec2> wallVerts;
        wallVerts.push_back(vec2(worldMin.x, worldMax.y));
        wallVerts.push_back(vec2(worldMin.x, worldMin.y));
        wallVerts.push_back(vec2(worldMax.x, worldMin.y));
        wallVerts.push_back(vec2(worldMax.x, worldMax.y));
        walls = Polyline(wallVerts, true, world);	// (loop = true)

        // Create two static bodies
		//construct static shapes (dynamic = false)
        redCircle = Circle(vec2(-5,2), 0.5, false, world);
        whiteBox = Box(vec2(5,2), vec2(0.9,0.9), false, world);
    }

    void run() {
        float fps = 60, dt = 1.0/fps;
        while (!shouldQuit()) {
            handleInput();
            advanceState(dt);
            drawGraphics();
            waitForNextFrame(dt);
        }
    }

    vec2 randomVec2() {
        return vec2(2.*rand()/RAND_MAX-1, 2.*rand()/RAND_MAX-1);
    }

    void addCircle() {
        vec2 position = vec2(-5,7) + 0.5*randomVec2();
		Circle temp = Circle(position, 0.5, true, world); //(dynamic = true)
        circles.push_back(temp);
    }

    void addBox() {
        vec2 position = vec2(-5,7) + 0.5*randomVec2();
		Box temp = Box(position, vec2(1.2, 0.6), true, world); //(dynamic = true)
        boxes.push_back(temp);
    }

    void addPolyline(vector<vec2> vertices) {
		Polyline temp = Polyline(vertices, false, world); //(dynamic = false)
        polylines.push_back(temp);
    }

    void clear() {
        for (int i = 0; i < circles.size(); i++)
            circles[i].destroy(world);
        circles.clear();
        for (int i = 0; i < boxes.size(); i++)
            boxes[i].destroy(world);
        boxes.clear();
        for (int i = 0; i < polylines.size(); i++)
            polylines[i].destroy(world);
        polylines.clear();
    }

    void onKeyDown(SDL_KeyboardEvent &e) {
        uiHelper.onKeyDown(e);
    }
    void onKeyUp(SDL_KeyboardEvent &e) {
        uiHelper.onKeyUp(e);
    }
    void onMouseButtonDown(SDL_MouseButtonEvent &e) {
        uiHelper.onMouseButtonDown(e);
    }
    void onMouseButtonUp(SDL_MouseButtonEvent &e) {
        uiHelper.onMouseButtonUp(e);
    }
    void onMouseMotion(SDL_MouseMotionEvent &e) {
        uiHelper.onMouseMotion(e);
    }

    void advanceState(float dt) {
        // TODO: Step the Box2D world by dt.
		world->Step(dt, 8, 3);
    }

    void drawGraphics() {
        // Light gray background
        glClearColor(0.8,0.8,0.8, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST); // don't use z-buffer because 2D
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(1,1);
        // Apply camera transformation
        camera.apply(this);
        // Draw current polyline
        vector<vec2> currentPolyline = uiHelper.getPolyline();
        if (currentPolyline.size() >= 2)
            draw.polyline(mat4(), currentPolyline, vec3(0.6,0.6,0.6));

        // TODO: Modify these draw calls to draw the bodies with the
        // correct positions and angles.

        // Draw red circle and white box.
        draw.circle(0.0f, redCircle.center, redCircle.radius, vec3(1,0,0));
        draw.box(0.0f, whiteBox.center, whiteBox.size, vec3(1,1,1));

        // Draw all the other circles, boxes, and polylines
        for (int i = 0; i < circles.size(); i++)
			draw.circle(circles[i].getRotation(), circles[i].getPosition(), circles[i].radius, vec3(0, 0, 0));
        for (int i = 0; i < boxes.size(); i++)
			draw.box(boxes[i].getRotation(), boxes[i].getPosition(), boxes[i].size, vec3(0, 0, 0));
        for (int i = 0; i < polylines.size(); i++)
            draw.polyline(mat4(), polylines[i].vertices, vec3(0,0,0));

        // Finish
        SDL_GL_SwapWindow(window);
    }

    void attachMouse(vec2 worldPoint) {

        // TODO: Check if there is a circle or box that contains the
        // world point. If so, set mouseJoint to be a new b2MouseJoint
        // attached to the body with the point as target. The most
        // important parameters of b2MouseJointDef are bodyA (a static
        // Box2D body, e.g. the walls), bodyB (the chosen body being
        // moved) and target (the world-space point to pull bodyB
        // towards). Recommended values for other parameters you will
        // need to set are:
        //   collideConnected: true
        //   maxForce: 100
        //   frequencyHz: 2
        //   dampingRatio: 0.5
		bool foundContained = false;
		b2MouseJointDef mdef;

		//loop through circles and boxes to check for containment
		for (int i = 0; i < circles.size(); i++)
		{
			if (circles[i].contains(worldPoint))
			{
				foundContained = true;

				//initialize joint definition
				mdef.collideConnected = true;
				mdef.maxForce = 100;
				mdef.frequencyHz = 2;
				mdef.dampingRatio = 0.5;
				mdef.target = b2Vec2(worldPoint.x, worldPoint.y);
				mdef.bodyA = walls.getBody();
				mdef.bodyB = circles[i].getBody();

				mouseJoint = (b2MouseJoint*)world->CreateJoint(&mdef);

				return; //return because we're done! -- no need to look anymore
			}
		}
		
		if (foundContained == false) //just to be safe..
		{
			for (int i = 0; i < boxes.size(); i++)
			{
				if (boxes[i].contains(worldPoint))
				{
					foundContained = true;

					//initialize joint definition
					mdef.collideConnected = true;
					mdef.maxForce = 100;
					mdef.frequencyHz = 2;
					mdef.dampingRatio = 0.5;
					mdef.target = b2Vec2(worldPoint.x, worldPoint.y);
					mdef.bodyA = walls.getBody();
					mdef.bodyB = boxes[i].getBody();

					mouseJoint = (b2MouseJoint*)world->CreateJoint(&mdef);

					return; //return because we're done! -- no need to look anymore
				}
			}
		}
		
    }

    void moveMouse(vec2 worldPoint) {

        // TODO: If mouseJoint is not null, use SetTarget() to update
        // its target to the given point.

		if (mouseJoint != NULL)
		{
			mouseJoint->SetTarget(b2Vec2(worldPoint.x, worldPoint.y));
		}
    }

    void detachMouse() {

        // TODO: If mouseJoint is not null, destroy it and set it to
        // null.

		if (mouseJoint != NULL)
		{
			world->DestroyJoint(mouseJoint);
			mouseJoint = NULL;
		}
    }

};

int main(int argc, char **argv) {
    PencilPhysics physics;
    physics.run();
    return EXIT_SUCCESS;
}
