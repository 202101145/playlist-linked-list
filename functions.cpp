/* @Author
 * Student Name: Ugurcan Polat
 */

#include "functions.h"

struct song_node *first;
struct song_node *firstChr;
struct song_node *firstSrt;
struct song_node *firstRan;

using namespace std;

void list::openDatabase() {
	// Opens the database file. See the writeToFile() function to
	// see why the file will be opened with "r" privilege.
	fp = fopen("songbook.txt", "r");
	
	// If it is unable to open the file give error.
	if(!fp) {
		cout << "Cannot open the database file" << endl;
		return;
	}

	// Set the position indicator to the beginning of file.
	rewind(fp);
}

void list::readFromDatabase() {	
	struct song buffer;
	char line[100];

	// Open the database file.
	openDatabase();
	
	// Calculates the number of songs in the file.
	for (num_songs = 0; !feof(fp); num_songs++) {
		// If the file has a empty line at the end of the file, break
		// the loop without counting that line.
		if(fgets(line, 255, fp) == NULL)
			break;

		// Only count default song list. Do not read the lines after
		// "*****" which indicates following list is a user added list.
		if(strcmp(line, "*****\r\n") == 0)
			break;
	}
	
	// Sets the position indicator to the beginning of the file.
	rewind(fp);

	// Initialize the 'head' nodes of the lists: Without Sort,
	// Chronological, Sorted and Random.
	first    = NULL;
	firstChr = NULL;
	firstSrt = NULL;
	firstRan = NULL;
	
	for(int count = 0; count < num_songs; count++) {	
		// Reads the line. It reads informations until it sees the 
		// tab character '\t' and stores information in corresponding
		// variables in struct buffer. 							    
		fscanf(fp, "%[^\t]\t%[^\t]\t%d\t%s", buffer.name, buffer.singer,
											 &buffer.year,buffer.style);
		
		// fscanf reads carriage return '\r' and '\n' characters with
		// the information for name. Eliminate these characters from 
		// name, because they are unnecessary. It does this operation
		// by shifting other characters to left and making null the 
		// shifted characters.					
		if (buffer.name[0] == '\r' && buffer.name[1] == '\n') {
			int check = strlen(buffer.name);
			for (int j = 0; j < (check - 1); j++) {
				buffer.name[j] = buffer.name[j+2];
				buffer.name[j+2] = '\0';
			}
		}
		
		// Insert song to the lists Without Sort, Chronological and
		// Sorted, respectively.
		insertNodeWOSort(buffer);
		insertNodeChrnlgcl(buffer);
		insertNodeSorted(buffer);
	}

	// Creates a random playlist with information of the list inserted
	// by insertNodeWOSort().
	createRandomList();

	// Initialize and create default playlists: Chronological, Sorted
	// and Random.
	initializePlaylists();

	// Close the database file.
	fclose(fp);
}

void list::songPrinter(song_node *node) {
	// Print songs in a list. 'node' should be head of the list.
	struct song_node *index = node;
	for (int count = 0; count < (node->parent)->songnumber; count++) {
		cout << setw(2) << right << count+1 << ". ";
		cout << setw(40) << left << (index->data)->name << " | ";
		cout << setw(20) << left << (index->data)->singer << " | ";
		cout << setw(4) << (index->data)->year << " | ";
		cout << setw(20) << left << (index->data)->style << endl;
		index = index->next;
	}
}

void list::copyString(char **dest, char *src) {
	// Delete the destination string.
	delete [] *dest;

	// Determine the new length, allocate the destination with the
	// new lenght and copy the information from source to it.
	int new_length = strlen(src) + 1;
	*dest = new char[new_length];
	strcpy(*dest, src);
}

void list::insertNodeWOSort(song buffer) {
	song_node *last;
	song_node *new_node = new song_node;
	song *data = new song; 

	new_node->next = NULL;
	new_node->prev = NULL;
	new_node->parent = NULL;

	copyString(&(data->name),   buffer.name);
	copyString(&(data->singer), buffer.singer);
	data->year = buffer.year;
	copyString(&(data->style),  buffer.style);

	new_node->data = data;

	// If it is the first node to be inserted, insert it to the head of
	// the list
	if (first == NULL)
		first = new_node;
	
	else {
		// If first node of the list is the only node of the list,
		// assign 'last' to 'first'. Otherwise, hence this is a circular 
		// doubly linked listed 'last' will be the node comes before
		// head of the list.
		if (first->prev != NULL)
			last = first->prev;
		else
			last = first;
			
		last->next = new_node;
		new_node->prev = last;
		last = new_node;
		first->prev = last;
		last->next = first;
	}
}

