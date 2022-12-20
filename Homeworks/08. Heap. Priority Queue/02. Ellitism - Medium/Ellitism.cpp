#include <iostream>
#include <algorithm>
#include <vector>
#include <iomanip>

struct ByMinimum
{
	bool operator()(const int& a, const int& b)
	{
		return a > b;
	}
};

void AddPersonToHeap(std::vector<int>& people, int currPerson, int sortWay);
void PopHeap(std::vector<int>& people, int sortWay);
void RegisterNewPerson(std::vector<int>& richerPeople, std::vector<int>& humblerPeople, double median, int peopleCountDiff, int currPerson);
void ReCalculateMedian(std::vector<int>& richerPeople, std::vector<int>& humblerPeople, double& median, int& peopleCountDiff);

int main()
{
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout << std::fixed << std::setprecision(1);

	int citizens;
	std::cin >> citizens;

	// The idea is to store the extremal(closest to the median) elements at the top of the heap. We will track the size of both groups as a median splits a num sequence evenly into 2 groups
	std::vector<int> richerPeople; // minHeap
	std::vector<int> humblerPeople; // maxHeap

	// Note that this solution does not use a stable sort and transfer, as we first move and then sort.
	int currPersonBudget;
	double median = 0;
	int peopleCountDiff;
	for (int i = 0; i < citizens; i++)
	{
		std::cin >> currPersonBudget;
		peopleCountDiff = (int)(richerPeople.size() - humblerPeople.size());
		RegisterNewPerson(richerPeople, humblerPeople, median, peopleCountDiff, currPersonBudget);
		ReCalculateMedian(richerPeople, humblerPeople, median, peopleCountDiff);

		std::cout << median << '\n';
	}

	return 0;
}

void AddPersonToHeap(std::vector<int>& people, int currPerson, int sortWay) // Sort == 0 is min, Sort == 1 is max
{
	people.push_back(currPerson);
	sortWay == 0 ? push_heap(people.begin(), people.end(), ByMinimum()) : push_heap(people.begin(), people.end());
}

void PopHeap(std::vector<int>& people, int sortWay)
{
	sortWay == 0 ? pop_heap(people.begin(), people.end(), ByMinimum()) : pop_heap(people.begin(), people.end());
	people.pop_back();
}

void RegisterNewPerson(std::vector<int>& richerPeople, std::vector<int>& humblerPeople, double median, int peopleCountDiff, int currPersonBudget)
{
	switch (peopleCountDiff)
	{
	case -1: // humbler are more
		if (currPersonBudget < median) // We need to add the person to the humbler, but they are more, so we will transfer the richest person of the pack to the other group
		{
			AddPersonToHeap(richerPeople, *humblerPeople.begin(), 0); // Add the extremal element of the other group and put it in the new one
			PopHeap(humblerPeople, 1); // Remove the person from the other group
			AddPersonToHeap(humblerPeople, currPersonBudget, 1); // Add the new citizen to it's corresponding group
		}
		else  // just equalize the difference in numbers
			AddPersonToHeap(richerPeople, currPersonBudget, 0);
		break;

	case 0: // Even number of groups, no transfering should be done
		if (currPersonBudget < median)
			AddPersonToHeap(humblerPeople, currPersonBudget, 1); // Richest person will stand on top
		else
			AddPersonToHeap(richerPeople, currPersonBudget, 0); // Poorest person will stand on top
		break;

	case 1: // the rich people are more
		if (currPersonBudget < median) /// just equalize the difference in numbers
			AddPersonToHeap(humblerPeople, currPersonBudget, 1);
		else // We need to add the person to the richer, but they are more, so we will transfer the poorest person of the pack to the other group
		{
			AddPersonToHeap(humblerPeople, *richerPeople.begin(), 1);
			PopHeap(richerPeople, 0);
			AddPersonToHeap(richerPeople, currPersonBudget, 0); // Add the new citizen to it's corresponding group
		}
	}
}

void ReCalculateMedian(std::vector<int>& richerPeople, std::vector<int>& humblerPeople, double& median, int& peopleCountDiff)
{
	peopleCountDiff = (int)(richerPeople.size() - humblerPeople.size()); // recalculate the number of people in order to extract a valid median
	switch (peopleCountDiff)
	{
	case -1: // humbler are more, so the median is their top
		median = *humblerPeople.begin();
		break;
	case 0:
		median = (*humblerPeople.begin() + *richerPeople.begin()) / 2.0;
		break;
	case 1:
		median = *richerPeople.begin();
	}
}