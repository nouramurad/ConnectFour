#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <tuple>
#include <functional>
#include <windows.h>
#include <algorithm>
#include <iterator>
#include <stdlib.h>

#include "Game.hpp"

// constructor
Game::Game()
{

	// seeding the random number generator
	std::srand(std::time(0));

	// setting the window / tile dimensions and computing the grid dimensions
	this->winw = 1146;
	this->winh = 867;
	mapRows = 6;
	mapCols = 7;

	Done = false;
	firstP = false;
	counter = 0;
	// create the window and set framerate limiting
	// n.b., on your first assignment you need to set framerate limiting yourself; this function is not permitted
	this->window.create(sf::VideoMode(this->winw, this->winh), "COMP2501 Demo");
	this->window.setFramerateLimit(15);

	mapSprites = new sf::Sprite *[mapRows];
	for (int i = 0; i < mapRows; i++) {
		mapSprites[i] = new sf::Sprite[mapCols];
	}
	mapSprites2 = new sf::Sprite *[mapRows];
	for (int i = 0; i < mapRows; i++) {
		mapSprites2[i] = new sf::Sprite[mapCols];
	}
	for (int i = 0; i < mapRows; i++)
	{
		for (int j = 0; j < mapCols; j++)
		{
			board[i][j] = 0;
		}
	}
	for (int i = 0; i < mapRows; i++)
	{
		for (int j = 0; j < mapCols; j++)
		{
			Secondboard[i][j] = board[i][j];
		}
	}

}

// destructor
Game::~Game()
{ }

// game loop
void Game::loop()
{

	while (this->window.isOpen())
	{

		inputs();
		update();
		render();

	}

}

// retrieve and process input
void Game::inputs() {

	sf::Event event;

	// consider each event generated by the window
	while (window.pollEvent(event))
	{

		switch (event.type)
		{

			// if the event is the window close button being clicked, close the window
		case sf::Event::Closed:
			window.close();
			break;

			// if the event is a key press...
		case sf::Event::KeyPressed:

			switch (event.key.code)
			{

				// ...and it happens to be the escape key, close the window
			case sf::Keyboard::Escape:
				window.close();
				break;

			}

		}

	}
}

////////////////// game logic //////////////////////

