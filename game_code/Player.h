// Player.h

#ifndef PLAYER_INCLUDED
#define PLAYER_INCLUDED

#include "Actor.h"
#include <vector>

class GameObject;

const int BASEHEALTH = 20;
const char PLAYER = '@';

////////////////////////////////////////////////////////////////////
// Player Class Declaration
////////////////////////////////////////////////////////////////////

class Player : public Actor
{
  public:
  	/////////////////////////////////////////////////////////////////
	// Constructor and Destructor
	Player();
	~Player();

	/////////////////////////////////////////////////////////////////
	// Accessors
	int maxHitPts() const {return m_maxHit;}
	
	/////////////////////////////////////////////////////////////////
	// Setters
	bool setMaxHitPts(int maxHitPts);

	// Name and symbol
	virtual std::string name() const {return "Player";}
	virtual char symbol() const      {return '@';}

	/////////////////////////////////////////////////////////////////
	// Game Functions

	void attemptRegainHealth();
	std::string attemptPickUp(GameObject* object); // Pick up object
	void displayInventory() const;
	std::string attemptReadScroll(char key);// Read scroll 
	std::string attemptWieldWeapon(char key);// Change Weapon 
	void cheat(); // Cheat function
	
  private:
	/////////////////////////////////////////////////////////////////
	
	int m_maxHit; // Max hit points
	std::vector<GameObject*> m_inventory;// Inventory 

	// Helper functions
	int keyToSubscript(char key) const;

	/////////////////////////////////////////////////////////////////
};

/////////////////////////////////////////////////////////////////////
// Inline Implementations
inline
bool Player::setMaxHitPts(int maxHitPts)
{
	if(maxHitPts < 0 || maxHitPts > MAXHIT) return false;
	m_maxHit = maxHitPts; return true;
}

inline
void Player::attemptRegainHealth()
	// If health less than max hit, regain 1 hit point
{
	if(hitPts() < m_maxHit) setHitPts(hitPts()+1);
}

inline
int Player::keyToSubscript(char key) const
// Converts a character key into a subscript for player's inventory
// Returns -1 if invalid key
{
	int sub =  key - 'a';
	return (sub < m_inventory.size()) ? sub : -1; 
}

#endif // PLAYER_INCLUDED