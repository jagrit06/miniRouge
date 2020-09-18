#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <queue>
#include <algorithm>

#include "Actor.h"
#include "Player.h"
#include "Monster.h"
#include "Weapon.h"
#include "Scroll.h"
#include "Dungeon.h"
#include "utilities.h"

using namespace std;

////////////////////////////////////////////////////////////////////
// Monster Class Implementations
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
// Helper Functions

int Monster::idealShortestDistToPlayer() const
	// Returns the ideal shortest number of steps to player, 
	// Ignoring any obstacles
{
	return (  abs(dungeon()->player()->row() - row())
			+ abs(dungeon()->player()->col() - col()) );
}

void Monster::dumbSmellMove(int smellDist)
	// If the monster can reach the player in smellDist steps
	// Ignoring walls or other monsters, move monster closer to 
	// the player. If the player is one step away, this function
	// does nothing since the monster should attack the player
{
	// Finding shortest distance to player
	int idealDist = idealShortestDistToPlayer();
	// If player outside smell range or monster right next to 
	// player (since monster is supposed to attack), do nothing
	if( idealDist > smellDist || idealDist <= 1)
		return;
	// If player in smell range

	// First looking to move in horizontal direction
	int colDiff = dungeon()->player()->col() - col();
	int change = 0; // Direction in which col would have to change to get monster closer to the player
	
	// Trying to change monster's column
	if(colDiff < 0) // Player is to the monster's left
		change = -1;
	else if(colDiff > 0) // Player is to the monster's right
		change = 1;

	if(change != 0 && dungeon()->canPlayerOccupy(row(),col()+change)) 
	// If change in column will bring monster closer to player
	// and monster can occupy new position (same conditions as player apply)
	{
		setPos(row(),col()+change);//Move monster to the new position
		return;
	}
	// Else, looking at moving in row direction
	else
	{
		int rowDiff = dungeon()->player()->row() - row();
		change = 0; // Direction in which row would have to change to get monster closer to the player

		// Trying to change monster's row
		if(rowDiff < 0) // Player is above the monster
			change = -1;
		else if(rowDiff > 0) // Player is below the monster
			change = 1;

		if(change != 0 && dungeon()->canPlayerOccupy(row() + change,col())) 
		// If change in row will bring monster closer to player
		// and monster can occupy new position (same conditions as player apply)
		{
			setPos(row() + change,col());//Move monster to the new position
			return;
		}
	}
}

////////////////////////////////////////////////////////////////////
// Derived Monster Classes Implementations
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
// Goblin

Goblin::Goblin(int row, int col, Dungeon* dungeon, int smellDist)
	: Monster(row, col, dungeon, randInt(15,20)/*hitPts*/,  
	 1/*armorPts*/,  3/*strengthPts*/, 1/*dexterityPts*/,
	 0 /*sleepPts*/, new ShortSword(row, col, dungeon)/*weapon*/), 
	 m_smellDist(smellDist)
{}

GameObject* Goblin::dropDead()
	// Create and return pointer to a new object after monster dies 
	// If object not created, return nullptr
{
	GameObject* toDrop = nullptr;
	// If Goblin is dead, it drops magic axe or magic fangs with 1 in 3 chance
	// where it dies if the position is free
	if(isDead() && trueWithProbability(1.0/3.0)
		&& !dungeon()->objectAt(row(), col())
		&& !dungeon()->isIdol(row(), col())
		&& !dungeon()->isStair(row(), col())     ) 
	{
		if(trueWithProbability(0.5)) // Either drop a magic axe
			toDrop = new MagicAxe(row(), col(), dungeon());
		else // or drop magic fangs of sleepPts
			toDrop = new SleepFangs(row(), col(), dungeon());
	}
	return toDrop;
}