int Game::checkPossibleMoves(int player, int state[6][7])
{
	int ret = 0;
	for (int i = 5; i > -1; i--)
	{
		for (int j = 0; j < 7; j++)
		{
			if (state[i][j] == player)
			{
				if (!(i + 1 < 5 && state[i + 1][j] == player) && !(j - 1 > 0 && state[i][j - 1] == player) && !(i + 1 < 5 && j - 1 > 0 && state[i + 1][j - 1] == player))
				{
					if (i - 1 > -1 && state[i-1][j] == player)
					{
						ret++;
						if (i - 2 > -1 && state[i - 2][j] == player)
						{
							ret++;
							if (i - 3 > -1 && state[i - 3][j] == player)
							{
								ret+=300;
								//std::cout << "Won" << std::endl;
							}
						}
					}
					if (j + 1 < 7 && state[i][j+1] == player)
					{
						ret++;
						if (j + 2 < 7 && state[i][j + 2] == player)
						{
							ret++;
							if (j + 3 < 7 && state[i][j + 3] == player)
							{
								ret+=300;
							//	std::cout << "Won" << std::endl;
							}
						}
					}
					if (i - 1 > -1 && j + 1 < 7 && state[i-1][j+1] == player)
					{
						ret++;
						if (i - 2 > -1 && j + 2 < 7 && state[i - 2][j + 2] == player)
						{
							ret++;
							if (i - 3 > -1 && j + 3 < 7 && state[i - 3][j + 3] == player)
							{
								ret+=300;
								std::cout << "Won" << std::endl;
							}
						}
					}
				}
			}
		}
	}
	return ret;
}
int Game::simpleEval(int state[6][7])
{
	int p1, p2;
	p1 = checkPossibleMoves(1, state);
	p2 = checkPossibleMoves(2, state);
	if (p1 >= p2)
	{
		//std::cout << "here" << std::endl;
		return p1;
	}
	if (p1 < p2)
	{
		//std::cout << "here" << std::endl;
		return p2;
	}
	if (p1 >= 100 || p2 >= 100)
	{
		return 100;
	}
}
int Game::eval(int state[6][7])
{
	int p1, p2;
	p1 = checkPossibleMoves(1, state);
	p2 = checkPossibleMoves(2, state);
	if (p1 >= 100 )
	{
		//std::cout << "here" << std::endl;
		return 500;
		firstP = true;
	}
	if (p2 >= 100)
	{
		return -500;
	}
	else
	{
		return p1 - p2;
	}
}
bool Game::finished(int state[6][7])
{
	int counter = 0;
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			if (state[i][j] == 0)
			{
				counter++;
			}
		}
	}
	if (eval(state) > 100 ||eval(state) < -100)
	{
		//std::cout << "Won" << std::endl;
		Done = true;
	//	std::cout <<simpleEval(state) << std::endl;
		for (int i = 0; i < 6; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				Secondboard[i][j]= state[i][j];
			}
		//	std::cout << "here" << std::endl;
		}
		return true;
	}
	if(counter == 0)
	{
		return true;
	}
	return false;
}
std::vector<Game::Move> Game::move(int player, int state[6][7])
{
	int counted = 0;
	std::vector<Move> moves;
	Move move;
	for (int i = 5; i > -1; i--)
	{
		for (int j = 0; j < 7; j++)
		{
			move.state[i][j] = state[i][j];
			counted += state[i][j];
		}
	}
	moves.push_back(move);

	if (finished(state))
	{
		moves.erase(moves.begin());
		return moves;
	}
	if(counted >0)
	{
		for (int j = 0; j < 7; j++)
		{
			int maxSpot = -999;
			for (int i = 0; i < 6; i++)
			{
				if (state[i][j] == 0)
				{
					maxSpot = std::max(maxSpot, i);
				}
			}
			if (maxSpot >= 0)
			{
				state[maxSpot][j] = player;
				Move random;

				for (int i = 0; i < mapRows; i++)
				{
					for (int j = 0; j < mapCols; j++)
					{
						random.state[i][j] = state[i][j];

					}
				}
				for (int i = 0; i < mapRows; i++)
				{
					for (int j = 0; j < mapCols; j++)
					{
						state[i][j] = moves.at(0).state[i][j];

					}
				}
				if (!search(random.state, finals))
				{
					moves.push_back(random);
					finals.push_back(random);
					//std::cout << finals.size() << std::endl;
				}
			}
		}
	}
	else if (counted == 0)
	{
		int j = rand() % 7;
		int maxSpot = -9999;
		for (int i = 0; i < 6; i++)
		{
			if (state[i][j] == 0)
			{
				maxSpot = std::max(maxSpot, i);
			}
		}
		if (maxSpot >= 0)
		{
			state[maxSpot][j] = player;
			Move random;

			for (int i = 0; i < mapRows; i++)
			{
				for (int j = 0; j < mapCols; j++)
				{
					random.state[i][j] = state[i][j];

				}
			}
			for (int i = 0; i < mapRows; i++)
			{
				for (int j = 0; j < mapCols; j++)
				{
					state[i][j] = moves.at(0).state[i][j];

				}
			}
			if (!search(random.state, finals))
			{
				moves.push_back(random);
				finals.push_back(random);
				//std::cout << finals.size() << std::endl;
			}
		}
	}
	moves.erase(moves.begin());
	return moves;

}
std::vector<Game::Move> Game::bettermove(int player, int state[6][7])
{
	std::vector<Move> moves;
	Move move;
	for (int i = 5; i > -1; i--)
	{
		for (int j = 0; j < 7; j++)
		{
			move.state[i][j] = state[i][j];

		}
	}
	moves.push_back(move);
	int compare = checkPossibleMoves(player, state);
	int count = 0;
	if (finished(state))
	{
		moves.erase(moves.begin());
		return moves;
	}

	for (int j = 0; j < 7; j++)
	{
		int maxSpot = -999;
		for (int i = 0; i < 6; i++)
		{
			if (state[i][j] == 0)
			{
				maxSpot = std::max(maxSpot, i);
			}
		}
		if (maxSpot >= 0)
		{
			state[maxSpot][j] = player;
			Move random;

			for (int i = 0; i < mapRows; i++)
			{
				for (int j = 0; j < mapCols; j++)
				{
					random.state[i][j] = state[i][j];

				}
			}
			for (int i = 0; i < mapRows; i++)
			{
				for (int j = 0; j < mapCols; j++)
				{
					state[i][j] = moves.at(0).state[i][j];

				}
			}
			if (!search(random.state, finals))
			{
				 std::cout << "old " << checkPossibleMoves(player, moves.back().state) << " new " << checkPossibleMoves(player, random.state) << std::endl;
				if (player == 1 && checkPossibleMoves(player, random.state) >= checkPossibleMoves(player, moves.back().state))
				{
					moves.push_back(random);
					finals.push_back(random);
				}
				else if (player == 2 )
				{
					moves.push_back(random);
					finals.push_back(random);
				}
				//std::cout << finals.size() << std::endl;
			}
		}
	}
	moves.erase(moves.begin());
	return moves;

}
int Game::minMove(int h, int depth, int state[6][7], int alpha, int beta)
{
	
	std::vector<Move> moves;
	if (finished(state) || depth >= h)
	{
		for (int k = 0; k < mapRows; k++)
		{
			for (int j = 0; j < mapCols; j++)
			{
				board[k][j] = state[k][j];
			}
			//	std::cout<< std::endl;
		}
		return simpleEval(state);
		
	}
	int value = 99999;
	moves = bettermove(2,state);
	counting = counting + moves.size();
//	std::cout <<"in minimive" << moves.size() << std::endl;
	for (int i = 0; i < moves.size(); i++)
	{
		value = std::min(value, maxMove(h, depth + 1, moves.at(i).state, alpha, beta));
		beta = std::min(beta, value);
		if (beta <= alpha)
		{
			return beta;
		}
		
	}
	return value;
}
int Game::maxMove(int h, int depth, int state[6][7], int alpha, int beta)
{std::vector<Move> moves;
if (finished(state) || depth >= h)
{
	for (int k = 0; k < mapRows; k++)
	{
		for (int j = 0; j < mapCols; j++)
		{
			board[k][j] = state[k][j];
		}
		//	std::cout<< std::endl;
	}
	return simpleEval(state);
	
}

int value = -99999;
moves = bettermove(1, state);
counting = counting + moves.size();
//std::cout << "in maxmive" << moves.size() << std::endl;
for (int i = 0; i < moves.size(); i++)
{
	value = std::max(value, minMove(h, depth + 1, moves.at(i).state, alpha, beta));
	alpha = std::max(alpha, value);
	if (alpha >= beta)
	{
		return alpha;
	}
}
return value;
}
int Game::miniMax(int h, int depth, int game[6][7], int alpha, int beta, bool player)
{

	std::vector<Move> operators;
	if (player)
	{
		int value[7];
		int maxOp = -99999;
		operators = bettermove(1,game);
		//	operators = moves1h(game);

		for (int i = 0; i < operators.size(); i++)
		{
			maxOp = std::max(maxOp, maxMove(h, depth + 1, operators.at(i).state, alpha, beta));
			value[i] = (maxMove(h, depth + 1, operators.at(i).state, alpha, beta));
		}
		//finals.push_back(operators.at(maxPos(value)));
		for (int i = 0; i < 6; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				board[i][j] = operators.at(maxPos(value)).state[i][j];
			}
		}
		return value[(minPos(value))];
	}
	if (!player)
	{
		int value[7];
		int maxOp = 99999;
		operators = bettermove(2, game);
		for (int i = 0; i < operators.size(); i++)
		{
			maxOp = std::min(maxOp, minMove(h, depth + 1, operators.at(i).state, alpha, beta));
			value[i] = (minMove(h, depth + 1, operators.at(i).state, alpha, beta));
			
		}
	//	finals.push_back(operators.at(maxPos(value)));
		for (int i = 0; i < 6; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				board[i][j] = operators.at(maxPos(value)).state[i][j];
			}
		}
		return value[(maxPos(value))];
	}
	
}
void Game::play()
{
	bool p1 = true;
	bool p2 = false;
	while (!finished(board) && !Done)
	{
		std::cout << "number of nodes visited so far = " << finals.size() << std::endl << std::endl;
		if (p1)
		{
			//maxN(board, 2, 1);
			miniMax(1, 0, board, -99, 99, p1);
			p1 = false;
			p2 = true;
		}
		if (p2)
		{
			//maxN(board, 2, 2);
			miniMax(1, 0, board, -99, 99, p1);
			p1 = true;
			p2 = false;
		}
		for (int i = 0; i < 6; i++)
		{
			for (int j = 0; j < 7; j++)
			{
		//	std::cout << board[i][j];
			}
			//std::cout << std::endl;
		}
	}
}
int Game::maxPos(int values[7])
{
	return std::distance(values, std::max_element(values, values + 7));
}
int Game::minPos(int values[7])
{
	return std::distance(values, std::min_element(values, values + 7));
}
bool Game::search(int arr[6][7], std::vector<Move> moves)
{
	bool found = false;
	if (moves.size() == 0)
	{
		return found;
	}
	for (int i = 0; i < moves.size(); i++)
	{

		if (equal(arr, moves.at(i).state))
		{
			found = true;
		}
	}
	return found;
}
bool Game::equal(int arr1[6][7], int arr2[6][7])
{
	int count = 0;
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			if (arr1[i][j] == arr2[i][j])
			{
				count = count + 1;
			}
		}
	}
	if (count == 42)
	{
		return true;
	}
	else
	{
		return false;
	}
}
int* Game::maxN(int game[6][7], int depth, int player)
{
	if (finished(game) || depth <= 0)
	{
		for (int k = 0; k < mapRows; k++)
		{
			for (int j = 0; j < mapCols; j++)
			{
				board[k][j] = game[k][j];
			}
		}
		return eval2(game);
	}
	else
	{	
		int value = -99999;
		int tuple[2];
		int* ret;
		tuple[0] = 0;
		tuple[1] = 0;
		ret = tuple;
		std::vector<Move> moves = move(player, game);
		for (int i = 0; i < moves.size(); i++)
		{
			int checker = maxN(moves.at(i).state, depth - 1, player)[player - 1];
			//value = std::max(value, maxN(moves.at(i).state, depth - 1, player)[player - 1]);
		//	std::cout << checker << std::endl;
			if (value != checker)
			{
				value = checker;
			//	std::cout << "Here" << std::endl;
				ret[0] = maxN(moves.at(i).state, depth - 1, player)[0];
				ret[1] = maxN(moves.at(i).state, depth - 1, player)[1];
				
				counter++;
				for (int k = 0; k < mapRows; k++)
				{
					for (int j = 0; j < mapCols; j++)
					{
						board[k][j] = moves.at(i).state[k][j];
					}
					//std::cout<< counter<<std::endl;
				}
			}
		}
	//	std::cout << ret[0] << std::endl;
		//std::cout << ret[1] << std::endl;
		return ret;
	}
}
int* Game::eval2(int state[6][7])
{
	int p1, p2;
	int result[2];
	int* ret;
	p1 = checkPossibleMoves(1, state);
	p2 = checkPossibleMoves(2, state);
	if (p1 >= 100)
	{
		result[0] = 500;
		result[1] = p2;
		ret = result;
	}
	if (p2 >= 100)
	{
		result[0] = p1;
		result[1] = 500;
		ret = result;
	}
	else
	{
		result[0] = p1;
		result[1] = p2;
		ret = result;
	}
	return ret;
}
/*
bool Game::search(int arr[][6], std::vector<Move> moves)
int Game::eval(int state[6][7]);
int Game::checkPossibleMoves(bool player, int state[6][7]);
bool Game::finished(int state[6][7]);
int Game::miniMax(int h, int depth, int game[6][7], int alpha, int beta, bool player);
int Game::maxMove(int h, int depth, int state[6][7], int alpha, int beta);
int Game::minMove(int h, int depth, int state[6][7], int alpha, int beta);
std::vector<Game::Move> Game::move(int state[6][7]);
void Game::play();
*/