void list::insertNodeChrnlgcl(song buffer){
	song_node *lastChr;
	song_node *new_node = new song_node;
	song *data = new song; 

	new_node->next = NULL;
	new_node->prev = NULL;
	new_node->parent = NULL;
		
	copyString(&(data->name),   buffer.name);
	copyString(&(data->singer), buffer.singer);
	data->year = buffer.year;
	copyString(&(data->style),  buffer.style);

	new_node->data = data;
	
	if (firstChr == NULL) {
		firstChr = new_node;
		return;
	}

	// If first node of the list is the only node of the list,
	// assign 'last' to 'first'. Otherwise, hence this is a circular 
	// doubly linked listed 'last' will be the node comes before
	// head of the list.
	if (firstChr->prev != NULL)
		lastChr = firstChr->prev;
	else
		lastChr = firstChr;

	// Start comparing starting from the head (firstChr).
	// 'tail' will be used to store address of the current node before
	// jump to the next node.
	song_node *position = firstChr;
	song_node *tail = NULL;

	// Following loop will insert songs by chronological order.
	do {
		
		if (data->year < (position->data)->year) {

			// If the node that will be inserted comes before current
			// head of the list, insert and maket it the new head.
			if (position == firstChr) {
				new_node->next = firstChr;
				firstChr->prev = new_node;
				firstChr = new_node;
				firstChr->prev = lastChr;
				lastChr->next = firstChr;
				return;
			}

			// Insert the new node before the current node. 
			new_node->next = position;
			new_node->prev = tail;
			position->prev = new_node;
			tail->next = new_node;
			return;
		}

		// Tail will be the current position which will be used while
		// inserting the new node which will be next node of the new node.
		tail = position;
		position = position->next;
			
	} while (firstChr->next != NULL && position != firstChr);

	// If the new node comes after all the nodes in the list, insert the
	// new node to end of the list.
	lastChr->next = new_node;
	new_node->prev = lastChr;
	lastChr = new_node;
	firstChr->prev = lastChr;
	lastChr->next = firstChr;
}

void list::insertNodeSorted(song buffer) {
	song_node *lastSrt;
	song_node *new_node = new song_node;
	song *data = new song; 

	new_node->next = NULL;
	new_node->prev = NULL;
	new_node->parent = NULL;
		
	copyString(&(data->name),   buffer.name);
	copyString(&(data->singer), buffer.singer);
	data->year = buffer.year;
	copyString(&(data->style),  buffer.style);

	new_node->data = data;

	if (firstSrt == NULL) {
		firstSrt = new_node;
		return;
	}

	if (firstSrt->prev != NULL)
		lastSrt = firstSrt->prev;
	else
		lastSrt = firstSrt;
	
	song_node *position = firstSrt;
	song_node *tail = NULL;
	
	do {

		// If the singer name of the new node comes before the current
		// node, insert the new node before the current node.
		if (strcmp(data->singer, (position->data)->singer) < 0) {
			if (position == firstSrt) {
				new_node->next = firstSrt;
				firstSrt->prev = new_node;
				firstSrt = new_node;
				firstSrt->prev = lastSrt;
				lastSrt->next = firstSrt;
				return;
			}
			
			new_node->next = position;
			new_node->prev = tail;
			position->prev = new_node;
			tail->next = new_node;
			return;
		}

		// If the singer names of the new node and the current node are
		// same, check the song names.
		else if (strcmp(data->singer, (position->data)->singer) == 0) {
			if (strcmp(data->name, (position->data)->name) < 0) {
				new_node->next = position;
				new_node->prev = tail;
				position->prev = new_node;
				tail->next = new_node;				
				return;
			}
		}
		
		tail = position;
		position = position->next;
			
	} while(firstSrt->next != NULL && position != firstSrt);
	
	lastSrt->next = new_node;
	new_node->prev = lastSrt;
	lastSrt = new_node;
	firstSrt->prev = lastSrt;
	lastSrt->next = firstSrt;
}

