// main.cpp

#include <termios.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <iostream>

using namespace std;

#include "Config.h"
#include "Window.h"
#include "Board.h"
#include "Tetromino.h"

// Declare termios attributes
struct itimerval tv;

// Game state variables
bool isGameOver;
bool isPieceActive;
bool pcFlag;
int rotState;
int holding;

// Holds the current piece and swaps the current piece with the held piece
int Hold(int holding, Board* field, Board* currentPiece)
{	
	// Find active piece ID and wipe currentPiece
	int active;
	for (int yPos = 0; yPos < F_HEIGHT+2; yPos++) {
		for (int xPos = 0; xPos < F_WIDTH; xPos++) {
			// Get ID value (not pivot)
			if (currentPiece->Get(yPos,xPos) != 0) {
				if (currentPiece->Get(yPos,xPos) > 7) {
					active = currentPiece->Get(yPos,xPos)-5;
				} else {
					active = currentPiece->Get(yPos,xPos);
				}
			}
			// Delete cell
			currentPiece->Set(yPos,xPos,0);
		}
	}

	// Check for existing piece in hold
	if (holding == 9) {
		// No piece in hold, get new piece
		isPieceActive = false;
	} else {
		// Existing piece in hold, get held piece
		Tetromino::GetNewPiece(holding-1, field, currentPiece);
	}
	rotState = 0;
	pcFlag = false;
	return active;
}

// Prints the current frame
void Update(Board* field, Board* currentPiece, int* bag1, int* bag2, int order, int level, int score, int lines, int combo)
{
	// Top indent
	Window::Format(0);
	cout << endl;

	// TODO: This is disgusting, but it works
	for (int yPos = 0; yPos < F_HEIGHT+2; yPos++) {
		// Side indent
		Window::Format(0);
		cout << "  ";

		for (int xPos = 0; xPos < F_WIDTH+8; xPos++) {
			// Default formatting
			Window::Format(0);
			if (xPos == 0 || xPos == F_WIDTH+1) {
				// Render field walls
				Window::Format(7);
				cout << "  ";
			} else if ((bag1[order] < 2) && (xPos == F_WIDTH+2 && yPos >= 2 && yPos < 4)) {
				// Center preview I, O pieces
				Window::Format(0);
				cout << "   ";
			} else if ((bag1[order] >= 2) && (xPos == F_WIDTH+2 && yPos >= 2 && yPos < 4)) {
				// Center preview J, L, S, T, Z pieces
				Window::Format(0);
				cout << "    ";
			} else if ((holding-1 < 2) && (xPos == F_WIDTH+2 && yPos >= 7 && yPos < 9)) {
				// Center hold I, O pieces
				Window::Format(0);
				cout << "   ";
			} else if ((holding-1 >= 2) && (xPos == F_WIDTH+2 && yPos >= 7 && yPos < 9)) {
				// Center hold J, L, S, T, Z pieces
				Window::Format(0);
				cout << "    ";
			} else if (yPos == 12 && xPos == F_WIDTH+3) {
				// Line clear display
				switch (lines) {
					case 2: {cout << " DOUBLE "; break;}
					case 3: {cout << " TRIPLE "; break;}
					case 4: {cout << " TETRIS!"; break;}
					case 8: {cout << " PERFECT"; break;}
				}
			} else if (yPos == 13 && xPos == F_WIDTH+3) {
				switch (lines) {
					case 8: {cout << " CLEAR "; break;}
				}
			} else if (yPos == 15 && xPos == F_WIDTH+3) {
				if (combo > 1)
					cout << " x" << combo;
			} else if (yPos == 17 && xPos == F_WIDTH+3) {
				cout << "LEVEL " << level;
			} else if (yPos == 19 && xPos == F_WIDTH+3) {
				cout << "SCORE: ";
			} else if (yPos == 20 && xPos == F_WIDTH+3) {
				cout << score;
			} else if ((yPos == 0 && xPos < F_WIDTH) || xPos == F_WIDTH+2) {
				// Render field top and vertical divider
				Window::Format(0);
				cout << "  ";
			} else if ((yPos == F_HEIGHT+1 && xPos < F_WIDTH+1) ||
					   (yPos == 0 && xPos > F_WIDTH+2) ||
					   (yPos == F_HEIGHT-15 && xPos > F_WIDTH+2) ||
					   (yPos == F_HEIGHT-10 && xPos > F_WIDTH+2)) {
				// Render field bottom and horizontal dividers
				Window::Format(7);
				cout << "  ";
			} else if (xPos >= F_WIDTH+3 && xPos < F_WIDTH+7 && yPos >= 2 && yPos < 4) {
				// Render preview
				Tetromino::GetPieceColor(tetromino[bag1[order]][yPos-2][xPos-(F_WIDTH+3)]);
				cout << "  ";
			} else if (xPos >= F_WIDTH+3 && xPos < F_WIDTH+7 && yPos >= 7 && yPos < 9 && holding != 9) {
				// Render hold
				Tetromino::GetPieceColor(tetromino[holding-1][yPos-7][xPos-(F_WIDTH+3)]);
				cout << "  ";
			} else if (currentPiece->Get(yPos-1,xPos-1) != 0) {
				// Render current piece
				Tetromino::GetPieceColor(currentPiece->Get(yPos-1,xPos-1));
				cout << "  ";
			} else {
				// Render field
				Tetromino::GetPieceColor(field->Get(yPos-1,xPos-1));
				cout << "  ";
			}
		}
		Window::Format(0);
		cout << endl;
	}
}

