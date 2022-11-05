#include <iostream>

long long MyPow(int base, int power);

// A slightly modified ternary search to suit our task needs
int CalculateFriendsCandy(long long startIndex, long long endIndex, long long searchedPosition);

void PrintNeededCandy(long long startIndex, long long totalFriendsIndex, int invitedFriends);


// We shoudn't keep the candies count in array, we should calculate based on given position, because we don't want to calculate for each person, but only for the invited friends
int main()
{
	int power, invitedFriends;
	std::cin >> power >> invitedFriends;
	long long friends = MyPow(3, power);
	PrintNeededCandy(0, friends - 1, invitedFriends);

	return 0;
}

long long MyPow(int base, int power)
{
	long long result = 1;
	while (power >= 1)
	{
		result *= base;
		power--;
	}
	return result;
}

int CalculateFriendsCandy(long long startIndex, long long endIndex, long long searchedPosition)
{
	int candies = 1; // candy is int because they are approximately the length of recursion/ count of iteration cycles

	while (startIndex < endIndex)
	{
		if (endIndex - startIndex < 3) // we have a minimum interval
		{
			if (searchedPosition == startIndex || searchedPosition == endIndex) // if it is on one of the end positions;
			{
				candies++;
				break;
			}
			else
				break; // if it is in the middle of the 3 index interval a.k.a [0,1,2] and we want pos 1
		}

		long long midLeftIndex = (endIndex - startIndex) / 3 + startIndex;
		long long midRightIndex = endIndex - (endIndex - startIndex) / 3;

		// it is in the middle, the recursion/iteration(in my implementation) will not continue as we already know the count
		if (midLeftIndex < searchedPosition && midRightIndex > searchedPosition)
			break;

		// we don't have == midIndex in our solution, we should add it to the breakPointers like this one, in order to make a new interval
		// A new interval means a new iteration, so the candies count will go up
		if (midLeftIndex >= searchedPosition)
		{
			candies++;
			endIndex = midLeftIndex;
		}
		else if (midRightIndex <= searchedPosition)
		{
			candies++;
			startIndex = midRightIndex;
		}
	}

	// instead of using returns in the cycle, I had to put the return at the bottom, because the programme doesn't know that I will 100% of the time reach a return statement in the cycle
	return candies; 
}

void PrintNeededCandy(long long startIndex, long long totalFriendsIndex, int invitedFriends)
{
	long long currFriendNumber;
	for (long long i = 0; i < invitedFriends; i++)
	{
		std::cin >> currFriendNumber;
		std::cout << CalculateFriendsCandy(startIndex, totalFriendsIndex, currFriendNumber - 1) << "\n";
	}
}