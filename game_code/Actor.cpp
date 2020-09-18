#include <iostream>
#include <string>
#include <cstdlib>

#include "Actor.h"
#include "Weapon.h"
#include "Dungeon.h"
#include "utilities.h"

using namespace std;

////////////////////////////////////////////////////////////////////
// Base Actor Class Implementations
////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
// Constructor and Destructor 

Actor::Actor(int row, int col, Dungeon* dungeon,int hitPts, 
		int armorPts, int strengthPts, int dexterityPts, 
		int sleepPts, Weapon* weapon)
	: m_dungeon(dungeon), m_weapon(weapon)
{
	// Verifying and setting position
	if (!setPos(row, col))
	{
		cerr << "Error: Invalid Position passed to Actor Contructor" << endl;
		exit(1);
	}

	// Verifying and setting stats
	if (!setHitPts(hitPts)             ||
		!setArmorPts(armorPts)         ||
		!setStrengthPts(strengthPts)   ||
		!setDexterityPts(dexterityPts) ||
		!setSleepPts(sleepPts)
	)
	{
		cerr << "Error: Invalid Stats passed to Actor Contructor" << endl;
		exit(1);
	}
}

Actor::~Actor()
{
	delete m_weapon;
}

/////////////////////////////////////////////////////////////////
// Setters

// Position
bool Actor::setPos(int row, int col)
{
	if(row < 0 || col < 0) 
		return false;
	m_row = row;
	m_col = col;
	return true;
}

/////////////////////////////////////////////////////////////////
// Helper Functions

bool Actor::takeDamage(int damage)
	// Reduce hit points by damage taken
	// !!! Does not kill the actor
{
	if(damage < 0) return false;

	// Reduce hit points by damage taken
	m_hit -= damage;
	return true;
}

/////////////////////////////////////////////////////////////////
// Game Functions

string Actor::attemptAttack(Actor* defender)
	// Try to attack the defender with weapon 
{
	// Indicating attempt to attack with weapon
	string result = this->name() + " " + 
					this->weapon()->action() + " " +
					defender->name();

	int attackerPts = this->dexterityPts() 
					  + this->weapon()->dexterityBns();
	int defenderPts = defender->dexterityPts() + defender->armorPts();	
	if(randInt(1, attackerPts) >= randInt(1, defenderPts))
		// Attacker hits defender with weapon
	{
		// Calucate the damage done
		int damagePts = randInt(0, this->strengthPts() 
								+ weapon()->damageAmt() - 1); 
		// Do the damage
		defender->takeDamage(damagePts);
		if(defender->isDead())
		{
			// Indicating defender is dead
			return result + " dealing a final blow.";  
		}

		// Do weapon special moves, if any
		result +=" and hits" + weapon()->specialMove(this, defender);
		
		return result;
	}
	// Attacker misses
	return result  + " and misses.";
}