#include "Tetromino.h"

Tetromino::Tetromino()
{
}

// Formats tetromino with their respective colour
void Tetromino::GetPieceColor(int pID)
{
	switch(pID)
	{
		case 0: {Window::Format(0); break;} // Empty
		case 1: {Window::Format(6); break;} // I
		case 2: {Window::Format(3); break;} // O
		case 3: case 8: {Window::Format(5); break;} // T
		case 4: case 9: {Window::Format(2); break;} // S
		case 5: case 10: {Window::Format(1); break;} // Z
		case 6: case 11: {Window::Format(4); break;} // J
		case 7: case 12: {Window::Format(8); break;} // L
	}
}

// Copy new piece of pID [0-6] to currentPiece array
int Tetromino::GetNewPiece(int pID, Board* field, Board* currentPiece)
{
	for (int yPos = 0; yPos < 2; yPos++) {
		for (int xPos = 0; xPos < 4; xPos++) {
			if (field->Get(yPos,xPos) != 0)
				return 1;
			currentPiece->Set(yPos,F_WIDTH/2-2+xPos,tetromino[pID][yPos][xPos]);
		}
	}
	
	return 0;
}

// Moves elements in currentPiece array in y/x direction, returns 1 if successful
int Tetromino::MovePiece(int dy, int dx, Board* field, Board* currentPiece)
{

	// Initialise temporary array with edge buffer
	int tempField[F_HEIGHT+4][F_WIDTH+2] = {0};

	// Copy currentPiece to temporary array
	for (int yPos = 0; yPos < F_HEIGHT+2; yPos++) {
		for (int xPos = 0; xPos < F_WIDTH; xPos++) {
			tempField[yPos+1][xPos+1] = currentPiece->Get(yPos,xPos);
			// Check if movement will go out-of-bounds
			if (tempField[yPos+1][xPos+1] != 0 &&
			(yPos-dy >= F_HEIGHT || xPos-dx >= F_WIDTH || xPos-dx < 0)) {
				return 0;
			}
		}
	}

	// Check if movement will collide with field
	for (int yPos = 0; yPos < F_HEIGHT; yPos++) {
		for (int xPos = 0; xPos < F_WIDTH; xPos++) {
			if (tempField[yPos+1+dy][xPos+1+dx] != 0 && field->Get(yPos,xPos) != 0) {
				return 0;
			}
		}
	}

	// Allow movement
	for (int yPos = 0; yPos < F_HEIGHT+2; yPos++) {
		for (int xPos = 0; xPos < F_WIDTH; xPos++) {
			currentPiece->Set(yPos,xPos,tempField[yPos+1+dy][xPos+1+dx]);
		}
	}

	return 1;
}

