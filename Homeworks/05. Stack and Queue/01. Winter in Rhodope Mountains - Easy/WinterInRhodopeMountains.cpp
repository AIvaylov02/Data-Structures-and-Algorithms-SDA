#include <iostream>
#include <utility>

struct Node
{
	std::pair<int, int> coordinates;
	Node* next;
	Node(std::pair<int, int> coordinates, Node* next = nullptr);
};

Node::Node(std::pair<int, int> coordinates, Node* next)
{
	this->coordinates = coordinates;
	this->next = next;
}

// first in, first out -> push_front i pop_back, tail i head
class Queue
{
public:
	Queue() = default;

	Queue(const Queue& other);
	Queue& operator=(const Queue& other);
	~Queue();

	bool IsEmpty() const;
	void Enqueue(std::pair<int, int> coordinates); //push_back
	std::pair<int, int> Peek_Front() const;
	std::pair<int, int> Peek_Back() const;
	void Dequeue(); // pop_front

	void PrintQueue() const;
	int getCount() const;

private:
	Node* head = nullptr;
	Node* tail = nullptr;

	int count = 0;
	void free();
	void copyFrom(const Queue& other);
};


bool Queue::IsEmpty() const
{
	return head == nullptr;
}

void Queue::Enqueue(std::pair<int, int> coordinates)
{
	if (IsEmpty())
	{
		head = new Node(coordinates);
		tail = head;
	}
	else
	{
		tail->next = new Node(coordinates);
		tail = tail->next;
	}
	count++;
}

std::pair<int, int> Queue::Peek_Front() const
{
	//if (IsEmpty())
		//throw std::exception("Queue is empty!");
	return head->coordinates;
}

std::pair<int, int> Queue::Peek_Back() const
{
	//if (IsEmpty())
	//	throw std::exception("Queue is empty!");
	return tail->coordinates;
}

void Queue::Dequeue()
{
	//if (IsEmpty())
	//	throw std::exception("Queue is empty!");

	Node* oldHead = head;
	if (head == tail) // 1 element
		tail = nullptr;

	head = head->next;
	delete oldHead;
	count--;
}

void Queue::free()
{
	Node* current = head;
	while (current != nullptr)
	{
		Node* toBeDeleted = current;
		current = current->next;
		delete toBeDeleted;
	}
	tail = head = nullptr;
	count = 0;
}

void Queue::copyFrom(const Queue& other)
{
	Node* current = other.head;
	while (current != nullptr)
	{
		Enqueue(current->coordinates);
		current = current->next;
	}
}

Queue::Queue(const Queue& other)
{
	copyFrom(other);
}
Queue& Queue::operator=(const Queue& other)
{
	if (this != &other)
	{
		free();
		copyFrom(other);
	}
	return *this;
}
Queue::~Queue()
{
	free();
}

void Queue::PrintQueue() const
{
	Node* current = head;
	while (current != nullptr)
	{
		std::cout << current->coordinates.first << '-' << current->coordinates.second << ' ';
		current = current->next;
	}
	std::cout << '\n';
}
int Queue::getCount() const
{
	return count;
}

int CalcRectangleArea(int a, int b);
bool** createArray(int rows, int cols);
void emptyArray(bool** arr, int rows);

void ParseRow(int& row, int basketRows);
void ParseColumn(int& col);
void addFirstRottenApples(bool** basket, int basketRows, int basketCols, Queue& applesQueue, int& rottenApplesCount);
bool IsHealthy(int appleX, int appleY, int basketRows, int basketCols, bool** basket); // works with already translated to pc coordinates

void waitNDays(bool** basket, int basketRows, int basketCols, Queue& applesQueue, int& rottenApplesCount, int days);


int main()
{
	int rows, columns, days; // they should be int, since max size rows and cols is 1000*1000 = 1'000'000, which is integer
	std::cin >> rows >> columns >> days;

	bool** basket = createArray(rows, columns); // bool array of our visited values - if the apple is already rotten it will contain a 1 at that spot; if not -> it will be 0 
	Queue followingRottenApples; // should be queue of pairs because it will store coordinates(tuple) of the next apple to be checked
	int rottenApplesCount = 0;

	addFirstRottenApples(basket, rows, columns, followingRottenApples, rottenApplesCount);

	// start traversing the array and queue and find more rotten apples
	waitNDays(basket, rows, columns, followingRottenApples, rottenApplesCount, days);
	int remainingApples = CalcRectangleArea(rows, columns) - rottenApplesCount;
	std::cout << remainingApples;

	emptyArray(basket, rows); // free dynamic memory
	return 0;
}





