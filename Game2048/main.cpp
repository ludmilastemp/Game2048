#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <random>
#include <iostream>

using namespace sf; using namespace std;

int blockWidht = 120; int check = 0;
const int fieldSize = 4; 
int grid[fieldSize + 2][fieldSize + 2] = { 19 }; //логическое представление поля 
Clock clock1;


void AddingSquare(){
	int ch = 0; int i; int j; int x;
	while (ch == 0) {
		std::random_device rnd;
		std::mt19937 rng(rnd());
		std::uniform_int_distribution<int> uni(1, 13);
		i = uni(rng) % 4; j = uni(rnd) % 4; 
		if (grid[i][j] == 0) { 
			ch = 1; 
			if (i % 6 == 0) grid[i][j] = 2; 
			else grid[i][j] = 1;
		} 
	}
}

int main()
{
	for (int i = 0; i < 36; ++i) grid[i % 6][i / 6] = 19;
	RenderWindow window(VideoMode(720, 640), "2048");
	window.setFramerateLimit(90);
	Texture texture;
	texture.loadFromFile("D:\\Proga\\Game2048\\x64\\Debug\\2048.png");
	Texture field;
	field.loadFromFile("D:\\Proga\\Game2048\\x64\\Debug\\field.png");
	Texture backGround;
	backGround.loadFromFile("D:\\Proga\\Game2048\\x64\\Debug\\backGround.png");
	Sprite sprite[20];
	sprite[17].setTexture(backGround);
	sprite[18].setTexture(field);
	sprite[17].setPosition(0, 0);
	sprite[18].setPosition(93, 70);
	int n = 0;
	for (int j = 0; j < 4; j++)
		for (int i = 0; i < 4; i++)
		{
			n++;
			sprite[n].setTexture(texture);
			sprite[n].setTextureRect(IntRect(i * (blockWidht + 3) + 1, j * (blockWidht + 4), blockWidht - 7, blockWidht - 8));
			grid[i + 1][j + 1] = 0;
			sprite[grid[j + 1][i + 1]].setPosition(107 + i * (blockWidht + 11), 85 + j * (blockWidht + 10));
		}
	grid[2][3] = 1; grid[2][4] = 1; blockWidht = 140;
	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed) window.close();
			else if (event.type == Event::KeyPressed) {
				int dx; int dy; int temp[16] = { 0 }; int merge[16] = { 0 };
				if (event.key.code == Keyboard::Up) { dx = 0; dy = -1; }
				else if (event.key.code == Keyboard::Right) { dx = 1; dy = 0; }
				else if (event.key.code == Keyboard::Down) { dx = 0; dy = 1; }
				else if (event.key.code == Keyboard::Left) { dx = -1; dy = 0; }
				float speed = 35; int cont = 1;
				for (int i = 0; i < 16; ++i) { merge[i] = 0; }
				for (int k = 0; k < 6 && cont == 1; ++k) {
					cont = 0;
					for (int i = 0; i < 16; ++i) {
						temp[i] = 0; if (merge[i] == 4) merge[i] = 3; else if (merge[i] != 3) merge[i] = 0; }
					for (int shift = 0; shift <= blockWidht; shift += speed)
					{
						window.clear();
						window.draw(sprite[17]);
						window.draw(sprite[18]);
						for (int x = 0; x < 16; ++x) {
							int i = x / 4 + 1; int j = x % 4 + 1;
							if ((dx == 0 && dy == 1) || (dx == 1 && dy == 0)) { i = 5 - i; j = 5 - j; }
							if (shift == 0 && grid[i][j] != 0) {
								if (grid[i + dy][j + dx] == 0) {
									merge[(i - 1) * 4 + (j - 1)] = 1; temp[x] = grid[i][j]; grid[i][j] = 0; check = 1; cont = 1;
								}
								else if (grid[i + dy][j + dx] == grid[i][j] && merge[(i - 1) * 4 + (j - 1)] != 3) {
									merge[(i - 1) * 4 + (j - 1)] = 2; temp[x] = grid[i][j]; grid[i][j] = 0; check = 1; cont = 1;
								}
							}
							else if (shift == blockWidht) {
								if (merge[(i - 1) * 4 + (j - 1)] == 1) grid[i + dy][j + dx] = temp[x];
								else if (merge[(i - 1) * 4 + (j - 1)] == 2) {
									++grid[i + dy][j + dx]; temp[x] = 0; merge[(i - 1) * 4 + (j - 1)] = 3; merge[(i - 1 + dy) * 4 + (j - 1) + dx] = 4;
								}
							}
							if (grid[i][j] != 0) {
								sprite[grid[i][j]].setPosition(j * (blockWidht - 9) - 24, i * (blockWidht - 10) - 45);
								window.draw(sprite[grid[i][j]]);
							}
							if (temp[x] != 0) {
								sprite[temp[x]].setPosition(j * (blockWidht - 9) - 24 + shift * dx, i * (blockWidht - 10) - 45 + shift * dy);
								window.draw(sprite[temp[x]]);
							}
						}
						for (int x = 0; x < 16; ++x) {
							int i = x / 4 + 1; int j = x % 4 + 1;
							if ((dx == 0 && dy == 1) || (dx == 1 && dy == 0)) { i = 5 - i; j = 5 - j; }
							if (merge[(i - 1 + dy) * 4 + (j - 1) + dx] == 4) {
								sprite[grid[i + dy][j + dx]].setPosition((j + dx) * (blockWidht - 9) - 24 - 5, (i + dy) * (blockWidht - 10) - 45 - 5);
								sprite[grid[i + dy][j + dx]].setScale(1 * 1.05, 1 * 1.05);
								window.draw(sprite[grid[i + dy][j + dx]]);
							}
						}
						window.display();
						Time time2 = clock1.restart();
						Time time1 = clock1.getElapsedTime();
						int s = time1.asMilliseconds();
						while (s < 10) {
							Time time1 = clock1.getElapsedTime();
							s = time1.asMilliseconds();
						}
						for (int i = 0; i < 16; ++i) { sprite[grid[i / 4 + 1][(i % 4) + 1]].setScale(1 * 1, 1 * 1); }
					}
				}
				if (check == 1) { AddingSquare(); check = 0; }
			}
		}
		window.clear(Color::Black);
		window.draw(sprite[17]);
		window.draw(sprite[18]);
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
			{
				n = grid[j + 1][i + 1];
				sprite[n].setPosition(107 + i * (blockWidht - 9), 85 + j * (blockWidht - 10));
				window.draw(sprite[n]);
			}
		window.display();
	}

	return 0;
}