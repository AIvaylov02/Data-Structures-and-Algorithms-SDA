#include <iostream>
#include <queue>
#include <vector>


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


// Above is my implementations queue


struct ByMinimum // default comparator wouldn't work as priorityqueue is not template class, moreover it is an instantiable class
{
	bool operator()(const int& a, const int& b)
	{
		return a > b;
	}
};

void FillVector(int repairsCount, std::vector<int>& input);
void DoRepairs(int iterRepairs, int groupSize, int batchesPlanned, Queue& repairs, std::vector<int>& input);

int main()
{
	std::cin.tie(NULL);
	std::ios_base::sync_with_stdio(false);

	int currIterationRepairs, queriesGroupSize, maxGroupsExecuted;
	std::cin >> currIterationRepairs >> queriesGroupSize >> maxGroupsExecuted;
	int repairsCount;
	std::cin >> repairsCount;

	std::vector<int> orders = std::vector<int>(repairsCount);
	FillVector(repairsCount, orders);

	Queue doneOrders;
	DoRepairs(currIterationRepairs, queriesGroupSize, maxGroupsExecuted, doneOrders, orders);
	doneOrders.PrintQueue();
	return 0;
}

void FillVector(int repairsCount, std::vector<int>& input)
{
	int currOrder;
	for (int i = 0; i < repairsCount; i++)
	{
		std::cin >> currOrder;
		input[i] = currOrder;
	}
}

void DoRepairs(int iterRepairs, int groupSize, int batchesPlanned, Queue& repairs, std::vector<int>& input)
{
	std::priority_queue<int, std::vector<int>, ByMinimum> toBeRepaired; // we have a min heap, just remove n times the top, when we process an order
	int currIndex, doneRepairs;
	currIndex = doneRepairs = 0;

	bool shortGroup = false;
	while (currIndex < input.size() && doneRepairs < batchesPlanned)
	{
		for (int i = 0; i < groupSize; i++) // insert the next group of elements.
		{
			if (currIndex == input.size())
			{
				shortGroup = true;
				break;
			}

			toBeRepaired.push(input[currIndex]);
			currIndex++;
		}

		if (shortGroup) // we have reached the end of the shift
			break;

		for (int i = 0; i < iterRepairs; i++) // add the smallest numbers to the result and remove them from the Heap
		{
			repairs.Enqueue(toBeRepaired.top());
			toBeRepaired.pop();
		}
		doneRepairs++; // we have processed a batch of products

	}

	int queriesDone = doneRepairs * iterRepairs;
	while (queriesDone < batchesPlanned * iterRepairs && queriesDone < currIndex) // fulfill maintenances until the day plan has been completed
	{
		repairs.Enqueue(toBeRepaired.top());
		toBeRepaired.pop();
		queriesDone++;
	}
}