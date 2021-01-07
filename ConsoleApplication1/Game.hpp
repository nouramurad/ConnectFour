#pragma once

#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <fstream>

class Game
{

public:

	int counter;
	sf::RenderWindow window;

	int winw, winh;

	std::vector<std::vector<sf::RectangleShape>> cells;

	std::vector<std::vector<bool>> last;

	typedef struct Move
	{
		int state[6][7];
	};

	Game();
	~Game();

	void loop();
	void inputs();
	void update();
	void render();

	///////////////// playable ///////////////////

	int simpleEval(int state[6][7]);
	int eval(int state[6][7]);
	int checkPossibleMoves(int player, int state[6][7]);
	bool finished(int state[6][7]);
	std::vector<Game::Move> move(int player, int state[6][7]);
	std::vector<Game::Move> bettermove(int player, int state[6][7]);

	int miniMax(int h, int depth, int game[6][7], int alpha, int beta, bool player);
	int maxMove(int h, int depth, int state[6][7], int alpha, int beta);
	int minMove(int h, int depth, int state[6][7], int alpha, int beta);
	void play();
	int maxPos(int values[7]);
	int minPos(int values[7]);

	bool search(int arr[6][7], std::vector<Move> moves);
	bool equal(int arr1[6][7], int arr2[6][7]);

	int* maxN(int game[6][7], int depth, int player);
	int* eval2(int state[6][7]);
	std::vector<Game::Move> finals;
	std::vector<Game::Move> maxn;
	///////////////// set up ////////////////////
	int mapRows, mapCols;

	sf::Sprite **mapSprites;
	sf::Sprite **mapSprites2;
	int counting;

	sf::Sprite boardsprite;
	sf::Image player1;
	sf::Image player2;
	sf::Image empty;
	sf::Texture player2Texture;
	sf::Texture player1Texture;
	sf::Texture emptyTexture;
	int crateY;
	int floorY;

	bool Done;
	bool firstP;

	int  board[6][7];
	int  Secondboard[6][7];

};