void list::insertNodeRandom(int random_num) {
	song_node *lastRan;
	song_node *node;
	song_node *new_node = new song_node;
	song *data = new song;

	// Following loop will find the song from unsorted list which
	// has been selected randomly (random_num) in createRandomList(). 
	node = first;
	for (int count = 0; count < random_num; count++) {
		node = node->next;
	}
			
	new_node->next = NULL;
	new_node->prev = NULL;
	new_node->parent = NULL;
			
	copyString(&(data->name),   (node->data)->name);
	copyString(&(data->singer), (node->data)->singer);
	data->year = (node->data)->year;
	copyString(&(data->style),  (node->data)->style);

	new_node->data = data;

	if (firstRan == NULL) 
		firstRan = new_node;
	
	else {
		if (firstRan->prev != NULL)
			lastRan = firstRan->prev;
		else
			lastRan = firstRan;
			
		lastRan->next = new_node;
		new_node->prev = lastRan;
		lastRan = new_node;
		firstRan->prev = lastRan;
		lastRan->next = firstRan;
	}
}

void list::createRandomList() {
	// ranMemory will store inserted song positions to avoid inserting
	// same song more than once.
	int *ranMemory = new int [num_songs];
	int random_num, num_inserted;

	struct playlist *control = NULL;

	// Destroy current Random list to prepare creating new one.
	if (firstRan != NULL) {

		// Store the parent playlist.
		if (playlist_count != 0)
			control = firstRan->parent;
			
		struct song_node *current = firstRan;
		struct song_node *next = firstRan;
		struct song *data;
		
		for (int count = 0; count < num_songs; count ++) {
			current = next;
			
			if (count != num_songs - 1)
				next = current->next;
				
			data = current->data;
			
			delete [] data->name;
			delete [] data->singer;
			delete [] data->style;
			delete data;
			
			delete current;
			
		}
		firstRan = NULL;
	}

	// Feed the rand() seed and select a random number between 0 to
	// num_songs (which is number of songs in unsorted list).
	srand(time(NULL));
	random_num = rand() % num_songs;

	// This loop will randomly insert all songs of the unsorted list to 
	// random list.
	for(num_inserted = 0; num_inserted < num_songs; num_inserted++) {
		// This loop checks if the selected random number is inserted
		// before or not. 
		for (int count = 0; count < num_inserted;) {
			// If the random number is already in the inserted songs
			// list, select another random number. Then check the new
			// random number from beginning. 
			if (random_num == *(ranMemory+count)) {
				random_num = rand() % num_songs;
				count = 0;
			}
			else
				count++;
		}

		// Store the position of the song that will be inserted.
		*(ranMemory+num_inserted) = random_num;
		insertNodeRandom(random_num);
	}

	// Free the memory of ranMemory which did its job.
	delete [] ranMemory;

	// If control is not empty (so it is Random playlist), make firstRan
	// its head song node. Then, make control parent of all songs in list. 
	if (control != NULL) {
		control->head = firstRan;

		struct song_node *current = firstRan;
		for (int count = 0; count < control->songnumber; count++) {
			current->parent = control;
			current = current->next;
		} 
	}
}

void list::initializePlaylists() {
	struct song_node *check = NULL;

	// The chronological playlist.
	struct playlist *chr = new playlist;

	// Initially songnumber will be the number of songs in unsorted list.
	chr->songnumber = num_songs;
	delete [] chr->name;
	chr->name = new char [strlen("CHRONOLOGICAL")+1];
	strcpy(chr->name, "CHRONOLOGICAL");
	chr->head = firstChr;
	playlist_count = 1;

	// Assign the parents of all songs in the list to playlist.
	check = firstChr;
	for (int count = 0; count < num_songs; count++) {
		check->parent = chr;
		check = check->next;
	}
	
	struct playlist *srt = new playlist;

	srt->songnumber = num_songs;
	delete [] srt->name;
	srt->name = new char [strlen("SORTED")+1];
	strcpy(srt->name, "SORTED");
	srt->head = firstSrt;
	playlist_count++;

	check = firstSrt;
	for (int count = 0; count < num_songs; count++) {
		check->parent = srt;
		check = check->next;
	}
	
	struct playlist *ran = new playlist;
	
	ran->songnumber = num_songs;
	delete [] ran->name;
	ran->name = new char [strlen("RANDOM")+1];
	strcpy(ran->name, "RANDOM");
	ran->head = firstRan;
	playlist_count++;

	check = firstRan;
	for (int count = 0; count < num_songs; count++) {
		check->parent = ran;
		check = check->next;
	}

	// Connect playlists: Chronological list will be head, Sorted
	// and Random list will come after it. 
	head = chr;
	head->next = srt;
	srt->prev = head;
	srt->next = ran;
	ran->prev = srt;
	ran->next = head;
	head->prev = ran;
}

