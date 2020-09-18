// Dungeon.h

#ifndef DUNGEON_INCLUDED
#define DUNGEON_INCLUDED

#include <vector>

const int MAXROWS = 18;
const int MAXCOLS = 70;

const char WALL = '#';
const char STAIR = '>';
const char IDOL = '&';

class Player;
class Monster;
class GameObject;

////////////////////////////////////////////////////////////////////
// Dungeon Class Declaration
////////////////////////////////////////////////////////////////////

class Dungeon
{
  public:
  	/////////////////////////////////////////////////////////////////
	// Constructor and Destructor
	Dungeon(int level, Player* player, int goblinSmellDist, int rows = MAXROWS, int cols = MAXCOLS);
	~Dungeon();
	
	/////////////////////////////////////////////////////////////////
	// Accessors	

	int rows() const {return m_rows;}
	int cols() const {return m_cols;}

	int level() const {return m_level;}
	Player* player() const {return m_player;}
	
	/////////////////////////////////////////////////////////////////
	// Helper Functions

	bool isWall(int row, int col) const;
	bool isStair(int row, int col) const;
	bool isIdol(int row, int col) const;
	bool canPlayerOccupy(int row, int col) const;

	Monster* monsterAt(int row, int col) const;
	GameObject* objectAt(int row, int col) const;

	void produceOpenPos(int& row, int& col) const; 
		// Can be location of an object
	void produceEmptyPos(int& row, int& col) const;
		// Cannot be location of an object

	/////////////////////////////////////////////////////////////////
	// Game Functions

	// Display the grid with result printed out
	void display(std::string result) const;
	// Kill monster and drop any items on tthe dungeon
	bool killMonster(Monster* monster);
	// Remove object from dungeon
	bool removeObject(GameObject* object);
	// Move monsters on the dungeon
	std::string attemptMoveMonsters();

	/////////////////////////////////////////////////////////////////
  private:
	/////////////////////////////////////////////////////////////////
	char m_baseGrid[MAXCOLS][MAXCOLS]; // Base grid 

	int m_rows;  // Number of rows in Dungeon
	int m_cols;  // Number of rows in Dungeon
	
	int m_level; // Level of Dungeon
	Player* m_player;  // Pointer to player

	int m_goblinSmellDist; // Goblin smell distance

	std::vector<GameObject*> m_objects; // collection of GameObjects
	std::vector<Monster*> m_monsters;   // collection of Monsters

	/////////////////////////////////////////////////////////////////
	// Private Helper Functions
	
	// Generate empty rooms on the base grid and connect them
	void generateRooms();
	// Makes new objects to drop in the dungeon
	GameObject* addNewObject(); 
	// Makes new monsters to place in the dungeon
	Monster* addNewMonster();
	// Add player to valid random position on Dungeon
	bool addPlayer(Player* p);

	// Helper structure
	struct Room
	{
		// Top left corner of room
		int m_r, m_c;
		// Height and Width of the room
		int m_dr,m_dc;

		Room(int r, int dr, int c, int dc) 
		: m_r(r), m_c(c), m_dr(dr), m_dc(dc) {}
		Room() {} 
	};

	/////////////////////////////////////////////////////////////////
};

/////////////////////////////////////////////////////////////////////
// Inline Implementations

inline
bool Dungeon::isWall(int row, int col) const
	// return's true if position is a wall or outside grid
{
	bool isWall = true;  
	if(row >= 0 && row < rows() && col >= 0 && col < cols())
		isWall = (m_baseGrid[row][col] == WALL);
	return isWall;
}

inline 
bool Dungeon::isStair(int row, int col) const
	// return true iff there are stairs in the positon
{
	bool isStair = false;  
	if(row >= 0 && row < rows() && col >= 0 && col < cols())
		isStair = (m_baseGrid[row][col] == STAIR);
	return isStair;	
}

inline 
bool Dungeon::isIdol(int row, int col) const
	// return true iff there is idol in the positon
{
	bool isIdol = false;  
	if(row >= 0 && row < rows() && col >= 0 && col < cols())
		isIdol = (m_baseGrid[row][col] == IDOL);
	return isIdol;	
}

inline
bool Dungeon::canPlayerOccupy(int row, int col) const
{
	return (!isWall(row, col) && !monsterAt(row, col));
}

#endif // DUNGEON_INCLUDED