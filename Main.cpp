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


int readIntFromString(string text) {
	int number = 0;
	for (int i = 0; i < text.size(); i++) {
		if (isdigit(text[i]))
			number = 10 * number + (int) text[i] - 48;
		else
			break;
	}
	return number;
}

//unfinished, to do
vector<int> readIntFromString(string text, int numbersAmount) {
	vector<int> v;
	int number;
	int currentId = 0;
	bool numberStarted = false;

	for (int j = 0; j < numbersAmount; j++) {
		number = 0;
		for (int i = currentId; i < text.size(); i++) {
			if (isdigit((char) text[i]) and not numberStarted) {
				number = 10 * number + (int) text[i] - 48;
				cout << numberStarted << "  ";
				numberStarted = true;
			} else {
				numberStarted = false;
				currentId = i + 1;
				cout << "YEAH";
				break;
			}
		}
		cout << currentId << " ";
		cout << numberStarted << "  ";
		numberStarted = false;
		v.push_back(number);
	}
	cout << "\n\n";
	return v;
}



class Field {
private:
	vector<vector<int> > gameField;
	vector<vector<int> > actualGameField;
	int height;
	int width;

	vector<int> cards;
	vector<int> allCards;
	int pairsAmount;

	bool allCardsFound;


public:
	Field(int fieldHeight, int fieldWidth) {
		height = fieldHeight;
		width = fieldWidth;
		pairsAmount = fieldWidth * fieldHeight / 2;
		allCardsFound = false;

		// generate cards
		for (int i = 0; i < pairsAmount; i++) {
			cards.push_back(i + 1);
			cards.push_back(i + 1);
			allCards.push_back(i + 1);
		}


		// generate field
		vector<int> row;
		vector<int> rowForAct;
		int card;

		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				card = takeCard();
				rowForAct.push_back(0);
				row.push_back(card);
			}
			actualGameField.push_back(rowForAct);
			gameField.push_back(row);
			row.clear();
		}
	}

	int takeCard(int position) {
		int card = cards[position];
		cards.erase(cards.begin() + position);
		return card;
	}

	int takeCard() {
		int randomPos = rand() % cards.size();
		return takeCard(randomPos);
	}

private:
	void printVector(vector<int> v) {
		for (int i = 0; i < v.size(); i++)
			cout << v[i] << " ";
		cout << "\n";
	}

	void printField(vector<vector<int> > field) {
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

	bool getAllCardsFound() {return allCardsFound;}

	bool pickTwoCards(int x1, int y1, int x2, int y2) { 
		try {
			int card1 = pickCard(x1, y1);
			int card2 = pickCard(x2, y2); 

			if (((x1 == x2) && (y1 == y2))) {
				cout << "Positions must differ" << "\n";
				return false;
			}
			if ((actualGameField[y1][x1] != 0) || (actualGameField[y2][x2] != 0)) {
				cout << "You already guessed this card! Please pick a card you didn't guess yet" << "\n";
				return false;
			}

			actualGameField[y1][x1] = gameField[y1][x1];
			actualGameField[y2][x2] = gameField[y2][x2];

			if (card1 == card2) {
				ClearScreen();
				cout << "Great Job! The cards are equal" << "\n";
				printActualGameField();
			} else {
				ClearScreen();
				cout << "The cards are not equal" << "\n";
				printActualGameField();
				actualGameField[y1][x1] = 0;
				actualGameField[y2][x2] = 0;
			}
			return true;
		} catch (const runtime_error& e) {
			cout << "Please use valid coordinates" << "\n";
			return false;
		}
	}


	int pickCard(int x, int y) {
		if (x >= 0 and x < width and y >= 0 and y < height) {
			return gameField[y][x];
		} else throw runtime_error("Invalid coordinates (out of array length or less then zero)");
	}
};


bool execute(Field field, int limit) {
	int x1, y1, x2, y2;
	
	bool validRound;
	int attemp = 0;

	while (true) {
		cin >> x1 >> y1 >> x2 >> y2;

		validRound = field.pickTwoCards(x1, y1, x2, y2);
		
		if (validRound) {
			attemp++;
			if (attemp == limit)
				break;
		}

		if (field.getAllCardsFound())
			return true;
	}
	return false;
}


int main()
{
	// random seed init
	srand(static_cast<unsigned int>(time(0)));

	int height = 4;
	int width = 6;

	cout << "The size of the field is " << height << " x " << width << "\n";

	// limit of attempts
	string input;

	cout << "Please enter the amount of attempts you want to have: ";
	cin >> input;

	int limit = readIntFromString(input);

	if (limit == 0) {
		main();
		return 0;
	}


	cout << "\n";

	string answer;

	while (true) {
		Field field(height, width);
		bool win = execute(field, limit);

		if (win) {
			cout << "Congradulations! You have a great memory!\nWant to play again? (y/n)\n";
			cin >> answer;
		} else {
			cout << "This time you got unlucky :c\nBut you still have a chance to beat this game!\nWant to play again? (y/n)\n";
			cin >> answer;
		}
		cout << "\n";
		
		if (answer == "n") {
			cout << "Hope you have spent your time nicely, see you soon c:" << "\n";
			break;
		}
	}

	return 0;
}