void list::createList() {
	struct playlist *lastList;
	struct playlist *new_list = new playlist;

	new_list->head = NULL;
	new_list->songnumber = 0;

	cout << endl << "NEW PLAYLIST >" << endl;
	cout << "1. Create with songs of a specific style" << endl;
	cout << "2. Create with songs of a specific singer" << endl;
	cout << "3. Create with a combination of existing playlists" << endl;
	cout << "4. Create with a combination of existing songs" << endl;
	cout << endl << "Enter a choice (1, 2, 3, 4): ";

	int choice;
	cin >> choice;

	if (choice == 1) {
		struct song_node *check = first;
		struct song_node *firstNew = NULL;
		struct song_node *lastNew = NULL;
		char style[STYLE_LENGTH];

		cout << "Enter the style: ";
		// Get rid of '\n' characters in the buffer.
		while (getchar() != '\n');
		// Scan until user presses enter.
		scanf("%[^\n]", style);

		// Find the selected style songs and insert to the new playlist.  
		for (int count = 0; count < num_songs; count++) {
			if (strcmp((check->data)->style, style) == 0) {
				createNode(&firstNew, &lastNew, check);
				(new_list->songnumber)++;
			}
			check = check->next;
		}

		// Give name of the style to name of the new playlist.
		copyString(&(new_list->name), style);
		new_list->head = firstNew;
	}

	else if (choice == 2) {
		struct song_node *check = first;
		struct song_node *firstNew = NULL;
		struct song_node *lastNew = NULL;
		char singer[SINGER_LENGTH];

		cout << "Enter the singer: ";
		while (getchar() != '\n');
		scanf("%[^\n]", singer);
		
		for (int count = 0; count < num_songs; count++) {
			if (strcmp((check->data)->singer, singer) == 0) {
				createNode(&firstNew, &lastNew, check);
				(new_list->songnumber)++;
			}
			check = check->next;
		}
		
		copyString(&(new_list->name), singer);
		new_list->head = firstNew;
	}

	else if (choice == 3) {
		struct playlist *check = NULL;
		struct song_node *firstNew = NULL;
		struct song_node *lastNew = NULL;
		struct song_node *insert = NULL;
		int selection;

		// This loop will concatenate selected playlists until user
		// enters '-1'.
		do {
			cout << endl;
			check = head;
			for (int count = 0; count < playlist_count; count++) {
				cout << count+1 << ". ";
				cout << check->name << endl;
				check = check->next;
			}
			
			cout << endl << "Playlist to add ('-1' to end): ";
			cin >> selection;
			
			if (selection == -1)
				break;
				
			check = head;

			// Go to selected playlist.
			for (int count = 1; count < selection; count++) {
				check = check->next;
			}

			// If user selected the Random list, then shuffle the
			// songs in the Random list every time it is called.
			// HOWEVER, hence we are not allowed to create new data,
			// we only allowed to point to data of existing songs.
			// Shuffling Random list each time will cause to inserted
			// songs to lose its data. This situation only happens if
			// user concatenetes more than one Random playlist. To avoid
			// that Random list will not be shuffled while creating new 
			// playlist by concateneting Random playlists.
			/* if (strcmp(check->name, "RANDOM") == 0)
				createRandomList(); */

			// Number of songs in new list will increase by number
			// of songs in added playlist.
			new_list->songnumber += check->songnumber;

			// Insert all songs in the selected playlist.
			insert = check->head;
			for (int count = 0; count < check->songnumber; count++) {
				createNode(&firstNew, &lastNew, insert);
				insert = insert->next;
			}

			// Assign the head of the list.
			if (new_list->head == NULL) 
				new_list->head = firstNew;

			
		} while (selection != -1);

		// Only ask for the name of the new list if user added songs.
		if (new_list->head != NULL) {
			cout << "Enter name of the playlist: ";
			while (getchar() != '\n');
			scanf("%[^\n]",new_list->name);
		}
	}
	
	else if (choice == 4) {
		struct song_node *check = NULL;
		struct song_node *firstNew = NULL;
		struct song_node *lastNew = NULL;

		int selection;

		do {
			// Print songs in sorted list, then ask for a song to add
			// to the new list.
			cout << endl;
			songPrinter(firstSrt);
			cout << endl << "Song to add ('-1' to end): ";
			cin >> selection;
			
			if (selection == -1)
				break;

			// Go to the selected song in sorted list.
			check = firstSrt;
			for (int count = 1; count < selection; count++) {
				check = check->next;
			}

			(new_list->songnumber)++;

			// Insert the selected song to the new list.
			createNode(&firstNew, &lastNew, check);

			if (new_list->head == NULL) 
				new_list->head = firstNew;
				
		} while (selection != -1);

		if (new_list->head != NULL) {
			cout << "Enter name of the playlist: ";
			while (getchar() != '\n');
			scanf("%[^\n]",new_list->name);
		}
	}

	else {
		cout << endl << "Wrong input... " << endl;
		delete new_list;
		return;
	}

	// If user hasn't added any song to new list, there is no need to
	// take action.
	if (new_list->songnumber == 0) {
		delete new_list;
		return;
	}

	// New playlist will be inserted at the end of the list of playlists.
	lastList = head->prev;
	new_list->prev = lastList;
	lastList->next = new_list;
	lastList = new_list;
	head->prev = lastList;
	lastList->next = head;
	playlist_count++;

	struct song_node *index = new_list->head;
	for (int count = 0; count < new_list->songnumber; count++) {
		index->parent = new_list;
		index = index->next;
	}
	// Print the new list.
	cout << endl;
	songPrinter(new_list->head);
}

