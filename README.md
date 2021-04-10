# Tetris++
Modern Tetris with SRS rotation and hold, fully written in C++.

![Preview](https://i.imgur.com/0gChQOI.png)

### Project Description
- The aim of this project is to develop a working copy of the Tetris game in command line with C++, with support for modern functionalities.
- Tetris is a traditional tile-matching video game, where it has the very simple goal of destroying lines of block before the player reaches the top, or otherwise "topping out".
- There are 7 "Tetromino" available for the player to stack with, they are the I, O, T, S, Z, J, and L shaped pieces. Each tetromino is made of four square blocks, hence the "tetro-" prefix.
- The game takes place on a 20x10 "board", or playfield, where pieces spawn at the top of the board, falling down at a constant rate.
- The player can:
	- Move the current piece left [LeftArrow] or right [RightArrow].
	- Rotate the current piece clockwise [Z] or anticlockwise [X, UpArrow].
	- Speed up the descent of the current piece by "soft-dropping" [DownArrow].
	- Instantly drop the piece by "hard-dropping" [Space].
	- Hold the current piece [C].
- By clearing lines, the player earns "points". The amount of points gained is exponentially increased with the number of lines cleared simultaneously.

### Installation

Tetris++ requires [WSL Ubuntu](https://ubuntu.com/wsl).

Built and tested on WSL version 2004.2020.812.0

```sh
$ make all
$ ./tetrispp
```

### How is this different from traditional, old-school Tetris?

- In modern Tetris, pieces are given to the player in a random 7-bag system,
	  such that each piece has the equal chance of spawning in every 7 pieces and
	  no piece can appear more than twice in a row. This is as opposed to traditional
	  Tetris, where pieces are spawned strictly random with no sequence or bag system.
- Along with the 7-bag system, modern Tetris also features a "preview" window,
	  situated next to the playfield that shows which pieces are coming next.
- Modern Tetris uses a more advanced rotation algorithm than traditional Tetris,
	  called the "Super Rotation System (SRS)", where pieces no longer rotate solely
	  based on a set pivot mathematically in a matrix, but rather rotates in a way
	  such that when the rotation is obstructed by another tile, the piece will be
	  placed into four different positions with slightly different offsets in the grid.
	  This allows for more creative setups and nuanced spins that would have been
	  impossible in the older games.
- Another feature that modern Tetris has over traditional Tetris is the ability
	  to "hold" unwanted pieces, to be used later. This allows the player to avoid
	  situations where no piece can fit on the board, or to create skillful setups
	  that can be used to gain more points.
- Modern Tetris also rewards the player with extra points for making tricky 
	  "T-spin" setups, where the player rotates the T piece in such a manner that
	  allows normally "unreachable" lines to be cleared. A 1-line clear with a
	  T-spin is called a "T-spin single", a 2-line clear would be a "T-spin double",
	  and the most difficult maximum 3-line clear is a "T-spin triple".

### Class overview:
+ Window.cpp // Controls the terminal, game states, IO, other utils
	+ Board.cpp // Controls the current piece, and the playfield
		+ Tetromino.cpp // Controls properties of pieces and movement
