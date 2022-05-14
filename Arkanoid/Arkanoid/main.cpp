#include <cmath>
#include <time.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

using namespace std;
using namespace sf;

unsigned int window_width{ 800 }, window_height{ 600 };
constexpr float ball_size{ 8.f }, ball_velocity{ 4.f };
constexpr float platform_width{ 80.f }, platform_height{ 10.f }, platform_velocity{ 6.f };
constexpr float bonus_width{ 40.f }, bonus_height{ 10.f }, bonus_velocity{ 6.f };
constexpr float block_width{ 60.f }, block_height{ 35.f }, moving_block_velocity{ 3.f };
constexpr int block_xcount{ 11 }, block_ycount{ 4 };
constexpr int color_change{ 60 };
constexpr float extra_velocity{ 7.f };
Time bonus_time{ seconds(5.0f) };
int score{ 0 };

template <class T1, class T2>
bool isColliding(T1& a, T2& b) {
	return a.right() >= b.left() && a.left() <= b.right() && a.bottom() >= b.top() && a.top() <= b.bottom();
}

class Rectangle {
public:
	RectangleShape shape;
	float x() { return shape.getPosition().x; }
	float y() { return shape.getPosition().y; }
	float left() { return x() - shape.getSize().x / 2.f; }
	float right() { return x() + shape.getSize().x / 2.f; }
	float top() { return y() - shape.getSize().y / 2.f; }
	float bottom() { return y() + shape.getSize().y / 2.f; }
};

class Player : public Rectangle {
	Vector2f velocity;
public:
	
	Player(float x, float y) {
		shape.setPosition(x, y);
		shape.setSize({ platform_width, platform_height });
		shape.setFillColor({ 205,209,228,255 });
		shape.setOrigin(platform_width / 2.f, platform_height / 2.f);
	}

	void update() {
		shape.move(velocity);

		if ((Keyboard::isKeyPressed(Keyboard::Key::Left) ||
			Keyboard::isKeyPressed(Keyboard::Key::A)) && left() > 0)
			velocity.x = -platform_velocity;
		else if ((Keyboard::isKeyPressed(Keyboard::Key::Right) ||
			Keyboard::isKeyPressed(Keyboard::Key::D)) && right() < window_width)
			velocity.x = platform_velocity;
		else velocity.x = 0;
	}
};

class Ball {
	Clock clock;
	Time time;
public:
	bool destroyed{ false };
	CircleShape shape;
	Vector2f velocity{ 0, 0 };

