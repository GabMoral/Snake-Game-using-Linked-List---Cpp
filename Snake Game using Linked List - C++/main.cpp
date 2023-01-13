#include <iostream>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <Windows.h>
#include <conio.h>
#include <time.h>
#undef max
using namespace std;

const char HORZ_WALL = 205;
const char VERT_WALL = 186;
const char TOPL_CORN = 201;
const char TOPR_CORN = 187;
const char BTTML_CORN = 200;
const char BTTMR_CORN = 188;
const char SNAKE_HEAD = 178;
const char SNAKE_TAIL = 176;
const char FRUIT = 15;

#define COLS 40
#define ROWS 25

class Node {
public:
	int x, y;
	int isDead;
	int head;
	Node* Next;
};
//Initialize game
void init(Node* n, int* ptrX, int* ptrY, int* ptrFruitX, int* ptrFruitY, int* ptrTailLen, int* ptrScore, char* ptrDir) {
	n->x = COLS / 2;
	n->y = ROWS / 2;
	n->isDead = 0;
	n->head = 1;
	n->Next = NULL;
	*ptrX = COLS / 2;
	*ptrY = ROWS / 2;
	*ptrFruitX = rand() % COLS;
	*ptrFruitY = rand() % ROWS;
	*ptrTailLen = 0;
	*ptrScore = 0;
	*ptrDir = ' ';
}
//Resets screen
void ResetScreenPosition() {
	HANDLE hOut;
	COORD Position;
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	Position.X = 0;
	Position.Y = 0;
	SetConsoleCursorPosition(hOut, Position);
}
//Draws screen
void drawScreen(Node* n, int fruitX, int fruitY, int tailLength, int score, int highscore) {
	Node* temp = n;
	for (int i = 0; i <= COLS + 1; i++) {
		if (i == 0)
			cout << TOPL_CORN;
		else if (i == COLS + 1)
			cout << TOPR_CORN;
		else
			cout << HORZ_WALL;
	}
	printf("\n");
	for (int i = 0; i < ROWS; i++) {
		cout << VERT_WALL;
		for (int j = 0; j < COLS; j++) {
			n = temp;
			int printing = 0;
			if (n != NULL) {
				if (i == n->y && j == n->x && n->head == 1) {
					cout << SNAKE_HEAD;
					n = n->Next;
					printing = 1;
				}
				if (tailLength > 0) {
					while (n != NULL) {
					if (j == n->x && i == n->y && n->head == 0 && !printing) {
						
						cout << SNAKE_TAIL;
						
						printing = 1;
						}
					n = n->Next;
					}
				}
			}
			if (i == fruitY && j == fruitX && !printing) {
				cout << FRUIT;
			}
			else {
				if (j % 2 == 0 && !printing)
					cout << "-";
				else if (j % 2 == 1 && !printing)
					cout << " ";
			}
		}
		cout << VERT_WALL;
		printf("\n");
	}
	for (int i = 0; i <= COLS + 1; i++) {
		if (i == 0)
			cout << BTTML_CORN;
		else if (i == COLS + 1)
			cout << BTTMR_CORN;
		else
			cout << HORZ_WALL;
	}
	cout << "\n";
	cout << "Score: " << score << endl;
	cout << "Highscore: " << highscore << endl;
}
//Gets player input
void getInput(char* dir, int tailLength, int* gameOver) {
	if (_kbhit()) {
		switch (_getch()) {
		case 'w':
			if (*dir == 's' && tailLength > 0)
				break;
			*dir = 'w';
			break;
		case 'a':
			if (*dir == 'd' && tailLength > 0)
				break;
			*dir = 'a';
			break;
		case 's':
			if (*dir == 'w' && tailLength > 0)
				break;
			*dir = 's';
			break;
		case 'd':
			if (*dir == 'a' && tailLength > 0)
				break;
			*dir = 'd';
			break;
		case 'q':
			*gameOver = 1;
			break;
		}
	}
}
//Calculates snake movement
void movementLogic(Node* n, int* tailLength, int* x, int* y, char dir, int* gameOver, int* fruitX, int* fruitY, int* score) {
	*y = n->y;
	*x = n->x;
		switch (dir) {
		case 'w':
			n->y -= 1;
			break;
		case 'a':
			n->x -= 1;
			break;
		case 's':
			n->y = (n->y + 1) % ROWS;
			break;
		case 'd':
			n->x = (n->x + 1) % COLS;
			break;
		}
		Node* temp = n;
		while (temp->Next != NULL) {
			temp = temp->Next;
			if (n->x == temp->x && n->y == temp->y)
				*gameOver = 1;
		}
		if (n->x < 0) {
			n->x = COLS - 1;
		}
		if (n->y < 0) {
			n->y = ROWS - 1;
		}
		if (n->x == *fruitX && n->y == *fruitY) {
			*score += 10;
			*fruitX = rand() % COLS;
			*fruitY = rand() % ROWS;
			*tailLength += 1;
			Node* newNode = new Node();
			newNode->head = 0;
			newNode->isDead = 0;
			newNode->x = *x;
			newNode->y = *y;
			newNode->Next = n->Next;
			n->Next = newNode;
		}
		int nextX = 0, nextY = 0;
		while (n->Next != NULL) {
			if (n->head == 1) {
				nextX = n->Next->x;
				nextY = n->Next->y;
				n->Next->x = *x;
				n->Next->y = *y;
				n = n->Next;
			}
			else {
				*x = nextX;
				*y = nextY;
				nextX = n->Next->x;
				nextY = n->Next->y;
				n->Next->x = *x;
				n->Next->y = *y;
				n = n->Next;
			}
		}
}
//Checks for a new highscore
void newBest(int currentScore, int* highscore, ofstream* fscore) {
	if (*highscore < currentScore) {
		*highscore = currentScore;
		fscore->open("highscore.txt");
		if (!fscore->is_open()) {
			cout << "Error\n";
		}
		else {
			cout << "New best!!!\n";
			*fscore << *highscore;
		}
		fscore->close();
	}
}
//Quit game menu
void quit(int* quitGame, int* gameOver) {
	int invalidInput;
	char playAgain;
	do {
		cout << "Would you like to play again? Yes or No (Y or N). ";
		cin >> playAgain;
		//read past invalid input
		//scanf("%*[^\n]");
		invalidInput = cin.fail();
		if (playAgain != 'Y' && playAgain != 'N') {
			cout << "Type Y or N.\n";
			//validInput = 0;
		}
		else if (playAgain == 'N') {
			cout << "Thanks for playing.\n";
			//validInput = 1;
			*quitGame = 1;
		}
		else if (playAgain == 'Y') {
			cout << "New game starting.\n";
			//validInput = 1;
			*quitGame = 0;
			*gameOver = 0;
			system("pause");
			system("cls");
		}
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');	//clear buffer
	} while (invalidInput == 1);
}
int main() {
	srand(time(NULL));
	int x, y, fruitX, fruitY, tailLength, score, highscore = 0, gameOver = 0, quitGame = 0;
	char dir;
	Node* head = new Node();
	//Read highscore from text file
	ifstream inFile;
	ofstream outFile;
	inFile.open("highscore.txt");
	if (inFile.is_open()) {
		inFile >> highscore;
		inFile.close();
	}
	//FILE* fscore = fopen("highscore.txt", "r");
	//fscanf(fscore, "%d", &highscore);
	//fclose(fscore);
	while (quitGame == 0) {
		//Initialize game
		init(head, &x, &y, &fruitX, &fruitY, &tailLength, &score, &dir);
		while (gameOver == 0) {
			//Draw game screen
			drawScreen(head, fruitX, fruitY, tailLength, score, highscore);
			//Input
			getInput(&dir, tailLength, &gameOver);
			//Logic
			movementLogic(head, &tailLength, &x, &y, dir, &gameOver, &fruitX, &fruitY, &score);
			//New best highscore
			newBest(score, &highscore, &outFile);
			//Resets screen
			ResetScreenPosition();
			Sleep(99);
		}
		//Play again?
		quit(&quitGame, &gameOver);
	}
	system("pause");
	return 0;
}