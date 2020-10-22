#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include <iostream>

using namespace std;

int getkey() {
    int character;
    struct termios orig_term_attr;
    struct termios new_term_attr;

    // Set the terminal to raw mode
    tcgetattr(fileno(stdin), &orig_term_attr);
    memcpy(&new_term_attr, &orig_term_attr, sizeof(struct termios));
    new_term_attr.c_lflag &= ~(ECHO|ICANON);
    new_term_attr.c_cc[VTIME] = 0;
    new_term_attr.c_cc[VMIN] = 0;
    tcsetattr(fileno(stdin), TCSANOW, &new_term_attr);

    // Read a character from the stdin stream without blocking
    // Returns EOF (-1) if no character is available
    character = fgetc(stdin);

    // Restore the original terminal attributes
    tcsetattr(fileno(stdin), TCSANOW, &orig_term_attr);

    return character;
}

int main()
{
    int key;

	// Seed random
    srand(time(NULL));

    for (;;) {
        key = getkey();
        // Terminate loop on ESC (0x1B) or Ctrl-D (0x04) on STDIN
        if (key == 0x1B || key == 0x04) {
            break;
        }
        else {

			cout << "Keypress detected: " << key << endl;
        }
    }

    return 0;
}
