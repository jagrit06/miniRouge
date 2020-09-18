// Weapon.h

#ifndef WEAPON_INCLUDED
#define WEAPON_INCLUDED

#include <string>
#include "GameObject.h"

class Actor;

const char WEAPON = ')';

////////////////////////////////////////////////////////////////////
// Base Weapon Class Declarations
////////////////////////////////////////////////////////////////////

class Weapon : public GameObject
{
  public:
  	/////////////////////////////////////////////////////////////////
	// Constructor and Destructor
	Weapon(int row, int col, Dungeon* dungeon, int dmg, int dexBns)
		: GameObject(row, col, dungeon),
		  m_dmg(dmg), m_dexBns(dexBns)
	{}

	virtual ~Weapon() {}

	// Symbol
	virtual char symbol() const {return WEAPON;}

	/////////////////////////////////////////////////////////////////
	// Accessors
	int damageAmt() const {return m_dmg;}
	int dexterityBns() const {return m_dexBns;}
	
	/////////////////////////////////////////////////////////////////
	// Game Functions
	
	// Every weapon has a unique action
	virtual std::string action() const = 0;
	
	// Most weapons don't have a special move,
	// so indicating nothing happens
	virtual std::string specialMove(Actor* attacker, Actor* defender) {return ".";}
 
  private:
	/////////////////////////////////////////////////////////////////

	int m_dmg;
	int m_dexBns;

	/////////////////////////////////////////////////////////////////
};

////////////////////////////////////////////////////////////////////
// Derived Weapon Classes
////////////////////////////////////////////////////////////////////

class ShortSword : public Weapon
{
  public:
	ShortSword(int row, int col, Dungeon* dungeon)
	: Weapon(row, col, dungeon, 2 /*dmg*/, 0 /*dexBns*/)
	{}

	// Name and action
	virtual std::string name() const 
		{return "short sword";}
	virtual std::string action() const 
		{return "slashes short sword at";}
};

class LongSword : public Weapon
{
  public:
	LongSword(int row, int col, Dungeon* dungeon)
	: Weapon(row, col, dungeon, 4 /*dmg*/, 2 /*dexBns*/)
	{}

	// Name and action
	virtual std::string name() const 
		{return "long sword";}
	virtual std::string action() const 
		{return "swings long sword at";}
};

class Mace : public Weapon
{
  public:
	Mace(int row, int col, Dungeon* dungeon)
	: Weapon(row, col, dungeon, 2 /*dmg*/, 0 /*dexBns*/)
	{}

	// Name and action
	virtual std::string name() const 
		{return "mace";}
	virtual std::string action() const 
		{return "swings mace";}
};

class MagicAxe : public Weapon
{
  public:
	MagicAxe(int row, int col, Dungeon* dungeon)
	: Weapon(row, col, dungeon, 5 /*dmg*/, 5 /*dexBns*/)
	{}

	// Name and action
	virtual std::string name() const 
		{return "magic axe";}
	virtual std::string action() const 
		{return "chops magic axe at";}	
};

class SleepFangs : public Weapon
{
  public:
	SleepFangs(int row, int col, Dungeon* dungeon)
	: Weapon(row, col, dungeon, 3 /*dmg*/, 3 /*dexBns*/)
	{}

	// Name and action
	virtual std::string name() const 
		{return "magic fangs of sleep";}
	virtual std::string action() const 
		{return "strikes magic fangs at";}

	// SleepFangs have special effects
	virtual std::string specialMove(Actor* attacker, Actor* defender);
};

#endif // WEAPON_INCLUDED