#include <iostream>

struct Node
{
	int value;
	Node* next;
	Node(int value, Node* next = nullptr);
};

Node::Node(int value, Node* next)
{
	this->value = value;
	this->next = next;
}

class Stack
{
public:
	Stack() = default;

	Stack(const Stack& other);
	Stack& operator=(const Stack& other);
	~Stack();

	bool IsEmpty() const;
	int Peek() const;
	void Pop();
	void Push(int element);

	void Print() const;
private:
	Node* head = nullptr;

	void free();
	void copyFrom(const Stack& other);
	void copyStackRecursively(Node* otherCurr);
};

bool Stack::IsEmpty() const
{
	return head == nullptr;
}
int Stack::Peek() const
{
	//if (IsEmpty())
		// throw std::exception("Stack is empty!");
	return head->value;
}
void Stack::Pop()
{
	//if (IsEmpty())
		// throw std::exception("Stack is empty!");

	Node* toBeDeleted = head;
	head = head->next;
	delete toBeDeleted;
}
void Stack::Push(int element)
{
	head = new Node(element, head);
}

void Stack::free()
{
	while (head != nullptr)
	{
		Node* nextCell = head->next;
		delete head;
		head = nextCell;
	}
	//head = nullptr;
}
void Stack::copyFrom(const Stack& other)
{
	if (other.head == nullptr)
	{
		head = nullptr;
		return;
	}
	Node* otherCurr = other.head;
	copyStackRecursively(otherCurr);
}

void Stack::copyStackRecursively(Node* otherCurr)
{
	// We have to use recursion in order to properly get to the last element first and push it in our new Stack. Alternative would be to use 2 stacks to copy 1
	// 2 Inversions = 1 straight move
	if (otherCurr != nullptr) 
	{
		copyStackRecursively(otherCurr->next);
		Push(otherCurr->value);
	}
}

Stack::Stack(const Stack& other)
{
	copyFrom(other);
}
Stack& Stack::operator=(const Stack& other)
{
	if (this != &other)
	{
		free();
		copyFrom(other);
	}
	return *this;
}
Stack::~Stack()
{
	free();
}

void Stack::Print() const
{
	Node* curr = head;
	while (curr != nullptr)
	{
		std::cout << curr->value << ' ';
		curr = curr->next;
	}
	std::cout << '\n';
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
	void Enqueue(int element); //push_back
	int Peek_Front() const;
	int Peek_Back() const;
	void Dequeue(); // pop_front

	int findMinElem() const;

	void PrintQueue() const;

private:
	Node* head = nullptr;
	Node* tail = nullptr;

	void free();
	void copyFrom(const Queue& other);
};


bool Queue::IsEmpty() const
{
	return head == nullptr;
}

void Queue::Enqueue(int element)
{
	if (IsEmpty())
	{
		head = new Node(element);
		tail = head;
	}
	else
	{
		tail->next = new Node(element);
		tail = tail->next;
	}
}

int Queue::Peek_Front() const
{
	//if (IsEmpty())
		//throw std::exception("Queue is empty!");
	return head->value;
}

int Queue::Peek_Back() const
{
	//if (IsEmpty())
	//	throw std::exception("Queue is empty!");
	return tail->value;
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
}

void Queue::copyFrom(const Queue& other)
{
	Node* current = other.head;
	while (current != nullptr)
	{
		Enqueue(current->value);
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
		std::cout << current->value << ' ';
		current = current->next;
	}
	std::cout << '\n';
}


// Above are my implementations of stack and queue

// tells if personA is from Sredetz and personB is from Burevo
bool AreFromDifferentFactions(int personA, int personB);
void ParseTraveller(Stack& burevo, Queue& sredetz, int type, bool& metBurevo); // parses a traveller based on type to a given group
//int Compare2Travellers(int burevoPassenger, int sredetzPassenger);
void Challenge(Stack& burevo, Queue& sredetz);
void PassQueue(Queue& sredetz, Queue& endResult);
void reverseStack(Stack& burevo, Stack& chronologicalBurevo);
void PassStack(Stack& burevo, Queue& endResult);

int main()
{
	int travellers;
	std::cin >> travellers;
	Stack burevo;
	Queue sredetz, passedVillagers;

	bool metBurevo = false;
	int currPerson, prevPerson;
	currPerson = prevPerson = 0; // DEFAULT_INVALID_VALS
	for (int currPersonIndex = 0; currPersonIndex < travellers; currPersonIndex++)
	{
		std::cin >> currPerson;

		if (currPersonIndex != travellers - 1) // we have a potentially new sequence
		{
			if (AreFromDifferentFactions(prevPerson, currPerson))
			{
				// battle with the currentGiven
				Challenge(burevo, sredetz);
				PassQueue(sredetz, passedVillagers); // add Sredetz people only to result because they won't find anyone else to challenge - Burevo is printed at the final
			}
		}


		ParseTraveller(burevo, sredetz, currPerson, metBurevo);
		if (currPersonIndex == travellers - 1)
		{
			if (!burevo.IsEmpty() && !sredetz.IsEmpty())
			{
				Challenge(burevo, sredetz);
				// add Sredetz people only to result because they won't find anyone else to challenge - Burevo is printed at the final
				PassQueue(sredetz, passedVillagers);
			}
		}

		// corner case check for the first people before fight encounters
		if (!metBurevo)
		{
			PassQueue(sredetz, passedVillagers);
			continue; // we don't want to initialise prevPerson
		}
		prevPerson = currPerson;
	}

	// If there are still Burevo people after the last fight encounter -> they should just cross
	PassStack(burevo, passedVillagers);


	// Print Result Part: if not -> nothing will happen
	if (!passedVillagers.IsEmpty())
		passedVillagers.PrintQueue();

	return 0;
}

bool AreFromDifferentFactions(int personA, int personB)
{
	return personA < 0 && personB > 0;
}
void ParseTraveller(Stack& Burevo, Queue& Sredetz, int type, bool& metBurevo)
{
	if (type > 0)
	{
		Burevo.Push(type);
		metBurevo = true;
	}
	else if (type < 0)
		Sredetz.Enqueue(type);
}

void Challenge(Stack& burevo, Queue& sredetz)
{
	while (!burevo.IsEmpty() && !sredetz.IsEmpty())
	{
		int burevoFighter = burevo.Peek();
		int sredetzFighter = sredetz.Peek_Front();

		if (burevoFighter > abs(sredetzFighter))
			sredetz.Dequeue();
		else if (burevoFighter < abs(sredetzFighter))
			burevo.Pop();
		else // they have an equal score, remove both
		{
			sredetz.Dequeue();
			burevo.Pop();
		}
	}
}

void PassQueue(Queue& sredetz, Queue& endResult)
{
	while (!sredetz.IsEmpty())
	{
		endResult.Enqueue(sredetz.Peek_Front());
		sredetz.Dequeue();
	}
}
void reverseStack(Stack& burevo, Stack& chronologicalBurevo)
{
	while (!burevo.IsEmpty())
	{
		chronologicalBurevo.Push(burevo.Peek());
		burevo.Pop();
	}
}
void PassStack(Stack& burevo, Queue& endResult)
{
	Stack chronologicalBurevo;
	reverseStack(burevo, chronologicalBurevo); // The Burevo stack needs to be reversed in order to keep the chronological order in place
	while (!chronologicalBurevo.IsEmpty())
	{
		endResult.Enqueue(chronologicalBurevo.Peek());
		chronologicalBurevo.Pop();
	}
}
