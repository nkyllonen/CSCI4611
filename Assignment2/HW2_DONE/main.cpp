#include "engine.hpp"
#include "draw.hpp"
#include <glm/glm.hpp>
#include <string.h>
using namespace std;
using namespace Draw;
using glm::vec2;
using glm::vec3;
using glm::vec4;
////////////////////////////////////////////////////////////////
class Car {
public:
    vec3 position;		//meters
    vec3 velocity;		//meters/sec
	vec3 acceleration;	//meters/sec/sec
	vec3 shapeDim;		//meters : describes 3d dimensions of car

    float collisionRadius;
    float thrustFactor, dragFactor;
	/*-------------------------------------------------
		draw : calls functions to display body and
				shadow
	-------------------------------------------------*/
    void draw() {        
		drawBody(position);
		drawShadow(position);
    }
	/*-------------------------------------------------
		reset : sets car's member variables to
				initial values
	-------------------------------------------------*/
	void reset() {
		collisionRadius = 2.5;
		position = vec3(0, 1, 45); // center of car is 1 m above ground
		velocity = vec3(0, 0, 0);
		thrustFactor = 150;
		dragFactor = 5;
		acceleration = vec3(0, -9.8, 0);
		shapeDim = vec3(3, 2, 4);
	}
	/*-------------------------------------------------
		drawBody : draws body of the car with respect
					to the vector p
	-------------------------------------------------*/
	void drawBody(vec3 p) {
		glColor3f(0.8,0.2,0.2);
        //excluding front and back, wrap around sides CW
		glBegin(GL_QUAD_STRIP);
		//top
		glVertex3f(p.x - shapeDim.x / 2.0, p.y + shapeDim.y / 2.0, p.z + shapeDim.z / 2.0);
		glVertex3f(p.x - shapeDim.x / 2.0, p.y + shapeDim.y / 2.0, p.z - shapeDim.z / 2.0);
		glVertex3f(p.x + shapeDim.x / 2.0, p.y + shapeDim.y / 2.0, p.z + shapeDim.z / 2.0);
		glVertex3f(p.x + shapeDim.x / 2.0, p.y + shapeDim.y / 2.0, p.z - shapeDim.z / 2.0);
		//right side
		glVertex3f(p.x + shapeDim.x / 2.0, p.y - shapeDim.y / 2.0, p.z + shapeDim.z / 2.0);
		glVertex3f(p.x + shapeDim.x / 2.0, p.y - shapeDim.y / 2.0, p.z - shapeDim.z / 2.0);
		//bottom
		glVertex3f(p.x - shapeDim.x / 2.0, p.y - shapeDim.y / 2.0, p.z + shapeDim.z / 2.0);
		glVertex3f(p.x - shapeDim.x / 2.0, p.y - shapeDim.y / 2.0, p.z - shapeDim.z / 2.0);
		//left side
		glVertex3f(p.x - shapeDim.x / 2.0, p.y + shapeDim.y / 2.0, p.z + shapeDim.z / 2.0);
		glVertex3f(p.x - shapeDim.x / 2.0, p.y + shapeDim.y / 2.0, p.z - shapeDim.z / 2.0);
		glEnd();
		//draw front and back
		glBegin(GL_QUADS);
		//front
		glVertex3f(p.x - shapeDim.x / 2.0, p.y + shapeDim.y / 2.0, p.z + shapeDim.z / 2.0);
		glVertex3f(p.x + shapeDim.x / 2.0, p.y + shapeDim.y / 2.0, p.z + shapeDim.z / 2.0);
		glVertex3f(p.x + shapeDim.x / 2.0, p.y - shapeDim.y / 2.0, p.z + shapeDim.z / 2.0);
		glVertex3f(p.x - shapeDim.x / 2.0, p.y - shapeDim.y / 2.0, p.z + shapeDim.z / 2.0);
		//back
		glVertex3f(p.x - shapeDim.x / 2.0, p.y + shapeDim.y / 2.0, p.z - shapeDim.z / 2.0);
		glVertex3f(p.x + shapeDim.x / 2.0, p.y + shapeDim.y / 2.0, p.z - shapeDim.z / 2.0);
		glVertex3f(p.x + shapeDim.x / 2.0, p.y - shapeDim.y / 2.0, p.z - shapeDim.z / 2.0);
		glVertex3f(p.x - shapeDim.x / 2.0, p.y - shapeDim.y / 2.0, p.z - shapeDim.z / 2.0);
		glEnd();
	}
	/*-------------------------------------------------
		drawShadow : draws shadow beneath the car
						with respect to the vector p
	-------------------------------------------------*/
	void drawShadow(vec3 p) {
		glEnable(GL_BLEND);
		glColor4f(0.2, 0.2, 0.2, 0.5); //4f for the alpha channel
		glBegin(GL_POLYGON);
		glVertex3f(p.x - shapeDim.x / 2.0, 0.1, p.z + shapeDim.z / 2.0);
		glVertex3f(p.x - shapeDim.x / 2.0, 0.1, p.z - shapeDim.z / 2.0);
		glVertex3f(p.x + shapeDim.x / 2.0, 0.1, p.z - shapeDim.z / 2.0);
		glVertex3f(p.x + shapeDim.x / 2.0, 0.1, p.z + shapeDim.z / 2.0);
		glEnd();
	}
};
////////////////////////////////////////////////////////////////
class Ball {
public:
	vec3 position;
	vec3 velocity;
	vec3 acceleration;
	float collisionRadius;
	/*-------------------------------------------------
		draw : displays ball according to its position
	-------------------------------------------------*/
	void draw() {
		glColor3f(1, 1, 1);
		//draw the unitsphere at position vector
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glTranslatef((float) position.x, (float) position.y, (float) position.z);
		unitSphere();
		glPopMatrix();
		drawShadow();
	}
	/*-------------------------------------------------
		drawShadow : draws shadow beneath the ball
	-------------------------------------------------*/
	void drawShadow() {
		float rads = 0.0; //radian measurement
		float edgeX = 0.0;
		float edgeZ = 0.0;

		glEnable(GL_BLEND);
		glColor4f(0.2,0.2,0.2,0.5); //4f for the alpha channel
		glBegin(GL_POLYGON);

		while (rads < 2 * M_PI) //omit = 2pi since for polygons, each vertex is unique
		{
			edgeX = position.x + collisionRadius*cos(rads);
			edgeZ = position.z + collisionRadius*sin(rads);
			glVertex3f(edgeX, 0.1, edgeZ);
			rads += 0.25;
		}
		glEnd();
	}
	/*-------------------------------------------------
		reset : sets ball's member variables to
				initial values
	-------------------------------------------------*/
	void reset() {
		float a = ((rand() % 314)+1) / 100.0; //roughly random number between 0 and PI
		position = vec3(0, 20, 0); //center of ball is 20 m above ground
		velocity = vec3(25*cos(a), 10, 25*sin(a));
		acceleration = vec3(0, -9.8, 0);
		collisionRadius = 1.0;
	}
};
////////////////////////////////////////////////////////////////
class CarSoccer: public Engine {
public:
    SDL_Window *window;
    Car car;
	Ball ball;
	int blueGoals = 0;
	int yellowGoals = 0;
	/*-------------------------------------------------*/
    CarSoccer() {
        window = createWindow("Car Soccer", 1280, 720);
		car.reset();
		ball.reset();
    }
	/*-------------------------------------------------*/
    ~CarSoccer() {
        SDL_DestroyWindow(window);
    }
	/*-------------------------------------------------
		run : looping function that runs the game
	-------------------------------------------------*/
    void run() {
        float fps = 60, timeStep = 1/fps;
        while (!shouldQuit()) {
            Engine::handleInput();
            simulate(timeStep);
            drawGraphics();
            Engine::waitForNextFrame(timeStep);
        }
    }
	/*-------------------------------------------------*/
    void simulate(float timeStep) {
        // An oversimplified dynamics model for the car
        vec2 dir = getControlDirection();
        if (glm::length(dir) > 0)
            dir = glm::normalize(dir);
        vec3 thrust = car.thrustFactor*vec3(dir.x, 0, -dir.y);
        vec3 drag = car.dragFactor*car.velocity;

		//check car-to-field collision
		if ((car.position.y - 1) < 0) { //below the field
			car.position.y = 1;
			car.velocity = vec3(0, 0, 0);
		}
		else if ((car.position.y - 1) == 0) { //on the field
			//update car position
			car.velocity += (thrust - drag)*timeStep;
			car.position += car.velocity*timeStep;
		}
		else { //above the field
			car.velocity += car.acceleration*timeStep;
			car.position += car.velocity*timeStep;
		}

        // Handle ball/wall, car/wall, and ball/car collisions here
		//check ball-car collision
		vec3 diffVec = (car.position - ball.position);
		float diffLen = length(diffVec);
		float radiusSum = car.collisionRadius + ball.collisionRadius;
		vec3 relVel, cNorm;

		if (diffLen <= radiusSum) {
			//collision normal
			cNorm = (ball.position - car.position) / diffLen;

			//remove any overlap
			if (diffLen < radiusSum) {
				ball.position += cNorm*(radiusSum - diffLen);
			}

			//relative velocity
			relVel = ball.velocity - car.velocity;

			//reflect relVel about cNorm
			relVel = reflectVec(relVel, cNorm);
			ball.velocity = car.velocity + relVel;
		}

		//check ball-to-field collision
		if ((ball.position.y - ball.collisionRadius) <= 0) {
			ball.position.y = ball.collisionRadius + 0.1; //move it a bit more so that it doesn't get stuck
			ball.velocity = reflectVec(ball.velocity, vec3(0, 1, 0));
			ball.velocity = (ball.velocity * (float)0.8); //small reduction in velocity after bounce
		}
		else {
			//update ball position
			ball.velocity += ball.acceleration*timeStep;
			ball.position += ball.velocity*timeStep;
		}

		//check ball-to-wall collisions
		vec3 ballAtWall = atWall(ball.position, ball.collisionRadius);

		if (ballAtWall != vec3(0, 0, 0)) {
			//check if in a goal
			if (ballAtWall == vec3(0, 0, 1)) { //at front wall
				if (inGoal(-50)) {
					OutputDebugStringA("Entered blue goal\n");
					blueGoals++;
					ball.reset();
					car.reset();
				}
				//OutputDebugStringA("Hit front wall, but no goal\n");
			}
			else if (ballAtWall == vec3(0, 0, -1)) { //at back wall
				if (inGoal(50)) {
					OutputDebugStringA("Entered yellow goal\n");
					yellowGoals++;
					ball.reset();
					car.reset();
				}
				//OutputDebugStringA("Hit back wall, but no goal\n");
			}

			ball.velocity = reflectVec(ball.velocity, ballAtWall);
			ball.position = updatePosAfterWall(ball.position, ballAtWall, ball.collisionRadius); //determine new position
		}

		//check car-to-wall collisions
		vec3 carAtWall = atWall(car.position, car.collisionRadius);

		if (carAtWall != vec3(0, 0, 0)) {
			car.velocity = reflectVec(car.velocity, carAtWall);
			car.position = updatePosAfterWall(car.position, carAtWall, car.collisionRadius); //determine new position
		}
    }
	/*-------------------------------------------------*/
    vec2 getControlDirection() {
        vec2 dir(0,0);
        if (Engine::isKeyDown(SDL_SCANCODE_LEFT))
            dir.x--;
        if (Engine::isKeyDown(SDL_SCANCODE_RIGHT))
            dir.x++;
        if (Engine::isKeyDown(SDL_SCANCODE_UP))
            dir.y++;
        if (Engine::isKeyDown(SDL_SCANCODE_DOWN))
            dir.y--;
        return dir;
    }
	/*-------------------------------------------------*/
    void addLight(GLenum light, vec4 position, vec3 color) {
        glEnable(light);
        glLightfv(light, GL_POSITION, &position[0]);
        glLightfv(light, GL_DIFFUSE, &color[0]);
    }
	/*-------------------------------------------------
		drawGraphics : sets up graphics
	-------------------------------------------------*/
    void drawGraphics() {
        // Allow lines to show up on top of filled polygons
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(1,1);
        // Set up the camera in a good position to see the entire field
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(60, 16/9., 1,1000);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(0,60,70, 0,0,10, 0,1,0);
        // Clear window
        glClearColor(0.2,0.2,0.2, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Add some lights
        addLight(GL_LIGHT0, vec4(+1,0.5,+1,0), vec3(0.5,0.5,0.5));
        addLight(GL_LIGHT1, vec4(+1,0.5,-1,0), vec3(0.5,0.5,0.5));
        addLight(GL_LIGHT2, vec4(-1,0.5,-1,0), vec3(0.5,0.5,0.5));
        addLight(GL_LIGHT3, vec4(-1,0.5,+1,0), vec3(0.5,0.5,0.5));
        glEnable(GL_LIGHTING);
        // Draw the ground as a quadrilateral
        glColor3f(0.3,0.6,0.2);
        glBegin(GL_QUADS);
        glNormal3f(0,1,0);
        glVertex3f(-50,0,-60);
        glVertex3f(+50,0,-60);
        glVertex3f(+50,0,+60);
        glVertex3f(-50,0,+60);
        glEnd();
        // Draw the car
        car.draw();
        
        glDisable(GL_LIGHTING);
		//edge of field
		drawRect(100, 80, -40, 0, -50);
		//draw center line
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);
		glVertex3f(-40, 0, 0);
		glVertex3f(40, 0, 0);
		glEnd();
		//draw penalty boxes
		drawRect(15, 40, -20, 0, -50);
		drawRect(15, 40, -20, 0, 35);
		//draw center circle
		drawCircle(10, 0, 0);
		//draw walls
		drawRect(100, 80, -40, 35, -50);
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);
		glVertex3f(-40, 0, -50);
		glVertex3f(-40, 35, -50);
		glVertex3f(40, 0, -50);
		glVertex3f(40, 35, -50);
		glVertex3f(40, 0, 50);
		glVertex3f(40, 35, 50);
		glVertex3f(-40, 0, 50);
		glVertex3f(-40, 35, 50);
		glEnd();

