/*
Author: Aakash Venkataraman
Class :  ECE6122 - A
Last Date Modified: 10-26-2022
Description:
This program implements a pacman game using SFML. It also includes many features, like pacman going through tunnel, pacman eating ghost after eating powerup.
All features mentioned in the question have been implemented.
*/

// Include important C++ libraries here
#include <sstream>
#include <SFML/Graphics.hpp>
#include <vector>

// Make code easier to type with "using namespace"
using namespace sf;



int next_direction(int curr_dir, int x, int y);
int power_up_time = 0;
int left = 0, right = 0, up = 0, down = 0;

int pac_direction = 0;
bool ghost_eat_pac = false;
bool no_coins_remaining = false;
bool ghost0_gone = false, ghost1_gone = false, ghost2_gone = false, ghost3_gone = false;
bool power_up_status[4] = { true,true,true,true };
bool pac_powered_up = false;

const int GHOSTS = 4;

int grid[17][17]{ {1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1},
	{1,0,0,1,0,0,0,1,0,1,0,0,0,1,0,0,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,0,0,1,0,1,0,0,0,0,0,1,0,1,0,0,1},
	{1,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,1},
	{0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0},
	{0,0,0,1,0,1,1,1,1,1,1,1,0,1,0,0,0},
	{1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1},
	{0,0,0,1,0,1,0,1,1,1,0,1,0,1,0,0,0},
	{0,0,0,1,0,1,1,1,1,1,1,1,0,1,0,0,0},
	{0,0,0,1,0,1,0,0,0,0,0,1,0,1,0,0,0},
	{1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1},
	{1,1,0,1,1,1,1,1,1,1,1,1,1,1,0,1,1},
	{0,1,0,1,0,0,0,0,0,0,0,1,0,1,0,1,0},
	{1,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,1},
	{1,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1} };

// List of function declarations which are used later in the program
void updateBranches(int seed);
void ghost_movement(Sprite& mov_ghost, int dir);
void pac_movement(Sprite& mov_pac, int dir);
void update_coins(int a, int b);
void check_ghost_ate_pac(Sprite& ghost1, Sprite& ghost2, Sprite& ghost3, Sprite& ghost4, Sprite& pac);
void check_no_coins_remaining();
void power_up_pac(int a, int b);

int path_finder(int i, int j, int curr_dir)
{
	int return_dir = 0;
	int possible_dir[4] = { 0,0,0,0 };



	if (i == 0 && j == 0)
	{
		possible_dir[0] = 0;
		possible_dir[1] = 0;
		possible_dir[2] = grid[i][j + 1];
		possible_dir[3] = grid[i + 1][j];
	}
	else if (i == 0 && j == 16)
	{
		possible_dir[0] = grid[i][j - 1];
		possible_dir[1] = 0;
		possible_dir[2] = 0;
		possible_dir[3] = grid[i + 1][j];
	}
	else if (i == 16 && j == 0)
	{
		possible_dir[0] = 0;
		possible_dir[1] = grid[i - 1][j];
		possible_dir[2] = grid[i][j + 1];
		possible_dir[3] = 0;
	}
	else if (i == 16 && j == 16)
	{
		possible_dir[0] = grid[i][j - 1];
		possible_dir[1] = grid[i - 1][j];
		possible_dir[2] = 0;
		possible_dir[3] = 0;
	}
	else if (i == 0)
	{
		return 2;

	}
	else if (j == 0)
	{
		return 1;

	}
	else if (i == 16)
	{
		return -2;

	}
	else if (j == 16)
	{
		return -1;

	}
	else if (curr_dir == -1)
	{
		if (grid[i][j - 1] == 1)
			return -1;

		if (grid[i - 1][j] == 1)
			possible_dir[1] = 1;
		if (grid[i][j + 1] == 1)
			possible_dir[2] = 1;
		if (grid[i + 1][j] == 1)
			possible_dir[3] = 1;
	}
	else if (curr_dir == 1)
	{
		if (grid[i][j + 1] == 1)
			return 1;
		if (grid[i][j - 1] == 1)
			possible_dir[0] = 1;
		if (grid[i - 1][j] == 1)
			possible_dir[1] = 1;
		if (grid[i + 1][j] == 1)
			possible_dir[3] = 1;


	}
	else if (curr_dir == -2)
	{
		if (grid[i - 1][j] == 1)
			return -2;
		if (grid[i][j - 1] == 1)
			possible_dir[0] = 1;
		if (grid[i][j + 1] == 1)
			possible_dir[2] = 1;
		if (grid[i + 1][j] == 1)
			possible_dir[3] = 1;

	}
	else if (curr_dir == 2)
	{
		if (grid[i + 1][j] == 1)
			return 2;
		if (grid[i][j - 1] == 1)
			possible_dir[0] = 1;
		if (grid[i - 1][j] == 1)
			possible_dir[1] = 1;
		if (grid[i][j + 1] == 1)
			possible_dir[2] = 1;


	}

	while (1)
	{
		int k = rand() % 4;
		if (possible_dir[k] != 0)
		{
			if (k == 0)
				return_dir = -1;
			else if (k == 1)
				return_dir = -2;
			else if (k == 2)
				return_dir = 1;
			if (k == 3)
				return_dir = 2;
			break;
		}
	}

	return return_dir;
}
Sprite ghost[GHOSTS];