void list::createNode(song_node **start, song_node **end, song_node *insert) {
	// This function for creating a new playlist.
	
	song_node *new_node = new song_node;

	new_node->next = NULL;
	new_node->prev = NULL;
	new_node->parent = NULL;

	// Data of new song node points to data of existing song. 
	new_node->data = insert->data;
	
	if (*start == NULL)
		*start = *end = new_node;

	else {
		(*end)->next = new_node;
		new_node->prev = *end;
		*end = new_node;
		(*start)->prev = *end;
		(*end)->next = *start;
	}
}

void list::addSong(int position) {
	int song_selection, playlist_selection, diff, order = 1;
	bool is_user_added_playlist = false;
	struct playlist *check = head;

	// If playlist hasn't selected before calling the function, ask
	// user for playlist.
	if (position == -1) {
		cout << endl;
		for (int count = 0; count < playlist_count; count++) {
			if (strcmp(check->name, "CHRONOLOGICAL") != 0 &&
				strcmp(check->name, "SORTED") != 0  &&
				strcmp(check->name, "RANDOM") != 0) {
					
				cout << order << ". " << check->name << endl;
				order++;
				is_user_added_playlist = true;
			}
			
			check = check->next;
		}

		if (!is_user_added_playlist) {
			cout << endl << "There is no user added list." << endl;
			return;
		}
		
		cout << "Select a playlist: ";
		cin >> playlist_selection;
		// User only add songs to playlist created by user. Hence,
		// it must skip first default playlists.
		diff = 2;
	}

	// If playlist selected already, determine its position.
	else {
		playlist_selection = position - 1;
		diff = 0;
	}

	cout << endl;
	songPrinter(firstSrt);

	cout << "Song to add: ";
	cin >> song_selection;

	// Go to selected song.
	struct song_node *index = firstSrt;
	for (int count = 1; count < song_selection; count++) {
		index = index->next;
	}

	// Go to selected playlist.
	for (int count = 0; count < playlist_selection + diff; count++) {
		check = check->next;
	}

	// Get data from selected song to buffer.
	song buffer;
	copyString(&(buffer.name), (index->data)->name);
	copyString(&(buffer.singer), (index->data)->singer);
	buffer.year = (index->data)->year;
	copyString(&(buffer.style), (index->data)->style);

	// Check if new song is going to be inserted to default playlis.
	// If so, insert by order.
	if (check->head == firstChr)
		insertNodeChrnlgcl(buffer);

	else if (check->head == firstSrt)
		insertNodeSorted(buffer);

	else if (strcmp(check->name, "RANDOM") == 0) {
		insertNodeWOSort(buffer);
		num_songs++;
		// Shuffle the Random list.
		createRandomList();
	}

	else {
		createNode(&(check->head), &((check->head)->prev), index);
		((check->head)->prev)->parent = check;
	}

	(check->songnumber)++;
	
	cout << endl;
	songPrinter(check->head);

	cout << endl << "Song added." << endl;
}

