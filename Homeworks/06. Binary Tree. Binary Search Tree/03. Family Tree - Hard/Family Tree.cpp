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

    int operator[](int index) const;
    int getCount() const;

    void PrintQueue() const;

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
    count++;
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
    //    throw std::exception("Queue is empty!");
    return tail->value;
}

void Queue::Dequeue()
{
    //if (IsEmpty())
    //    throw std::exception("Queue is empty!");

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
        Enqueue(current->value);
        current = current->next;
    }
    count = other.count;
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

int Queue::operator[](int index) const
{
    if (IsEmpty())
        return -1;

    Node* current = head;
    for (int i = 0; i < index; i++)
    {
        if (current == nullptr)
            return -1;
        current = current->next;
    }
    return current->value;
}

int Queue::getCount() const
{
    return count;
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


// TASK SOLUTION STARTS HERE!
// Above is my implementation of Queue, nothing interesting

const int DEFAULT_INVALID_VAL = 0;
struct PersonInfo
{
    Queue children; // holds the info of this person children indexes in the grand array
	int childrenCount = DEFAULT_INVALID_VAL;
	int entryTimePoint = DEFAULT_INVALID_VAL; // tells at which point we have entered through this node to traverse it's subtree
	int exitTimePoint = DEFAULT_INVALID_VAL; // tells at which point we have exited through this node, thus successfully having traversed it's subtree
};

void FillArray(PersonInfo* familyTree, int familyTreeSize, int& oldestKid); // builds the whole tree correcly

//Given an already built tree, adds the correct time to each Node, works recursively by traversing each and every Node in all directions
void AddTimers(int& currentTime, PersonInfo* familyTree, const int familyTreeSize, int child);

bool AreRelated(int ancestor, int child, const PersonInfo* familyTree); // checks if one node is ancestor to another
void PrintResult(bool DNA_Test);
void CompleteDNARequests(int requestsCount, const PersonInfo* familyTree, const int familyTreeSize); // uses are related and swaps it to check both ways if necessary

void printTreeTimers(PersonInfo* familyTree, int startPoint); // prints them postorderly

int main()
{
	std::ios::sync_with_stdio(false);
	std::cin.tie(NULL);


	int familyTreeSize;
	std::cin >> familyTreeSize;
	PersonInfo* familyTree = nullptr;
	if (familyTreeSize != 0)
	{
		familyTree = new PersonInfo[familyTreeSize];
		int startTime, child;
		startTime = DEFAULT_INVALID_VAL;
		child = -1;
		FillArray(familyTree, familyTreeSize, child); // corrects the tree starting pos as well as fill the tree
		AddTimers(startTime, familyTree, familyTreeSize, child);
		//printTreeTimers(familyTree, child); //used for debugging
	}


	int requests;
	std::cin >> requests;
	CompleteDNARequests(requests, familyTree, familyTreeSize);

	delete[] familyTree;
	return 0;
}

void FillArray(PersonInfo* familyTree, int familyTreeSize, int& oldestKid)
{
	int parent, child;
	for (int i = 0; i < familyTreeSize - 1; i++) // parent must be 0 if familyTreeSize = 1
	{
		std::cin >> parent >> child;
		if (i == 0)
			oldestKid = parent; // this sets are our starting position in the tree afterwards
        familyTree[parent].children.Enqueue(child);
		familyTree[parent].childrenCount++;
	}
}

void AddTimers(int& currentTime, PersonInfo* familyTree, const int familyTreeSize, int child)
{
	if (child != -1) // no such child exists
	{
		familyTree[child].entryTimePoint = currentTime;
		currentTime++;

		for (int currChild = 0; currChild < familyTree[child].childrenCount; currChild++)
			AddTimers(currentTime, familyTree, familyTreeSize, familyTree[child].children[currChild]); // adds timers to all children subtrees if such exist


		familyTree[child].exitTimePoint = currentTime;
		currentTime++;
	}
}

bool AreRelated(int ancestor, int child, const PersonInfo* familyTree)
{
	return familyTree[ancestor].entryTimePoint < familyTree[child].entryTimePoint&&
		familyTree[ancestor].exitTimePoint > familyTree[child].exitTimePoint;
}

void PrintResult(bool DNA_Test)
{
	if (DNA_Test)
		std::cout << "YES\n";
	else
		std::cout << "NO\n";
}

void CompleteDNARequests(int requestsCount, const PersonInfo* familyTree, const int familyTreeSize)
{
	int potentialParent, potentialSuccessor;
	for (int i = 0; i < requestsCount; i++)
	{
		std::cin >> potentialParent >> potentialSuccessor;
		if (familyTreeSize <= 1)
			PrintResult(false);
		else if (potentialParent >= familyTreeSize || potentialSuccessor >= familyTreeSize) // invalid indexes given
			PrintResult(false);
		else if (AreRelated(potentialParent, potentialSuccessor, familyTree))
			PrintResult(true);
		else // are not related
			PrintResult(false);
	}
}

void printTreeTimers(PersonInfo* familyTree, int startPoint)
{
	if (startPoint != -1)
	{
        for (int i = 0; i < familyTree[startPoint].childrenCount; i++)
            printTreeTimers(familyTree, familyTree[startPoint].children[i]);
        std::cout << "Node: " << startPoint << " with start time: " << familyTree[startPoint].entryTimePoint << " and end time: " << familyTree[startPoint].exitTimePoint << '\n';
	}
}