int ghost0_dir, ghost1_dir, ghost2_dir, ghost3_dir;

Sprite spriteGhost[4];

Time dt;

int coins[17][17];


int main()
{

	for (int m = 0; m < 17; m++)
		for (int n = 0; n < 17; n++)
			coins[m][n] = grid[m][n];

	coins[6][5] = coins[6][6] = coins[6][7] = coins[6][8] = coins[6][9] = coins[6][10] = coins[6][11] = 0;
	coins[7][4] = coins[7][5] = coins[7][11] = coins[7][12] = 0, coins[8][4] = coins[8][11] = 0;
	coins[9][5] = coins[9][6] = coins[9][7] = coins[9][8] = coins[9][9] = coins[9][10] = coins[9][11] = 0;

	// Create a video mode object
	VideoMode vm(578, 578);

	// Create and open a window for the game
	RenderWindow window(vm, "PacMan!!!");// , Style::Fullscreen);

	// Create a texture to hold a graphic on the GPU
	Texture textureBackground;

	// Load a graphic into the texture
	textureBackground.loadFromFile("graphics\\maze.bmp");

	// Create a sprite
	Sprite spriteBackground;

	// Attach the texture to the sprite
	spriteBackground.setTexture(textureBackground);

	// Set the spriteBackground to cover the screen
	spriteBackground.setPosition(0, 0);

	// Make a tree sprite
	Texture texturePac;
	texturePac.loadFromFile("graphics\\pacman.bmp");
	Sprite spritePac;
	spritePac.setTexture(texturePac);
	spritePac.setPosition(276, 420);

	// make 3 ghost sprites from 1 texture
	Texture textureGhost0, textureGhost1, textureGhost2, textureGhost3;
	// Load 1 new texture
	textureGhost0.loadFromFile("graphics\\orange_ghost.bmp");
	textureGhost1.loadFromFile("graphics\\pink_ghost.bmp");
	textureGhost2.loadFromFile("graphics\\blue_ghost.bmp");
	textureGhost3.loadFromFile("graphics\\red_ghosts.bmp");

	// 3 New sprites withe the same texture
	spriteGhost[0].setTexture(textureGhost0);
	spriteGhost[1].setTexture(textureGhost1);
	spriteGhost[2].setTexture(textureGhost2);
	spriteGhost[3].setTexture(textureGhost3);

	// Position the ghosts off screen
	spriteGhost[0].setPosition(272, 204);
	spriteGhost[1].setPosition(272, 204);
	spriteGhost[2].setPosition(272, 204);
	spriteGhost[3].setPosition(272, 204);

	// Are the ghosts currently on screen?
	bool ghost0Alive = false;
	bool ghost1Alive = false;
	bool ghost2Alive = false;
	bool ghost3Alive = false;

	// Variables to control time itself
	Clock clock;
	// Time bar

	Time gameTimeTotal;
	float timeRemaining = 6.0f;

	// Track whether the game is running
	bool paused = true;
	// Draw some text

	sf::Text messageText;

	// We need to choose a font
	sf::Font font;
	font.loadFromFile("fonts/KOMIKAP_.ttf");

	// Set the font to our message
	messageText.setFont(font);

	// Assign the actual message
	messageText.setString("Press Enter to start!");

	// Make it really big
	messageText.setCharacterSize(25);

	// Choose a color
	messageText.setFillColor(Color::White);

	// Position the text
	FloatRect textRect = messageText.getLocalBounds();
	messageText.setOrigin(textRect.left +
		textRect.width / 2.0f,
		textRect.top +
		textRect.height / 2.0f);

	messageText.setPosition(578 / 2.0f, 578 / 2.0f);

	ghost[0].setTexture(textureGhost0);
	ghost[0].setPosition(272, 204);
	ghost[1].setTexture(textureGhost1);
	ghost[1].setPosition(272, 204);
	ghost[2].setTexture(textureGhost2);
	ghost[2].setPosition(272, 204);
	ghost[3].setTexture(textureGhost3);
	ghost[3].setPosition(272, 204);

	// Prepare 5 branches
//	Texture textureBranch;
//	textureBranch.loadFromFile("graphics/pink_ghost.png");

	sf::CircleShape powerup[4];
	for (int y = 0; y < 4; y++)
	{

		if (power_up_status[y] == true)
			powerup[y].setRadius(4);
		else
			powerup[y].setRadius(0);

		powerup[y].setOutlineColor(sf::Color::White);
		powerup[y].setOutlineThickness(2);
	}

	powerup[0].setPosition(17, 51);
	powerup[1].setPosition(561, 51);
	powerup[2].setPosition(17, 425);
	powerup[3].setPosition(561, 425);

	sf::CircleShape coin;
	coin.setRadius(2);
	coin.setOutlineColor(sf::Color::White);

	while (window.isOpen())
	{
		int pac_x_corr = spritePac.getPosition().x + 17;
		int pac_y_corr = spritePac.getPosition().y + 17;

		if (pac_direction == -2)
			pac_y_corr += 34;

		else if (pac_direction == -1)
			pac_x_corr += 34;

		else if (pac_direction == 2)
			pac_y_corr -= 34;

		else if (pac_direction == 1)
			pac_x_corr -= 34;

		pac_direction = 0;

		pac_x_corr = floor(pac_x_corr / 34);
		pac_y_corr = floor(pac_y_corr / 34);

		if (Keyboard::isKeyPressed(Keyboard::Left) || left)
		{
			left = 1;
			right = 0;
			up = 0;
			down = 0;

			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				right = 1;
				left = 0;
			}
			else if (Keyboard::isKeyPressed(Keyboard::Up))
			{
				up = 1;
				left = 0;
			}
			else if (Keyboard::isKeyPressed(Keyboard::Down))
			{
				down = 1;
				left = 0;
			}


			if (pac_x_corr == 0 && pac_y_corr == 7)
				spritePac.setPosition(561, 238);

			else if (pac_x_corr == 0)
				spritePac.setPosition(spritePac.getPosition());

			else if(grid[pac_y_corr][pac_x_corr-1] != 0)
			  pac_direction = -1;
		}
		else if (Keyboard::isKeyPressed(Keyboard::Right) || right)
		{
			left = 0;
			right = 1;
			up = 0;
			down = 0;

			if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				left = 1;
				right = 0;
			}
			else if (Keyboard::isKeyPressed(Keyboard::Up))
			{
				up = 1;
				right = 0;
			}
			else if (Keyboard::isKeyPressed(Keyboard::Down))
			{
				down = 1;
				right = 0;
			}


			if (pac_x_corr == 16 && pac_y_corr == 7)
				spritePac.setPosition(17, 238);

			else if (pac_x_corr == 16)
				spritePac.setPosition(spritePac.getPosition());

			else if (grid[pac_y_corr][pac_x_corr + 1] != 0)
			  pac_direction = 1;
			else if (grid[pac_y_corr][pac_x_corr + 1] == 0)
				spritePac.setPosition(spritePac.getPosition());
			pac_direction = 1;

		}
		else if (Keyboard::isKeyPressed(Keyboard::Up) || up)
		{
			left = 0;
			right = 0;
			up = 1;
			down = 0;

			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				right = 1;
				up = 0;
			}
			else if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				left = 1;
				up = 0;
			}
			else if (Keyboard::isKeyPressed(Keyboard::Down))
			{
				down = 1;
				up = 0;
			}
			if (pac_y_corr == 0)
			{
			  spritePac.setPosition(spritePac.getPosition());
			}

			else if (pac_y_corr > 0)
			{
			   if (grid[pac_y_corr - 1][pac_x_corr] != 0)
				pac_direction = -2;
			}
				

			
		}
		else if (Keyboard::isKeyPressed(Keyboard::Down) || down)
		{
			left = 0;
			right = 0;
			up = 0;
			down = 1;

			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				right = 1;
				down = 0;
			}
			else if (Keyboard::isKeyPressed(Keyboard::Up))
			{
				up = 1;
				down = 0;
			}
			else if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				left = 1;
				down = 0;
			}

			else if (pac_y_corr == 16)
				spritePac.setPosition(spritePac.getPosition());

			else if (grid[pac_y_corr+1][pac_x_corr] != 0)
			  pac_direction = 2;
			
		}


		pac_movement(spritePac, pac_direction);
		check_ghost_ate_pac(spriteGhost[0], spriteGhost[1], spriteGhost[2], spriteGhost[3], spritePac);
		check_no_coins_remaining();



		for (int y = 0; y < 4; y++)
		{
			if (power_up_status[y] == true)
				powerup[y].setRadius(4);
			else
				powerup[y].setRadius(0);
		}

		/*
		****************************************
		Handle the players input
		****************************************
		*/

		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			ghost_eat_pac = 1; //Just setting to end the game

		}

		// Start the game
		if (Keyboard::isKeyPressed(Keyboard::Return))
		{
			paused = false;

			// Reset the time and the score
		//	score = 0;
			timeRemaining = 500000;

		}

		/*
		****************************************
		Update the scene
		****************************************
		*/
		if (!paused)
		{

			// Measure time
			dt = clock.restart();

			// Subtract from the amount of time remaining
			timeRemaining -= dt.asSeconds();

			power_up_time -= dt.asSeconds();
			// size up the time bar

			if (power_up_time <= 0)
				pac_powered_up = 0;



			if (timeRemaining <= 0.0f || ghost_eat_pac == true || no_coins_remaining == true) {

				// Pause the game
				paused = true;
				pac_direction = 0;



				// Change the message shown to the player
				messageText.setString("Game Over!!");

				//Reposition the text based on its new size
				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left +
					textRect.width / 2.0f,
					textRect.top +
					textRect.height / 2.0f);

				messageText.setPosition(578 / 2.0f, 578 / 2.0f);
				window.close();

			}




			if (ghost0_gone)
			{
				spriteGhost[0].setPosition(1000, 1000);
			}
			else if (!ghost0Alive)
			{
				ghost0_dir = 1;
				spriteGhost[0].setPosition(272, 204);
				ghost0Alive = true;
			}
			else
			{
				ghost0_dir = next_direction(ghost0_dir, spriteGhost[0].getPosition().x, spriteGhost[0].getPosition().y);
				ghost_movement(spriteGhost[0], ghost0_dir);
			}

			if (ghost1_gone)
			{
				spriteGhost[1].setPosition(1000, 1000);
			}
			else if (!ghost1Alive)
			{
				spriteGhost[1].setPosition(272, 204);
				ghost1Alive = true;
				ghost1_dir = -1;
			}
			else
			{
				ghost1_dir = next_direction(ghost1_dir, spriteGhost[1].getPosition().x, spriteGhost[1].getPosition().y);
				ghost_movement(spriteGhost[1], ghost1_dir);
			}

			if (ghost2_gone)
			{
				spriteGhost[2].setPosition(1000, 1000);
			}
			else if (!ghost2Alive)
			{
				spriteGhost[2].setPosition(272, 204);
				ghost2Alive = true;
				ghost2_dir = 2;
			}
			else
			{
				ghost2_dir = next_direction(ghost2_dir, spriteGhost[2].getPosition().x, spriteGhost[2].getPosition().y);
				ghost_movement(spriteGhost[2], ghost2_dir);
			}


			if (ghost3_gone)
			{
				spriteGhost[3].setPosition(1000, 1000);
			}
			else if (!ghost3Alive)
			{
				spriteGhost[3].setPosition(272, 204);
				ghost3Alive = true;
				ghost3_dir = -2;
			}
			else
			{
				ghost3_dir = next_direction(ghost3_dir, spriteGhost[3].getPosition().x, spriteGhost[3].getPosition().y);
				ghost_movement(spriteGhost[3], ghost3_dir);
			}

		}// End if(!paused)

		 /*
		 ****************************************
		 Draw the scene
		 ****************************************
		 */

		 // Clear everything from the last frame
		window.clear();

		// Draw our game scene here
		window.draw(spriteBackground);

		for (int i = 0; i < 17; i++)
			for (int j = 0; j < 17; j++)
			{
				if (coins[i][j] == 1)
				{
					coin.setPosition(17 + j * 34, 17 + i * 34);
					window.draw(coin);
				}
			}

		// Draw the ghosts
		window.draw(spriteGhost[0]);
		window.draw(spriteGhost[1]);
		window.draw(spriteGhost[2]);
		window.draw(spriteGhost[3]);

		// Draw the pacman
		window.draw(spritePac);

		for (int i = 0; i < 4; i++) {
			window.draw(powerup[i]);
		}

		if (paused)
		{
			// Draw our message
			window.draw(messageText);
		}

		// Show everything we just drew
		window.display();


	}

	return 0;
}