void list::play(char opt, int sel) {
	char choice;
	struct song_node *play;

	// Shuffle the Random list.
	createRandomList();
	
	// If option hasn't been selected before calling the function, ask
	// user.
	if (opt == '*') {
		cout << endl << "PLAY >" << endl;
		cout << "Choose an option" << endl;
		cout << "L: Play a playlist" << endl;
		cout << "S: Play a playlist starting from a specific song" << endl;
		cout << "P: Play a single song" << endl;
		cout << "E: Exit" << endl;

		cout << endl << "Enter a choice: ";
		cin >> choice;
	}

	else
		choice = opt;

	if (choice == 'L') {
		int sel_list;
		
		struct playlist *check = head;
		// If playlist hasn't been selected before calling the function,
		// ask user.
		if (sel == -1) {
			for (int count = 0; count < playlist_count; count++) {
				cout << count+1 << ". " << check->name << endl;
				check = check->next;
			}

			cout << endl << "Select a playlist: ";
			cin >> sel_list;
		}

		else
			sel_list = sel;

		for (int count = 0; count < sel_list - 1; count++) {
			check = check->next;
		}

		play = check->head;
	}

	else if (choice == 'S') {
		int sel_list, sel_song;
		
		struct playlist *check = head;
		for (int count = 0; count < playlist_count; count++) {
			cout << count+1 << ". " << check->name << endl;
			check = check->next;
		}
		
		cout << endl << "Select a playlist: ";
		cin >> sel_list;

		for (int count = 0; count < sel_list-1; count++) {
			check = check->next;
		}

		cout << endl;
		songPrinter(check->head);

		cout << endl << "Select a song: ";
		cin >> sel_song;

		struct song_node *index = check->head;
		for (int count = 1; count < sel_song; count++) {
			index = index->next;
		}
		
		play = index;
	}

	else if (choice == 'P') {
		int sel_song;
	
		cout << endl;
		songPrinter(firstSrt);

		cout << endl << "Select a song: ";
		cin >> sel_song;

		struct song_node *index = firstSrt;	
		for (int count = 1; count < sel_song; count++) {
			index = index->next;
		}

		cout << "playing..." << endl;
		cout << (index->data)->name << " - ";
		cout << (index->data)->singer << " - ";
		cout << (index->data)->style << endl;

		return;
	}

	else if (choice == 'E') 
		return;

	else {
		cout << endl << "Wrong input." << endl << endl;
		return;
	}

	while (choice != 'E') {
		cout << "playing..." << endl;
		cout << (play->data)->name << " - ";
		cout << (play->data)->singer << " - ";
		cout << (play->data)->style << endl;
		cout << "NEXT(N) - PREVIOUS(P) - EXIT(E)" << endl;
		while (getchar() != '\n');
		choice = getchar();

		if (choice == 'N')
			play = play->next;

		else if (choice == 'P')
			play = play->prev;

		else if (choice == 'E')
			return;

		else
			cout << "Wrong input." << endl;
	} 
}

