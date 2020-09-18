// utilities.cpp

// Note:  YOU MUST NOT MAKE ANY CHANGE TO THIS FILE!

#include "utilities.h"
#include <random>
#include <utility>
using namespace std;

static random_device rd;
static default_random_engine generator(rd());

  // Return a uniformly distributed random integer from lowest to highest inclusive
int randInt(int lowest, int highest)
{
    if (highest < lowest)
        swap(highest, lowest);
    uniform_int_distribution<> distro(lowest, highest);
    return distro(generator);
}

  // Return a uniformly distributed random integer from 0 to limit-1 inclusive
int randInt(int limit)
{
    return randInt(0, limit-1);
}

  // return true with a given probability
bool trueWithProbability(double p)
{
    uniform_real_distribution<> distro(0, 1);
    return distro(generator) < p;
}

#ifdef _MSC_VER  //  Microsoft Visual C++

#include <windows.h>
#include <conio.h>

char getCharacter()
{
    int c = _getch();
    if (c != 0xE0  &&  c != 0x00)  // first of the two sent by arrow keys
        return c;
    else
    {
        c = _getch();
        switch (c)
        {
          case 'K':  return ARROW_LEFT;
          case 'M':  return ARROW_RIGHT;
          case 'H':  return ARROW_UP;
          case 'P':  return ARROW_DOWN;
          default:   return '?';
        }
    }
}

void clearScreen()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
    COORD upperLeft = { 0, 0 };
    DWORD dwCharsWritten;
    FillConsoleOutputCharacter(hConsole, TCHAR(' '), dwConSize, upperLeft,
							&dwCharsWritten);
    SetConsoleCursorPosition(hConsole, upperLeft);
}

#else  // not Microsoft Visual C++, so assume UNIX interface

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <termios.h>
using namespace std;

static const char* ESC_SEQ = "\x1B[";  // ANSI Terminal escape sequence:  ESC [
static const char* ESC_SEQ_X = "\xEF\x9C";  // Xcode Console arrow key escape sequence

class TerminalModeSetter
{
  public:
    TerminalModeSetter()
    {
        tcgetattr(STDIN_FILENO, &m_old);
        m_new = m_old;
        m_new.c_lflag &= ~(ICANON | ECHO);
        m_new.c_cc[VMIN] = 1;
        m_new.c_cc[VTIME] = 0;
        tcsetattr(STDIN_FILENO, TCSANOW, &m_new);
    }
    ~TerminalModeSetter()
    {
        tcsetattr(STDIN_FILENO, TCSANOW, &m_old);
    }
  private:
    termios m_old;
    termios m_new;
};

char getCharacter()
{
    static TerminalModeSetter dummy;
    char ch;
    if (!cin.get(ch))
        return '?';
    if (ch != ESC_SEQ[0]  &&  ch != ESC_SEQ_X[0])
        return ch;
    if (!cin.get(ch)  ||  (ch != ESC_SEQ[1]  &&  ch != ESC_SEQ_X[1])  ||  !cin.get(ch))
        return '?';
    switch (ch)
    {
      case 'D': case '\x82':  return ARROW_LEFT;
      case 'C': case '\x83':  return ARROW_RIGHT;
      case 'A': case '\x80':  return ARROW_UP;
      case 'B': case '\x81':  return ARROW_DOWN;
      default:   return '?';
    }
}

// Note to Xcode users:  clearScreen() will just write a newline instead
// of clearing the window if you launch your program from within Xcode.
// That's acceptable.  (The Xcode output window doesn't have the capability
// of being cleared.)

void clearScreen()  // will just write a newline in an Xcode output window
{
    static const char* term = getenv("TERM");
    if (term == nullptr  ||  strcmp(term, "dumb") == 0)
        cout << endl;
     else
        cout << ESC_SEQ << "2J" << ESC_SEQ << "H" << flush;
}

#endif