void pac_movement(Sprite& mov_pac, int dir)
{
	if (dir == -1)
	{
		mov_pac.setPosition(
			mov_pac.getPosition().x -
			(80 * dt.asSeconds()),
			mov_pac.getPosition().y);
	}
	else if (dir == 1)
	{
		mov_pac.setPosition(
			mov_pac.getPosition().x +
			(80 * dt.asSeconds()),
			mov_pac.getPosition().y);
	}
	else if (dir == -2)
	{
		mov_pac.setPosition(
			mov_pac.getPosition().x,
			mov_pac.getPosition().y - (80 * dt.asSeconds()));
	}
	else if (dir == 2)
	{
		mov_pac.setPosition(
			mov_pac.getPosition().x,
			mov_pac.getPosition().y + (80 * dt.asSeconds()));
	}

	if (dir != 0)
		update_coins(floor(mov_pac.getPosition().x / 34), floor(mov_pac.getPosition().y / 34));
}

void power_up_pac(int a, int b)
{
	if (power_up_status[0] && a == 0 && b == 1)
	{
		power_up_status[0] = false;
		pac_powered_up = true;
		power_up_time = 5000;
	}
	else if (power_up_status[1] && a == 16 && b == 1)
	{
		power_up_status[1] = false;
		pac_powered_up = true;
		power_up_time = 5000;
	}
	else if (power_up_status[2] && a == 0 && b == 12)
	{
		power_up_status[2] = false;
		pac_powered_up = true;
		power_up_time = 5000;
	}
	else if (power_up_status[3] && a == 16 && b == 12)
	{
		power_up_status[3] = false;
		pac_powered_up = true;
		power_up_time = 5000;
	}
}