void list::removeSong(int sel) {
	int sel_list, sel_song;
	struct playlist *check = head;
	
	// If song hasn't been selected before calling the function,
	// ask user.
	if (sel == -1) {
		cout << endl << "REMOVE SONG >" << endl;	
		for (int count = 0; count < playlist_count; count++) {
			cout << count+1 << ". " << check->name << endl;
			check = check->next;
		}

		cout << endl << "Select a playlist: ";
		cin >> sel_list;
	}

	else
		sel_list = sel;

	for (int count = 0; count < sel_list-1; count++) {
		check = check->next;
	}

	cout << endl;
	
	songPrinter(check->head);

	cout << endl << "Select a song: ";
	cin >> sel_song;

	struct song_node *index = check->head;	
	for (int count = 1; count < sel_song; count++) {
		index = index->next;
	}

	// Connect the next and previous songs of the removed song.
	(index->prev)->next = index->next;
	(index->next)->prev = index->prev;

	// If removed song is head of its parent, assign the new head.
	if ((index->parent)->head == index)
		(index->parent)->head = index->next;
	
	((index->parent)->songnumber)--;

	// All songs in a playlist created by user are using the 'data' of   
	// an existing song by pointing to 'data' of the song it points to.  
	// Deleting 'data' would cause songs of another playlist to lose its  
	// data. Hence it is disabled.
	/*
	* delete [] (index->data)->name;
	* delete [] (index->data)->singer;
	* delete [] (index->data)->style;
	* delete index->data; */
	
	delete index;

	cout << endl << "Song has been removed." << endl;
}

void list::deleteList(int sel) {
	int sel_list;
	struct playlist *index = head;

	// If playlist hasn't been selected before calling the function,
	// ask user.	
	if (sel == -1) {
		cout << endl << "DELETE PLAYLIST >" << endl;
		for (int count = 0; count < playlist_count; count++) {
			cout << count+1 << ". " << index->name << endl;
			index = index->next;
		}
		
		cout << "Select a playlist: ";
		cin >> sel_list;
	}

	else
		sel_list = sel;
	
	for (int count = 0; count < sel_list-1; count++) {
		index = index->next;
	}

	(index->prev)->next = index->next;
	(index->next)->prev = index->prev;

	if (index == head)
		head = index->next;

	struct song_node *next_position = index->head;
	struct song_node *position;
	for (int count = 0; count < index->songnumber; count++) {
		position = next_position;

		next_position = position->next;

		// All songs in a playlist created by user are using the 'data'    
		// of an existing song by pointing to 'data' of the song it   
		// points to. Deleting 'data' would cause songs of another   
		// playlist to lose their data. Hence it is disabled.
		/*
		delete [] (position->data)->name;
		delete [] (position->data)->singer;
		delete [] (position->data)->style;
		delete position->data; */
		
		delete position; 
	}

	delete [] index->name;
	delete index;

	playlist_count--;
	
	cout << endl << "Playlist has been deleted." << endl;
}

void list::printList() {
	int sel_list;
	struct playlist *check;
	char choice;

	// Print playlists.
	cout << endl << "PRINT PLAYLIST >" << endl;	
	check = head;
	for (int count = 0; count < playlist_count; count++) {
		cout << count+1 << ". " << check->name << endl;
		check = check->next;
	}

	cout << endl << "Select a playlist: ";
	cin >> sel_list;

	// Go to selected playlist.
	for (int count = 0; count < sel_list-1; count++) {
		check = check->next;
	}

	do {
		cout << endl;
		cout << sel_list << ". " << check->name << endl;

		cout << "NEXT(N) - PREVIOUS(P) - SELECT(S) - EXIT(E)" << endl;
		while (getchar() != '\n');
		choice = getchar();

		if (choice == 'N') {
			check = check->next;
			if (sel_list == playlist_count)
				sel_list = 1;
			else
				sel_list++;
		}

		else if (choice == 'P') {
			check = check->prev;
			if (sel_list == 1)
				sel_list = playlist_count;
			else
				sel_list--;
		}

		else if (choice == 'S') {
			char option;
			cout << "DELETE THE PLAYLIST(D) - ADD SONG(A) - ";
			cout << "REMOVE SONG(R) - PLAY(P)" << endl;
			while (getchar() != '\n');
			option = getchar();

			// Call functions with selected playlist.
				
			if (option == 'D')
				deleteList(sel_list);

			else if (option == 'A')
				addSong(sel_list);

			else if (option == 'R')
				removeSong(sel_list);

			// Call play function with 'L' which says play playlist.
			else if (option == 'P')
				play('L', sel_list);

			else 
				cout << endl << "Wrong input." << endl << endl;

			return;
		}

		else if (choice == 'E')
			return;

		else {
			cout << endl << "Wrong input." << endl << endl;
			return;
		}

		// Shuffle the Random playlist.
		createRandomList();
	} while (choice != 'E');
}

