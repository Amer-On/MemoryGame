#include <iostream>
#include <ctime>
#include <string>

/*
	by Amer On (Alexey Oleynik)
	https://github.com/Amer-On/MemoryGame
*/

// dumb method to clear the console
void clearScreen() {
	for (int i = 0; i < 10; i++) {
		std::cout << "\n\n\n\n\n\n\n\n\n\n";
	}
}


class List {
private:
	int *list;
	int size;

public:
	List() {
		list = new int[0];
		size = 0;
	}

	void add(int newEl) {
		int *newList = new int[size + 1];
		for (int i = 0; i < size; i++) {
			newList[i] = list[i];
		}

		newList[size] = newEl;
		list = newList;
		delete [] newList;
		size++;
	}

	void remove(int elToRem) {
		int posToRem;
		bool hasElToRem = false;

		for (int i = 0; i < size; i++) {
			if (list[i] == elToRem) {
				hasElToRem = true;
				posToRem = i;
				break;
			}
		}

		if (!hasElToRem)
			return;
		else extract(posToRem);
	}

	void extract(int position) {
		int *newList = new int[size - 1];
		for (int i = 0; i < position; i++)
			newList[i] = list[i];
		for (int i = position; i < size - 1; i++)
			newList[i] = list[i + 1];

		size -= 1;
		list = newList;
		delete [] newList;
	}

	int getSize() {
		return size;
	}

	int getEl(int position) {
		return list[position];
	}
};




// the field class, which rules the game
class Field {
private:
	int height, width;
	// observableField is the field the player is playing on. Only guessed cards are showed
	int **observableField;
	// gameField contains all the right numbers, all the card values are visible
	int **gameField;

	int cardsLeft;
	List cardsList;

public:
	Field(int fieldHeight, int fieldWidth) {
		height = fieldHeight;
		width = fieldWidth;

		cardsLeft = fieldWidth * fieldHeight / 2;

		cardsList = List();
		// generate cards
		for (int i = 0; i < cardsLeft; i++) {
			cardsList.add(i + 1);
			cardsList.add(i + 1);
		}

		generateObservableGameField();

		generateGameField();
	}

private:
	void generateGameField() {
		gameField = new int*[height];

		for (int i = 0; i < height; i++) {
			gameField[i] = new int[width];
			for (int j = 0; j < width; j++)
				gameField[i][j] = takeCard();
		}
	}

	void generateObservableGameField() {
		observableField = new int*[height];

		for (int i = 0; i < height; i++) {
			observableField[i] = new int[width];
			for (int j = 0; j < width; j++) 
				observableField[i][j] = 0;
			std::cout << "\n";
		}
	}

  // translate funcs to print out the coords in the way they are inputted
	int translateX(int x) {return x + 1;}

	int translateY(int y) {return height - y;}

	int takeCard(int position) {
		int card = cardsList.getEl(position);
		cardsList.extract(position);
		return card;
	}

	int takeCard() {return takeCard((int) (rand() % cardsList.getSize()));}


	void printField(int **field) {
		std::cout << "\n";
		std::string output = "";
		int spacesArray[height][width];
		int number;

		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				number = field[i][j];
				if (number / 10 > 0)
					output += std::to_string(number) + " ";
				else
					output += std::to_string(number) + "  ";
			}
			output += "\n";
		}

		std::cout << output << "\n";
	}

	void printObservableField() {printField(observableField);}

	void printGameField() {printField(gameField);}

	int turnOverCard(int x, int y) {
		if (x >= 0 and x < width and y >= 0 and y < height) {
			if (observableField[y][x] == 0)
				return gameField[y][x];
		} 
		return 0;
	}

	void readCoords(int *x, int *y) {
		while (true) {
			int inputX, inputY;
				
			if (!(std::cin >> inputX >> inputY)) {
				std::cin.clear();
	      std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
				std::cout << "Nan found. Please use numbers instead of text: ";
				continue;
			}
				
			*x = inputX - 1;
			*y = height - inputY;
			break;
		}
	}

	int pickSecondCard(int x1, int y1, int *x2, int *y2) {
		while (true) {
			readCoords(x2, y2);

			if (*x2 == x1 && *y2 == y1) {
				std::cout << "The coordinates of the cards are equal, please enter different coordinates: ";
				continue;
			}

			int cardVal = turnOverCard(*x2, *y2);
			if (cardVal == 0) {
				std::cout << "Card already opened or coordinates are invalid. Please enter another coordinates: " << "\n";
				continue;
			}
			return cardVal;
		}
	}

