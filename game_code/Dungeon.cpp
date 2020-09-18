#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

#include "Dungeon.h"
#include "Actor.h"
#include "Player.h"
#include "Monster.h"
#include "GameObject.h"
#include "Scroll.h"
#include "Weapon.h"
#include "utilities.h"

using namespace std;

class Player;

////////////////////////////////////////////////////////////////////
// Dungeon Class Implementation
////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
// Constructor and Destructor

Dungeon::Dungeon(int level, Player* player, int goblinSmellDist, int rows , int cols )
	: m_rows(rows), m_cols(cols), m_level(level), m_goblinSmellDist(goblinSmellDist)
{
	if(rows < 0 || rows > MAXROWS || cols < 0 || cols > MAXCOLS)
	{
		cerr << "Error: Invalid row or column number passed to Dungeon Contructor" << endl;
		exit(1);
	}

	// Generating rooms
	generateRooms();
	
	// Generating monsters
	int monsterNum = randInt(2, 5*(level + 1) + 1); // No. of monsters to add
	for (int i = 0; i < monsterNum; i++)
	{
		addNewMonster();  // Add new monster to the dungeon
	}

	// Adding stair or idol to base grid depending on level
	int newRow, newCol;
	produceOpenPos(newRow, newCol);
	m_baseGrid[newRow][newCol] = (level == 4)? IDOL : STAIR; 

	// Generating objects
	int objectNum = randInt(2,3); // Number of GameObjects to add
	for (int i = 0; i < objectNum; i++)
	{
		addNewObject();  // Add new object to the dungeon
	}

	// Placing Player on a random position
	addPlayer(player);
}

Dungeon::~Dungeon()
{
	// Clear objects
	for(int i = 0; i < m_objects.size(); i++)
	{
		delete m_objects[i]; 
	}
	// Clear Monsters
	for(int i = 0; i < m_monsters.size(); i++)
	{
		delete m_monsters[i]; 
	}
}

/////////////////////////////////////////////////////////////////
// Helper Functions

void Dungeon::produceOpenPos(int& row, int& col) const
	// Produce a random position in the Dungeon that the player 
	// can occupy. Can be location of an object
{
	int newRow, newCol;
	// Keep producing new positions until a free position is found
	do{
		newRow = randInt(rows());
		newCol = randInt(cols());
	}while(!canPlayerOccupy(newRow, newCol));
	row = newRow;
	col = newCol;
}

void Dungeon::produceEmptyPos(int& row, int& col) const
	// Produce a totally free random position in the Dungeon
	// Cannot be location of an object
{
	int newRow, newCol;
	// Keep producing new positions until a free position is found
	do{
		newRow = randInt(rows());
		newCol = randInt(cols());
	}while(!(m_baseGrid[row][col] != ' ')
			&& !monsterAt(row, col)
			&& !objectAt(row, col));
	row = newRow;
	col = newCol;
}

Monster* Dungeon::monsterAt(int row, int col) const
	// If there is a monster at (row, col), return a pointer to it
	// If there isn't a monster there, return nullptr
{
	if(isWall(row, col)) return nullptr; //Monster can't be on a wall

	// Loop over list of all Monsters and check for monster at
	for(int i = 0; i < m_monsters.size(); i++)
	{
		if(row ==m_monsters[i]->row() && col ==m_monsters[i]->col())
			return m_monsters[i];
	} 
	// If nothing found
	return nullptr;
}

GameObject* Dungeon::objectAt(int row, int col) const
	// If there is an object at (row, col), return a pointer to it
	// If there isn't an object there, return nullptr
{
	if(isWall(row, col)) return nullptr; // Object can't be on a wall

	// Loop over list of all objects and check for object at
	for(int i = 0; i < m_objects.size(); i++)
	{
		if(row == m_objects[i]->row() && col == m_objects[i]->col())
			return m_objects[i];
	} 
	// If nothing found
	return nullptr;
}

/////////////////////////////////////////////////////////////////
// Private Helper Functions

