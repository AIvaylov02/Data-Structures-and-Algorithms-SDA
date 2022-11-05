#include <iostream>

struct Node
{
	int data;
	Node* previous;
	Node* next;

	Node(const int& data = -1, Node* previous = nullptr, Node* next = nullptr);

	// although we have unmanaged dynamic memory, there is no way to clear it, as it intersects, with one another
};

Node::Node(const int& data, Node* previous, Node* next)
{
	this->data = data;
	this->previous = previous;
	this->next = next;
}

// 100'001 in order to avoid index maths
const int MAX_PACKAGES_POOL = 100'001;

// The list in double-int-linked
class DoubleIntLinkedList
{
public:
	DoubleIntLinkedList() = default;

	void push_front(const int& data);
	void push_back(const int& data);

	void pop_back();

	DoubleIntLinkedList(const DoubleIntLinkedList& other);
	DoubleIntLinkedList& operator=(const DoubleIntLinkedList& other);
	~DoubleIntLinkedList();

	void print() const;

	// function for this specific task, the others are from my implementation of double linked list
	bool updateList(const int& searchedElement, const int maxCollectionElementsCount, Node** valuesContainer);


private:
	Node* head = nullptr;
	Node* tail = nullptr;

	int count = 0;

	void free();
	void copyFrom(const DoubleIntLinkedList& other);
};

void DoubleIntLinkedList::push_front(const int& data)
{
	// Create a new head, which next element will go to the old head(old first elem)
	Node* newHead = new Node(data, nullptr, head);

	if (head != nullptr) // we already have at least one element, so we want to make a backward connection with the previously added one to the new one
		head->previous = newHead;
	head = newHead; // make the head point to the new head

	if (tail == nullptr) // if it has only 1 element, it should be incremented
		tail = head;

	count++;
}

void DoubleIntLinkedList::push_back(const int& data)
{
	if (head == nullptr)
	{
		head = new Node(data);
		tail = head;
	}
	else
	{
		tail->next = new Node(data, tail);
		tail = tail->next;
	}

	count++;
}

void DoubleIntLinkedList::pop_back()
{
	//if (head == nullptr) // head or tail, it doesn't matter, it means we have an empty list
	//	throw std::exception("Invalid operation, list it empty!");
	count--;

	if (head == tail && head != nullptr) // only 1 element is present
	{
		delete head;
		head = tail = nullptr;
		return;
	}


	Node* toBeDeleted = tail;
	tail = tail->previous;
	tail->next = nullptr;
	delete toBeDeleted;
}

void DoubleIntLinkedList::print() const
{
	Node* currElem = head;
	//if (currElem == nullptr)
	//	throw std::exception("Invalid operation, list it empty!");

	while (currElem != nullptr)
	{
		std::cout << currElem->data << ' ';
		currElem = currElem->next;
	}
	std::cout << '\n';
}



void DoubleIntLinkedList::free()
{
	Node* current = head;
	while (current != nullptr)
	{
		Node* next = current->next;
		delete current;
		current = next;
	}

	head = tail = nullptr;
}

void DoubleIntLinkedList::copyFrom(const DoubleIntLinkedList& other)
{
	Node* current = other.head;
	while (current != nullptr)
	{
		push_back(current->data); // pushback keeps track of head and tail
		current = current->next;
	}
}

DoubleIntLinkedList::DoubleIntLinkedList(const DoubleIntLinkedList& other)
{
	copyFrom(other);
}

DoubleIntLinkedList& DoubleIntLinkedList::operator=(const DoubleIntLinkedList& other)
{
	if (this != &other)
	{
		free();
		copyFrom(other);
	}
	return *this;
}

DoubleIntLinkedList::~DoubleIntLinkedList()
{
	free();
}

bool DoubleIntLinkedList::updateList(const int& searchedElement, const int maxCollectionElementsCount, Node** valuesContainer)
{
	Node* searchedElemPos = valuesContainer[searchedElement];

	//Key is not in our collection
	if (searchedElemPos == nullptr)
	{
		if (maxCollectionElementsCount == count)
		{
			valuesContainer[tail->data] = nullptr;
			pop_back();
		}

		push_front(searchedElement);
		valuesContainer[searchedElement] = head;

		return false;
	}

	// we have a valid element in our collection
	if (searchedElemPos == head) // the searched element is already at the list front, we must just return true
		return true;

	if (searchedElemPos == tail) // if it is at last position, pop_back and add it to front
		pop_back();
	else // it is in the middle of the list somewhere
	{
		Node* prevElemPos = searchedElemPos->previous;
		prevElemPos->next = searchedElemPos->next;
		searchedElemPos->next->previous = prevElemPos;
		delete searchedElemPos;
		count--;
	}

	// By deleting and putting an element, we change it's address, thus making the address in the container invalid. This solution should fix that.
	push_front(searchedElement);
	valuesContainer[searchedElement] = head;
	return true;
}



void printResult(bool isFound)
{
	if (isFound)
		std::cout << "true\n";
	else
		std::cout << "false\n";
}

// The task can be broken down to these pieces:
// 1) If element is not in list, std::print false
// 2) If there is no space to add an element, remove the last element
// 3) If element is found, print true and put the element at front

int main()
{
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(NULL);

	int queries, maxMemory;
	std::cin >> queries >> maxMemory;

	Node** valuesContainer = new Node * [MAX_PACKAGES_POOL]; // Indexes as values - 1, elements as ptr to the elements . This will give us constant search time
	for (int i = 0; i < MAX_PACKAGES_POOL; i++)
		valuesContainer[i] = nullptr;


	DoubleIntLinkedList laptopPackages;
	int currRequest;
	bool IsContainedInLibrary;
	
	for (int i = 0; i < queries; i++)
	{
		std::cin >> currRequest;
		IsContainedInLibrary = laptopPackages.updateList(currRequest, maxMemory, valuesContainer);
		printResult(IsContainedInLibrary);
	}

	delete[] valuesContainer; // the deletion of the nodes will happen through the linkedList;
	return 0;
}