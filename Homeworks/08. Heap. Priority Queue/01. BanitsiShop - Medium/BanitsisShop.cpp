#include <iostream>
#include <vector>
#include <algorithm>

struct Order
{
	Order(int start, int duration, int index);
	int getStartTime() const;
	int getDuration() const;
	int getEntryIndex() const;

private:
	int startAt;
	int timeForCompletion;
	int entryIndex;
};

// Comparator functions for the method of the sorting used by the Heap_push and Heap_pop methods
bool ByFirstTime(const Order& first, const Order& second);
bool ByShorterExecution(const Order& first, const Order& second);

void FillArray(std::vector<Order>& orders, int ordersCount);

void AddOrdersForCompletion(std::vector<Order>& orders, int currTime, std::vector<Order>& takenUnderway);
void CompleteOrder(int& currTime, std::vector<Order>& takenUnderway, const std::vector<Order>& orders);
void CompleteDayshift(std::vector<Order>& orders, int& currTime, std::vector<Order>& takenUnderway);



int main()
{
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(NULL);

	int ordersCount;
	std::cin >> ordersCount;

	std::vector<Order> orders; // will helps us sort the given raw orders into giveable portions into takenUnderway (Min Heap by using FirstTime, a.k.a Entry Time)
	FillArray(orders, ordersCount); // to heap insert using entryIndex

	std::vector<Order> takenUnderway; // holds information about the shortest orders available (Min Heap by using Shorter execution, a.k.a time for completion)
	int currTime = 0;
	CompleteDayshift(orders, currTime, takenUnderway);
	return 0;
}



Order::Order(int start, int duration, int index)
{
	startAt = start;
	timeForCompletion = duration;
	entryIndex = index;
}


int Order::getStartTime() const
{
	return startAt;
}

int Order::getDuration() const
{
	return timeForCompletion;
}

int Order::getEntryIndex() const
{
	return entryIndex;
}



void FillArray(std::vector<Order>& orders, int ordersCount)
{
	int banitsaTimeAt, banitsaTimeNeeded;
	for (int index = 0; index < ordersCount; index++)
	{
		std::cin >> banitsaTimeAt >> banitsaTimeNeeded;
		orders.push_back(Order(banitsaTimeAt, banitsaTimeNeeded, index));
	}
	stable_sort(orders.begin(), orders.end(), &ByFirstTime); // Sort the array at last as it should be faster. Back element is the smallest one
}

bool ByFirstTime(const Order& first, const Order& second)
{
	return first.getStartTime() > second.getStartTime();
}

bool ByShorterExecution(const Order& first, const Order& second)
{
	int firstTime = first.getDuration();
	int secondTime = second.getDuration();
	if (firstTime == secondTime)
		return first.getEntryIndex() > second.getEntryIndex();
	else
		return first.getDuration() > second.getDuration();
}


void AddOrdersForCompletion(std::vector<Order>& orders, int currTime, std::vector<Order>& takenUnderway)
{
	while (!orders.empty() && orders.back().getStartTime() <= currTime)
	{
		takenUnderway.push_back(orders.back());
		push_heap(takenUnderway.begin(), takenUnderway.end(), ByShorterExecution); // put and sort the first order to the designated work area
		orders.pop_back();  // remove it from the waiting queue of the entry point
	}
}
void CompleteOrder(int& currTime, std::vector<Order>& takenUnderway, const std::vector<Order>& orders)
{
	if (!takenUnderway.empty()) // if there have been added valid orders to the workshop already
	{
		std::cout << takenUnderway.front().getEntryIndex() << " ";
		currTime += takenUnderway.front().getDuration();
		pop_heap(takenUnderway.begin(), takenUnderway.end(), ByShorterExecution);
		takenUnderway.pop_back();
	}
	else // no orders at this point of time
	{
		if (!orders.empty())
			currTime = orders.back().getStartTime();
		else
			currTime++;
	}
}

void CompleteDayshift(std::vector<Order>& orders, int& currTime, std::vector<Order>& takenUnderway)
{
	while (!orders.empty()) // we traverse until there are no waiting orders
	{
		AddOrdersForCompletion(orders, currTime, takenUnderway);
		CompleteOrder(currTime, takenUnderway, orders);
	}

	while (!takenUnderway.empty()) // while we have no uncompleted orders
		CompleteOrder(currTime, takenUnderway, orders);
}
