#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Rectangle.h"
using namespace sf;

class Player : public Rectangle {
	Vector2f velocity;
public:
	Player(float x, float y);
	void update();
};