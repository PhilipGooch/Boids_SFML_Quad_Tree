#pragma once
#include "Input.h"
//#include "Vector2D.h"

#define WIDTH 1800
#define HEIGHT 880

#define PI 3.14159265359

using namespace sf;
using namespace std;
//using namespace Vector2D;

class Application
{
public:
	Application(RenderWindow* window);
	~Application();

public:
	void run();
	

private:
	void handleInput();
	void update(float delta_time);
	void render();

	void spawnParticles();

	Vector2f separation(Vector2f position, Vector2f velocity);
	Vector2f alignment(Vector2f position, Vector2f velocity);
	Vector2f cohesion(Vector2f position, Vector2f velocity);

private:

	RenderWindow* m_window;
	Input m_input;
	bool m_running;
	
	vector<Vector2f> m_position;
	vector<Vector2f> m_velocity;
	vector<Vector2f> m_acceleration;
	vector<Vertex> m_vertices;

	int m_particles;
	float m_perception;
	float m_max_force;
	float m_max_speed;
	float m_separation_weight;
	float m_alignment_weight;
	float m_cohesion_weight;
	float m_delta_force;

	int m_fps;
	Font m_font;
	Text m_fps_text;

	Text m_debug_text[6];
};

