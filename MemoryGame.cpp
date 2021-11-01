#include <iostream>
#include <ctime>
#include <vector>
#include <string>

/*
	by Amer On (Alexey Oleynik)
	https://github.com/Amer-On/MemoryGame
*/

int readIntFromString(std::string text) {
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
		throw std::runtime_error("No number found");
	return number;
}


class Field {
private:
	std::vector<std::vector<int> > gameField, actualGameField;

	std::vector<int> cards;

	int height, width;
	int cardsAmount;

public:
	Field() {}

	Field(int fieldHeight, int fieldWidth) {
		height = fieldHeight;
		width = fieldWidth;

		cardsAmount = fieldWidth * fieldHeight / 2;

		generateFields();
	}

private:
	void generateFields() {		
		generateCards();
		std::vector<int> row;
		std::vector<int> rowForAct;

		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				rowForAct.push_back(0);
				row.push_back(takeCard());
			}
			gameField.push_back(row);
			actualGameField.push_back(rowForAct);
			row.clear();
			rowForAct.clear();
		}
	}

	void generateCards() {
		for (int i = 0; i < cardsAmount; i++) {
			cards.push_back(i + 1);
			cards.push_back(i + 1);
		}
	}

	int takeCard() {return takeCard((int) (rand() % cards.size()));}

	int takeCard(int id) {
		int card = cards[id];
		cards.erase(cards.begin() + id);
		return card;
	}

	void printField(std::vector<std::vector<int> > field) {
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
		std::cout << output;
		std::cout << "\n";
	}

	void printGameField() {return printField(gameField);}

public:
	void printActualGameField() {return printField(actualGameField);}

	int turnOverCard(int x, int y) {
		if (x >= 0 and x < width and y >= 0 and y < height) {
			if (actualGameField[y][x] == 0)
				return gameField[y][x];
			else 
				throw std::logic_error("Card already opened");
		} else 
			throw std::runtime_error("Invalid coordinates");
	}

	void openCard(int x, int y) {actualGameField[y][x] = gameField[y][x];}

	void closeCard(int x, int y) {actualGameField[y][x] = 0;}

	int decreaseCardsAmount() {
		cardsAmount -= 1;
		return cardsAmount;
	}
};


class Game {
private:
	Field field;

	int height, width;
	int limit;

public:
	Game(int fieldHeight, int fieldWidth, int attemptsLimit) {
		height = fieldHeight;
		width = fieldWidth;
		limit = attemptsLimit;

		field = Field(fieldHeight, fieldWidth);
	}

private:
	void readCoords(int *x, int *y) {
		try {
			std::string inputX, inputY;
			
			std::cin >> inputX >> inputY;

			*x = readIntFromString(inputX);
			*y = readIntFromString(inputY);
		} catch (const std::runtime_error& e) {
			std::cout << "Please enter numbers instead of text" << "\n";
			readCoords(&*x, &*y);
			return;
		}

		*x -= 1;
		*y = height - *y;
	}

	int pickFirstCard(int *x1, int *y1) {
		std::cout << "Enter coordinates of the first card: ";
		readCoords(&*x1, &*y1);

		try {
			return field.turnOverCard(*x1, *y1);
		} catch (const std::runtime_error& e) {
			std::cout << "Please enter valid coordinates\n\n";
			return pickFirstCard(&*x1, &*y1);
		} catch (const std::exception& e) {
			std::cout << "You already opened this card, please choose another one\n\n";
			return pickFirstCard(&*x1, &*y1);
		}
	}

	int pickSecondCard(int x1, int y1, int *x2, int *y2) {
		std::cout << "Enter the coordinates of the second card: ";
		readCoords(&*x2, &*y2);

		if (*x2 == x1 && *y2 == y1) {
			std::cout << "The coordinates of the cards are equal, please enter different coordinates" << "\n";
			return pickSecondCard(x1, y1, &*x2, &*y2);
		}

		try {
			return field.turnOverCard(*x2, *y2);
		} catch (const std::runtime_error& e) {
			std::cout << "Please enter valid coordinates" << "\n";
			return pickSecondCard(x1, y1, &*x2, &*y2);
		} catch (const std::exception& e) {
			std::cout << "You already opened this card, please enter another coordinates" << "\n";
			return pickSecondCard(x1, y1, &*x2, &*y2);
		}
	}

	bool playRound() {
		int x1, y1;
		int card1 = pickFirstCard(&x1, &y1);
		clearScreen();

		field.openCard(x1, y1);
		std::cout << "Coordinates of the card: " << translateX(x1) << " " << translateY(y1) << "\n";
		field.printActualGameField();

		int x2, y2;
		int card2 = pickSecondCard(x1, y1, &x2, &y2);
		clearScreen();
		
		field.openCard(x2, y2);
		std::cout << "Coordinates of the first card: " << translateX(x1) << " " << translateY(y1) << "\n";
		std::cout << "Coordinates of the second card: " << translateX(x2) << " " << translateY(y2) << "\n";
		field.printActualGameField();

		if (card1 == card2) {
			std::cout << "Congradulations! The cards are equal!" << "\n";
			
			if (field.decreaseCardsAmount() == 0)
				return true;
			return false;
		} else {
			field.closeCard(x1, y1);
			field.closeCard(x2, y2);
			std::cout << "The cards differ!" << "\n";
			return false;
		}

	}

public:
	bool playGame(int limit) {
		for (int attemp = 0; attemp < limit; attemp++) {
			if (playRound())
				return true;
			std::cout << limit - attemp - 1 << " attempts left\n\n";;
		}
		std::cout << "\n!!!You have ran out of attempts!!!\n\n\n";
		return false;
	}

private:
	void clearScreen() {
		for (int i = 0; i < 10; i++) 
		std::cout << "\n\n\n\n\n\n\n\n\n\n";
	}

	int translateX(int x) {return x + 1;}

	int translateY(int y) {return height - y;}
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
	try {
		std::string input;
		std::cin >> input;
		std::cout << "\n";

		return readIntFromString(input);
	} catch (const std::runtime_error& e) {
		std::cout << "Please enter a number instead of text: ";
		return inputLimit();
	}
}

// help player with input format
void printInputFormat(int height, int width) {
	std::cout << "\nThe input format of coordinates\n\nFirstly you type in x coordinate, ";
	std::cout << "then you type in y coordinate using space as a limiter\n";
	std::cout << "Left bottom corner has coordinates (1, 1), ";
	std::cout << "top right corner has coordinates (" << width << ", " << height << ")" << "\n\n"; 
}

int main() {
	srand(static_cast<unsigned int>(time(0)));

	int height = 4;
	int width = 6;
	std::cout << "The size of the field is " << height << " x " << width << "\n";
	
	printInputFormat(height, width);

	while (true) {
		std::cout << "Please enter the amount of attempts you want to have: ";
		int limit = inputLimit();
		Game game(height, width, limit);

		if (game.playGame(limit))
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