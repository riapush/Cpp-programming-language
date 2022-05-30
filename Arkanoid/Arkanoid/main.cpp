#include <cmath>
#include <time.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Rectangle.h"
#include "Player.h"
#include "Ball.h"
#include "Block.h"

using namespace std;
using namespace sf;


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
	Font font;
	Text score_table, esc;
	Player player{ static_cast<float>(window_width / 2), static_cast<float>(window_height - 50) };
	vector<unique_ptr<Block>> blocks;
	RenderWindow window{ {window_width, window_height}, "Arkanoid Game" };
	int block_xcount{ 11 }, block_ycount{ 4 };

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

	void Play() {
		window.setFramerateLimit(60);
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
	}

public:
	Game() {
	srand(static_cast<unsigned int>(time(0)));
	balls.emplace_back(static_cast<float>(window_width / 2), static_cast<float>(window_height / 2));
	blocks = generateBlocks();
	Play();
	}
};

int main() {
	auto* game = new Game();
	delete game;
	return 0;
}