#include <iostream>
#include <exception>

template <typename T>
struct Node
{
	T data;
	Node* next;

	Node(const T& data = T(), Node* next = nullptr);

	// although we have unmanaged dynamic memory, there is no way to clear it, as it intersects, with one another
};

template <typename T>
Node<T>::Node(const T& data, Node* next)
{
	this->data = T(data);
	this->next = next;
}




template <typename T>
class DoubleLinkedList
{
public:
	DoubleLinkedList() = default;

	void push_front(const T& data);
	void push_back(const T& data);

	bool is_empty() const;

	void pop_front();
	void pop_back();

	void print() const;
	const T& peek_front() const;
	const T& peek_back() const;

	void insertAtIndex(const T& data, int position);
	const T& getAtIndex(int position);
	void removeAtIndex(int position);

	DoubleLinkedList(const DoubleLinkedList<T>& other);
	DoubleLinkedList<T>& operator=(const DoubleLinkedList<T>& other);
	~DoubleLinkedList();

private:
	Node<T>* head = nullptr;
	Node<T>* tail = nullptr;

	void free();
	void copyFrom(const DoubleLinkedList<T>& other);
};

template <typename T>
void DoubleLinkedList<T>::push_front(const T& data)
{
	// Create a new head, which next element will go to the old head(old first elem)
	Node<T>* newHead = new Node<T>(data, head);
	head = newHead; // make the head point to the new head

	if (tail == nullptr) // if it has only 1 element, it should be incremented
		tail = head;
}

template <typename T>
void DoubleLinkedList<T>::push_back(const T& data)
{
	if (is_empty())
	{
		head = new Node<T>(data);
		tail = head;
	}
	else
	{
		tail->next = new Node<T>(data);
		tail = tail->next;
	}
}

template <typename T>
bool DoubleLinkedList<T>::is_empty() const
{
	return head == nullptr;
}

template <typename T>
void DoubleLinkedList<T>::pop_front()
{
	//if (is_empty())
	//	throw std::exception("Invalid operation, list it empty!");

	Node<T>* oldHead = head;
	if (tail == head) // Corner check for lenght 1, a.k.a head == tail. It makes no difference if we do the check now, or after we change the current head pos
		tail = nullptr;
	head = head->next; // We move the current head onto the next element, while deleting the old one
	delete oldHead;
}

template <typename T>
void DoubleLinkedList<T>::pop_back()
{
	//if (is_empty()) // head or tail, it doesn't matter, it means we have an empty list
	//	throw std::exception("Invalid operation, list it empty!");

	if (head == tail && head != nullptr) // only 1 element is present
	{
		delete head;
		head = tail = nullptr;
		return;
	}


	Node<T>* previousElem = head;
	// we want to stop before tail (or in corner case of just 2 elements, stop before the nullptr)
	while (previousElem->next != tail && previousElem->next != nullptr)
		previousElem = previousElem->next;

	delete tail;
	previousElem->next = nullptr;
	tail = previousElem;
}

template <typename T>
void DoubleLinkedList<T>::print() const
{
	Node<T>* currElem = head;
	//if (is_empty())
	//	throw std::exception("Invalid operation, list it empty!");

	while (currElem != nullptr)
	{
		std::cout << currElem->data << ' ';
		currElem = currElem->next;
	}
	std::cout << '\n';
}


template <typename T>
const T& DoubleLinkedList<T>::peek_front() const
{
	//if (is_empty())
	//	throw std::exception("Invalid operation, list it empty!");
	
	return head->data;
}

template <typename T>
const T& DoubleLinkedList<T>::peek_back() const
{
	//if (is_empty())
	//	throw std::exception("Invalid operation, list it empty!");

	return tail->data;
}

template <typename T>
void DoubleLinkedList<T>::insertAtIndex(const T& data, int position)
{
	if (position == 0)
	{
		push_front(data);
		return;
	}

	//if (is_empty()) // the list is empty but we want to add element at different pos
	//	throw std::exception("Invalid operation, list it empty!");

	Node<T>* curr = head;
	Node<T>* previous = nullptr;

	// find the correct position of the Node using 2 pointers
	for (int i = 0; i < position; i++)
	{
		//if (curr == nullptr)
		//	throw std::exception("Invalid operation, index is greater than max elements!");

		previous = curr;
		curr = curr->next;
	}

	// insert the Node at the correct position
	previous->next = new Node<T>(data, curr);

	if (tail == previous) // we have added the item at the previous last element
		tail = previous->next; // or is = curr;

	/*curr = new Node(data, curr->next);    does the same thing as the code above
	previous->next = curr;*/
}

template <typename T>
const T& DoubleLinkedList<T>::getAtIndex(int position)
{
	//if (is_empty())
	//	throw std::exception("Invalid operation, list it empty!");

	Node<T>* current = head;
	for (int i = 0; i < position; i++)
	{
		if (current->next == nullptr)
			throw std::exception("Invalid operation, index is greater than max elements!");
		else
			current = current->next;
	}

	return current->data;
}