int CalcRectangleArea(int a, int b)
{
	return a * b;
}

bool** createArray(int rows, int cols)
{
	bool** basket = new bool* [rows];
	for (int i = 0; i < rows; i++)
		basket[i] = new bool[cols] {0}; // set aside memory for columns and fill each member of it with 0
	return basket;
}
void emptyArray(bool** arr, int rows)
{
	for (int i = 0; i < rows; i++)
		delete[] arr[i];
	delete[] arr;
}

void ParseRow(int& row, int basketRows)
{
	row = basketRows - row; // in order to invert it from human format to pc one ( 8 - 4 = 4), 8 -2 = 6, which is order 7 in human language
}
void ParseColumn(int& col)
{
	col--; // to compensate for the machine/human offset
}

bool IsHealthy(int appleX, int appleY, int basketRows, int basketCols, bool** basket)
{
	if (appleX < 0 || appleX >= basketRows)
		return false;
	if (appleY < 0 || appleY >= basketCols)
		return false;
	return basket[appleX][appleY] != true; // a.k.a if it is healthy returns 1, otherwise returns 0
}

void addFirstRottenApples(bool** basket, int basketRows, int basketCols, Queue& applesQueue, int& rottenApplesCount)
{
	// parse the rotten apples to the queue and the array
	const int MAX_ROTTEN_APPLES = 2;
	const int DEFAULT_INVALID_VAL = 0;
	int rottenAppleX, rottenAppleY;
	for (int i = 0; i < MAX_ROTTEN_APPLES; i++)
	{
		rottenAppleX = rottenAppleY = DEFAULT_INVALID_VAL;
		std::cin >> rottenAppleX >> rottenAppleY;

		if (rottenAppleX != DEFAULT_INVALID_VAL)
		{
			// in order to translate human counting to machine one by using offsets
			ParseRow(rottenAppleX, basketRows);
			ParseColumn(rottenAppleY);
			if (!IsHealthy(rottenAppleX, rottenAppleY, basketRows, basketCols, basket)) // corner case check if the second apple has the same coordinates as the first one
				continue;

			basket[rottenAppleX][rottenAppleY] = true;
			std::pair<int, int> currAppleCoords(rottenAppleX, rottenAppleY);
			applesQueue.Enqueue(currAppleCoords);
			rottenApplesCount++;
		}
	}
}

void waitNDays(bool** basket, int basketRows, int basketCols, Queue& applesQueue, int& rottenApplesCount, int days)
{
	for (int i = 0; i < days; i++)
	{
		int currIterationApples = applesQueue.getCount(); // tells the number of apples which must be seen in this iteration - FIRST IN, FIRST OUT of queue is used for that
		while (currIterationApples > 0)
		{
			int currRottenX = applesQueue.Peek_Front().first;
			int currRottenY = applesQueue.Peek_Front().second;
			applesQueue.Dequeue();

			// upwards
			if (IsHealthy(currRottenX - 1, currRottenY, basketRows, basketCols, basket))
			{
				std::pair<int, int> newRottenApple(currRottenX - 1, currRottenY);
				applesQueue.Enqueue(newRottenApple);
				basket[currRottenX - 1][currRottenY] = true;
				rottenApplesCount++;
			}
			// downwards --
			if (IsHealthy(currRottenX + 1, currRottenY, basketRows, basketCols, basket))
			{
				std::pair<int, int> newRottenApple(currRottenX + 1, currRottenY);
				applesQueue.Enqueue(newRottenApple);
				basket[currRottenX + 1][currRottenY] = true;
				rottenApplesCount++;
			}
			// left --
			if (IsHealthy(currRottenX, currRottenY - 1, basketRows, basketCols, basket))
			{
				std::pair<int, int> newRottenApple(currRottenX, currRottenY - 1);
				applesQueue.Enqueue(newRottenApple);
				basket[currRottenX][currRottenY - 1] = true;
				rottenApplesCount++;
			}
			// right -- 
			if (IsHealthy(currRottenX, currRottenY + 1, basketRows, basketCols, basket))
			{
				std::pair<int, int> newRottenApple(currRottenX, currRottenY + 1);
				applesQueue.Enqueue(newRottenApple);
				basket[currRottenX][currRottenY + 1] = true;
				rottenApplesCount++;
			}
			currIterationApples--;
		}
	}
}