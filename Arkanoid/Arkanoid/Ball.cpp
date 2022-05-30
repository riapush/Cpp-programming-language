#include "Ball.h"

constexpr unsigned int window_width{ 800 }, window_height{ 600 };
constexpr float ball_size{ 8.f }, ball_velocity{ 4.f };
constexpr float bonus_width{ 40.f }, bonus_height{ 10.f }, bonus_velocity{ 6.f };
constexpr float extra_velocity{ 7.f };
Time bonus_time{ seconds(5.0f) };
int score{ 0 };

vector<Ball> balls;

Ball::Ball(float x, float y) {
	int sign = rand() % 2;
	float x_velocity = (sign) ? ball_velocity : -ball_velocity;
	sign = rand() % 2;
	float y_velocity = (sign) ? -ball_velocity : ball_velocity;
	velocity.x = x_velocity;
	velocity.y = y_velocity;
	shape.setPosition(x, y);
	shape.setRadius(ball_size);
	shape.setFillColor(Color::Red);
	shape.setOrigin(ball_size, ball_size);
	time = clock.restart();
}

void Ball::update() {
	time = clock.getElapsedTime();
	if (time > bonus_time) {
		destroyed = true;
	}
	shape.move(velocity);

	if (left() < 0 || right() > window_width) velocity.x = -velocity.x;

	if (top() < 0) velocity.y = -velocity.y;
	else if (bottom() > window_height) {
		velocity.y = -velocity.y;
		score--;
	}
}

void Ball::checkCollision(Player& player) {
	if (!isColliding(player, *this)) return;

	velocity.y = -velocity.y;
}

void Ball::checkCollision(Ball& other) {
	if (!isColliding(*this, other)) return;
	if (x() == other.x()) {
		velocity.y *= (-1);
		other.velocity.y *= (-1);
	}
	else if (y() == other.y()) {
		velocity.x *= (-1);
		other.velocity.x *= (-1);
	}
	else {
		velocity.x *= (-1);
		velocity.y *= (-1);
		other.velocity.x *= (-1);
		other.velocity.y *= (-1);
	}


}