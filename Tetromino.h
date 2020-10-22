#ifndef TETROMINO_H
#define TETROMINO_H

#include "Config.h"
#include "Window.h"
#include "Board.h"

// Piece data
const int tetromino[7][2][4] = {{{0,0,0,0}, // I: 1
								 {1,1,1,1}},
								{{0,2,2,0}, // O: 2
								 {0,2,2,0}},
								{{0,3,0,0}, // T: 3, 8
								 {3,8,3,0}},
								{{0,4,4,0}, // S: 4, 9
								 {4,9,0,0}},
								{{5,5,0,0}, // Z: 5, 10
								 {0,10,5,0}},
								{{6,0,0,0}, // J: 6, 11
								 {6,11,6,0}},
								{{0,0,7,0}, // L: 7, 12
								 {7,12,7,0}}};

// J, L, S, T, Z Tetromino Wall Kick Data in (y,x)
// Table from https://tetris.wiki/Super_Rotation_System
const int srsKick[16][5][2]  = {{{0,0},{0,-1},{-1,-1},{+2,0},{+2,-1}},	//L->0 : 3+1 % 4 = 0 - J, L, S, T, Z
								{{0,0},{0,-1},{+1,-1},{-2,0},{-2,-1}},	//0->R : 0+1 % 4 = 1
								{{0,0},{0,+1},{-1,+1},{+2,0},{+2,+1}},	//R->2 : 1+1 % 4 = 2
								{{0,0},{0,+1},{+1,+1},{-2,0},{-2,+1}},	//2->L : 2+1 % 4 = 3
								{{0,0},{0,+1},{-1,+1},{+2,0},{+2,+1}},	//R->0 : 1-1 % 4 = 0
								{{0,0},{0,-1},{+1,-1},{-2,0},{-2,-1}},	//2->R : 2-1 % 4 = 1
								{{0,0},{0,-1},{-1,-1},{+2,0},{+2,-1}},	//L->2 : 3-1 % 4 = 2
								{{0,0},{0,+1},{+1,+1},{-2,0},{-2,+1}},	//0->L : 0-1 % 4 = 3
								{{0,0},{0,+1},{0,-2},{-2,+1},{+1,-2}},	//L->0 : 3+1 % 4 = 0 - I
								{{0,0},{0,-2},{0,+1},{-1,-2},{+2,+1}},	//0->R : 0+1 % 4 = 1
								{{0,0},{0,-1},{0,+2},{+2,-1},{-1,+2}},	//R->2 : 1+1 % 4 = 2
								{{0,0},{0,+2},{0,-1},{+1,+2},{-2,-1}},	//2->L : 2+1 % 4 = 3
								{{0,0},{0,+2},{0,-1},{+1,+2},{-2,-1}},	//R->0 : 1-1 % 4 = 0
								{{0,0},{0,+1},{0,-2},{-2,+1},{+1,-2}},	//2->R : 2-1 % 4 = 1
								{{0,0},{0,-2},{0,+1},{-1,-2},{+2,+1}},	//L->2 : 3-1 % 4 = 2
								{{0,0},{0,-1},{0,+2},{+2,-1},{-1,+2}}};	//0->L : 0-1 % 4 = 3

class Tetromino : public Board
{
	public:
		Tetromino();
		~Tetromino();

		// Public access methods
		static void GetPieceColor(int pID);
		static int GetNewPiece(int pID, Board* field, Board* currentPiece);
		static int MovePiece(int dy, int dx, Board* field, Board* currentPiece);
		static int Offset3(int rot, int state, Board* field, Board* currentPiece, int cY, int cX);
		static int Offset4(int rot, int state, Board* field, Board* currentPiece, int cY, int cX);
		static int Rotate(int rot, int state, Board* field, Board* currentPiece);
		static int SetPiece(Board* field, Board* currentPiece);
		static void HardDrop(Board* field, Board* currentPiece);
};

#endif