// Keypress event handler
int GetKeyInput(Board* field, Board* currentPiece)
{
	int keyPress = getchar();

	switch(keyPress)
	{
		case KEY_LEFT: {Tetromino::MovePiece(0,1, field, currentPiece); break;}
		case KEY_RIGHT: {Tetromino::MovePiece(0,-1, field, currentPiece); break;}
		case KEY_DOWN: {Tetromino::MovePiece(-1,0, field, currentPiece); break;}
		case KEY_UP:
		case KEY_ROTATE_CW: {rotState = Tetromino::Rotate(1, rotState, field, currentPiece); break;}
		case KEY_ROTATE_CCW: {rotState = Tetromino::Rotate(-1, rotState, field, currentPiece); break;}
		case KEY_ROTATE_180: {
			rotState = Tetromino::Rotate(1, rotState, field, currentPiece);
			rotState = Tetromino::Rotate(1, rotState, field, currentPiece);
			break;
		}
		case KEY_HARD_DROP: {Tetromino::HardDrop(field, currentPiece); break;}
		case KEY_HOLD: {holding = Hold(holding, field, currentPiece); break;}
		case KEY_EXIT: {isGameOver = true; break;}
	}

	return keyPress;

}

// Executes on exit
void Quit(int score, struct termios oldAttr)
{
	Window::ShowCursor(true);
	tcsetattr(0, TCSANOW, &oldAttr);
	cout << endl << "   Game over! Your score: " << score << endl << endl;
	return;

}

// SIGINT handler, code from: https://stackoverflow.com/questions/1641182/how-can-i-catch-a-ctrl-c-event
void SignalHandler(int signal)
{
	switch(signal) {
		case SIGTERM:
		case SIGINT:
		case SIGSEGV:
			isGameOver = true;
			break;
		case SIGALRM:
			// Increase game speed by 0.1% every tick
			tv.it_value.tv_usec -= tv.it_value.tv_usec / 10000;
			setitimer(0, &tv, NULL);
			break;
	}
}

