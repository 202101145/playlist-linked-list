/* @Author
 * Student Name: Ugurcan Polat
 */

#ifndef functions_h
#define functions_h

#include <iomanip>  
#include <iostream> 
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <time.h> 

#define SNAME_LENGTH  50
#define LNAME_LENGTH  50
#define STYLE_LENGTH  30
#define SINGER_LENGTH 50

struct playlist;

struct song {
	char* name   = new char[SNAME_LENGTH];
	char* singer = new char[SINGER_LENGTH];
	char* style  = new char[STYLE_LENGTH]; 
	int year;
};

struct song_node {
	song* data;
	song_node* next;
	song_node* prev;
	playlist* parent;
};

struct playlist {
	int songnumber;
	char* name = new char[LNAME_LENGTH];
	song_node* head;
	playlist* next;
	playlist* prev;
};

struct list {
	FILE *fp;
	int num_songs;
	int playlist_count;
	playlist* head;
	void openDatabase();
	void readFromDatabase();
	void songPrinter(song_node*);
	void copyString(char**, char*);
	void insertNodeWOSort(song);
	void insertNodeChrnlgcl(song);
	void insertNodeSorted(song);
	void insertNodeRandom(int);
	void createRandomList();
	void initializePlaylists();
	void createNode(song_node**, song_node**, song_node*);
	void play(char, int);
	void printList();
	void createList();
	void addSong(int);
	void removeSong(int);
	void deleteList(int);
	void writeToFile();
	void exit();
};

#endif
