#ifndef BOARD_H
#define BOARD_H

class Board : public Window
{
	public:
		Board();
		Board(int type);
		~Board();
		
		// Public access methods
		int Get(int yPos, int xPos);
		void Set(int yPos, int xPos, int val);
		
		static int* GenerateBag();
		static int ClearLine(Board* field, bool pc);
		
	protected:
		int** boardData;
		int maxY;
		int maxX;
};

#endif