//////////////////////////////////////////////////////

// update the internal representation of the game world
void Game::update() {

	player1.loadFromFile("Assets/player1.png");
	player1.createMaskFromColor(sf::Color::White);
	player2.loadFromFile("Assets/player2.png");
	player2.createMaskFromColor(sf::Color::White);
	empty.loadFromFile("Assets/empty.png");
	empty.createMaskFromColor(sf::Color::White);

	player1Texture.loadFromImage(player1);
	player1Texture.setSmooth(true);
	player2Texture.loadFromImage(player2);
	player2Texture.setSmooth(true);
	emptyTexture.loadFromImage(empty);
	emptyTexture.setSmooth(true);
	crateY = player2Texture.getSize().y;
	floorY = player1Texture.getSize().y;


	for (int i = 0; i < mapRows; i++)
	{
		for (int j = 0; j < mapCols; j++)
		{
			mapSprites[i][j].setPosition(j * 160 + 30, i * 140 +15);
			if (board[i][j] == 2)
			{
				mapSprites[i][j].setTexture(this->player2Texture);
			}
			if (board[i][j] == 1)
			{
				mapSprites[i][j].setTexture(this->player1Texture);
			}
			if (board[i][j] == 0)
			{
				mapSprites[i][j].setTexture(this->emptyTexture);
			}
		}
	}
	if(!Done)
	{play(); }
	
	//sf::Time t1 = sf::milliseconds(90);
	for (int k = 0; k < mapRows; k++)
	{
		for (int j = 0; j < mapCols; j++)
		{
			board[k][j] = Secondboard[k][j];
		}
		//	std::cout<< std::endl;
	}
	
}

// render the visual representation of the world
void Game::render() {

	// clear the window
	this->window.clear();

	// draw each rectangle shape and update the 2d vector of Booleans accordingly
//	this->window.draw(boardsprite);
	sf::Color color(12, 89, 241);
	this->window.clear(color);
	for (int i = 0; i < mapRows; i++) {
		for (int j = 0; j < mapCols; j++) {
			this->window.draw(mapSprites2[i][j]);
			this->window.draw(mapSprites[i][j]);

		}
	}
	// display the window
	this->window.display();

}