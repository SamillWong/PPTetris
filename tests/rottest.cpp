#include <cmath>
#include <iostream>

using namespace std;

int array[20][10] = {};

int Rotate(int rot)
{
    // Rotation variables
    int nX, nY, cX, cY;

    // Find rotation pivot of current piece
    for (int yPos = 0; yPos < 20; yPos++) {
        for (int xPos = 0; xPos < 10; xPos++) {
            if (array[yPos][xPos] == 2) { // J,L,S,T,Z
                cY = yPos;
                cX = xPos;
				cout << "cY: " << cY << " cX: " << cX << endl;

                int tempPiece[3][3] = {};
                int rotatedPiece[3][3] = {};

                // Copy currentPiece to 3x3 temporary array
				cout << "tempCopy:" << endl;
                for (int yPos = 0; yPos < 3; yPos++) {
                    for (int xPos = 0; xPos < 3; xPos++) {
                        tempPiece[yPos][xPos] = array[cY-1+yPos][cX-1+xPos];
						cout << tempPiece[yPos][xPos];
                    }
					cout << endl;
                }

				cout << endl;

                // Rotate temporary array
				cout << "rotateTemp:" << endl;
                for (int yPos = 0; yPos < 3; yPos++) {
                    for (int xPos = 0; xPos < 3; xPos++) {
						switch(rot) {
	                    	case 1: rotatedPiece[yPos][xPos] = tempPiece[3-xPos-1][yPos]; break;
							case -1: rotatedPiece[yPos][xPos] = tempPiece[xPos][3-yPos-1]; break;
						}
						cout << rotatedPiece[yPos][xPos];
                    }
					cout << endl;
                }

				cout << endl;

                // Copy rotatedPiece back to currentPiece
				cout << "recopy:" << endl;
                for (int yPos = 0; yPos < 3; yPos++) {
                    for (int xPos = 0; xPos < 3; xPos++) {
                        array[cY-1+yPos][cX-1+xPos] = rotatedPiece[yPos][xPos];
                    }
                }

				return 0;

			}
		}
	}

}

int main(void) {

	array[9][5] = 1;
	for (int i = 0; i < 3; i++) {
		array[10][4+i] = i+1;
	}

	for (int y = 0; y < 20; y++) {
		for (int x = 0; x < 10; x++) {
			cout << array[y][x];
		}
		cout << endl;
	}

	Rotate(1);
	cout << endl;

	for (int y = 0; y < 20; y++) {
		for (int x = 0; x < 10; x++) {
			cout << array[y][x];
		}
		cout << endl;
	}

}
