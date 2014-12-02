#include <iostream>
#include <sstream>
#include <string>

template <typename T>
class Node {
private:
	//The value
	T val;
	//The pointers to next and previous nodes
	Node* next;
	Node* prev;

	Node(Node& other);
	Node operator=(Node& other);

public:

	Node(T value) {
		//Initialize with the stored value
		val = value;
		next = nullptr;
		prev = nullptr;
	}

	//Return a reference so that the value can be modified
	T& getVal() {
		return val;
	}

	//Insert a new node after this one
	void insert(Node* n) {
		//Fix the link between the new node and next
		if (nullptr != next) {
			next->prev = n;
		}
		n->next = next;

		//Fix the link from this node to the next one
		n->prev = this;
		next = n;
	}

	//Insert a new node before this node
	//Return a pointer to the new node
	Node* insertBefore(Node* n) {
		//Fix the link between prev and the new node
		if (nullptr != prev) {
			prev->next = n;
		}
		n->prev = prev;

		//Fix the link from this node to the new one
		n->next = this;
		prev = n;
	}

	~Node() {
		if (nullptr != prev) {
			prev->next = next;
		}
		if (nullptr != next) {
			next->prev = prev;
		}
	}

	Node* getNext() {
		return next;
	}

	Node* getPrev() {
		return prev;
	}
};

template <typename T>
class LList {
private:
	Node<T> *head;
	Node<T> *tail;

	//Don't allow copying of the entire list since it won't work correctly
	//without us writing a specific implementation
//	LList(LList& other) = delete;
//	LList operator=(LList& other) = delete;
public:
	LList() {
		head = nullptr;
		tail = nullptr;
	}

	

	~LList() {
		if (nullptr != head) {
			while (nullptr != head->getNext()) {
				delete head->getNext();
			}
			delete head;
		}
	}

	void push_back(T val) {
		Node<T>* n = new Node<T>(val);
		if (nullptr == head) {
			head = n;
			tail = head;
		}
		else {
			tail->insert(n);
			tail = n;
		}
	}
	
	T & back() const {
		return tail->getVal();
	}

	class iterator {
	private:
		Node<T>* location;
	public:
		//Define the value_type. This helps with other templates.
		typedef T value_type;

		//Constructor
		iterator(Node<T>* start) {
			location = start;
		}

		//Increment operator: prefix version
		iterator& operator++() {
			//We should throw an error here, but for simplicity we'll just stop
			//at the end of the list. To learn more, read about exceptions.
			if (nullptr != location) {
				location = location->getNext();
			}
			//Return a reference to this iterator, so use * to convert from a pointer
			return *this;
		}

		//Increment operator: postfix version
		//The postfix version is distinguished by an unnused integer argument
		iterator operator++(int unnused) {
			iterator copy = *this;
			++(*this);
			//Return the copy that is still in the previous location
			return copy;
		}

		//operator* will get a reference to the value at the iterator's location
		T& operator*() {
			//If we are at the end then we cannot return a value
			if (nullptr != location) {
				return location->getVal();
			}
			//Again, we should throw an exception to be "correct" here, but for
			//simplicity we will do nothing
		}

		//operator!= checks if two iterators are at the same location
		//Takes a const iterator& to guarantee we will not modify the other iterator
		bool operator!=(const iterator& other) {
			return location != other.location;
		}

		//Get the distance from one iterator to another
		int operator-(const iterator& other) {
			//Easily defined as a recursive function
			if (*this != other) {
				return 1 + (++(*this) - other);
			}
			else {
				return 0;
			}
		}


	};

	//Return an iterator to the beginning
	iterator begin() {
		return iterator(head);
	}

	//Return one past the last element
	iterator end() {
		return iterator(nullptr);
	}
};



class Cell {
public:
	// String representation of data in this cell
	virtual double toString();
};

double Cell::toString()
{
	std::cout << "";
	return 0;
}

class NumericCell : public Cell {
private:
	double contents;
public:
	NumericCell(double data)
	{
		contents = data;
	}
	double toString();
};

