#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>

#include "Player.h"
#include "Actor.h"
#include "Scroll.h"
#include "Weapon.h"
#include "Dungeon.h"
#include "utilities.h"

using namespace std;

////////////////////////////////////////////////////////////////////
// Player Class Implementation
////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
// Constructor and Destructor

Player::Player()
	: Actor(0, 0, nullptr/*dungeon*/, BASEHEALTH/*hitPts*/, 
	2/*armorPts*/, 2/*strengthPts*/, 2/*dexterityPts*/, 
	0/*sleepPts*/, new ShortSword(0,0, nullptr)/*weapon*/) , 
	m_maxHit(BASEHEALTH)
{
	m_inventory.push_back(weapon()); // Placing ShortSword in inventory
}

Player::~Player()
{
	// Loop over the inventory
	for(int i = 0; i < m_inventory.size(); i++)
	{
		//If not weapon(weapon deleted in actor base class destructor)
		if(m_inventory[i] != weapon()) 
			delete m_inventory[i]; // Delete the GameObjects
	}
}

/////////////////////////////////////////////////////////////////////
// Game Functions

string Player::attemptPickUp(GameObject* object)
	// Pick up object, remove from Dungeon
	// and add to player's inventory if not full
	// Return result of pick up attempt
{
	if(!object) 	// Invalid object passed
	{
		return "";          // Do Nothing
	}
		
	if(m_inventory.size() == 26)  // If inventory full
	{
		// Don't pick up object
		return "Your knapsack is full; you can't pick that up.";
	}
	
	// Else pick up object and return string
	m_inventory.push_back(object); // Placing object in inventory
	dungeon()->removeObject(object); // Removing object
	
	if(object->symbol() == SCROLL)// If object is a scroll
		return "You pick up a scroll called " + object->name(); 
	// Else object is a weapon
	return "You pick up " + object->name(); 
}

void Player::displayInventory() const
	// Clear screen and display the player's inventory
{
	clearScreen();
	vector<GameObject*>::const_iterator itr = m_inventory.begin();
	char key = 'a';
	for(; itr != m_inventory.end(); itr++, key++)
	{
		string print = ""; 
		if((*itr)->symbol() == SCROLL) // If object is scroll
			print = "A scroll called "; // Add prefix
			// There is no prefic if object is weapon
		cout << key << ". " << print + (*itr)->name() << endl;
	}
}

string Player::attemptReadScroll(char key)
	// If object corresponding to key in inventory is a scroll
	// Read that scroll (have the scroll's effect on the user)
	// Remove scroll from inventory if read
	// Return result of reading effect
{
	int sub = keyToSubscript(key);
	if(sub == -1) // If inavlid key
		return ""; // Do nothing

	// Verify if requested object is scroll
	Scroll* toRead = dynamic_cast<Scroll*>(m_inventory[sub]);  
	if(!toRead) // If requested object is not scroll
		return "You can't read a " + m_inventory[sub]->name(); 
	
	// Read the scroll
	toRead->haveEffect(this);
	string name = toRead->name();
	string effect = toRead->effect();

	// Remove scroll from vector
	m_inventory.erase(m_inventory.begin()+sub);

	delete toRead; // Destroy scroll

	// Return result
	return "You read the scroll called " + name + "\n" + effect;
}

string Player::attemptWieldWeapon(char key)
	// If object corresponding to key in inventory is a weapon
	// Wield that weapon (change user's weapon to that weapon)
	// Return result of change weapon attempt
	// Note: Weapon not removed from inventory after change
{
	int sub = keyToSubscript(key);
	if(sub == -1) // If inavlid key
		return ""; // Do nothing

	// Verify if requested object is weapon
	Weapon* toWield = dynamic_cast<Weapon*>(m_inventory[sub]);  
	if(!toWield) // If requested object is not weapon
		return "You can't wield " + m_inventory[sub]->name();

	setWeapon(toWield); // Change player's weapon

	return "You are wielding " + toWield->name();
}

void Player::cheat()
	// Cheat by giving player the following stat boosts
{
	if(maxHitPts() < 50) setMaxHitPts(50);
	if(hitPts() < 50) setHitPts(50);
	if(strengthPts() < 9) setStrengthPts(9);
}