#include <iostream>
#include <string>
#include <cstdlib>

#include "GameObject.h"
#include "Scroll.h"
#include "Dungeon.h"
#include "Player.h"
#include "utilities.h"

using namespace std;

////////////////////////////////////////////////////////////////////
// Derived Scroll Classes Implementations
////////////////////////////////////////////////////////////////////

void TeleportationScroll::haveEffect(Player* user)
	// The player is randomly moved to another place in the 
	// level that is not occupied by a wall or a monster.
{
	// Produce new open position on the dungeon
	int newRow, newCol;
	dungeon()->produceOpenPos(newRow, newCol);
	// Move user to new position
	user->setPos(newRow, newCol);
}

void ArmorScroll::haveEffect(Player* user)
	// The player's armor points are increased by a 
	// random integer from 1 to 3.
{
	int newArmor = (user->armorPts() + randInt(1, 3));
	// Increasing only to max armor pts if new armor pts is greater
	if(newArmor > MAXARMOR) newArmor = MAXARMOR;
	user->setArmorPts(newArmor);
}

void StrengthScroll::haveEffect(Player* user)
	// The player's strength points are increased by a 
	// random integer from 1 to 3.
{
	int newStrength = (user->strengthPts() + randInt(1, 3));
	// Increasing only to max strength pts if new strength pts is greater
	if(newStrength > MAXSTRENGTH) newStrength = MAXSTRENGTH;
	user->setStrengthPts(newStrength);	
}

void HealthScroll::haveEffect(Player* user)
	// The player's maximum hit point value is increased by a 
	// random integer from 3 to 8. This scroll does not affect 
	// the player's current number of hit points.
{
	int newMaxHit = (user->maxHitPts() + randInt(3, 8));
	// Increasing only to max hit pts if new max hit pts is greater
	if(newMaxHit > MAXHIT) newMaxHit = MAXHIT;
	user->setMaxHitPts(newMaxHit);	
}

void DexterityScroll::haveEffect(Player* user)
	// The player's dexterity is increased by 1.
{
	int newDex = (user->dexterityPts() + 1);
	// Increasing only to max dex pts if new dex pts is greater
	if(newDex > MAXDEXTERITY) newDex = MAXDEXTERITY;
	user->setDexterityPts(newDex);	
}