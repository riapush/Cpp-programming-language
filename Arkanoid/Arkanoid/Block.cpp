#include "Block.h"
#include "Ball.h"

constexpr unsigned int window_width{ 800 }, window_height{ 600 };
constexpr float ball_size{ 8.f }, ball_velocity{ 4.f };
constexpr float bonus_width{ 40.f }, bonus_height{ 10.f }, bonus_velocity{ 6.f };
constexpr float block_width{ 60.f }, block_height{ 35.f }, moving_block_velocity{ 3.f };
constexpr int block_xcount{ 11 }, block_ycount{ 4 };
constexpr int color_change{ 60 };
constexpr float extra_velocity{ 7.f };
extern Time bonus_time;
extern int score;

Block::Block(float x, float y) {
	shape.setPosition(x, y);
	shape.setSize({ block_width, block_height });
	shape.setFillColor({ 255,76,48,255 });
	shape.setOrigin(block_width / 2.f, block_height / 2.f);
}

void Block::checkCollision(Ball& ball) {
	if (!isColliding(*this, ball)) return;

	health--;
	score++;
	if (!health) destroyed = true;
	shape.setFillColor({ 255,76,48,static_cast<Uint8>(255 - color_change * (2 - health)) });

	float overlapLeft{ ball.right() - this->left() };
	float overlapRight{ this->right() - ball.left() };
	float overlapTop{ ball.bottom() - this->top() };
	float overlapBottom{ this->bottom() - ball.top() };

	bool ballFromLeft{ abs(overlapLeft) < abs(overlapRight) };
	bool ballFromTop{ abs(overlapTop) < abs(overlapBottom) };

	float minOverlapX{ ballFromLeft ? overlapLeft : overlapRight };
	float minOverlapY{ ballFromTop ? overlapTop : overlapBottom };

	if (abs(minOverlapX) < abs(minOverlapY))
		ball.velocity.x = -ball.velocity.x;
	else
		ball.velocity.y = -ball.velocity.y;
}

IndestructibleBlock::IndestructibleBlock(float x, float y) : Block(x, y) { shape.setFillColor({ 52,45,113,255 }); }

void IndestructibleBlock::checkCollision(Ball& ball){
	if (!isColliding(*this, ball)) return;

	float overlapLeft{ ball.right() - this->left() };
	float overlapRight{ this->right() - ball.left() };
	float overlapTop{ ball.bottom() - this->top() };
	float overlapBottom{ this->bottom() - ball.top() };

	bool ballFromLeft(abs(overlapLeft) < abs(overlapRight));
	bool ballFromTop(abs(overlapTop) < abs(overlapBottom));

	float minOverlapX{ ballFromLeft ? overlapLeft : overlapRight };
	float minOverlapY{ ballFromTop ? overlapTop : overlapBottom };

	if (abs(minOverlapX) < abs(minOverlapY))
		ball.velocity.x = -ball.velocity.x;
	else
		ball.velocity.y = -ball.velocity.y;
}

void IndestructibleBlock::checkCollision(Player& player) { Block::checkCollision(player); }

FasteningBlock::FasteningBlock(float x, float y) :Block(x, y) { ; }

void FasteningBlock::checkCollision(Ball& ball) {
	if (!isColliding(*this, ball)) {
		if (fastened) {
			time = clock.getElapsedTime();
			if (time > bonus_time) {
				ball.velocity.x = (ball.velocity.x > 0) ? ball_velocity : -ball_velocity;
				ball.velocity.y = (ball.velocity.y > 0) ? ball_velocity : -ball_velocity;
				fastened = false;
				ball.shape.setFillColor(Color::Red);
			}
		}
		return;
	}


	health--;
	fastened = true;
	time = clock.restart();
	score++;
	if (!health) destroyed = true;
	shape.setFillColor({ 255,76,48,static_cast<Uint8>(255 - color_change * (2 - health)) });

	float overlapLeft{ ball.right() - this->left() };
	float overlapRight{ this->right() - ball.left() };
	float overlapTop{ ball.bottom() - this->top() };
	float overlapBottom{ this->bottom() - ball.top() };

	bool ballFromLeft{ abs(overlapLeft) < abs(overlapRight) };
	bool ballFromTop{ abs(overlapTop) < abs(overlapBottom) };

	ball.velocity.x = ballFromLeft ? -extra_velocity : extra_velocity;
	ball.velocity.y = ballFromTop ? -extra_velocity : extra_velocity;
	ball.shape.setFillColor({ 255,255,255,255 });
}

