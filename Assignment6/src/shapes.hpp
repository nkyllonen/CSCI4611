#ifndef SHAPES_HPP
#define SHAPES_HPP

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <vector>


#include <Box2D/Box2D.h>

using namespace std;
using glm::vec2;

// TODO: Modify these classes to keep track of the corresponding Box2D
// bodies. Feel free to add helpful methods like
//     mat4 getTransformation()
// and
//     vec2 getLocalPoint(vec2 worldPoint).

class Circle {
private:
	b2Body *body;
	bool isDynamic;

public:
    vec2 center;
    float radius;

    Circle() {}
    Circle(vec2 center, float radius, bool dynamic, b2World* world) {
		this->center = center;
		this->radius = radius;
		isDynamic = dynamic;

		b2BodyDef bodyDef;
		b2FixtureDef fixtureDef;

		if (isDynamic)
		{
			bodyDef.type = b2_dynamicBody;
		}
		bodyDef.position.Set(center.x, center.y);
		body = world->CreateBody(&bodyDef);

		//define shape parameters
		b2CircleShape shape;
		shape.m_radius = this->radius;

		if (isDynamic)
		{
			//define fixture
			fixtureDef.shape = &shape;
			fixtureDef.density = .2;
			fixtureDef.friction = .4;
			fixtureDef.restitution = .4;

			//create fixture to body
			body->CreateFixture(&fixtureDef);
		}
		else
		{
			//if static,  can pass the shape directly to
			//the body without creating a fixture definition
			body->CreateFixture(&shape, 0.0f);
		}
    }
	/*-------------------------------------------
		contains : determines if the given
					worldPoint lies within the
					bounds of the shape
	-------------------------------------------*/
    bool contains(vec2 worldPoint) {
		//transform worldPoint to local space
		b2Vec2 localPoint = body->GetLocalPoint(b2Vec2(worldPoint.x, worldPoint.y));
		vec2 v2LP = vec2(localPoint.x, localPoint.y);

        return glm::length(v2LP) <= radius;
    }
    void destroy(b2World* world)
	{
		world->DestroyBody(body);
	}
	/*-------------------------------------------
		getPosition : returns position of body
	-------------------------------------------*/
	vec2 getPosition()
	{
		b2Vec2 pos = body->GetPosition();
		return vec2(pos.x, pos.y);
	}
	/*-------------------------------------------
		getRotation : returns angle of rotation
						of body
	-------------------------------------------*/
	float getRotation()
	{
		float angle = body->GetAngle();
		return angle;
	}
	/*-------------------------------------------
		getBody : returns ptr to body
	-------------------------------------------*/
	b2Body* getBody()
	{
		return body;
	}
};

class Box {
private:
	b2Body *body;
	bool isDynamic;

public:
    vec2 center;
    vec2 size;

    Box() {}
    Box(vec2 center, vec2 size, bool dynamic, b2World* world) {
        this->center = center;
        this->size = size;
		isDynamic = dynamic;

		b2BodyDef bodyDef;
		b2FixtureDef fixtureDef;

		if (isDynamic)
		{
			bodyDef.type = b2_dynamicBody;
		}
		bodyDef.position.Set(center.x, center.y);
		body = world->CreateBody(&bodyDef);

		//define shape parameters
		b2PolygonShape shape;
		shape.SetAsBox(size.x / 2.0, size.y / 2.0);

		if (isDynamic)
		{
			//define fixture
			fixtureDef.shape = &shape;
			fixtureDef.density = .2;
			fixtureDef.friction = .4;
			fixtureDef.restitution = .4;

			//create fixture to body
			body->CreateFixture(&fixtureDef);
		}
		else
		{
			//if static,  can pass the shape directly to
			//the body without creating a fixture definition
			body->CreateFixture(&shape, 0.0f);
		}
    }
	/*-------------------------------------------
		contains : determines if the given
					worldPoint lies within the
					bounds of the shape
	-------------------------------------------*/
    bool contains(vec2 worldPoint) {
		//transform worldPoint to local space
		b2Vec2 localPoint = body->GetLocalPoint(b2Vec2(worldPoint.x, worldPoint.y));
		vec2 v2LP = vec2(localPoint.x, localPoint.y);

        return (abs(v2LP.x) <= size.x/2 && abs(v2LP.y) <= size.y/2);
    }
	void destroy(b2World* world)
	{
		world->DestroyBody(body);
	}
	/*-------------------------------------------
		getPosition : returns position of body
	-------------------------------------------*/
	vec2 getPosition()
	{
		b2Vec2 pos = body->GetPosition();
		return vec2(pos.x, pos.y);
	}
	/*-------------------------------------------
		getRotation : returns angle of rotation
						of body
	-------------------------------------------*/
	float getRotation()
	{
		float angle = body->GetAngle();
		return angle;
	}
	/*-------------------------------------------
		getBody : returns ptr to body
	-------------------------------------------*/
	b2Body* getBody()
	{
		return body;
	}
};

class Polyline {
private:
	b2BodyDef bodyDef;
	b2Body *body;

public:
    vector<vec2> vertices;
    Polyline() {}
    Polyline(vector<vec2> vertices, bool loop, b2World* world) {
        this->vertices = vertices;
		int s = vertices.size();

		//create chain shape
		vector<b2Vec2> verts;
		for (int i = 0; i < s; i++)
		{
			verts.push_back(b2Vec2(vertices[i].x, vertices[i].y));
		}

		b2ChainShape chain;

		if (loop)
		{
			chain.CreateLoop(&verts[0], verts.size());
		}
		else
		{
			chain.CreateChain(&verts[0], verts.size());
		}

		// bodyDef.position.Set(0,0);
		body = world->CreateBody(&bodyDef);
		body->CreateFixture(&chain, 0.0f);
    }
	void destroy(b2World* world)
	{
		world->DestroyBody(body);
	}
	/*-------------------------------------------
		getBody : returns ptr to body
	-------------------------------------------*/
	b2Body* getBody()
	{
		return body;
	}
};

#endif
