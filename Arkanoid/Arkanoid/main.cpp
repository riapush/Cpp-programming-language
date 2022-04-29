#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

using namespace std;
using namespace sf;

unsigned int window_width{ 800 }, window_height{ 600 };
constexpr float ball_size{ 10.f }, ball_velocity{ 7.f };
constexpr float platform_width{ 80.f }, platform_height{ 10.f }, platform_velocity{ 7.f };
constexpr float bonus_width{ 40.f }, bonus_height{10.f}, bonus_velocity{ 6.f };
constexpr float block_width{ 60.f }, block_height{ 20.f };
constexpr int block_xcount{ 11 }, block_ycount{ 4 };
constexpr int color_change{ 60 };

class Ball {
public:
	CircleShape shape;
	Vector2f velocity{ -ball_velocity, -ball_velocity };
	int score{ 0 };

	Ball(float x, float y) {
		shape.setPosition(x, y);
		shape.setRadius(ball_size);
		shape.setFillColor(Color::Red);
		shape.setOrigin(ball_size, ball_size);
	}

	void update() {
		shape.move(velocity);

		if (left() < 0) velocity.x = ball_velocity;
		else if (right() > window_width) velocity.x = -ball_velocity;

		if (top() < 0) velocity.y = ball_velocity;
		else if (bottom() > window_height) {
			velocity.y = -ball_velocity;
			score--;
		}
	}

	float x() { return shape.getPosition().x; }
	float y() { return shape.getPosition().y; }
	float left() { return x() - shape.getRadius(); }
	float right() { return x() + shape.getRadius(); }
	float top() { return y() - shape.getRadius(); }
	float bottom() { return y() + shape.getRadius(); }
};

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
public:
	Vector2f velocity;

	Player(float x, float y) {
		shape.setPosition(x, y);
		shape.setSize({ platform_width, platform_height });
		shape.setFillColor({205,209,228,255});
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


template <class T1, class T2>
bool isColliding(T1& a, T2& b) {
	return a.right() >= b.left() && a.left() <= b.right() && a.bottom() >= b.top() && a.top() <= b.bottom();
}

class Block : virtual public Rectangle {
public:
	bool destroyed{ false };
	int health = 2;

	Block(float x, float y) {
		shape.setPosition(x, y);
		shape.setSize({ block_width, block_height });
		shape.setFillColor({255,76,48,255});
		shape.setOrigin(block_width / 2.f, block_height / 2.f);
	}

	virtual void checkCollision(Ball& ball) {
		if (!isColliding(*this, ball)) return;

		health--;
		ball.score++;
		if (!health) destroyed = true;
		shape.setFillColor({ 255,static_cast<Uint8>(76 + color_change * health),static_cast<Uint8>(48 + color_change * health),255 });

		float overlapLeft{ ball.right() - this->left() };
		float overlapRight{ this->right() - ball.left() };
		float overlapTop{ ball.bottom() - this->top() };
		float overlapBottom{ this->bottom() - ball.top() };

		bool ballFromLeft{ abs(overlapLeft) < abs(overlapRight) };
		bool ballFromTop{ abs(overlapTop) < abs(overlapBottom) };

		float minOverlapX{ ballFromLeft ? overlapLeft : overlapRight };
		float minOverlapY{ ballFromTop ? overlapTop : overlapBottom };

		if (abs(minOverlapX) < abs(minOverlapY))
			ball.velocity.x = ballFromLeft ? -ball_velocity : ball_velocity;
		else
			ball.velocity.y = ballFromTop ? -ball_velocity : ball_velocity;
	}

};

class IndestructableBlock :public Block {
public:

	IndestructableBlock(float x, float y) : Block(x, y) { shape.setFillColor({ 52,45,113,255 }); }

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
			ball.velocity.x = ballFromLeft ? -ball_velocity : ball_velocity;
		else
			ball.velocity.y = ballFromTop ? -ball_velocity : ball_velocity;
	}
};

/////////////////////////////////////////////////////////

class BonusBlock : public Block {

public:
	bool activated{ false };
	Vector2f velocity{ 0,0 };

	BonusBlock(float x, float y) : Block(x, y) { ; }

	void checkCollision(Ball& ball)override {
		if (!isColliding(*this, ball)) return;

		health--;
		if (!health) activated = true;
		if(!activated) shape.setFillColor({ 255,static_cast<Uint8>(76 + color_change * health),static_cast<Uint8>(48 + color_change * health),255 });
		else {
			shape.setSize({ bonus_height, bonus_width });
			shape.setFillColor({ 147, 250, 165,255 });
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
			ball.velocity.x = ballFromLeft ? -ball_velocity : ball_velocity;
		else
			ball.velocity.y = ballFromTop ? -ball_velocity : ball_velocity;
	}

	void update() {
		shape.move(velocity);
		if (this->y() > window_height) { // need to check collision with player and whether we lower than the bottom of window.
			//destroyed = true;
		}
	}

};
//////////////////////////////////////////////////////////

void checkCollision(Player& player, Ball& ball) {
	if (!isColliding(player, ball)) return;

	ball.velocity.y = -ball_velocity;

	if (ball.x() < player.x()) ball.velocity.x = -ball_velocity;
	else ball.velocity.x = ball_velocity;
}

void checkCollision(Player& player, BonusBlock& bonus_block) {
	if (!isColliding(player, bonus_block)) return;

	if (bonus_block.x() < player.x()) bonus_block.velocity.x = -ball_velocity;
	else bonus_block.velocity.x = ball_velocity;

	bonus_block.destroyed = true;
}

int main() {
	Ball ball{ static_cast<float>(window_width / 2), static_cast<float>(window_height / 2) };
	Player paddle{ static_cast<float>(window_width / 2), static_cast<float>(window_height - 50) };
	vector<Block> blocks;

	for (int x{ 0 }; x < block_xcount; ++x)
		for (int y{ 0 }; y < block_ycount; ++y)
			blocks.emplace_back((x + 1) * (block_width + 3) + 22, (y + 2) * (block_height + 3));

	RenderWindow window{ {window_width, window_height}, "Arkanoid Game" };
	window.setFramerateLimit(60);

	Font font;
	Text score_table;
	if (!font.loadFromFile("PixelEmulator-xq08.ttf")) { /* raise error*/ }

	while (true) {
		window.clear({ 82,78,183,255 });

		if (Keyboard::isKeyPressed(Keyboard::Key::Escape)) break;

		score_table.setFont(font);
		string str_score = to_string(ball.score);
		score_table.setString("score: "+ str_score);
		score_table.setCharacterSize(24);
		score_table.setPosition(0, 0);

		ball.update();
		paddle.update();
		checkCollision(paddle, ball);
		for (auto& block : blocks) block.checkCollision(ball);
		blocks.erase(remove_if(begin(blocks), end(blocks), [](const Block& mBrick) { return mBrick.destroyed; }), end(blocks));

		window.draw(score_table);
		window.draw(ball.shape);
		window.draw(paddle.shape);
		for (auto& block : blocks) window.draw(block.shape);
		window.display();
	}

	return 0;
}