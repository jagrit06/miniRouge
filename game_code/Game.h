// Game.h

#ifndef GAME_INCLUDED
#define GAME_INCLUDED

class Dungeon;
class Player;

////////////////////////////////////////////////////////////////////
// Game Class Declarations
////////////////////////////////////////////////////////////////////

class Game
{
  public:
    Game(int goblinSmellDistance);
	~Game();

    void play();

  private:
	Player* m_player;       // Player
	Dungeon* m_currDungeon; // current level Dungeon 
	int m_goblinSmellDist;  // Goblin smell distance

	// Private Helper Functions
	bool commandToNewPos(char command, int& newRow, int& newCol);
	void proceedToNextLevel();
};

#endif // GAME_INCLUDED