// SRS offset algorithm for J, L, S, T, Z pieces
int Tetromino::Offset3(int rot, int state, Board* field, Board* currentPiece, int cY, int cX)
{
	int tempPiece[3][3] = {};
	int rotatedPiece[3][3] = {};

	// Copy currentPiece to 3x3 temporary array
	for (int yPos = 0; yPos < 3; yPos++) {
		for (int xPos = 0; xPos < 3; xPos++) {
			if (cY-1+yPos >= F_HEIGHT+2 || cY-1+yPos < 0 || cX-1+xPos >= F_WIDTH || cX-1+xPos < 0) {
				tempPiece[yPos][xPos] = 0;
			} else {
				tempPiece[yPos][xPos] = currentPiece->Get(cY-1+yPos,cX-1+xPos);
			}
		}
	}

	// Rotate temporary array
	for (int yPos = 0; yPos < 3; yPos++) {
		for (int xPos = 0; xPos < 3; xPos++) {
			switch(rot) {
				case 1: rotatedPiece[yPos][xPos] = tempPiece[3-xPos-1][yPos]; break;
				case -1: rotatedPiece[yPos][xPos] = tempPiece[xPos][3-yPos-1]; break;
			}
		}
	}

	// Declare offset
	int offsetYX[2] = {0,0};

	// Check whether if piece fits
	for (int yPos = 0; yPos < 3; yPos++) {
		for (int xPos = 0; xPos < 3; xPos++) {
			// Offset is required
			if (rotatedPiece[yPos][xPos] != 0 && (field->Get(cY-1+yPos,cX-1+xPos) != 0 ||
				cY-1+yPos > F_HEIGHT+2 || cX-1+xPos > F_WIDTH-1 || cX-1+xPos < 0))
			{
				int final;
				switch (rot) {
					case 1: {final = Window::Modulo((state+rot),4); break;}
					case -1: {final = Window::Modulo((state+rot),4)+4; break;}
				}

				bool offsetSuccess;
				
				// Offset tests
				for (int test = 0; test < 5; test++) {

					offsetSuccess = true;

					for (int yPos = 0; yPos < 3; yPos++) {
						for (int xPos = 0; xPos < 3; xPos++) {
							// Check whether if more offset is required
							if (cY-1+yPos-srsKick[final][test][0] > F_HEIGHT+2 ||
								cX-1+xPos+srsKick[final][test][1] > F_WIDTH-1 ||
								cX-1+xPos+srsKick[final][test][1] < 0){
								offsetSuccess = false;
								break;
							} else {
								if (rotatedPiece[yPos][xPos] != 0 &&
									(field->Get(cY-1+yPos-srsKick[final][test][0],cX-1+xPos+srsKick[final][test][1]) != 0))
								{
									 offsetSuccess = false;
									 break;
								}
							}
						}
						if (!offsetSuccess)
							break;
					}

					// Offset test sucessful, wipe currentPiece and paste rotatedPiece
					if (offsetSuccess) {
						offsetYX[0] = srsKick[final][test][0];
						offsetYX[1] = srsKick[final][test][1];
						// Clear currentPiece
						for (int yPos = 0; yPos < F_HEIGHT+2; yPos++) {
							for (int xPos = 0; xPos < F_WIDTH; xPos++) {
								currentPiece->Set(yPos,xPos,0);
							}
						}
						// Copy rotatedPiece back to currentPiece
						for (int yPos = 0; yPos < 3; yPos++) {
							for (int xPos = 0; xPos < 3; xPos++) {
								currentPiece->Set(cY-1+yPos-offsetYX[0],cX-1+xPos+offsetYX[1],rotatedPiece[yPos][xPos]);
							}
						}
						return Window::Modulo((state+rot),4);
					}
				}

				// All offset tests failed, piece cannot rotate
				return state;
			}							
		}
	}
	
	// No offset required
	for (int yPos = 0; yPos < 3; yPos++) {
		for (int xPos = 0; xPos < 3; xPos++) {
			currentPiece->Set(cY-1+yPos,cX-1+xPos,rotatedPiece[yPos][xPos]);
		}
	}
			
	return Window::Modulo((state+rot),4);
}