void Dungeon::generateRooms()
	// Set base grid to have randomly generated rooms and connect them
{
	// Set everything on the grid to walls
	for(int i = 0; i < rows(); i++)
		for(int j = 0; j < cols(); j++)
			m_baseGrid[i][j] = WALL;

	// Find the number of rooms to be placed together 
	// in horizontal direction
	int numOfRoomsC = randInt(4,6);

	// Finding max width
	int maxDc = (cols() - 3 - numOfRoomsC)/numOfRoomsC;
	// Fidning Max height
	int maxDr = (rows() - 5);

	Room prevRoom(0,0,0,0);

	// Generate each room
	for(int j = 0; j < numOfRoomsC; j++)
	{	
		Room thisRoom; // Generate a new room

		thisRoom.m_dc = randInt(5, maxDc); // Deciding room width
		thisRoom.m_dr = randInt(3, maxDr); // Deciding room height

		// Deciding the top left corner depending on where the 
		// previous room is and the dimensions of this room
		int maxUpper = rows() - 1 - maxDr;
		int prevLower = prevRoom.m_r + prevRoom.m_dr;

		// Ensuring the new room shares a wall with the prev room
		if(prevLower != 0 && prevLower <  maxUpper) 
			maxUpper = prevLower - 1;
		
		thisRoom.m_r = randInt(2 ,maxUpper);
		thisRoom.m_c =  randInt(1 + prevRoom.m_c + prevRoom.m_dc, 
		 1 + prevRoom.m_c + prevRoom.m_dc + maxDc - thisRoom.m_dc);

		// Clear this room
		for(int i=thisRoom.m_r; i < thisRoom.m_r+thisRoom.m_dr; i++)
			for(int j=thisRoom.m_c; j<thisRoom.m_c+thisRoom.m_dc; j++)
				m_baseGrid[i][j] = ' ';

		// Clear a path between this and the last room
		if(prevRoom.m_dr != 0 && prevRoom.m_dc != 0)
		{
			// Determining bounds on corridor height

			// Highest a corridor can be
			int r1 = max(thisRoom.m_r, prevRoom.m_r);
			// Lowest a corridor can be
			int r2 = min(thisRoom.m_r + thisRoom.m_dr - 1, 
						prevRoom.m_r + prevRoom.m_dr - 1);
			
			int cr = randInt(r1, r2);  // Corridor height

			// Clear the corridor
			for(int i = prevRoom.m_c + prevRoom.m_dc; i < thisRoom.m_c; i++)
			{
				m_baseGrid[cr][i] = ' ';
			}	
		}
		prevRoom = thisRoom;
	}	
}

GameObject* Dungeon::addNewObject()
	// Adds a new dynamically allocated GameObject and drops 
	// in the Dungeon. Does not produce an object that needs to 
	// dropped by a monster's death
	// Returns pointer to the newly added item
{
	// Produce an open position to place the object in
	int row, col;
	produceOpenPos(row, col);

	GameObject* object;

	// Randomly selecting an item type
	int selection = randInt(0,9);

	switch(selection)
	{
		// Weapons (with additional cases to make weapon more likely)
		case 0: // Short Sword
		case 7:
			object = new ShortSword(row, col, this); break;
		case 1: // Long Sword
		case 8:
			object = new LongSword(row, col, this); break;
		case 2: // Mace
		case 9:
			object = new Mace(row, col, this); break;
		// Scrolls
		case 3:  // scroll of improve armor
			object = new ArmorScroll(row, col, this); break;
		case 4: //  scroll of raise strength
			object = new StrengthScroll(row, col, this); break;
		case 5: // scroll of enhance health
			object = new HealthScroll(row, col, this); break;
		case 6: // scroll of enhance dexterit
			object = new DexterityScroll (row, col, this); break;
	}

	// Pushing item onto collection of objects
	m_objects.push_back(object);

	// returning pointer to the newly added item
	return object;
}

Monster* Dungeon::addNewMonster()
	// Adds a new dynamically allocated Monster and places it  
	// in the Dungeon, depending on the level of the Dungeon
	// Return pointer to the newly added monster
{
	// Produce an open position to place the monster in
	int row, col;
	produceOpenPos(row, col);

	Monster* monster;

	// Setting choices according to Dungeon level
	int maxCase;
	if(level() <= 1) maxCase = 1;      // Appear at level 0 or deeper
	else if(level() <= 2) maxCase = 2; // Appear at level 2 or deeper
	else maxCase = 3;                  // Appear at level 3 or deeper
	
	// Randomly selecting a monster type depending on Dungeon level
	int selection = randInt(0,maxCase);

	switch(selection)
	{
		// Can appear at any level
		case 0: monster = new Goblin(row, col, this, m_goblinSmellDist); break;
		case 1: monster = new Snakewoman(row, col, this); break;
		// Can appear at level 2 or deeper
		case 2: monster = new Bogeyman(row, col, this); break;
		// Can appear at level 3 or deeper
		case 3: monster = new Dragon(row, col, this); break;		
	}

	// Pushing monster onto collection of monster
	m_monsters.push_back(monster);

	// returning pointer to the newly added monster
	return monster;
}

