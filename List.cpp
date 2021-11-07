#include <iostream>
#include <string>

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



	void printList() {
		std::cout << "Here is the list: " << "\n";
		for (int i = 0; i < size; i++) 
			std::cout << list[i] << " ";
		std::cout << "\n";
	}
};



int main() {
	std::cout << "Hello" << "\n";

	List list = List();

	list.add(5);
	list.add(6);
	list.add(1);
	list.add(2);
	list.add(7);
	list.add(123);
	list.add(5);

	std::cout << "size is " << list.getSize() << "\n";

	list.printList();

	list.remove(0);
	std::cout << "size is " << list.getSize() << "\n";
	list.extract(5);
	list.extract(2);
	list.extract(1);

	list.printList();
	std::cout << "size is " << list.getSize() << "\n";

	return 1;
}