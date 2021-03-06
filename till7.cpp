#include <iostream>

#include <sstream>

#include <string>

#include <limits>



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

	void insertBefore(Node* n) {

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

		//Return the loccation

		Node<T>* locate()

		{

			return location;

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

	//Inserts the value before the position pointed by pos and returns iterator pointing to insrted value

	iterator insert(iterator pos, const T& value) {


		if (nullptr == pos.locate()->getPrev())

		{

			Node<T>* n = new Node<T>(value);

			pos.locate()->insertBefore(n);

			head = n;

			return iterator(head);



		}

		else if (nullptr != pos.locate()) {

			Node<T>* n = new Node<T>(value);

			pos.locate()->insertBefore(n);

			return iterator(pos.locate()->getPrev());

		}

		else {

			//To insert at last position

			push_back(value);

			return iterator(tail);

		}

	}





	//Deletes the value at the position pointed by pos and returns iterator pointing to next location

	void erase(iterator pos) {



		if (head == pos.locate() && tail == pos.locate())

		{

			delete pos.locate();

			head = nullptr;

			tail = nullptr;

			//	return iterator(head);



		}

		else if (head == pos.locate())

		{

			head = head->getNext();

			delete pos.locate();

			//	return iterator(head);

		}

		else if (tail == pos.locate())

		{

			tail = tail->getPrev();

			delete pos.locate();

		}

		else {

			//	iterator next = pos.locate()->getNext();

			delete pos.locate();

			//	return iterator(next);

		}



	}



};







class Cell {

public:

	// String representation of data in this cell

	virtual std::string toString();

};



std::string  Cell::toString()

{

	return " ";

}



class NumericCell : public Cell {

private:

	double contents;

public:

	NumericCell(double data)

	{

		contents = data;

	}

	virtual std::string toString();

	void set(double new_data);

	double get();

};



std::string  NumericCell::toString()

{

	return std::to_string((double)contents);

}



void NumericCell::set(double new_data)

{

	contents = new_data;

}



double NumericCell::get()

{

	return contents;

}





class StringCell : public Cell {

private:

	std::string contents;

public:

	StringCell(std::string data)

	{

		contents = data;

	}

	virtual std::string toString();

};

std::string StringCell::toString()

{

	return contents;

}





typedef LList<LList<Cell*>> sheet;



class FunctionCell : public NumericCell {

	//	double contents;

	int f_row;

	int f_col_start;

	int f_col_end;

	std::string f_func_type;

	sheet &f_rows;

public:

	FunctionCell(int frow, int fcol_start, int fcol_end, std::string func_type, sheet& frows) :f_rows(frows), NumericCell(std::numeric_limits < double >::quiet_NaN())

	{

		f_row = frow;

		f_col_start = fcol_start;

		f_col_end = fcol_end;

		f_func_type = func_type;

	}

	virtual std::string toString();

	void min();

	void max();

	void mean();

	void update();

	void removefrow(int);

	void addfrow(int);



};

std::string FunctionCell::toString()

{

	return NumericCell::toString();

}



void FunctionCell::min()

{

	sheet::iterator row_it = f_rows.begin();

	for (int j = 0; j < f_row; ++j)

	{

		row_it++;

	}

	LList<Cell*>::iterator col_it = (*row_it).begin();

	for (int k = 0; k < f_col_start; ++k)

	{

		col_it++;

	}

	double min = DBL_MAX;

	double value;

	//std::cout << "Value list to find min are \n";

	for (int i = f_col_start; i <= f_col_end; ++i)

	{


		if (dynamic_cast<NumericCell*>(*col_it) != nullptr)

		{

			value = dynamic_cast<NumericCell*>(*col_it)->get();

			//This means the cell is Numeric

			if (value != std::numeric_limits < double >::quiet_NaN())

			{

				//	std::cout << value<<"\n";

				if (value < min)

				{

					min = value;

				}

			}

			col_it++;

		}

	}

	if (min == DBL_MAX)

	{

		std::cout << "Error: Bad input for set number";

	}

	else

	{

		set(min);

	}

}



void FunctionCell::max()

{

	sheet::iterator row_it = f_rows.begin();

	for (int j = 0; j < f_row; ++j)

	{

		row_it++;

	}

	LList<Cell*>::iterator col_it = (*row_it).begin();

	for (int k = 0; k < f_col_start; ++k)

	{

		col_it++;

	}

	double max = DBL_MIN;

	double value;

	for (int i = f_col_start; i <= f_col_end; ++i)

	{



		if (dynamic_cast<NumericCell*>(*col_it) != nullptr)

		{

			value = dynamic_cast<NumericCell*>(*col_it)->get();

			//This means the cell is Numeric

			if (value != std::numeric_limits < double >::quiet_NaN())

			{

				if (value > max)

				{

					max = value;

				}

			}

			col_it++;

		}

	}

	set(max);

}



void FunctionCell::mean()

{

	sheet::iterator row_it = f_rows.begin();

	for (int j = 0; j < f_row; ++j)

	{

		row_it++;

	}

	LList<Cell*>::iterator col_it = (*row_it).begin();

	for (int k = 0; k < f_col_start; ++k)

	{

		col_it++;

	}

	double sum = 0.0;

	double value;

	int count = 0;

	for (int i = f_col_start; i <= f_col_end; ++i)

	{



		if (dynamic_cast<NumericCell*>(*col_it) != nullptr)

		{

			value = dynamic_cast<NumericCell*>(*col_it)->get();

			//This means the cell is Numeric

			if (value != std::numeric_limits < double >::quiet_NaN())

			{

				++count;

				sum += value;


			}

		}

		col_it++;

	}

	set(sum / count);

}

void FunctionCell::addfrow(int row_loc)

{

	if (row_loc <= f_row)

	{

		f_row++;

	}

}

void FunctionCell::removefrow(int row_loc)

{

	if (row_loc = f_row)

	{

		set(std::numeric_limits<double>::quiet_NaN());

		f_func_type = "NA";//Removed the func_type property if the orw it operates is lost'

	}

	else if (row_loc<f_row)

	{

		f_row--;

	}

}

void FunctionCell::update()

{

	if (f_func_type.compare("min") == 0)

	{

		min();

	}

	else if (f_func_type.compare("max") == 0)

	{

		max();

	}

	else if (f_func_type.compare("mean") == 0)

	{

		mean();

	}

}





class Worksheet {

private:

	int w_width;

	int w_height;

	//Push back a new linked list to serve as a row

public:

	LList<LList<Cell*>> rows;

	Worksheet(const int width, const int height)

	{

		w_width = width;

		w_height = height;

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

	int refresh()

	{
		int count_updates = 0;
		LList<LList<Cell*>>::iterator start = rows.begin();

		LList<LList<Cell*>>::iterator end = rows.end();

		while (start != end){

			LList<Cell*>::iterator beg = (*start).begin();
			LList<Cell*>::iterator fin = (*start).end();

			LList<Cell*>::iterator it = beg;

			for (it = beg; it != fin; ++it)

			{

				if (dynamic_cast<FunctionCell*>(*it) != nullptr)

				{
					std::cout << "FunctionCell detected in refresh";
					double before = dynamic_cast<FunctionCell*>(*it)->get();
					dynamic_cast<FunctionCell*>(*it)->update();
					double after = dynamic_cast<FunctionCell*>(*it)->get();
					if (before != after)
					{
						count_updates++;
					}
					//std::cout << "FunctionCell update value";

					//std::cout << (**it).toString();

				}

			}

			start++;

		}
		return count_updates;
	}

	void set(int column, int row, double input)

	{

		if ((column>(w_width - 1)) || (row>(w_height - 1)) || (column<0) || (row<0))

		{

			std::cout << "Error: cell out of range\n";

		}

		else

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

			*it = new NumericCell(input);

		}
		int count = refresh();
		while (count > 0)
		{
			count = refresh();
		}
	}

	void set(int column, int row, std::string input)

	{

		if ((column>(w_width - 1)) || (row>(w_height - 1)) || (column<0) || (row<0))

		{

			std::cout << "Error: cell out of range\n";

		}

		else

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

			*it = new StringCell(input);

		}
		int count = refresh();
		while (count > 0)
		{
			count = refresh();
		}
	}

	//int frow, int col_start, int col_end, std::string func_type, sheet& rows

	void set(int column, int row, int frow, int col_start, int col_end, std::string func_type, sheet& frows)

	{

		if ((column>(w_width - 1)) || (row>(w_height - 1)) || (column<0) || (row<0))

		{

			std::cout << "Error: cell out of range\n";

		}

		else

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

			FunctionCell* fun = new FunctionCell(frow, col_start, col_end, func_type, frows);

			if (func_type.compare("min") == 0)

			{

				fun->min();

			}

			else if (func_type.compare("max") == 0)

			{

				fun->max();

			}

			else if (func_type.compare("mean") == 0)

			{

				fun->mean();

			}

			*it = fun;


		}
		int count = refresh();
		while (count > 0)
		{
			count = refresh();
		}

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

				if (it != beg)

				{//Number of commas should  be one less than number of columns

					std::cout << ",";

				}

				std::cout << (**it).toString();

			}

			std::cout << std::endl;

			start++;

		}

	}



	void addrow(int row)

	{

		if ((w_height == 0) && (row == 0)) //the list became empty due to removerow

		{

			std::cout << "We don't know what should be width of worksheet,its empty \n";

		}

		else if ((row>w_height) || (row<0))

		{

			std::cout << "Error: row out of range\n";

		}

		else

		{

			LList<LList<Cell*>>::iterator start = rows.begin();

			for (int i = 0; i<row; i++){

				start++;

			}

			LList<Cell*> newRow;

			start = rows.insert(start, newRow);

			for (int j = 0; j != w_width; j++) {

				(*start).push_back(new Cell());

			}

			++w_height;

			//addfrow(row);//Think how the function cell working on this row will get updated

			LList<LList<Cell*>>::iterator s = rows.begin();

			LList<LList<Cell*>>::iterator e = rows.end();

			while (s != e){

				LList<Cell*>::iterator b = (*start).begin();

				LList<Cell*>::iterator f = (*start).end();

				LList<Cell*>::iterator it = b;

				for (it = b; it != f; ++it)

				{

					if (dynamic_cast<FunctionCell*>(*it) != nullptr)

					{

						dynamic_cast<FunctionCell*>(*it)->addfrow(row);

					}

				}

				s++;

			}

			int count = refresh();
			while (count > 0)
			{
				count = refresh();
			}

		}

	}



	void removerow(int row)

	{

		if ((row >= w_height) || (row<0))

		{

			std::cout << "Error: row out of range\n";

		}

		else

		{

			LList<LList<Cell*>>::iterator start = rows.begin();

			for (int i = 0; i<row; i++){

				start++;

			}

			rows.erase(start);

			/*	for (int j = 0; j != w_width; j++) {

			(*start).push_back(new Cell());

			}

			*/

			--w_height;

			//removefrow();//Think how the function cell working on this new row will be updated
			// If sheet is empty no need to do anything
			if (w_height<0)
			{

			}
			else
			{
				std::cout << "Iterating through the new list\n";// Check what is causing crash
				LList<LList<Cell*>>::iterator s = rows.begin();

				LList<LList<Cell*>>::iterator e = rows.end();

				while (s != e){

					LList<Cell*>::iterator b = (*start).begin();

					LList<Cell*>::iterator f = (*start).end();

					LList<Cell*>::iterator it = b;

					for (it = b; it != f; ++it)

					{

						if (dynamic_cast<FunctionCell*>(*it) != nullptr)

						{
							std::cout << "Function Cell detected \n";
							dynamic_cast<FunctionCell*>(*it)->removefrow(row);

						}

					}

					s++;



				}

				
				int count = refresh();
				while (count > 0)
				{
					count = refresh();
				}
				
			}

		}

	}







};







