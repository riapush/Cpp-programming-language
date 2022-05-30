#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Player.h"
using namespace std;
using namespace sf;

class Ball {
	Clock clock;
	Time time;
public:
	bool destroyed{ false };
	CircleShape shape;
	Vector2f velocity{ 0, 0 };

	Ball(float x, float y);

	void update();

	float x() { return shape.getPosition().x; }
	float y() { return shape.getPosition().y; }
	float left() { return x() - shape.getRadius(); }
	float right() { return x() + shape.getRadius(); }
	float top() { return y() - shape.getRadius(); }
	float bottom() { return y() + shape.getRadius(); }

	virtual void checkCollision(Player& player);

	virtual void checkCollision(Ball& other);
};

template <class T1, class T2>
bool isColliding(T1& a, T2& b) {
	return a.right() >= b.left() && a.left() <= b.right() && a.bottom() >= b.top() && a.top() <= b.bottom();
}

extern vector<Ball> balls;