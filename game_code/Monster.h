// Monster.h

#ifndef MONSTER_INCLUDED
#define MONSTER_INCLUDED

#include <string>

#include "Dungeon.h"
#include "Actor.h"

class Weapon;
class GameObject;

////////////////////////////////////////////////////////////////////
// Base Monster Class Declarations
////////////////////////////////////////////////////////////////////

class Monster : public Actor
{
  public:
  	/////////////////////////////////////////////////////////////////
	// Constructor and Destructor
	Monster(int row, int col, Dungeon* dungeon, int hitPts,  
		int armorPts, int strengthPts, int dexterityPts, 
		int sleepPts, Weapon* weapon)
		: Actor(row, col, dungeon, hitPts, armorPts, strengthPts, 
				dexterityPts, sleepPts, weapon)
	{}

	virtual ~Monster() {}

	/////////////////////////////////////////////////////////////////
	// Game Functions

	// Monsters may drop an object when they die
	virtual GameObject* dropDead() = 0;
	// Move the monsters on the dungeon
	virtual void move() = 0;
	
	/////////////////////////////////////////////////////////////////
	// Helper Functions

	// Returns ideal shortest no. of steps from the monster to the player
	int idealShortestDistToPlayer() const; 
	// Moves monster closer to Player if possible and player in 
	// smellDist range
	void dumbSmellMove(int smellDist);
};

////////////////////////////////////////////////////////////////////
// Derived Monster Classes Declarations
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
// Goblin

class Goblin : public Monster
{
  public:
	Goblin(int row, int col, Dungeon* dungeon, int smellDist);

	// Name and symbol
	virtual std::string name() const {return "the Goblin";}
	virtual char symbol() const      {return 'G';}

	// Game Functions
	virtual GameObject* dropDead();
	virtual void move();

  private:
	int m_smellDist;
	
	// Helper structures
	int m_helperGrid[MAXROWS][MAXCOLS];
	struct Point
	{
		int r,c; // Coordinates of point

		// Constructors
		Point() {}
		Point(int r, int c) : r(r), c(c) {}
	};

	// Helper Functions
	void initializeHelperGrid();
	Point dirToNewPoint(int dir, Point start);
	int ptIdealDistToPlayer(const Point& p);
	
	// Recursive Shortest Path Function
	int shortestPathFrom(Point start, int maxSteps);
};

//  Inline implementations
inline
int Goblin::ptIdealDistToPlayer(const Point& p)
	// Gives ideal distance of p from the player
	// Ignoring walls and other monsters
{
	return ( abs(p.r - dungeon()->player()->row())
			+abs(p.c - dungeon()->player()->col()) );
}

////////////////////////////////////////////////////////////////////
// Dragon

class Dragon : public Monster
{
  public:
	Dragon(int row, int col, Dungeon* dungeon);
	
	// Name and symbol
	virtual std::string name() const {return "the Dragon";}
	virtual char symbol() const      {return 'D';}

	// Game Functions
	virtual GameObject* dropDead();
	virtual void move() {} // Dragon does not move
};

////////////////////////////////////////////////////////////////////
// Snakewoman

class Snakewoman : public Monster
{
  public:
	Snakewoman(int row, int col, Dungeon* dungeon);

	// Name and symbol
	virtual std::string name() const {return "the Snakewoman";}
	virtual char symbol() const      {return 'S';}

	// Game Functions
	virtual GameObject* dropDead();
	virtual void move();
};

////////////////////////////////////////////////////////////////////
// Bogeyman

class Bogeyman: public Monster
{
  public:
	Bogeyman(int row, int col, Dungeon* dungeon);

	// Name and symbol
	virtual std::string name() const {return "the Bogeyman";}
	virtual char symbol() const      {return 'B';}

	// Game Functions
	virtual GameObject* dropDead();
	virtual void move();
};

#endif // MONSTER_INCLUDED