// SRS offset algorithm for I piece
int Tetromino::Offset4(int rot, int state, Board* field, Board* currentPiece, int cY, int cX)
{
	int tempPiece[4][4] = {};
	int rotatedPiece[4][4] = {};
	
	// Check whether if piece is horizontal or vertical
	switch(state) {
		case 0: case 2: // Horizontal
		{
			for (int xPos = 0; xPos < 4; xPos++) {
				tempPiece[(state == 0) ? 1 : 2][xPos] = currentPiece->Get(cY,cX+xPos);
			}
			break;
		}
		case 1: case 3: // Vertical
		{
			for (int yPos = 0; yPos < 4; yPos++) {
				tempPiece[yPos][(state == 3) ? 2 : 1] = currentPiece -> Get(cY+yPos,cX);
			}
		}
	}

	// Rotate temporary array
	for (int yPos = 0; yPos < 4; yPos++) {
		for (int xPos = 0; xPos < 4; xPos++) {
			switch(rot) {
				case 1: rotatedPiece[yPos][xPos] = tempPiece[4-xPos-1][yPos]; break;
				case -1: rotatedPiece[yPos][xPos] = tempPiece[xPos][4-yPos-1]; break;
			}
		}
	}

	// Declare offset
	int offsetYX[2] = {0,0};

	// Check whether if piece fits
	for (int yPos = 0; yPos < 4; yPos++) {
		for (int xPos = 0; xPos < 4; xPos++) {
			// Offset is required
			if (rotatedPiece[yPos][xPos] != 0 && (field->Get(cY+yPos,cX+xPos) != 0 ||
				cY+yPos > F_HEIGHT+2 || cX+xPos > F_WIDTH-1 || cX+xPos < 0))
			{
				int final;
				switch (rot) {
					case 1: {final = 8+Window::Modulo((state+rot),4); break;}
					case -1: {final = 8+Window::Modulo((state+rot),4)+4; break;}
				}

				bool offsetSuccess;
				
				// Offset tests
				for (int test = 0; test < 5; test++) {

					offsetSuccess = true;

					for (int yPos = 0; yPos < 4; yPos++) {
						for (int xPos = 0; xPos < 4; xPos++) {
							// Check whether if more offset is required
							if (cY+yPos-srsKick[final][test][0] > F_HEIGHT+2 ||
								cX+xPos+srsKick[final][test][1] > F_WIDTH-1 ||
								cX+xPos+srsKick[final][test][1] < 0){
								offsetSuccess = false;
								break;
							} else {
								if (rotatedPiece[yPos][xPos] != 0 &&
									(field->Get(cY+yPos-srsKick[final][test][0],cX+xPos+srsKick[final][test][1]) != 0))
								{
									 offsetSuccess = false;
									 break;
								}
							}
						}
						if (!offsetSuccess)
							break;
					}

					// Offset test sucessful, wipe currentPiece and paste rotatedPiece
					if (offsetSuccess) {
						offsetYX[0] = srsKick[final][test][0];
						offsetYX[1] = srsKick[final][test][1];
						// Clear currentPiece
						for (int yPos = 0; yPos < F_HEIGHT+2; yPos++) {
							for (int xPos = 0; xPos < F_WIDTH; xPos++) {
								currentPiece->Set(yPos,xPos,0);
							}
						}
						// Copy rotatedPiece back to currentPiece
						for (int yPos = 0; yPos < 4; yPos++) {
							for (int xPos = 0; xPos < 4; xPos++) {
								currentPiece->Set(cY+yPos-offsetYX[0],cX+xPos+offsetYX[1],rotatedPiece[yPos][xPos]);
							}
						}
						return Window::Modulo((state+rot),4);
					}
				}

				// All offset tests failed, piece cannot rotate
				return state;
			}							
		}
	}
	
	// No offset required
	for (int yPos = 0; yPos < 4; yPos++) {
		for (int xPos = 0; xPos < 4; xPos++) {
			currentPiece->Set(cY+yPos,cX+xPos,rotatedPiece[yPos][xPos]);
		}
	}
			
	return Window::Modulo((state+rot),4);
}

// Rotates the current piece CW or CCW
int Tetromino::Rotate(int rot, int state, Board* field, Board* currentPiece)
{
	// Rotation variables
	int cX, cY;

	// Scan currentPiece for piece type
	for (int yPos = 0; yPos < F_HEIGHT+2; yPos++) {
		for (int xPos = 0; xPos < F_WIDTH; xPos++) {
			if (currentPiece->Get(yPos,xPos) > 7) { // J,L,S,T,Z
				// Get pivot position
				cY = yPos;
				cX = xPos;
				// Rotate
				return Offset3(rot, state, field, currentPiece, cY, cX);
			} else if (currentPiece->Get(yPos,xPos) == 1) { // I
				// Get first point
				cY = yPos;
				cX = xPos;
				// Rotate
				return Offset4(rot, state, field, currentPiece, cY, cX);
			} else if (currentPiece->Get(yPos,xPos) == 2) { // O
				// O piece does not rotate.
				return state;
			}
		}
	}
}

// Copy currentPiece to field array
int Tetromino::SetPiece(Board* field, Board* currentPiece)
{
	for (int yPos = 0; yPos < F_HEIGHT; yPos++) {
		for (int xPos = 0; xPos < F_WIDTH; xPos++) {
			if (field->Get(yPos,xPos) == 0) {
				field->Set(yPos,xPos,currentPiece->Get(yPos,xPos));
			}
		}
	}

	// Clear currentPiece field
	for (int yPos = 0; yPos < F_HEIGHT+2; yPos++) {
		for (int xPos = 0; xPos < F_WIDTH; xPos++) {
			currentPiece->Set(yPos,xPos,0);
		}
	}

	return 0;
}

// Drops current piece to bottom
void Tetromino::HardDrop(Board* field, Board* currentPiece)
{
	bool isFalling = true;

	while(isFalling)
	{
		isFalling = Tetromino::MovePiece(-1, 0, field, currentPiece);
	}

	Tetromino::SetPiece(field, currentPiece);
}

Tetromino::~Tetromino()
{
}
