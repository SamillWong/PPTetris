#include <iostream>
#include <sys/time.h>

using namespace std;

int main(void)
{

	srand(time(NULL));

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

		cout << "ID #" << i << ": " << newID << endl;
		cout << "isValidID = " << isValidID << endl;

        // Set newID as next ID
        if (isValidID) {
            bag[i] = newID;
            i++;
        }

    }

	cout << "Bag sequence: ";
	for (int i = 0; i < 7; i++) {
		cout << bag[i];
	}
	cout << endl;

}