        glEnable(GL_LIGHTING);

		ball.draw();

		//draw goals
		drawGoal(-50, vec3(0, 0.5, 0.5));
		drawGoal(50, vec3(0.5, 0.5, 0));

        SDL_GL_SwapWindow(window);
    }
	/*-------------------------------------------------*/
    void onKeyDown(SDL_KeyboardEvent &e) {
        // Check whether the space bar was pressed, and if so, reset the ball
        // to the center of the pitch and give it a "kick-off" velocity.
        // We found that a nice initial velocity is (25 cos(a), 10, 25 sin(a))
        // where a is a random number between 0 and pi.
		if (Engine::isKeyDown(SDL_SCANCODE_SPACE)) {
			ball.reset();
		}
		if (Engine::isKeyDown(SDL_SCANCODE_W)) {
			car.position.y += 5;
		}
		if (Engine::isKeyDown(SDL_SCANCODE_S)) {
			car.position.y -= 5;
		}
    }
	/*-------------------------------------------------
		drawRect : draws white rectangle of height h and
					width w, with top left coord at
					(topLeftX, topLeftZ)
	-------------------------------------------------*/
	void drawRect(float h, float w, float topLeftX, float topLeftY, float topLeftZ) {
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINE_STRIP); //draws N-1 connected line segments
		//drawing clockwise
		glVertex3f(topLeftX, topLeftY, topLeftZ);
		glVertex3f(topLeftX + w, topLeftY, topLeftZ);
		glVertex3f(topLeftX + w, topLeftY, topLeftZ + h);
		glVertex3f(topLeftX, topLeftY, topLeftZ + h);
		glVertex3f(topLeftX, topLeftY, topLeftZ);
		glEnd();
	}
	/*-------------------------------------------------
		drawCircle : draws white circle of radius r
						centered at (centerX, centerZ)
	-------------------------------------------------*/
	void drawCircle(float r, float centerX, float centerZ) {
		float rads = 0.0; //radian measurement
		float edgeX = 0.0;
		float edgeZ = 0.0;

		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINE_STRIP); //draws N-1 connected line segments
		//drawing clockwise
		while (rads <= 2*M_PI)
		{
			edgeX = r*cos(rads);
			edgeZ = r*sin(rads);
			glVertex3f(edgeX, 0, edgeZ);
			rads += 0.25;
		}
		glEnd();
	}
	/*-------------------------------------------------
		atWall : returns normal of wall object is
					in contact with
					returns zero if no wall
	-------------------------------------------------*/
	vec3 atWall(vec3 pos, int radius) {
		//left wall (x = -40)
		if ((pos.x - radius) <= -40) {
			return vec3(1, 0, 0);
		}
		//right wall (x = 40)
		if ((pos.x + radius) >= 40) {
			return vec3(-1, 0, 0);
		}
		//front wall (z = -50)
		if ((pos.z - radius) <= -50) {
			return vec3(0, 0, 1);
		}
		//back wall (z = 50)
		if ((pos.z + radius) >= 50) {
			return vec3(0, 0, -1);
		}
		//ceiling (y = 35)
		if ((pos.z - radius) <= -50) {
			return vec3(0, 0, 1);
		}
		else {
			return vec3(0, 0, 0);
		}
	}
	/*-------------------------------------------------
		reflectVec : returns v reflected off of a
						surface with normal n
	-------------------------------------------------*/
	vec3 reflectVec(vec3 v, vec3 n) {
		return (v - n*(float)(2.0 * dot(v, n))); //reflection = d - 2(d dot n)*n
	}
	/*-------------------------------------------------
		updatePosAfterWall : returns new, off-of-wall
								vec3 pos
	-------------------------------------------------*/
	vec3 updatePosAfterWall(vec3 pos, vec3 wNorm, int radius) {

		if (wNorm.x == 1) { //left wall
			pos.x = (-40 + (radius + 0.1));
		}
		else if (wNorm.x == -1) { //right wall
			pos.x = (40 - (radius + 0.1));
		}
		else if (wNorm.z == 1) { //front wall
			pos.z = (-50 + (radius + 0.1));
		}
		else if (wNorm.z == -1) { //back wall
			pos.z = (50 - (radius + 0.1));
		}

		return pos;
	}
	/*-------------------------------------------------
		drawGoal : draws a goal in the xy plane at z
					using given color
	-------------------------------------------------*/
	void drawGoal(float z, vec3 color) {
		glEnable(GL_BLEND);
		glColor4f(color.x, color.y, color.z, 0.5); //4f for the alpha channel

		glBegin(GL_LINES);
		for (int i = 0; i <= 20; i++) //verticle lines
		{
			glVertex3f(-10 + i, 10, z);
			glVertex3f(-10 + i, 0, z);
		}
		for (int i = 0; i <= 10; i++) //horizontal lines
		{
			glVertex3f(-10, 10 - i, z);
			glVertex3f(10, 10 - i, z);
		}
		glEnd();
	}
	/*-------------------------------------------------
		inGoal : check to see if ball is in a goal at z
	-------------------------------------------------*/
	bool inGoal(float z) {
		float bx = abs(ball.position.x) + ball.collisionRadius;
		float by = ball.position.y + ball.collisionRadius;
		float bz = abs(ball.position.z) + ball.collisionRadius;
		float zDiff = abs(z) - bz;
		if ((abs(zDiff) <= ball.collisionRadius) && (bx <= 10) && (by <= 10)) {
			return true;
		}
		else {
			return false;
		}
	}
};
////////////////////////////////////////////////////////////////
int main(int argc, char *argv[]) {
    CarSoccer game;
    game.run();
    return 0;
}