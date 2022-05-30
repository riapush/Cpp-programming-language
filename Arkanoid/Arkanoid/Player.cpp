#include "Player.h"

constexpr unsigned int window_width{ 800 }, window_height{ 600 };
constexpr float platform_width{ 80.f }, platform_height{ 10.f }, platform_velocity{ 6.f };

Player::Player(float x, float y) {
	shape.setPosition(x, y);
	shape.setSize({ platform_width, platform_height });
	shape.setFillColor({ 205,209,228,255 });
	shape.setOrigin(platform_width / 2.f, platform_height / 2.f);
}

void Player::update() {
	shape.move(velocity);

	if ((Keyboard::isKeyPressed(Keyboard::Key::Left) ||
		Keyboard::isKeyPressed(Keyboard::Key::A)) && left() > 0)
		velocity.x = -platform_velocity;
	else if ((Keyboard::isKeyPressed(Keyboard::Key::Right) ||
		Keyboard::isKeyPressed(Keyboard::Key::D)) && right() < window_width)
		velocity.x = platform_velocity;
	else velocity.x = 0;
}