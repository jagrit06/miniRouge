#include <iostream>
#include <string>
#include <cstdlib>

#include "Weapon.h"
#include "Actor.h"
#include "utilities.h"

using namespace std;

////////////////////////////////////////////////////////////////////
// Weapon Class Implementations
////////////////////////////////////////////////////////////////////

string SleepFangs::specialMove(Actor* attacker, Actor* defender)
	// Put the defender to sleep with a 1 in 5 chance
{
	if(trueWithProbability(1.0/5.0))
	{
		int newSleepTime = randInt(2, 6);
		// If the new sleep time is greater than the old one
		if(newSleepTime > defender->sleepPts())
		{
			// Put defender to sleep for the longer time
			defender->setSleepPts(newSleepTime);
			return ", putting " + defender->name() + " to sleep.";
		}
	}
	return "."; // Indicating nothing else happened
}