template <typename T>
void DoubleLinkedList<T>::removeAtIndex(int position)
{
	//if (is_empty()) // the list is empty but we want to remove an element from it
	//	throw std::exception("Invalid operation, list it empty!");

	if (position == 0)
	{
		pop_front();
		return;
	}

	Node<T>* current = head;
	Node<T>* previous = head;


	for (int i = 0; i < position; i++)
	{
		//if (current->next == nullptr)
		//	throw std::exception("Invalid operation, index is too great!");

		previous = current;
		current = current->next; // The implemented idea intends to skip the node and just bind the previous with the current->next
	}

	previous->next = current->next;
	if (tail == current)
		tail = previous;
	delete current;
}

template <typename T>
void DoubleLinkedList<T>::free()
{
	Node<T>* current = head;
	while (current != nullptr)
	{
		Node<T>* next = current->next;
		delete current;
		current = next;
	}

	head = tail = nullptr;
}

template <typename T>
void DoubleLinkedList<T>::copyFrom(const DoubleLinkedList<T>& other)
{
	Node<T>* current = other.head;
	while (current != nullptr)
	{
		push_back(current->data); // pushback keeps track of head and tail
		current = current->next;
	}
}

template <typename T>
DoubleLinkedList<T>::DoubleLinkedList(const DoubleLinkedList<T>& other)
{
	copyFrom(other);
}

template <typename T>
DoubleLinkedList<T>& DoubleLinkedList<T>::operator=(const DoubleLinkedList<T>& other)
{
	if (this != &other)
	{
		free();
		copyFrom(other);
	}
	return *this;
}

template <typename T>
DoubleLinkedList<T>::~DoubleLinkedList()
{
	free();
}

// default linked list code upwards

// functions related to task

void FillArr(int* arr, int totalPeople);

void RemoveUninterestingElements(const int* citizens, int currPersonIndex, DoubleLinkedList<int>& traversor);
long long FindLazinessSum(const int* citizens, int villagersNum, int groupCount);

int main()
{
	int villagersNum , groupSize;
	std::cin >> villagersNum >> groupSize;
	int* citizens = new int[villagersNum];
	FillArr(citizens, villagersNum);

	long long lazinessSum = FindLazinessSum(citizens, villagersNum, groupSize);
	std::cout << lazinessSum;

	delete[] citizens;
}

void FillArr(int* arr, int totalPeople)
{
	int currPersonLaziness;
	for (int i = 0; i < totalPeople; i++)
	{
		std::cin >> currPersonLaziness;
		arr[i] = currPersonLaziness;
	}
}

void RemoveUninterestingElements(const int* citizens, int currPersonIndex, DoubleLinkedList<int>& traversor)
{
	//We want to have valid comparisons. If the element in the array is a new 'extremal' (we want a minimum), 
	// remove the old elements so the new extremal will get to the front of the linked list
	// REMEMBER! - traversor keeps the indexes of extremal nums in the original array and not the initial values
	while (!traversor.is_empty() && citizens[currPersonIndex] <= citizens[traversor.peek_back()])
		traversor.pop_back();
}

long long FindLazinessSum(const int* citizens, int villagersNum, int groupCount)
{
	DoubleLinkedList<int> traversor; // will track indexes of extremal elements. If new element which is sooner and with extremal index is found, the old one would be replaced

	int currIndex = 0;
	long long lazinessSum = 0;
	while (currIndex < groupCount) // we will simulate a do-while cycle, thus meaning this first cycle will search only the first group with size groupCount
	{
		RemoveUninterestingElements(citizens, currIndex, traversor);

		traversor.push_back(currIndex); // if the element is not the searched extremal(minimum), push it to the back to fill the needed window quantity
		currIndex++;
	}

	int laziestPersonScore;
	while (currIndex < villagersNum) 
	{
		laziestPersonScore = citizens[traversor.peek_front()];
		lazinessSum += laziestPersonScore; // since traversors holds our searched extremal index at the front of the linked list, add it to the sum and continue

		while (!traversor.is_empty() && traversor.peek_front() <= currIndex - groupCount) // trim the group to the current searched window
			traversor.pop_front(); // we remove all, no matter extremal or not, thus we remove from the beginning because the elements are chronologically placed

		//Repeat the cycle of elimination of uninteresting values like the first big while cylce.
		RemoveUninterestingElements(citizens, currIndex, traversor);

		traversor.push_back(currIndex); // if the element is not the searched extremal(minimum), push it to the back to fill the needed window quantity
		currIndex++;
	}
	
	laziestPersonScore = citizens[traversor.peek_front()]; // add the last element to fulfil the throughout checking
	lazinessSum += laziestPersonScore;

	return lazinessSum;
}