void check_ghost_ate_pac(Sprite& ghost1, Sprite& ghost2, Sprite& ghost3, Sprite& ghost4, Sprite& pac)
{   //X co-ordinates of pacmans and ghosts
	int pac_x_coordinate = floor(pac.getPosition().x / 34);
	int ghost1_x_coordinate = floor(ghost1.getPosition().x / 34);
	int ghost2_x_coordinate = floor(ghost2.getPosition().x / 34);
	int ghost3_x_coordinate = floor(ghost3.getPosition().x / 34);
	int ghost4_x_coordinate = floor(ghost4.getPosition().x / 34);
	// Y co-ordinates of pacmans and ghosts
	int pac_y_coordinate = floor(pac.getPosition().y / 34);
	int ghost1_y_coordinate = floor(ghost1.getPosition().y / 34);
	int ghost2_y_coordinate = floor(ghost2.getPosition().y / 34);
	int ghost3_y_coordinate = floor(ghost3.getPosition().y / 34);
	int ghost4_y_coordinate = floor(ghost4.getPosition().y / 34);

	int ghost1_hit = (pac_x_coordinate == ghost1_x_coordinate && pac_y_coordinate == ghost1_y_coordinate);
	int ghost2_hit = (pac_x_coordinate == ghost2_x_coordinate && pac_y_coordinate == ghost2_y_coordinate);
	int ghost3_hit = (pac_x_coordinate == ghost3_x_coordinate && pac_y_coordinate == ghost3_y_coordinate);
	int ghost4_hit = (pac_x_coordinate == ghost4_x_coordinate && pac_y_coordinate == ghost4_y_coordinate);

	power_up_pac(pac_x_coordinate, pac_y_coordinate);

	if (ghost1_hit || ghost2_hit || ghost3_hit || ghost4_hit)
	{
		if (pac_powered_up)
		{
			if (ghost1_hit)
				ghost0_gone = true;
			else if (ghost2_hit)
				ghost1_gone = true;
			else if (ghost3_hit)
				ghost2_gone = true;
			else if (ghost4_hit)
				ghost3_gone = true;

		}
		else
			ghost_eat_pac = true;
	}
}