void Goblin::move()
// The goblin taks an ideal step if it can reach the player in 
// m_smellDist steps
// This function finds the shortest path length from every direction
// the Goblin can move in and takes an ideal step in that direction
// The shortest path is found using the recursive 
// int Goblin::shortestPathFrom(Point start, int maxSteps) function.
{
	Point curr(row(), col());
	int bestDir; 
	int shortestPath = 9999; // Arbitrarily large for comparison

	// Finding the direction with the shortest path length
	for(int dir = 0; dir < 4; dir++)
	{
		Point candPt = dirToNewPoint(dir, curr);
		
		if(dungeon()->canPlayerOccupy(candPt.r, candPt.c)
			&& ptIdealDistToPlayer(candPt) < m_smellDist)
			// If the Goblin can get to this point and possibly
			// reach the player in the remaining steps
		{
			initializeHelperGrid();

			// Finding path length from this move and comparing
			int candidate = shortestPathFrom(candPt, m_smellDist-1);

			if(candidate != -1 && // If there is a suitable path
				candidate < shortestPath) // which is shorter
			{
				shortestPath = candidate;
				bestDir = dir;
			}
		}
	}
	if(shortestPath == 9999) // If no valid path 
		return; // Do nothing

	// If valid path exists
	Point newPos = dirToNewPoint(bestDir, curr);
	setPos(newPos.r, newPos.c); // Move goblin to new position
}

void Goblin::initializeHelperGrid()
// Initializes the helper grid to have 0 in the spots where 
// the Goblin can go and 1 where the Goblin can't (other monsters
// and walls). The player's pos, however, is set to 0 here
{
	Monster* monster;
	for(int i = 0; i < dungeon()->rows(); i++)
	{
		for(int j = 0; j < dungeon()->cols(); j++)
		{
			// If position is a wall
			if(dungeon()->isWall(i, j))
				m_helperGrid[i][j] = 1;
			// If there is some other monster at position
			else if ((monster = dungeon()->monsterAt(i, j)) != nullptr
					&& monster != this)
				m_helperGrid[i][j] = 1;
			
			// Else, Goblin can take the position
			else
				m_helperGrid[i][j] = 0;
		}
	}
}

const int UP    = 0;
const int DOWN  = 1;
const int LEFT  = 2;
const int RIGHT = 3;

Goblin::Point Goblin::dirToNewPoint(int dir, Point start)
// Produces a new point if a step is taken from start in the given
// direction. It updates the pathLenTill associated with the point
{
	int newRow = start.r;
	int newCol = start.c;
	switch(dir)
	{
		case UP   : newRow--; break;
		case DOWN : newRow++; break;
		case LEFT : newCol--; break;
		case RIGHT: newCol++; break;
	}
	return Point(newRow, newCol);
}

int Goblin::shortestPathFrom(Point start, int maxSteps)
// Returns the shortest path length from the starting point
// to the player within max steps. Retuns -1 if no such path
// Uses helperGrid : grid representing the dungeon where 
// 		0 : Goblin can visit this position
// 		1 : Goblin cannot visit
// 		2 : Goblin has already visited
{
	////////////////////////////////////////////////////////
	// Base Cases:

	int idealDist = ptIdealDistToPlayer(start);

	// 0. If the player is outside the goblin's smell range
	if(idealDist > m_smellDist) 
	{
		m_helperGrid[start.r][start.c] = 2; // Mark it as visited so that the point isn't considered again
		return -1;
	}
	
	// 1. If it's not possible to reach the player in maxSteps
	if(idealDist > maxSteps) 
	{
		return -1;
	}

	// 2. If Goblin is right next to the player at this candidate
	if(idealDist == 1) 
	{
		return  1;
	}

	/////////////////////////////////////////////////////////
	// Recursive Case:

	m_helperGrid[start.r][start.c] = 2; // Mark starting point as visited

	// Consider the surrounding points to branch out 
	// potential path to player
	vector<Point> toConsider;
	for(int dir = 0; dir < 4; dir++)
		toConsider.push_back(dirToNewPoint(dir, start));

	int shortestPath = 9999; // Arbitrarily large for comparison

	for(int i = 0; i < 4; i++)
	{
		int row = toConsider[i].r;
		int col = toConsider[i].c;
		if(!dungeon()->isWall(row, col)  // If it's a valid point
			&& m_helperGrid[row][col] == 0) // not already visited
		{		
			// If a shorter path has been found, set that to the standard of max steps
			int avaliableSteps = (shortestPath != 9999) ? 
							shortestPath : (maxSteps - 1);
			
			// Shorest path from this point
			int pathLenFrom = shortestPathFrom(toConsider[i], avaliableSteps);

			// If path is valid and shorter
			if (pathLenFrom != -1 && pathLenFrom < shortestPath)
				shortestPath = pathLenFrom;
		}	
	}

	// Leave starting point open for consideration in other paths
	m_helperGrid[start.r][start.c] = 0; 

	// If no valid path found
	if(shortestPath == 9999) return -1;

	// Else, return the shortest path length from start
	return 1 + shortestPath;
}

