#include <iostream>

// - One approach is to create class Pair with currNum and count and to simulate something like counting sort
// - Another, cleverer approach is to create an array of max n int values, put there only numbers which are inside our given list and are possible contenders(are smaller than N,
// because we want the minimum (aka if something is 2'411'231'233 we will never really reach it, and so it won't matter) and are not negative)
int FindIvanchosTicket(const int* ticketsList, int ticketsCount);

int main()
{
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(NULL);

	int tickets;
	std::cin >> tickets;
	
	// allocate memory and enter the tickets in the list. ! They are in unsorted order -> Binary Search is not possible
	int* ticketsList = new int[tickets];
	for (int currTicket = 0; currTicket < tickets; currTicket++)
		std::cin >> ticketsList[currTicket];

	std::cout << FindIvanchosTicket(ticketsList, tickets);

	delete[] ticketsList;
	return 0;
}

int FindIvanchosTicket(const int* ticketsList, int ticketsCount)
{
	int* validTickets = new int[ticketsCount];
	for (int i = 0; i < ticketsCount; i++)
		validTickets[i] = 0;

	// determine our valid tickets array
	int currNum;
	for (int i = 0; i < ticketsCount; i++)
	{
		currNum = ticketsList[i];
		if (currNum < 1) // ticket is negative or null
			continue;
		if (currNum > ticketsCount) // curr ticket N is much greater than Ivancho's possible ticket
			continue;

		validTickets[currNum-1]++; // we compensate for the number to index transition as explained in the below comment but in reverse order
	}

	int ivanchosTicket = 1;
	bool allPosAreFilled = true; // worst corner check to see if every possible number has been met in our valid array (if so Ivancho's is the next ticket)
	for (int i = 0; i < ticketsCount; i++)
	{
		if (validTickets[i] == 0) // there is a valid value for ivancho's ticket in our array;
		{
			allPosAreFilled = false;
			ivanchosTicket = i + 1; // we add + 1 to compensate for the array starting from index 0 and going to N-1 instead from 1 to N
			break;
		}
	}

	if (allPosAreFilled)
		ivanchosTicket = ticketsCount + 1;

	delete[] validTickets;
	return ivanchosTicket;
}

// N.Log(N) is worst allowed algorithm complexity in this task