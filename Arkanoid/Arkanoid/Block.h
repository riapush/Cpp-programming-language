#pragma once
#include "Rectangle.h"
#include "Player.h"
#include "Ball.h"

class Block : public Rectangle {
protected:
	int health = 2;
public:
	bool destroyed{ false };

	Block() { ; }
	Block(float x, float y);

	virtual void checkCollision(Ball& ball);

	virtual void update() { ; }
	virtual void checkCollision(Player& player) { ; }
};

class IndestructibleBlock : public Block {
public:

	IndestructibleBlock(float x, float y);

	void checkCollision(Ball& ball)override;
	void checkCollision(Player& player)override;
};

class FasteningBlock :public Block {
	bool fastened{ false };
	Clock clock;
	Time time;

public:
	FasteningBlock(float x, float y);

	void checkCollision(Ball& ball)override;
	void checkCollision(Player& player) override { Block::checkCollision(player); }
};

class BonusBlock : public Block {
protected:
	bool released{ false };
	Vector2f velocity{ 0,0 };

public:
	BonusBlock(float x, float y);

	void checkCollision(Ball& ball)override;
	void update()override;
	void checkCollision(Player& player)override;

};

class MovingBlock :public BonusBlock {
	bool activated{ false };
	int moving_block_health = 3;
public:
	MovingBlock(float x, float y);

	void checkCollision(Ball& ball);

	void checkCollision(Player& player)override;

	void update()override;
};

class ExtraBallBlock :public BonusBlock {
public:
	ExtraBallBlock(float x, float y);

	void checkCollision(Ball& ball)override;
	void update()override;

	void checkCollision(Player& player);
};