BonusBlock::BonusBlock(float x, float y) : Block(x, y) { ; }

void BonusBlock::checkCollision(Ball& ball) {
	if (!isColliding(*this, ball)) return;

	health--;
	score++;
	if (!health) released = true;
	if (!released) shape.setFillColor({ 255,76,48,static_cast<Uint8>(255 - color_change * (2 - health)) });
	else {
		shape.setSize({ bonus_width, bonus_height });
		shape.setFillColor({ 216, 250, 8,255 });
		velocity.y = bonus_velocity;
	}

	float overlapLeft{ ball.right() - this->left() };
	float overlapRight{ this->right() - ball.left() };
	float overlapTop{ ball.bottom() - this->top() };
	float overlapBottom{ this->bottom() - ball.top() };

	bool ballFromLeft{ abs(overlapLeft) < abs(overlapRight) };
	bool ballFromTop{ abs(overlapTop) < abs(overlapBottom) };

	float minOverlapX{ ballFromLeft ? overlapLeft : overlapRight };
	float minOverlapY{ ballFromTop ? overlapTop : overlapBottom };

	if (abs(minOverlapX) < abs(minOverlapY))
		ball.velocity.x = -ball.velocity.x;
	else
		ball.velocity.y = -ball.velocity.y;
}

void BonusBlock::update() {
	shape.move(velocity);
	if (this->y() > window_height) {
		destroyed = true;
	}
}

void BonusBlock::checkCollision(Player& player) {
	if (!isColliding(player, *this)) return;

	destroyed = true;
}

MovingBlock::MovingBlock(float x, float y) :BonusBlock(x, y) { ; }

void MovingBlock::checkCollision(Ball& ball) {
	if (!released) {
		BonusBlock::checkCollision(ball);
		return;
	}
	if (y() > window_height) {
		destroyed = true;
		return;
	}
	if (!isColliding(*this, ball)) return;
	if (released && !activated) return;
	moving_block_health--;
	score++;
	if (!moving_block_health) destroyed = true;
	shape.setFillColor({ 216,250,8,static_cast<Uint8>(255 - color_change * (3 - moving_block_health)) });

	float overlapLeft{ ball.right() - this->left() };
	float overlapRight{ this->right() - ball.left() };
	float overlapTop{ ball.bottom() - this->top() };
	float overlapBottom{ this->bottom() - ball.top() };

	bool ballFromLeft{ abs(overlapLeft) < abs(overlapRight) };
	bool ballFromTop{ abs(overlapTop) < abs(overlapBottom) };

	float minOverlapX{ ballFromLeft ? overlapLeft : overlapRight };
	float minOverlapY{ ballFromTop ? overlapTop : overlapBottom };

	if (abs(minOverlapX) < abs(minOverlapY))
		ball.velocity.x = -ball.velocity.x;
	else
		ball.velocity.y = -ball.velocity.y;
}

void MovingBlock::checkCollision(Player& player) {
	if (!isColliding(player, *this)) {
		if (activated && !moving_block_health) {
			destroyed = true;
		}
		return;
	}
	activated = true;
	velocity.x = moving_block_velocity;
	velocity.y = 0;

	shape.setPosition(static_cast<float>(window_width / 2), static_cast<float>(window_height / 2));
	shape.setSize({ block_width, block_height });
}

void MovingBlock::update() {
	shape.move(velocity);
	if (left() < 0) velocity.x = moving_block_velocity;
	else if (right() > window_width) velocity.x = -moving_block_velocity;
}

ExtraBallBlock::ExtraBallBlock(float x, float y) :BonusBlock(x, y) { ; }

void ExtraBallBlock::checkCollision(Ball& ball) { BonusBlock::checkCollision(ball); }

void ExtraBallBlock::update() { BonusBlock::update(); }

void ExtraBallBlock::checkCollision(Player& player) {
	if (!isColliding(player, *this)) return;

	destroyed = true;
	balls.emplace_back(static_cast<float>(window_width / 2), static_cast<float>(window_height / 2));

}