void list::writeToFile() {
	struct playlist  *current_list;
	struct song_node *node;
	
	// To overwrite the file open it again with "w" privilege.
	// This will allow program to overwrite existing informations.
	// Opening the file with "r+" privilege causes some bugs such as:
	// if user removes a song, the number of songs will decrease
	// by 1. The original file has "number of songs" records, and  
	// the latest records has "number of songs-1" records. This will 
	// cause the last line of the original file to remain.							
	fp = fopen("songbook.txt", "w");

	current_list = head;

	for (int count = 0; count < playlist_count; count++) {
		// Print asteriks and name of the playlist if the list is added
		// by user.
		if (!(strcmp(current_list->name, "CHRONOLOGICAL") == 0 ||
			  strcmp(current_list->name, "SORTED") == 0 ||
			  strcmp(current_list->name, "RANDOM") == 0)) {
				fprintf(fp, "*****\r\n%s\r\n", current_list->name);		
		}

		node = current_list->head;

		// Print only the Chronological and user added playlists.
		if (!(strcmp(current_list->name, "SORTED") == 0 ||
			  strcmp(current_list->name, "RANDOM") == 0)) {
				  
			for (int count2 = 0; count2 < current_list->songnumber; count2++) {
				fprintf(fp, "%s\t%s\t%d\t%s\r\n",(node->data)->name,
												 (node->data)->singer,
												 (node->data)->year,
												 (node->data)->style);
				node = node->next;		
			}
		}
		
		current_list = current_list->next;
	}

	cout << endl << "Playlists has been written to file." << endl;

	// Close the file.
	fclose(fp);
}

void list::exit() {
	// Using 'current' and 'next' terms because destroying the current
	// variable means information of current->next will be lost as well.
	// To workaround the problem, current->next will be stored to 'next'. 
	struct playlist  *current_list;
	struct playlist  *next_list;
	struct song_node *node;
	struct song_node *next_node;
	struct song      *data;
	bool is_user_added_playlist;

	// Write the current information to file.
	writeToFile();

	// Initially current and next is head.
	current_list = next_list = head;

	// This will loop will free all playlists.
	for (int count = 0; count < playlist_count; count++) {
		// Go to the next list of the previously deleted list.
		current_list = next_list;

		// Only store the 'next' information if the current list is not
		// the last list.
		if (count != playlist_count - 1)
			next_list = current_list->next;

		// Check if the current playlist is default list or user added list.
		if (strcmp(current_list->name, "CHRONOLOGICAL") == 0 ||
			strcmp(current_list->name, "SORTED") == 0 ||
			strcmp(current_list->name, "RANDOM") == 0) {
				
			is_user_added_playlist = false;
		}

		else
			is_user_added_playlist = true;

		// Initially current and next song_node is head.
		node = next_node = current_list->head;

		// This loop will free all songs in the current playlist.  	
		for (int count2 = 0; count2 < current_list->songnumber; count2++) {
			node = next_node;
			
			if (count2 != current_list->songnumber - 1)
				next_node = node->next;
				
			data = node->data;

			// User added lists uses the data of Sorted list by
			// pointing to data of a song in the Sorted list. Hence,
			// we can only free the memory for song 'data' if the 
			// current list is not a list added by user. This data
			// has already been freed while freeing the sorted list. 
			if (!is_user_added_playlist) {
				delete [] data->name;
				delete [] data->singer;
				delete [] data->style;
				delete data;
			}

			// Free the memory for song_node.
			delete node;
		}

	// Free the name pointer and the current playlist. 
	delete [] current_list->name;
	delete current_list;
	}

	// This loop will delete and free the memory allocated for the
	// Without Sort list because this list is not belong to any playlist.
	node = next_node = first;
	for (int count = 0; count < current_list->songnumber; count++) {
		node = next_node;
			
		if (count != current_list->songnumber - 1)
			next_node = node->next;
				
		data = node->data;

		delete [] data->name;
		delete [] data->singer;
		delete [] data->style;
		delete data;
			
		delete node;
	}

	cout << endl << "Goodbye!";
}