int main(int argc, char* argv[])
{
	// Initialise signal handler
	struct sigaction siga;

	sigemptyset(&siga.sa_mask);

	siga.sa_flags = 0;
	siga.sa_handler = SignalHandler;

	sigaction(SIGALRM, &siga, NULL);
	sigaction(SIGTERM, &siga, NULL);
	sigaction(SIGINT,  &siga, NULL);
	sigaction(SIGSEGV, &siga, NULL);

	// Dummy SIGALRM handler to interrupt getchar() call
	// https://stackoverflow.com/questions/5322143/add-a-timeout-for-getchar
	tv.it_value.tv_usec = FRAME_USEC;
	SignalHandler(SIGALRM);

	// Initialise terminal attributes
	struct termios oldAttr;
	struct termios newAttr;

	tcgetattr(STDIN_FILENO, &oldAttr);
	newAttr = oldAttr;
	newAttr.c_lflag &= ~(ICANON|ECHO);
	newAttr.c_cc[VTIME] = 0;
	newAttr.c_cc[VMIN] = 0;
	tcsetattr(0, TCSANOW, &newAttr);
	
	Window::ShowCursor(false);

	// Seed rand()
	srand(time(NULL));

	// Initialise board
	Board* field = new Board(0);
	Board* currentPiece = new Board(1);
	
	// Initialise game state
	int debugTick = 0, moveTick = 0, holdTick = 0, lockTick = 0, textDelay = 0;
	int score = 0, level = 1, currentLines = 0, linesCleared = 0, lastClear = 0, combo = 0;
	int bagID = 0;
	int *bag1 = Board::GenerateBag();
	int *bag2 = Board::GenerateBag();
	holding = 9;
	pcFlag = false;

	// Clear terminal output
	cout << "\033[2J";

	// Game loop
	while (!isGameOver)
	{
		// Game tick (1000000 = 1s)
		debugTick++;
		moveTick++;
		textDelay++;

		// Game logic

		// Refresh game states
		isPieceActive = false;
		
		for (int yPos = 0; yPos < F_HEIGHT+2; yPos++) {
			for (int xPos = 0; xPos < F_WIDTH; xPos++) {
				if (currentPiece->Get(yPos,xPos) != 0) {
					isPieceActive = true;
				}
			}
		}

		// Generate new piece if no piece is active
		if (!isPieceActive && bagID < 7) {
			// Game over if no piece can spawn
			isGameOver = Tetromino::GetNewPiece(bag1[bagID], field, currentPiece);
			isPieceActive = true;
			rotState = 0;
			
			// Get lines cleared and calculate score
			linesCleared = Board::ClearLine(field, pcFlag);
			// Make perfect clear available
			pcFlag = true;
			currentLines += linesCleared;
			lastClear = linesCleared;
			
			switch (linesCleared) {
				case 1: {score += 80*(level+combo); break;} // Single
				case 2: {score += 200*(level+combo); break;} // Double
				case 3: {score += 600*(level+combo); break;} // Triple
				case 4: {score += 2400*(level+combo); break;} // Tetris
				case 8: {score += 7600*(level+combo); break;} // Perfect clear
			}
			// Calculate combo
			if (lastClear > 0 && linesCleared > 0){
				combo++;
			} else {
				combo = 0;
			}
			// Change level every 20 lines
			if (currentLines > 20) {
				level++;
				// Speed up by 5%
				tv.it_value.tv_usec -= tv.it_value.tv_usec / 20;
				currentLines = 0;
			}
			// Next piece
			bagID++;
		}

		// Generate new bag on end of bag sequence
		if (bagID == 7) {
			// Swap bag
			delete bag1;
			int *bag1 = new int[7];
			for (int i = 0; i < 7; i++)
				bag1[i] = bag2[i];
			// Get new bag
			delete bag2;
			int *bag2 = Board::GenerateBag();
			bagID = 0;
		}

		// Update
		cout << "\033[H";
		Update(field, currentPiece, bag1, bag2, bagID, level, score, lastClear, combo);

		// Input handler
		int key = GetKeyInput(field, currentPiece);

		// Gravity
		if (moveTick == 50) {
			(Tetromino::MovePiece(-1,0, field, currentPiece)) ? lockTick = 0 : lockTick++;
			if (lockTick == 2)
				Tetromino::SetPiece(field, currentPiece);
			moveTick = 0;
		}

		// Line clear text timeout
		if (textDelay == 250) {
			lastClear = 0;
			textDelay = 0;
		}

		// Debug info
		if (argv[1]) {
			cout << endl << "   ----- DEBUG INFO -----" << endl;
			cout << "   Tick: " << debugTick << endl;

			for (int y = 0; y < F_HEIGHT; y++) {
				cout << "   ";
				// Field values
				for (int x = 0; x < F_WIDTH; x++) {
					if (field->Get(y,x) > 7) {
						cout << field->Get(y,x)-5;
					} else {
						cout << field->Get(y,x);
					}
				}
				cout << "  ";
				// currentPiece values
				for (int x = 0; x < F_WIDTH; x++) {
					if (currentPiece->Get(y,x) > 7) {
						cout << currentPiece->Get(y,x)-5;
					} else {
						cout << currentPiece->Get(y,x);
					}
				}
				cout << endl;
			}
			cout << endl << "   B1 sequence: ";
			for (int i = 0; i < 7; i++) {
				cout << bag1[i];
			}
			cout << endl << "   B2 sequence: ";
			for (int i = 0; i < 7; i++) {
				cout << bag2[i];
			}
			cout << endl;
			cout << "   Keypress: " << key << endl;
			cout << "   rotState: " << rotState << endl;
			cout << "   holding: " << holding << endl;
			cout << "   holdTick: " << holdTick << endl;
			cout << "   pcFlag: " << pcFlag << endl;
			cout << "   level: " << level << endl;
		} else {
			cout << endl << "   How to play:" << endl;
			cout << "   [←] Move left     [→] Move right" << endl;
			cout << "   [↓] Soft drop     [_] Hard drop" << endl;
			cout << "   [Z] Rotate CCW    [X] Rotate CW" << endl;
			cout << "   [A] Rotate 180°   [C] Hold" << endl;
			cout << "   [Q] Quit game" << endl << endl;
			cout << "   < Tetris++ by samillwong >" << endl;
			cout << "   < https://github.com/SamillWong >" << endl;
		}

	}

	delete bag1;
	delete bag2;

	delete field;
	delete currentPiece;

	Quit(score, oldAttr);

	return 0;

}
