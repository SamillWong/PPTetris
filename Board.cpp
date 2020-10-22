// Board.cpp

#include <stdlib.h>

#include "Config.h"
#include "Window.h"
#include "Board.h"

// Default constructor for active playfield
Board::Board()
{
	boardData = new int*[F_HEIGHT];
	for (int i = 0; i < F_HEIGHT; i++)
		boardData[i] = new int[F_WIDTH]();
}

// Overloaded constructor for each board type
Board::Board(int type)
{
	switch (type) {
		case 0: // Active playfield
		{
			maxY = F_HEIGHT;
			maxX = F_WIDTH;
			boardData = new int*[F_HEIGHT];
			for (int i = 0; i < F_HEIGHT; i++)
				boardData[i] = new int[F_WIDTH]();
			break;
		}
		case 1: // Current piece
		{
			maxY = F_HEIGHT+2;
			maxX = F_WIDTH;
			boardData = new int*[F_HEIGHT+2];
			for (int i = 0; i < F_HEIGHT+2; i++)
				boardData[i] = new int[F_WIDTH]();
			break;
		}
	}
}

// boardData Getter
int Board::Get(int yPos, int xPos)
{
	// Prevent out of bounds access
	if (!(yPos >= maxY || yPos < 0 || xPos >= F_WIDTH || xPos < 0))
		return boardData[yPos][xPos];
}

// boardData Setter
void Board::Set(int yPos, int xPos, int val)
{
	// Prevent out of bounds access
	if (!(yPos >= maxY || yPos < 0 || xPos >= F_WIDTH || xPos < 0))
		boardData[yPos][xPos] = val;
}

// Randomly generates next 7-bag piece sequence
int* Board::GenerateBag()
{
	// Dynamically initialise bag array
	int* bag = new int[7];

	// Default bag to invalid value
	for (int bagID = 0; bagID < 7; bagID++) {
		bag[bagID] = 7;
	}

	// Declare function variables
	int i = 0;
	int newID;

	// Get number sequence
	while (i != 7)
	{
		// Assume newID as valid
		bool isValidID = true;

		// Get new random ID [0-6]
		newID = rand() % 7;

		// Reject newID if already in bag
		for (int bagID = 0; bagID < 7; bagID++) {
			if (bag[bagID] == newID) {
				isValidID = false;
				break;
			}
		}

		// Set newID as next ID
		if (isValidID) {
			bag[i] = newID;
			i++;
		}

	}

	return bag;
}

// Checks for clearable lines and deletes them, returns lines cleared
int Board::ClearLine(Board* field, bool pc)
{
	int linesCleared = 0;
	// Loop over all lines
	for (int yPos = 0; yPos < F_HEIGHT; yPos++) {
		// Assume line as clearable
		bool isClearable = true;
		// Loop over line elements
		for (int xPos = 0; xPos < F_WIDTH; xPos++) {
			// Set line as unclearable if empty space found
			if (field->Get(yPos,xPos) == 0) {
				isClearable = false;
				break;
			}
		}
		// Clear line
		if (isClearable) {
			linesCleared++;
			for (int line = 0; line < yPos; line++) {
				for (int xPos = 0; xPos < F_WIDTH; xPos++) {
					field->Set(yPos-line,xPos,field->Get(yPos-line-1,xPos));
				}
			}
		}
	}
	// Perfect clear
	for (int yPos = 0; yPos < F_HEIGHT; yPos++) {
		for (int xPos = 0; xPos < F_WIDTH; xPos++) {
			if (field->Get(yPos,xPos) != 0)
				return linesCleared;
		}
	}
	if (pc) {
		return 8;
	} else {
		return linesCleared;
	}
}

// Destructor
Board::~Board()
{
	delete boardData;
}
