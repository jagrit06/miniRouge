// Game.cpp

#include <iostream>

#include "Game.h"
#include "utilities.h"
#include "Player.h"
#include "Monster.h"
#include "Dungeon.h"
#include "GameObject.h"
#include "Weapon.h"
#include "Scroll.h"

using namespace std;

////////////////////////////////////////////////////////////////////
// Game Class Implementations
////////////////////////////////////////////////////////////////////

Game::Game(int goblinSmellDistance)
	: m_goblinSmellDist(goblinSmellDistance)
{
	// Creating a new player
	m_player = new Player();

	// Creating a first level Dungeon
	m_currDungeon = new Dungeon(0, m_player, goblinSmellDistance);
}

Game::~Game()
{
	delete m_currDungeon;
	delete m_player;
}

void Game::play()
{
	// Initializing variables 
	int newRow, newCol;
	Monster* monster;
	GameObject* object;
	string playerResult, monsterResult;

	bool hasWon = false;

	m_currDungeon->display(""); // Display the first level

	char command = getCharacter(); // Get the first command

    while (command != 'q')    // While the player has not quit
	{
		playerResult = ""; // Result is set to nothing by default

		// If player is sleeping, skipping executing command
		if(m_player->isAsleep())
		{
			m_player->reduceSleep(); // Reducing sleep time
			command = 's'; // Dummy command to skip command execution
		}

		// 1 in 10 change the player recovers health
		if(trueWithProbability(1.0/10.0))
		{
			m_player->attemptRegainHealth();
		}
		
		//////////////////////////////////////////////////////
		// Direction Command
		if(commandToNewPos(command, newRow, newCol)) 
		{
			if(m_currDungeon->isWall(newRow, newCol))
				; // Do nothing if player tires to walk into a wall

			else if(
				// If there is a monster at the new position
				(monster = m_currDungeon->monsterAt(newRow, newCol)) != nullptr) 
			{
				// Do attacking
				playerResult = m_player->attemptAttack(monster);
				if(monster->isDead()) // If player killed monster
				{
					// Remove monster from dungeon and drop any items
					m_currDungeon->killMonster(monster); 
				}
			}
			else // Player can move to the new position
			{
				// Move player
				m_player->setPos(newRow, newCol);
				// No update to result string needed here 
			}
		}

		//////////////////////////////////////////////////////
		// Pick up Command
		else if(tolower(command) == 'g')
		{
			if(m_currDungeon->isIdol(m_player->row(),m_player->col()))
			// If player is at idol
			{
				// Game Won!
				playerResult = "You pick up the golden idol\n";     
				playerResult += "Congratulations, you won!";
				hasWon = true; // Indicate game has been won
			}
			
			// Else if player is trying to pick some other obejet
			else if( (object = m_currDungeon->objectAt(m_player->row(),m_player->col())) != nullptr )
			// If there is an object at Player's pos
			{
				playerResult = m_player->attemptPickUp(object);
			}
		}

		//////////////////////////////////////////////////////
		// Display Inventory Command
		else if(command == 'i')
		{
			m_player->displayInventory();
			getCharacter(); // To exit inventory
		}
		
		//////////////////////////////////////////////////////
		// Wield Weapon Command
		else if(command == 'w')
		{
			m_player->displayInventory();
			// Try to wield weapon according to input
			playerResult = m_player->attemptWieldWeapon(getCharacter()); 
		}

		//////////////////////////////////////////////////////
		// Read Scroll Command
		else if(command == 'r')
		{
			m_player->displayInventory();
			// Try to read scroll according to input
			playerResult = m_player->attemptReadScroll(getCharacter()); 
		}

		//////////////////////////////////////////////////////
		// Cheat Command
		else if(command == 'c')
		{
			m_player->cheat();
		}

		//////////////////////////////////////////////////////
		// Go Down Stairs Command
		else if(command == '>' &&
				m_currDungeon->isStair(m_player->row(), m_player->col()) )
		{
			proceedToNextLevel();
		}

		//////////////////////////////////////////////////////
		// Move monsters
		string monsterResult = m_currDungeon->attemptMoveMonsters();

		// Get result of the move to display
		string result;
		if(playerResult != "" && monsterResult != "") 
		// If both player and monster return a result
		// Concatenating them with a line in the middle
			result = playerResult + "\n" + monsterResult;
		else 
			result = playerResult + monsterResult;
		
		//////////////////////////////////////////////////////
		// Checking if game needs to be ended
		if(m_player->isDead() || hasWon) // Player has won or is dead
		{ 
			result += "\nPress q to exit game.";
			m_currDungeon->display(result);
			while(getCharacter() != 'q'); // Wait till player quits
			break; // End game
		}

		//////////////////////////////////////////////////////
		// Display updated grid
		m_currDungeon->display(result);

		//////////////////////////////////////////////////////
		// Take new command
		command = getCharacter();
	}
}

/////////////////////////////////////////////////////////////////
// Private Helper Functions

bool Game::commandToNewPos(char command, int& newRow, int& newCol)
	// Take player move commands and convert them to new position
{
	int currRow = m_player->row();
	int currCol = m_player->col();

	switch(command)
	{
		case ARROW_UP:    currRow--; break; // Move Up 
		case ARROW_DOWN:  currRow++; break; // Move Down 
		case ARROW_LEFT:  currCol--; break; // Move Left 
		case ARROW_RIGHT: currCol++; break; // Move Right
		default: return false; // Indicating command was not a direction
	}

	// Update the new position
	newRow = currRow; 
	newCol = currCol;
	return true;
}

void Game::proceedToNextLevel()
{
	int currLevel = m_currDungeon->level();

	// Generate new level
	Dungeon* newLevel = new Dungeon(currLevel+1, m_player, m_goblinSmellDist);

	// Clear previous level
	delete m_currDungeon;

	// Update game
	m_currDungeon = newLevel;
}

// You will presumably add to this project other .h/.cpp files for the
// various classes of objects you need to play the game:  player, monsters,
// weapons, etc.  You might have a separate .h/.cpp pair for each class
// (e.g., Player.h, Boegeyman.h, etc.), or you might put the class
// declarations for all actors in Actor.h, all game objects in GameObject.h,
// etc.