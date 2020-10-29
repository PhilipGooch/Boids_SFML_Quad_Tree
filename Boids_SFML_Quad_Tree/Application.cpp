#include "Application.h"
#include <random>
#include <iostream>


// Global Vector functions.
////////////////////////////////////////////////////////////////////

float vDistance(Vector2f a, Vector2f b)
{
	return sqrtf(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
}

float vMagnitudeSquared(Vector2f a)
{
	return pow(a.x, 2) + pow(a.y, 2);
}

float vMagnitude(Vector2f a)
{
	return sqrt(pow(a.x, 2) + pow(a.y, 2));
}

Vector2f vDivide(Vector2f a, float divisor)
{
	a.x /= divisor;
	a.y /= divisor;
	return a;
}

Vector2f vMultiply(Vector2f a, float multiplier)
{
	a.x *= multiplier;
	a.y *= multiplier;
	return a;
}

Vector2f vNormalize(Vector2f a)
{
	float length = vMagnitude(a);
	if (length != 0)
	{
		a = vMultiply(a, 1.f / length);
	}
	return a;
}

Vector2f vSetMag(Vector2f a, float magnitude)
{
	a = vNormalize(a);
	a = vMultiply(a, magnitude);
	return a;
}

Vector2f vLimit(Vector2f a, float max)
{
	float magnitude_squared = vMagnitudeSquared(a);
	if (magnitude_squared > pow(max, 2))
	{
		a = vDivide(a, sqrt(magnitude_squared)); // normalize it, less maths than calling vNormalize.
		a = vMultiply(a, max);
	}
	return a;
}

////////////////////////////////////////////////////////////////////

Application::Application(RenderWindow* window) :
	m_window(window),
	m_running(true),
	m_fps(-1),
	m_particles(400),
	m_perception(25.f),
	m_max_force(0.00016f),
	m_max_speed(0.1f),
	m_separation_weight(1.2f),
	m_alignment_weight(2.1f),
	m_cohesion_weight(1.f),
	m_delta_force(0.1f)
{
	m_font.loadFromFile("arial.ttf");

	m_fps_text.setString(to_string(m_fps));
	m_fps_text.setCharacterSize(30);
	m_fps_text.setPosition(WIDTH - 80, HEIGHT - 50);
	m_fps_text.setFillColor(sf::Color::White);
	m_fps_text.setFont(m_font);

	m_debug_text[0].setString("Seperation: " + to_string(m_separation_weight));
	m_debug_text[1].setString("Alignment:  " + to_string(m_alignment_weight));
	m_debug_text[2].setString("Cohesion:   " + to_string(m_cohesion_weight));
	m_debug_text[3].setString("Perception: " + to_string(m_perception));
	m_debug_text[4].setString("Max Force:  " + to_string(m_max_force));
	m_debug_text[5].setString("Max Speed:  " + to_string(m_max_speed));
	for (int i = 0; i < 6; i++)
	{
		m_debug_text[i].setCharacterSize(20);
		m_debug_text[i].setPosition(50, 50 + i * 40);
		m_debug_text[i].setFillColor(sf::Color::White);
		m_debug_text[i].setFont(m_font);
	}


	spawnParticles();

	update(0.f);
}

Application::~Application()
{

}

void Application::run()
{
	Clock clock;
	Time fps_timer = Time::Zero;
	int frames = 0;

	while (m_running)
	{
		Event event;
		while (m_window->pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				m_running = false;
				break;
			case sf::Event::KeyPressed:
				m_input.setKeyDown(event.key.code);
				break;
			case sf::Event::KeyReleased:
				m_input.setKeyUp(event.key.code);
				break;
			default:
				break;
			}
		}

		Time delta_time = clock.restart();
		fps_timer += delta_time;
		frames++;
		if (fps_timer > sf::seconds(1))
		{
			m_fps = frames;
			frames = 0;
			fps_timer -= sf::seconds(1);
		}

		handleInput();
		update(28);
		//update(delta_time.asMilliseconds());
		render();
	}
	m_window->close();
}

void Application::handleInput()
{
	if (m_input.getKeyDown(Keyboard::Escape)) m_running = false;

	if (m_input.getKeyDown(Keyboard::Q)) m_separation_weight += m_delta_force;
	if (m_input.getKeyDown(Keyboard::A)) m_separation_weight -= m_delta_force;
	if (m_input.getKeyDown(Keyboard::W)) m_alignment_weight += m_delta_force;
	if (m_input.getKeyDown(Keyboard::S)) m_alignment_weight -= m_delta_force;
	if (m_input.getKeyDown(Keyboard::E)) m_cohesion_weight += m_delta_force;
	if (m_input.getKeyDown(Keyboard::D)) m_cohesion_weight -= m_delta_force;
	if (m_input.getKeyDown(Keyboard::R)) m_perception += 1.f;
	if (m_input.getKeyDown(Keyboard::F)) m_perception -= 1.f;
	if (m_input.getKeyDown(Keyboard::T)) m_max_force += 0.00001f;
	if (m_input.getKeyDown(Keyboard::G)) m_max_force -= 0.00001f;
	if (m_input.getKeyDown(Keyboard::Y)) m_max_speed += 0.02f;
	if (m_input.getKeyDown(Keyboard::H)) m_max_speed -= 0.02f;

	m_separation_weight = max(1.f, m_separation_weight);
	m_alignment_weight = max(1.f, m_alignment_weight);
	m_cohesion_weight = max(1.f, m_cohesion_weight);
	m_perception = max(10.f, m_perception);
	m_max_force = max(0.f, m_max_force);
	m_max_speed = max(0.f, m_max_speed);

	if (m_input.getKeyDown(Keyboard::Space)) spawnParticles();
}