public:
  // the function to run the round
	bool pickCards() {
		while (true) {
			std::cout << "Enter coordinates of the first card: ";
			
			int x1, y1;
			readCoords(&x1, &y1);
			int card1;


			card1 = turnOverCard(x1, y1);
			if (card1 == 0) {
				std::cout << "Card already opened or coordinates are invalid. Please enter another coordinates" << "\n";
				continue;
			}
			
			clearScreen();

			observableField[y1][x1] = gameField[y1][x1];
			std::cout << "Coordinates of the card: " << translateX(x1) << " " << translateY(y1) << "\n";
			printObservableField();

			std::cout << "Enter coordinates of the second card: ";
			int x2, y2;
			int card2 = pickSecondCard(x1, y1, &x2, &y2);
			clearScreen();
			
			observableField[y2][x2] = gameField[y2][x2];
			std::cout << "Coordinates of the first card: " << translateX(x1) << " " << translateY(y1) << "\n";
			std::cout << "Coordinates of the second card: " << translateX(x2) << " " << translateY(y2) << "\n";
			printObservableField();

			if (card1 == card2) {
				std::cout << "Congratulations! The cards are equal!" << "\n";
				cardsLeft -= 1;
				if (cardsLeft == 0)
					return true;
				return false;
			} else {
				observableField[y1][x1] = 0;
				observableField[y2][x2] = 0;
				std::cout << "The cards differ!" << "\n";
				return false;
			}
		}
	}
};

bool playAgain() {
	std::string wantsToReplay;
	std::cout << "\n" << "Play again? (y/n)" << "\n";
	std::cin >> wantsToReplay;

	if (wantsToReplay == "n")
		return false;
	else if (wantsToReplay == "y") 
		return true;
	else return playAgain();
}

int inputLimit() {
	while (true) {
		int input;
		if (!(std::cin >> input)) {
			std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
			std::cout << "Text entered instead of numbers. Please use numbers: ";
			continue;
		}
		std::cout << "\n";

		return input;
	}
}

// help player with input format
void printInputFormat(int width, int height) {
	std::cout << "\nThe input format of coordinates\n\nFirstly you type in x coordinate, ";
	std::cout << "then you type in y coordinate using space as a limiter\n";
	std::cout << "Left bottom corner has coordinates (1, 1), ";
	std::cout << "top right corner has coordinates (" << width << ", " << height << ")" << "\n\n"; 
}

// run the game
bool execute(Field field, int limit) {
	for (int attemp = 0; attemp < limit; attemp++) {
		if (field.pickCards())
			return true;
		std::cout << limit - attemp - 1 << " attempts left\n\n";
	}
	std::cout << "\n!!!You have ran out of attempts!!!\n";
	return false;
}

int main() {
	// random seed init
	srand(static_cast<unsigned int>(time(0)));

	// sizes of the field, they are not customizable due to conditions of the task
	int height = 4;
	int width = 6;
	std::cout << "The size of the field is " << height << " x " << width << "\n";

	std::cout << "Please enter the amount of attempts you want to have: ";

	int limit = inputLimit();

	// print the format of input
	printInputFormat(width, height);

	while (true) {
		Field field(height, width);
		bool win = execute(field, limit);
		std::cout << "\n\n";

		if (win)
			std::cout << "Congratulations! You have a great memory!\nWant to play again?\n";
		else {
			std::cout << "This time you got unlucky :c\n";
			std::cout << "But you still have a chance to beat this game!\nWant to play again?\n";
		}

		if (!playAgain()) {
			std::cout << "\n" <<"Hope you have spent your time nicely, see you soon c:" << "\n";
			break;
		}
	}

	return 0;
}