void ghost_movement(Sprite& mov_ghost, int dir)
{
	if (dir == -1)
	{
		mov_ghost.setPosition(
			mov_ghost.getPosition().x -
			(40 * dt.asSeconds()),
			mov_ghost.getPosition().y);
	}
	else if (dir == 1)
	{
		mov_ghost.setPosition(
			mov_ghost.getPosition().x +
			(40 * dt.asSeconds()),
			mov_ghost.getPosition().y);
	}
	else if (dir == -2)
	{
		mov_ghost.setPosition(
			mov_ghost.getPosition().x,
			mov_ghost.getPosition().y - (40 * dt.asSeconds()));
	}
	else if (dir == 2)
	{
		mov_ghost.setPosition(
			mov_ghost.getPosition().x,
			mov_ghost.getPosition().y + (40 * dt.asSeconds()));
	}
}

int next_direction(int curr_dir, int x, int y)
{
	if (curr_dir == -2)
		y += 34;

	if (curr_dir == -1)
		x += 34;

	int a = floor((x) / 34);
	int b = floor((y) / 34);

	int next_dir = path_finder(b, a, curr_dir);
	return next_dir;
}

void update_coins(int a, int b)
{
	coins[b][a] = 0;
}

void check_no_coins_remaining()
{
	int sum = 0;
	for (int i = 0; i < 17; i++)
		for (int j = 0; j < 17; j++)
		{
			sum += coins[i][j];
			if (sum != 0)
				break;
		}
	if (sum == 0)
		no_coins_remaining = true;
}



