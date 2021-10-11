#include <iostream>
#include <ctime>
#include <vector>

using namespace std;


// dumb method to clear the console
void ClearScreen() {
	for (int i = 0; i < 10; i++) {
		cout << "\n\n\n\n\n\n\n\n\n\n";
	}
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

	void pickTwoCards(int x1, int y1, int x2, int y2) { 
		if ((x1 == x2) && (y1 == y2)) {
			cout << "Positions must differ";
			return;
		}

		try {
			int card1 = pickCard(x1, y1);
			int card2 = pickCard(x2, y2); 

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
		} catch (const runtime_error& e) {
			cout << "Please use valid coordinates" << "\n";
			return;
		}
	}

	int pickCard(int x, int y) {
		if (((x >= 0) && (x < width)) && ((y >= 0) && (y < height)))
			return gameField[y][x];
		else throw runtime_error("Invalid coordinates (out of array length or less then zero)");
	}

};


bool execute(Field field) {
	int x1, y1, x2, y2;
	int cnt = 0;
	while (!field.getAllCardsFound()) {
		cin >> x1 >> y1 >> x2 >> y2;
		field.pickTwoCards(x1, y1, x2, y2);
		cnt++;
		if (cnt == 3)
			break;
	}
	return true;
}


int main()
{
	// random seed init
	srand(static_cast<unsigned int>(time(0)));

	int height = 4;
	int width = 6;

	Field field(height, width);
	bool win = execute(field);


	return 0;
}
