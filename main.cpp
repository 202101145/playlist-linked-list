/* @Author
 * Student Name: Ugurcan Polat
 */

#include "functions.cpp"

void menu();

struct list operations;

int main(void) {
	char choice;

	// Read from database and initialize default playlists.
	operations.readFromDatabase();
	
	cout << "\nSONG BOX APPLICATION (USING DOUBLY, CIRCULAR MULTI ";
	cout << "LINKED LIST)";

	do {
		menu();
		cin >> choice;

		switch(choice) {
			case 'P': case 'p': operations.play('*', -1);  break;
			case 'L': case 'l': operations.printList();    break;
			case 'A': case 'a': operations.addSong(-1);    break;
			case 'R': case 'r': operations.removeSong(-1); break;
			case 'C': case 'c': operations.createList();   break;
			case 'D': case 'd': operations.deleteList(-1); break;
			case 'W': case 'w': operations.writeToFile();  break;
			case 'E': case 'e': operations.exit();         break;
			default : cout << "\nWrong input." << endl;    break;
		}
		
	} while (choice != 'e' && choice != 'E');
	
	return 0;
}

void menu() {
	cout << endl << "Choose an operation" << endl;
	cout << "P: Play" << endl;
	cout << "L: Show all playlists" << endl;
	cout << "A: Add songs to a playlist" << endl;
	cout << "R: Remove songs from a playlist" << endl;
	cout << "C: Create a new playlist" << endl;
	cout << "D: Delete a playlist" << endl;
	cout << "W: Write to file (SAVE)" << endl;
	cout << "E: Exit" << endl << endl;
	cout << "Enter a choice (P,L,A,R,C,D,W,E): ";
}