	Ball(float x, float y) {
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

	void update() {
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

	float x() { return shape.getPosition().x; }
	float y() { return shape.getPosition().y; }
	float left() { return x() - shape.getRadius(); }
	float right() { return x() + shape.getRadius(); }
	float top() { return y() - shape.getRadius(); }
	float bottom() { return y() + shape.getRadius(); }

	virtual void checkCollision(Player& player) {
		if (!isColliding(player, *this)) return;

		velocity.y = -velocity.y;
	}

	virtual void checkCollision(Ball& other) {
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
};

vector<Ball> balls;

class Block : public Rectangle {
protected:
	int health = 2;
public:
	bool destroyed{ false };

	Block() { ; }
	Block(float x, float y) {
		shape.setPosition(x, y);
		shape.setSize({ block_width, block_height });
		shape.setFillColor({ 255,76,48,255 });
		shape.setOrigin(block_width / 2.f, block_height / 2.f);
	}

	virtual void checkCollision(Ball& ball) {
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

	virtual void update() { ; }
	virtual void checkCollision(Player& player) { ; }
};

class IndestructibleBlock : public Block {
public:

	IndestructibleBlock(float x, float y) : Block(x, y) { shape.setFillColor({ 52,45,113,255 }); }

	void checkCollision(Ball& ball)override {
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

	void checkCollision(Player& player)override { Block::checkCollision(player); }
};

class FasteningBlock :public Block {
	bool fastened{ false };
	Clock clock;
	Time time;

public:
	FasteningBlock(float x, float y) :Block(x, y) { ; }

	void checkCollision(Ball& ball)override {
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

	void checkCollision(Player& player) override { Block::checkCollision(player); }
};

class BonusBlock : public Block {
protected:
	bool released{ false };
	Vector2f velocity{ 0,0 };

public:
	BonusBlock(float x, float y) : Block(x, y) { ; }

	void checkCollision(Ball& ball)override {
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

	void update()override {
		shape.move(velocity);
		if (this->y() > window_height) {
			destroyed = true;
		}
	}

	void checkCollision(Player& player)override {
		if (!isColliding(player, *this)) return;

		destroyed = true;
	}

};

class MovingBlock :public BonusBlock {
	bool activated{ false };
	int moving_block_health = 3;
public:
	MovingBlock(float x, float y) :BonusBlock(x, y) { ; }

	void checkCollision(Ball& ball) {
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

	void checkCollision(Player& player)override {
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

	void update()override {
		shape.move(velocity);
		if (left() < 0) velocity.x = moving_block_velocity;
		else if (right() > window_width) velocity.x = -moving_block_velocity;
	}
};

class ExtraBallBlock :public BonusBlock {
public:
	ExtraBallBlock(float x, float y) :BonusBlock(x, y) { ; }

	void checkCollision(Ball& ball)override { BonusBlock::checkCollision(ball); }
	void update()override { BonusBlock::update(); }

	void checkCollision(Player& player)override {
		if (!isColliding(player, *this)) return;

		destroyed = true;
		balls.emplace_back(static_cast<float>(window_width / 2), static_cast<float>(window_height / 2));

	}
};

vector<unique_ptr<Block>> generateBlocks() {
	vector<unique_ptr<Block>> blocks;
	for (int x{ 0 }; x < block_xcount; ++x) {
		for (int y{ 0 }; y < block_ycount; ++y) {
			int situation = rand() % 8;
			unique_ptr<Block> block;
			switch (situation) {
			case 0:
				block = move(make_unique<Block>((x + 1) * (block_width + 3) + 22, (y + 2) * (block_height + 3)));
				break;
			case 1:
				block = move(make_unique<IndestructibleBlock>((x + 1) * (block_width + 3) + 22, (y + 2) * (block_height + 3)));
				break;
			case 2:
				block = move(make_unique<FasteningBlock>((x + 1) * (block_width + 3) + 22, (y + 2) * (block_height + 3)));
				break;
			case 3:
				block = move(make_unique<MovingBlock>((x + 1) * (block_width + 3) + 22, (y + 2) * (block_height + 3)));
				break;
			case 4:
				block = move(make_unique<ExtraBallBlock>((x + 1) * (block_width + 3) + 22, (y + 2) * (block_height + 3)));
				break;
			case 5:
				block = move(make_unique<Block>((x + 1) * (block_width + 3) + 22, (y + 2) * (block_height + 3)));
				break;
			case 6:
				block = move(make_unique<Block>((x + 1) * (block_width + 3) + 22, (y + 2) * (block_height + 3)));
				break;
			case 7:
				block = move(make_unique<Block>((x + 1) * (block_width + 3) + 22, (y + 2) * (block_height + 3)));
				break;
			}
			blocks.push_back(move(block));
		}
	}
	return blocks;
}

class Game {
	unsigned int window_width{ 800 }, window_height{ 600 };
	float ball_size{ 8.f }, ball_velocity{ 4.f };
	float platform_width{ 80.f }, platform_height{ 10.f }, platform_velocity{ 6.f };
	float bonus_width{ 40.f }, bonus_height{ 10.f }, bonus_velocity{ 6.f };
	float block_width{ 60.f }, block_height{ 35.f }, moving_block_velocity{ 3.f };
	int block_xcount{ 11 }, block_ycount{ 4 };
	int color_change{ 60 };
	float extra_velocity{ 7.f };
	Time bonus_time{ seconds(5.0f) };
	int score{ 0 };
	Font font;
	Text score_table, esc;
	vector<Ball> balls;

	Game() {
		srand(static_cast<unsigned int>(time(0)));
		balls.emplace_back(static_cast<float>(window_width / 2), static_cast<float>(window_height / 2));
	}
	Player player{ static_cast<float>(window_width / 2), static_cast<float>(window_height - 50) };
	vector<unique_ptr<Block>> blocks = generateBlocks();


};

int main() {
	srand(static_cast<unsigned int>(time(0)));
	balls.emplace_back(static_cast<float>(window_width / 2), static_cast<float>(window_height / 2));
	Player player{ static_cast<float>(window_width / 2), static_cast<float>(window_height - 50) };
	vector<unique_ptr<Block>> blocks = generateBlocks();


	RenderWindow window{ {window_width, window_height}, "Arkanoid Game" };
	window.setFramerateLimit(60);

	Font font;
	Text score_table, esc;
	if (!font.loadFromFile("PixelEmulator-xq08.ttf")) { /* raise error*/ }

	while (true) {
		window.clear({ 82,78,183,255 });

		if (Keyboard::isKeyPressed(Keyboard::Key::Escape)) break;

		score_table.setFont(font);
		score_table.setString("score: " + to_string(score));
		score_table.setCharacterSize(24);
		score_table.setPosition(0, 0);
		esc.setFont(font);
		esc.setCharacterSize(15);
		esc.setPosition(7, static_cast<float>(window_height - 20));
		esc.setString("Press Esc for exit");
		esc.setFillColor({ 255,255,255,150 });

		for (auto& ball : balls) {
			ball.update();
			ball.checkCollision(player);
		}
		for (int i = 0; i < static_cast<int>(balls.size()); ++i) {
			for (int j = 0; j < static_cast<int>(balls.size()); ++j) {
				if (i == j) continue;
				balls[i].checkCollision(balls[j]);
			}
		}
		player.update();
		for (auto& block : blocks) {
			for (auto& ball : balls) block->checkCollision(ball);
			block->checkCollision(player);
			block->update();
		}

		blocks.erase(remove_if(begin(blocks), end(blocks), [](const unique_ptr<Block>& block) { return block->destroyed; }), end(blocks));
		balls.erase(remove_if(begin(balls) + 1, end(balls), [](const Ball& ball) { return ball.destroyed; }), end(balls));

		window.draw(score_table);
		window.draw(esc);
		for (auto& ball : balls) window.draw(ball.shape);
		window.draw(player.shape);
		for (auto& block : blocks) window.draw(block->shape);
		window.display();
	}

	/*Game* game = new Game();
	delete game;*/
	return 0;
}