////////////////////////////////////////////////////////////////////
// Dragon

Dragon::Dragon(int row, int col, Dungeon* dungeon)
	: Monster(row, col, dungeon, randInt(20,25)/*hitPts*/,
	 4/*armorPts*/, 4/*strengthPts*/, 4/*dexterityPts*/,
	 0 /*sleepPts*/, new LongSword(row, col, dungeon)/*weapon*/)
{}

GameObject* Dragon::dropDead()
	// Create and return pointer to a new object after monster dies 
	// If object not created, return nullptr
{
	GameObject* toDrop = nullptr;
	// If Dragon is dead, it drops some scroll
	// where it dies if the position is free
	if(isDead() 
		&& !dungeon()->objectAt(row(), col())
		&& !dungeon()->isIdol(row(), col())
		&& !dungeon()->isStair(row(), col())     ) 
	{
		// Randonly selecting and producing a new scroll
		int selection = randInt(0,4);
		switch(selection)
		{
		// Scrolls
		case 0: // scroll of improve armor
			toDrop = new ArmorScroll(row(), col(), dungeon()); break;
		case 1: //  scroll of raise strength
			toDrop = new StrengthScroll(row(), col(), dungeon()); break;
		case 2: // scroll of enhance health
			toDrop = new HealthScroll(row(), col(), dungeon()); break;
		case 3: // scroll of enhance dexterity
			toDrop = new DexterityScroll(row(), col(), dungeon()); break;
		case 4: // scroll of teleportation
			toDrop = new TeleportationScroll(row(), col(), dungeon()); break;
		}
	}
	return toDrop;
}

////////////////////////////////////////////////////////////////////
// Snakewoman

Snakewoman::Snakewoman(int row, int col, Dungeon* dungeon)
	: Monster(row, col, dungeon,  randInt(3,6)/*hitPts*/,  
	 3/*armorPts*/, 2/*strengthPts*/, 3/*dexterityPts*/, 
	 0 /*sleepPts*/, new SleepFangs(row, col, dungeon)/*weapon*/)
{}

GameObject* Snakewoman::dropDead()
	// Create and return pointer to a new object after monster dies 
	// If object not created, return nullptr
{
	GameObject* toDrop = nullptr;
	// If Snakewoman is dead, it drops magic fangs of sleep
	// with 1 in 3 chance where it dies if the position is free
	if(isDead() && trueWithProbability(1.0/3.0)
		&& !dungeon()->objectAt(row(), col())
		&& !dungeon()->isIdol(row(), col())
		&& !dungeon()->isStair(row(), col())     ) 
	{
		// Make axe where the Snakewoman dies in the dungeon
		toDrop = new SleepFangs(row(), col(), dungeon());
	}
	return toDrop;
}

const int SNKWSMELLDIST = 3;

void Snakewoman::move()
{
	dumbSmellMove(SNKWSMELLDIST);
}

////////////////////////////////////////////////////////////////////
// Bogeyman

Bogeyman::Bogeyman(int row, int col, Dungeon* dungeon)
	: Monster(row, col, dungeon, randInt(5,10)/*hitPts*/, 
	  2 /*armorPts*/,  randInt(2,3)/*strengthPts*/, 
	  randInt(2,3)/*dexterityPts*/, 0 /*sleepPts*/, 
	  new ShortSword(row, col, dungeon)/*weapon*/)
{}

GameObject* Bogeyman::dropDead()
	// Create and return pointer to a new object after monster dies 
	// If object not created, return nullptr
{
	GameObject* toDrop = nullptr;
	// If Bogeyman is dead, it drops magic axe with 1 in 10 chance
	// where it dies if the position is free
	if(isDead() && trueWithProbability(1.0/10.0)
		&& !dungeon()->objectAt(row(), col())
		&& !dungeon()->isIdol(row(), col())
		&& !dungeon()->isStair(row(), col())     )  
	{
		// Make axe where the Bogeyman dies in the dungeon
		toDrop = new MagicAxe(row(), col(), dungeon());
	}

	return toDrop;
}

const int BGYSMELLDIST = 5;

void Bogeyman::move()
{
	dumbSmellMove(BGYSMELLDIST);
}