bool Dungeon::addPlayer(Player* p)
	// Add player to valid random position on Dungeon
{
	if (p)
	{
		m_player = p;  // Attaching Player to Dungeon
		m_player->setDungeon(this); // Attaching Dungeon to Player

		// Placing Player on valid position in Dungeon
		int pRow, pCol;
		produceOpenPos(pRow, pCol);
		m_player->setPos(pRow, pCol);

		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////
// Game Functions

void Dungeon::display(string result) const
	// Clear the screen and display the level
{
	// Making the display grid
	char displayGrid[MAXROWS][MAXCOLS] ; 

	// copying the base grid
	for(int i = 0; i < rows(); i++)
		for(int j = 0; j < cols(); j++)
			displayGrid[i][j] =  m_baseGrid[i][j];
	
	// Adding objects to the grid
	for(int i = 0; i < m_objects.size(); i++)
	{
		displayGrid[m_objects[i]->row()][m_objects[i]->col()] 
			= m_objects[i]->symbol();
	}

	// Adding monsters to the grid
	for(int i = 0; i < m_monsters.size(); i++)
	{
		displayGrid[m_monsters[i]->row()][m_monsters[i]->col()] 
			= m_monsters[i]->symbol();
	}

	// Adding player to the grid on top of everything else
	displayGrid[m_player->row()][m_player->col()] 
			= m_player->symbol();
		
	// Clearing screen
	clearScreen();

	// Printing the grid
	for(int i = 0; i < rows(); i++)
	{
		for(int j = 0; j < cols(); j++)
		{
			cout << displayGrid[i][j];
		}
		cout << endl;
	}
	// Outputting the stats
	cout << "Dungeon Level: " << level() << ", " 
		 << "Hit Points: " << player()->hitPts()       << ", " 
		 << "Armor: "      << player()->armorPts()     << ", " 
		 << "Strength: "   << player()->strengthPts()  << ", " 
		 << "Dexterity: "  << player()->dexterityPts() << endl;
	
	cout << endl;

	// Outputtng the result of the previous move
	cout << result << endl;
}

bool Dungeon::killMonster(Monster* monster)
// Kill a monster and drop any items 
{
	// Find monster in Dungeon
	vector<Monster*>::iterator toKill = 
	find(m_monsters.begin(), m_monsters.end(), monster);
	
	if(toKill == m_monsters.end() // If no such monster found	
		|| !(*toKill)->isDead())  // Or given monster not dead
		return false;

	// Get items from the monster
	GameObject* object = monster->dropDead();
	if(object) // If the monster dropped something
	{
		m_objects.push_back(object); // Add to dungeon
	}

	m_monsters.erase(toKill); // Remove monster from Dungeon
	delete monster; // Kill the monster
	return true;
}

bool Dungeon::removeObject(GameObject* object)
	// Remove object from the dungeon
{
	// Find object in Dungeon
	vector<GameObject*>::iterator toRemove = 
	find(m_objects.begin(), m_objects.end(), object);

	if(toRemove == m_objects.end()) // If no such object found
		return false;
	 
	m_objects.erase(toRemove);	 // Remove Object
	return true;
}

string Dungeon::attemptMoveMonsters()
	// Attempt to move all monsters on the dungeon
	// Attack player if monster is next to player
	// Return result of moving monsters
{
	string result = ""; // Result is nothing by default

	for(int i = 0; i < m_monsters.size(); i++)
	{
		// If monster asleep, do nothing and reduce sleep time
		if(m_monsters[i]->isAsleep())
		{
			m_monsters[i]->reduceSleep();
		}
		// If monster next to player, attack
		else if( abs(player()->row() - m_monsters[i]->row())
			+ abs(player()->col() - m_monsters[i]->col())
		 			== 1 )
		{
			if(result != "") result += "\n";
			result += m_monsters[i]->attemptAttack(m_player);
		}
		else // Else execute the monter's move
		{
			m_monsters[i]->move();
			// No update to result string needed
		}
	}
	return result;
}

