// Scroll.h

#ifndef SCROLL_INCLUDED
#define SCROLL_INCLUDED

#include <string>

#include "GameObject.h"

class Dungeon;
class Player;

const char SCROLL = '?';

////////////////////////////////////////////////////////////////////
// Base Scroll Class Declaration
////////////////////////////////////////////////////////////////////

class Scroll : public GameObject
{
  public:
  	/////////////////////////////////////////////////////////////////
	// Constructor and Destructor
	Scroll(int row, int col, Dungeon* dungeon)
		: GameObject(row, col, dungeon)
	{}

	virtual ~Scroll() {}

	// Symbol
	virtual char symbol() const {return SCROLL;}
	// Effect string
	virtual std::string effect() const = 0;

	/////////////////////////////////////////////////////////////////
	// Game Functions

	// Each scroll has a unique effect on its user
	virtual void haveEffect(Player* user) = 0;
};

////////////////////////////////////////////////////////////////////
// Derived Scroll Classes Declarations
////////////////////////////////////////////////////////////////////

class TeleportationScroll : public Scroll
{
  public:
	TeleportationScroll(int row, int col, Dungeon* dungeon)
		: Scroll(row, col, dungeon)
	{}

	// Name and effect
	virtual std::string name() const 
		{return "scroll of teleportation";}
	virtual std::string effect() const 
		{return "You feel your body wrenched in space and time.";}
	
	// Effect of reading scroll
	virtual void haveEffect(Player* user);
};

class ArmorScroll : public Scroll
{
  public:
	ArmorScroll(int row, int col, Dungeon* dungeon)
		: Scroll(row, col, dungeon)
	{}

	// Name and effect
	virtual std::string name() const 
		{return "scroll of improve armor";}
	virtual std::string effect() const 
		{return "Your armor glows blue.";}
	
	// Effect of reading scroll
	virtual void haveEffect(Player* user);
};

class StrengthScroll : public Scroll
{
  public:
	StrengthScroll(int row, int col, Dungeon* dungeon)
		: Scroll(row, col, dungeon)
	{}

	// Name and effect
	virtual std::string name() const 
		{return "scroll of raise strength";}
	virtual std::string effect() const 
		{return "Your muscles bulge.";}
	
	// Effect of reading scroll
	virtual void haveEffect(Player* user);
};

class HealthScroll : public Scroll
{
  public:
	HealthScroll(int row, int col, Dungeon* dungeon)
		: Scroll(row, col, dungeon)
	{}
	
	// Name and effect
	virtual std::string name() const 
		{return "scroll of enhance health";}
	virtual std::string effect() const 
		{return "You feel your heart beating stronger.";}
	
	// Effect of reading scroll
	virtual void haveEffect(Player* user);
};

class DexterityScroll : public Scroll
{
  public:
	DexterityScroll(int row, int col, Dungeon* dungeon)
		: Scroll(row, col, dungeon)
	{}

	// Name and effect
	virtual std::string name() const 
		{return "scroll of enhance dexterity";}
	virtual std::string effect() const 
		{return "You feel like less of a klutz.";}
	
	// Effect of reading scroll
	virtual void haveEffect(Player* user);
};

#endif // SCROLL_INCLUDED