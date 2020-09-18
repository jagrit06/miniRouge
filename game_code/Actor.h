// Actor.h

#ifndef ACTOR_INCLUDED
#define ACTOR_INCLUDED

#include <string>

const int MAXHIT = 99;
const int MAXARMOR = 99;
const int MAXSTRENGTH = 99;
const int MAXDEXTERITY = 99;
const int MAXSLEEP = 9;

class Weapon;
class GameObject;
class Dungeon;

////////////////////////////////////////////////////////////////////
// Base Actor Class Declarations
////////////////////////////////////////////////////////////////////

class Actor
{
  public:
	/////////////////////////////////////////////////////////////////
	// Constructor and Destructor

	Actor(int row, int col, Dungeon* dungeon, int hitPts, 
		int armorPts, int strengthPts, int dexterityPts, int sleepPts, Weapon* weapon);

	virtual ~Actor();
	
	/////////////////////////////////////////////////////////////////
	// Accessors

	// Dungeon
	Dungeon* dungeon() const {return m_dungeon;}

	// Position
	int row() const {return m_row;}
	int col() const {return m_col;}

	// Stats
	int hitPts() const       {return m_hit;}
	int armorPts() const     {return m_armor;}
	int strengthPts() const  {return m_strength;}
	int dexterityPts() const {return m_dexterity;}
	int sleepPts() const     {return m_sleep;}

	// Weapon
	Weapon* weapon() const   {return m_weapon;}

	// Name and symbol (Unique for each Actor)
	virtual std::string name() const = 0;
	virtual char symbol() const = 0;
	
	/////////////////////////////////////////////////////////////////
	// Setters

	// Position
	bool setPos(int row, int col);

	// Stats
	bool setHitPts(int hitPts);
	bool setArmorPts(int armorPts);
	bool setStrengthPts(int strengthPts);
	bool setDexterityPts(int dexterityPts);
	bool setSleepPts(int sleepPts);

	// Dungeon
	void setDungeon(Dungeon* dungeon) {m_dungeon = dungeon;}

	// Weapon
	void setWeapon(Weapon* weapon) {m_weapon = weapon;}

	/////////////////////////////////////////////////////////////////
	// Helper Functions

	bool isDead() const {return m_hit <= 0;}
	bool isAsleep() const {return m_sleep > 0;}
	bool takeDamage(int damage);
	void reduceSleep() {if(isAsleep()) m_sleep--;}

	/////////////////////////////////////////////////////////////////
	// Game Functions

	std::string attemptAttack(Actor* defender);

  private:
	/////////////////////////////////////////////////////////////////

	// Position
	int m_row;
	int m_col;

	// Dungeon
	Dungeon* m_dungeon;

	// Stats 
	int m_hit;
	int m_armor;
	int m_strength;
	int m_dexterity;
	int m_sleep;

	// Weapon
	Weapon* m_weapon;

	/////////////////////////////////////////////////////////////////
};

/////////////////////////////////////////////////////////////////////
// Inline Implementations

// Stats
inline
bool Actor::setHitPts(int hitPts)
{
	if(hitPts < 0 || hitPts > MAXHIT) return false;
	m_hit = hitPts; return true;
}

inline
bool Actor::setArmorPts(int armorPts)
{
	if(armorPts < 0 || armorPts > MAXARMOR) return false;
	m_armor = armorPts; return true;	
}

inline
bool Actor::setStrengthPts(int strengthPts)
{
	if(strengthPts < 0 || strengthPts > MAXSTRENGTH) return false;
	m_strength = strengthPts; return true;	
}

inline
bool Actor::setDexterityPts(int dexterityPts)
{
	if(dexterityPts < 0 || dexterityPts > MAXDEXTERITY) return false;
	m_dexterity = dexterityPts; return true;	
}

inline
bool Actor::setSleepPts(int sleepPts)
{
	if(sleepPts < 0 || sleepPts > MAXSLEEP) return false;
	m_sleep = sleepPts; return true;	
}

#endif // ACTOR_INCLUDED
