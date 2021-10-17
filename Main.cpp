#include <iostream>
#include <ctime>
#include <vector>
#include <string>

using namespace std;


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
	
	int x1, y1, x2, y2;

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

	int takeCard(int position) {
		cards.erase(cards.begin() + position);
		return cards[position];
	}

	int takeCard() {return takeCard((int) (rand() % cards.size()));}

private:
	void printVector(vector<int> v) {
		for (int i = 0; i < v.size(); i++)
			cout << v[i] << " ";
		cout << "\n";
	}

	void printField(vector<vector<int> > field) {
		cout << "\n";
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++)
				cout << field[i][j] << "  ";
			cout << "\n";
		}
		cout << "\n";
	}

public:
	void printCards() {printVector(cards);}

	void printAllCards() {printVector(allCards);}

	void printGameField() {printField(gameField);}

	void printActualGameField() {printField(actualGameField);}

	void readFirstCoords() {
		try {
			string x, y;
			
			cin >> x >> y;

			x1 = readIntFromString(x);
			y1 = readIntFromString(y);
		} catch (const runtime_error& e) {
			cout << "Please enter numbers instead of text";
			readFirstCoords();
		}
	}

	void readSecondCoords() {
		try {
			string x, y;
			cin >> x >> y;

			x2 = readIntFromString(x);
			y2 = readIntFromString(y);
		} catch (const runtime_error& e) {
			cout << "Please enter numbers instead of text";
			readSecondCoords();
		}
	}

	int pickSecondCard() {
		readSecondCoords();

		if (x2 == x1 && y1 == y2) {
			cout << "The coordinates of the cards are equal, please enter different coordinates" << "\n";
			return pickSecondCard();
		}

		try {
			return turnOverCard(x2, y2);
		} catch (const runtime_error& e) {
			cout << "Please enter valid coordinates" << "\n";
			return pickSecondCard();
		}
	}

	bool pickCards() {
		cout << "Enter coordinates of the first card: ";
		readFirstCoords();
		int card1;

		try {
			card1 = turnOverCard(x1, y1);
		} catch (const runtime_error& e) {
			cout << "Please enter valid coordinates" << "\n";
			return pickCards();
		}
		
		ClearScreen();

		actualGameField[y1][x1] = gameField[y1][x1];
		cout << "Coordinates of the card: " << x1 << " " << y1 << "\n";
		printActualGameField();

		cout << "Enter coordinates of the second card: ";
		int card2 = pickSecondCard();
		ClearScreen();
		
		actualGameField[y2][x2] = gameField[y2][x2];
		cout << "Coordinates of the first card: " << x1 << " " << y1 << "\n";
		cout << "Coordinates of the second card: " << x2 << " " << y2 << "\n";
		printActualGameField();

		if (card1 == card2) {
			cout << "Congradulations! The cards are equal!" << "\n";
			allCards.erase(allCards.begin() + card1 - 1);
			if (allCards.size() == 0) {
				return true;
			}
			return false;
		} else {
			actualGameField[y1][x1] = 0;
			actualGameField[y2][x2] = 0;
			cout << "The cards differ!" << "\n";
			return false;
		}

	}


	int turnOverCard(int x, int y) {
		if (x >= 0 and x < width and y >= 0 and y < height) {
			if (actualGameField[y][x] == 0)
				return gameField[y][x];
		}
		throw runtime_error("Invalid coordinates");
	}
};

// run the game
bool execute(Field field, int limit) {
	for (int attemp = 0; attemp < limit; attemp++) {
		if (field.pickCards())
			return true;
	}
	cout << "\n" <<"!!!You have ran out of attempts!!!" << "\n";
	return false;
}

// play the game once more/no
bool playAgain() {
	string answer;
	cout << "\n" << "Play again? (y/n)" << "\n";
	cin >> answer;

	if (answer == "n")
		return false;
	else if (answer == "y") 
		return true;
	else return playAgain();
}


int main()
{
	// random seed init
	srand(static_cast<unsigned int>(time(0)));

	// sizes of the field, they are not customizable due to conditions of the task
	int height = 4;
	int width = 6;
	cout << "The size of the field is " << height << " x " << width << "\n";

	string input;
	cout << "Please enter the amount of attempts you want to have: ";
	cin >> input;
	cout << "\n";

	int limit = readIntFromString(input);
	if (limit < 1) {
		main(); 
		return 0;
	}

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