void Application::update(float delta_time)
{
	m_fps_text.setString(to_string(m_fps));
	m_debug_text[0].setString("Seperation: " + to_string(m_separation_weight));
	m_debug_text[1].setString("Alignment:  " + to_string(m_alignment_weight));
	m_debug_text[2].setString("Cohesion:   " + to_string(m_cohesion_weight));
	m_debug_text[3].setString("Perception: " + to_string(m_perception));
	m_debug_text[4].setString("Max Force:  " + to_string(m_max_force));
	m_debug_text[5].setString("Max Speed:  " + to_string(m_max_speed));

	// particles are updated one at a time. so the next particle is using the updated position and velocity
	// of the previous particle. really they should all update at the same time. would require two arrays maybe?

	for (int i = 0; i < m_vertices.size(); i++)
	{
		m_acceleration[i] = Vector2f(0.f, 0.f);
		m_acceleration[i] += alignment(m_position[i], m_velocity[i]) * m_alignment_weight;
		m_acceleration[i] += cohesion(m_position[i], m_velocity[i]) * m_cohesion_weight;
		m_acceleration[i] += separation(m_position[i], m_velocity[i]) * m_separation_weight;


		//float max_velocity = 100.f;
		//if (!(abs(m_velocity[i].x) > max_velocity && abs(m_velocity[i].y) > max_velocity))
		{
			m_velocity[i] += m_acceleration[i] * delta_time;
			m_velocity[i] = vLimit(m_velocity[i], m_max_speed);
		}

		m_position[i] += m_velocity[i] * delta_time;

		if (m_position[i].x < 0) m_position[i].x += WIDTH;
		if (m_position[i].x > WIDTH) m_position[i].x -= WIDTH;
		if (m_position[i].y < 0) m_position[i].y += HEIGHT;
		if (m_position[i].y > HEIGHT) m_position[i].y -= HEIGHT;

		m_vertices[i].position.x = m_position[i].x;
		m_vertices[i].position.y = m_position[i].y;
	}
}

void Application::render()
{
	m_window->clear(Color::Black);

	m_window->draw(&m_vertices[0], m_vertices.size(), sf::Points);

	if (m_fps >= 0)
	{
		m_window->draw(m_fps_text);
	}

	for (int i = 0; i < 6; i++)
	{
		m_window->draw(m_debug_text[i]);
	}

	m_window->display();
}

void Application::spawnParticles()
{
	m_position.clear();
	m_velocity.clear();
	m_acceleration.clear();
	m_vertices.clear();

	for (int i = 0; i < m_particles; i++)
	{
		float x = (float)(rand() % WIDTH);
		float y = (float)(rand() % HEIGHT);
		float a = (float)(rand() % 360);
		float vx = cosf(a * PI / 180.f) * m_max_speed;
		float vy = sinf(a * PI / 180.f) * m_max_speed;
		float ax = 0.f;
		float ay = 0.f;
		m_position.push_back(Vector2f(x, y));
		m_velocity.push_back(Vector2f(vx, vy));
		m_acceleration.push_back(Vector2f(ax, ay));
	}

	for (int i = 0; i < m_particles; i++)
	{
		m_vertices.push_back(Vertex(Vector2f(), sf::Color::White));
	}
}

Vector2f Application::separation(Vector2f position, Vector2f velocity)
{
	Vector2f steering;
	int count = 0;
	for (int i = 0; i < m_position.size(); i++)
	{
		float distance = vDistance(position, m_position[i]);
		if (position != m_position[i] && distance < m_perception)
		{
			Vector2f difference = position - m_position[i];
			difference = vDivide(difference, pow(distance, 2));
			steering += difference;
			count++;
		}
	}
	if (count > 0)
	{
		steering = vDivide(steering, count);
		steering = vSetMag(steering, m_max_speed);
		steering -= velocity;
		steering = vLimit(steering, m_max_force);
	}
	return steering;
}

Vector2f Application::alignment(Vector2f position, Vector2f velocity)
{
	Vector2f steering;
	int count = 0;
	for (int i = 0; i < m_position.size(); i++)
	{
		float distance = vDistance(position, m_position[i]);
		if (position != m_position[i] && distance < m_perception)
		{
			steering += m_velocity[i];
			count++;
		}
	}
	if (count > 0)
	{
		steering = vDivide(steering, count);
		steering = vSetMag(steering, m_max_speed);
		steering -= velocity;
		steering = vLimit(steering, m_max_force);
	}
	return steering;
}

Vector2f Application::cohesion(Vector2f position, Vector2f velocity)
{
	Vector2f steering;
	int count = 0;
	for (int i = 0; i < m_position.size(); i++)
	{
		float distance = vDistance(position, m_position[i]);
		if (position != m_position[i] && distance < m_perception)
		{
			steering += m_position[i];
			count++;
		}
	}
	if (count > 0)
	{
		steering = vDivide(steering, count);
		steering -= position;
		steering = vSetMag(steering, m_max_speed);
		steering -= velocity;
		steering = vLimit(steering, m_max_force);
	}
	return steering;
}

