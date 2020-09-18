// GameObject.h

#ifndef GAMEOBJECT_INCLUDED
#define GAMEOBJECT_INCLUDED

#include <string>
#include "Dungeon.h"

////////////////////////////////////////////////////////////////////
// Base GameObject Class Declaration
////////////////////////////////////////////////////////////////////

class GameObject
{
  public:
	/////////////////////////////////////////////////////////////////
	// Constructor and Destructor

	GameObject(int row, int col, Dungeon* dungeon)
		: m_dungeon(dungeon), m_row(row), m_col(col)
	{}

	virtual ~GameObject() {}
	
	/////////////////////////////////////////////////////////////////
	// Accessors

	// Dungeon
	Dungeon* dungeon() const {return m_dungeon;}
	
	// Position
	int row() const {return m_row;}
	int col() const {return m_col;}

	// Name and symbol
	virtual std::string name() const = 0;
	virtual char symbol() const = 0;
	
	/////////////////////////////////////////////////////////////////
	// Setters

	// Position
	bool setPos(int row, int col);

	/////////////////////////////////////////////////////////////////

  private:
	/////////////////////////////////////////////////////////////////
	
	// Dungeon
	Dungeon* m_dungeon;

	// Position
	int m_row;
	int m_col;

	/////////////////////////////////////////////////////////////////
};

inline
bool GameObject::setPos(int row, int col)
{
	if(dungeon()->isWall(row, col)) 
		return false;
	m_row = row;
	m_col = col;
	return true;
}

#endif // GAMEOBJECT_INCLUDED
