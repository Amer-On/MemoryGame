#include <iostream>
#include <ctime>
#include <vector>
#include <string>

using namespace std;

/*
	by Amer On (Alexey Oleynik)
	https://github.com/Amer-On/MemoryGame
*/

// dumb method to clear the console
void ClearScreen() {
	for (int i = 0; i < 10; i++) {
		cout << "\n\n\n\n\n\n\n\n\n\n";
	}
}

// parsing int numbers from the strings to avoid some problems with inputs
int readIntFromString(string text) {
	int number = 0;
	bool hasNumber = false;

	for (int i = 0; i < text.size(); i++) {
		if (isdigit(text[i])) {
			number = 10 * number + (int) text[i] - 48;
			hasNumber = true;
		}
		else
			break;
	}
	if (not hasNumber)
		throw runtime_error("No number found");
	return number;
}

// the field class, which rules the game
class Field {
private:
	vector<vector<int> > gameField;
	vector<vector<int> > actualGameField;
	int height, width;

	vector<int> cards;
	vector<int> allCards;
	int cardsLeft;

public:
	Field(int fieldHeight, int fieldWidth) {
		height = fieldHeight;
		width = fieldWidth;

		// generate cards
		for (int i = 0; i < fieldWidth * fieldHeight / 2; i++) {
			cards.push_back(i + 1);
			cards.push_back(i + 1);
			allCards.push_back(i + 1);
		}

		cardsLeft = allCards.size();

		// generate field
		vector<int> row;
		vector<int> rowForAct;

		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				rowForAct.push_back(0);
				row.push_back(takeCard());
			}
			actualGameField.push_back(rowForAct);
			gameField.push_back(row);
			row.clear();
		}
	}

private:

	int translateX(int x) {return x + 1;}

	int translateY(int y) {return height - y;}

	int takeCard(int position) {
		int card = cards[position];
		cards.erase(cards.begin() + position);
		return card;
	}

	int takeCard() {return takeCard((int) (rand() % cards.size()));}

	void printVector(vector<int> v) {
		for (int i = 0; i < v.size(); i++)
			cout << v[i] << " ";
		cout << "\n";
	}

	// TODO: print field considering to the size of val (to make the field look symmetrical)
	void printField(vector<vector<int> > field) {
		cout << "\n";
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++)
				cout << field[i][j] << "  ";
			cout << "\n";
		}
		cout << "\n";
	}

	void printGameField() {printField(gameField);}

// some helping funcs to print data properly
public:
	void printCards() {printVector(cards);}

	void printAllCards() {printVector(allCards);}

	void printActualGameField() {printField(actualGameField);}

private:
	int turnOverCard(int x, int y) {
		if (x >= 0 and x < width and y >= 0 and y < height) {
			if (actualGameField[y][x] == 0)
				return gameField[y][x];
			else 
				throw logic_error("Card already opened");
		} else throw runtime_error("Invalid coordinates");
	}


	void readCoords(int *x, int *y) {
		try {
			string inputX, inputY;
			
			cin >> inputX >> inputY;

			*x = readIntFromString(inputX);
			*y = readIntFromString(inputY);
		} catch (const runtime_error& e) {
			cout << "Please enter numbers instead of text" << "\n";
			readCoords(&*x, &*y);
		}

		*x -= 1;
		*y = height - *y;
	}

	int pickSecondCard(int x1, int y1, int *x2, int *y2) {
		readCoords(&*x2, &*y2);

		if (*x2 == x1 && *y2 == y1) {
			cout << "The coordinates of the cards are equal, please enter different coordinates" << "\n";
			return pickSecondCard(x1, y1, &*x2, &*y2);
		}

		try {
			return turnOverCard(*x2, *y2);
		} catch (const runtime_error& e) {
			cout << "Please enter valid coordinates" << "\n";
			return pickSecondCard(x1, y1, &*x2, &*y2);
		} catch (const exception& e) {
			cout << "You already opened this card, please enter another coordinates" << "\n";
			return pickSecondCard(x1, y1, &*x2, &*y2);
		}
	}

public:
	bool pickCards() {
		cout << "Enter coordinates of the first card: ";
		
		int x1, y1;
		readCoords(&x1, &y1);
		int card1;

		try {
			card1 = turnOverCard(x1, y1);
		} catch (const runtime_error& e) {
			cout << "Please enter valid coordinates\n\n";
			return pickCards();
		} catch (const exception& e) {
			cout << "You already opened this card, please choose another one\n\n";
			return pickCards();
		}
		
		ClearScreen();

		actualGameField[y1][x1] = gameField[y1][x1];
		cout << "Coordinates of the card: " << translateX(x1) << " " << translateY(y1) << "\n";
		printActualGameField();

		cout << "Enter coordinates of the second card: ";
		int x2, y2;
		int card2 = pickSecondCard(x1, y1, &x2, &y2);
		ClearScreen();
		
		actualGameField[y2][x2] = gameField[y2][x2];
		cout << "Coordinates of the first card: " << translateX(x1) << " " << translateY(y1) << "\n";
		cout << "Coordinates of the second card: " << translateX(x2) << " " << translateY(y2) << "\n";
		printActualGameField();

		// TODO: remove card after it's found
		if (card1 == card2) {
			cout << "Congradulations! The cards are equal!" << "\n";
			cardsLeft -= 1;
			if (cardsLeft == 0)
				return true;
			return false;
		} else {
			actualGameField[y1][x1] = 0;
			actualGameField[y2][x2] = 0;
			cout << "The cards differ!" << "\n";
			return false;
		}

	}
};

bool playAgain() {
	string wantsToReplay;
	cout << "\n" << "Play again? (y/n)" << "\n";
	cin >> wantsToReplay;

	if (wantsToReplay == "n")
		return false;
	else if (wantsToReplay == "y") 
		return true;
	else return playAgain();
}

int inputLimit() {
	try {
		string input;
		cin >> input;
		cout << "\n";

		return readIntFromString(input);
	} catch (const runtime_error& e) {
		cout << "Please enter a number instead of text: ";
		return inputLimit();
	}
}

// help player with input format
void printInputFormat(int width, int height) {
	cout << "\nThe input format of coordinates\n\nFirstly you type in x coordinate, ";
	cout << "then you type in y coordinate using space as a limiter\n";
	cout << "Left bottom corner has coordinates (1, 1), ";
	cout << "top right corner has coordinates (" << width << ", " << height << ")" << "\n\n"; 
}

// run the game
bool execute(Field field, int limit) {
	for (int attemp = 0; attemp < limit; attemp++) {
		if (field.pickCards())
			return true;
		cout << limit - attemp - 1 << " attempts left\n";
	}
	cout << "\n!!!You have ran out of attempts!!!\n";
	return false;
}

int main()
{
	// random seed init
	srand(static_cast<unsigned int>(time(0)));

	// sizes of the field, they are not customizable due to conditions of the task
	int height = 4;
	int width = 6;
	cout << "The size of the field is " << height << " x " << width << "\n";

	cout << "Please enter the amount of attempts you want to have: ";

	int limit = inputLimit();

	// print the format of input
	printInputFormat(width, height);

	while (true) {
		Field field(height, width);
		bool win = execute(field, limit);
		cout << "\n\n";

		if (win)
			cout << "Congradulations! You have a great memory!\nWant to play again?\n";
		else 
			cout << "This time you got unlucky :c\nBut you still have a chance to beat this game!\nWant to play again?\n";
		
		if (!playAgain()) {
			cout << "\n" <<"Hope you have spent your time nicely, see you soon c:" << "\n";
			break;
		}
	}

	return 0;
}