#include <algorithm>

#include<exception>

#include<string>

int main(int argc, char** argv) {



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

		if (height < 0 || width<0)

			throw std::exception("received negative value");

		if (height == 0 || width == 0)

			throw std::exception("received Zero value");

	}

	catch (std::exception e) {





		// Handle failure of the stoi function

		std::cout << " Got bad values for width and height !\n";

		return 1;

	}

	Worksheet shikha(width, height);

	//shikha.init();

	//shikha.print();



	std::string line;

	while (std::getline(std::cin, line)) {

		// Convert the string into an input stream and read in the command

		std::istringstream ss(line);

		std::string command;

		ss >> command;

		//Reading quit command correctly

		if (command.compare("quit") == 0)

		{

			//Remeember to delete memory here

			return 0;

		}

		//Reading print command corrctly

		else if (command.compare("print") == 0)

		{

			shikha.print();

		}

		//Reading set command correctly

		else if (command.compare("set") == 0)

		{

			std::string type;



			int col, row;

			ss >> col >> row;

			//	std::cout << col << std::endl<<row << std::endl;



			ss >> type;

			//std::cout << type;

			//	std::cout << "\n";

			if (type.compare("number") == 0)

			{

				double x;

				if (ss >> x) {

					// std::cout << "Number is" << x << "\n";

					// number was read successfully

					// Now calling set function

					shikha.set(col, row, x);

					//	shikha.print();

				}

				else {

					// Input was not a number , ignore it

					std::cout << "Error: Bad input for set number\n";



				}

			}



			if (type.compare("string") == 0)

			{

				std::string str;

				if (std::getline(ss, str))

				{

					//remove the space in beginning of string

					str.erase(0, 1);

					shikha.set(col, row, str);

				}

				else

				{

					//Input was not a string

					std::cout << "Error: Bad input for set number\n";



				}

			}

			if ((type.compare("min") == 0) || (type.compare("max") == 0) || (type.compare("mean") == 0))

			{

				int row_off;

				int col_off_1;

				int col_off_2;

				if ((ss >> row_off) && (ss >> col_off_1) && (ss >> col_off_2)) {


					//	std::cout << "All offsets read" << "Row"<< row_off <<"Col1"<<col_off_1<<"Col2"<<col_off_2<<"\n";

					// number was read successfully

					// Now calling set function

					//int column, int row, int frow, int col_start, int col_end, std::string func_type, sheet& frows

					shikha.set(col, row, row_off, col_off_1, col_off_2, type, shikha.rows);

					// (*it)->min();

					//	shikha.print();

				}

				else {

					// Input was not a number , ignore it

					std::cout << "Error: Bad input for set number\n";



				}

			}



		}

		// Reading the addrow command correctly addrow x inserts new row before positon x

		else if (command.compare("addrow") == 0)

		{

			int x;

			if (ss >> x) {

				//number was read successfully

				std::cout<<"Adding a new row before" << x << "\n";

				shikha.addrow(x);

			}

			else{

				//Input is not a number,ignore it

				//std::cout<<"Error: Bad input for set number \n";

				std::cout << "Error: row out of range\n";

			}

		}

		else if (command.compare("removerow") == 0)

		{

			int x;

			if (ss >> x) {

				//number was read successfully

					std::cout << "Removing row at position : " << x << "\n";

				shikha.removerow(x);

			}

			else{

				//Input is not a number,ignore it

				//std::cout<<"Error: Bad input for set number \n";

				std::cout << "Error: row out of range\n";



			}



		}

		else

		{

			std::cout << "Error: unknown command\n";

		}

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