double NumericCell::toString()
{
	return contents;
}

class StringCell : public Cell {
private:
	std::string contents;
public:
	virtual double toString();
};


class FunctionCell : public NumericCell {
	double contents;
public:
	virtual double toString();
};

class Worksheet {
private:
	LList<LList<Cell*>> rows;
	//Push back a new linked list to serve as a row
public:
	Worksheet(const int width, const int height)
	{
		for (int i = 0; i < height; ++i) {
			LList<Cell*> empty;
			rows.push_back(empty);
			//Make a new empty cell in this row for every column
			for (int j = 0; j < width; ++j) {
				//The back() operator returns a reference to the last element in the list
				rows.back().push_back(new Cell());
			}
			//rows.push_back(LList<Cell*>());
		}
	}
		
	void set(int column, int row, std::string input)
		{
			LList<LList<Cell*>>::iterator start = rows.begin();
			for (int i = 0; i < row; ++i) {
				start++;
			}
			LList<Cell*>::iterator it = (*start).begin();
			for (int j = 0; j < column; ++j)
			{
				it++;
			}
			delete *it;
			*it = new NumericCell ((row + 1)*(column + 1));
		}

	void init()
	{
		LList<LList<Cell*>>::iterator start = rows.begin();
		LList<LList<Cell*>>::iterator end = rows.end();
		int count = 0;
		while (start != end) {
	
			count++;
			LList<Cell*>::iterator beg = (*start).begin();
			LList<Cell*>::iterator fin = (*start).end();
			LList<Cell*>::iterator it = beg;
			for (it = beg; it != fin; ++it)
			{
				count++;
				delete *it;
				*it = new NumericCell(count);
			}
			start++;
		}

	}

	void print()
	{
		LList<LList<Cell*>>::iterator start = rows.begin();
		LList<LList<Cell*>>::iterator end = rows.end();
		while (start != end) {
	
			LList<Cell*>::iterator beg = (*start).begin();
			LList<Cell*>::iterator fin = (*start).end();
			LList<Cell*>::iterator it = beg;
			for (it = beg; it != fin; ++it)
			{
				std::cout << (**it).toString()<<",";
			}
			std::cout << std::endl;
			start++;
		}
	}



};



#include <algorithm>
#include<exception>
#include<string>
int main(int argc, char** argv) {
   Worksheet shikha(2, 6);
	shikha.init();
	shikha.print();
	// Initialize the width and height of the spreadsheet
	if (argc < 3){
		std::cout << "Too few arguments!! Enter the height and the width\n";
		return 1;
	}
	int width = 0;
	int height = 0;
	
	
	try {
		// Attempt to set width and height from the user provided arguments
		width = std::stoi(std::string(argv[1]));
		height = std::stoi(std::string(argv[2]));
		if (height < 0|| width<0)
			throw std::exception("received negative value");
		if (height == 0 || width == 0)
			throw std::exception("received Zero value");
	}
	catch (std::exception e) {
	
		
		// Handle failure of the stoi function
		std::cout <<" Got bad values for width and height !\n";
		return 1;
	}
	
	std::string line;
	while (std::getline(std::cin, line)) {
		// Convert the string into an input stream and read in the command
		std::istringstream ss(line);
		std::string command;
		ss >> command;
		if (command == 'set')
		{
			std::string type;
			ss >> type;
			if (type == 'number')
			{
				int col, row;
				ss >> col >> row;
			}

		}
		// Process the command
		
	}

	/*
	LList<float> list;
	list.push_back(3.14);
	list.push_back(2.8);
	list.push_back(3.4);
	list.push_back(110000000000);

	//List explicitly using iterators
	for (LList<float>::iterator I = list.begin(); I != list.end(); ++I) {
		std::cout << "The node has value " << *I << '\n';
	}
	//Using standard library functions
	//std::sort(list.begin(), list.end());

	//List using range-based for loop
	for (float& f : list) {
		std::cout << "The node has value " << f << '\n';
	}

	//The destructor will automatically delete the memory from every Node*
*/
	return 0;
}