#include <iostream>
#include <utility>
#include <map>


int main()
{
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(NULL);


	int numbersCount;
	std::cin >> numbersCount;
	int* nums = new int[numbersCount];

	int currNum;
	for (int i = 0; i < numbersCount; i++)
	{
		std::cin >> currNum;
		nums[i] = currNum;
	}

	std::map<int, int> numContainer; // it will store the value as key and the index in the array as value
	int currArrSize, maxArrSize;
	currArrSize = maxArrSize = 0;
	int startOfSubArrayIndex;

	currNum = 0;
	while (currNum < numbersCount)
	{
		if (numContainer.empty())
			startOfSubArrayIndex = currNum;

		if (numContainer.find(nums[currNum]) == numContainer.end()) // the number is not present so we add it to the subarray
		{
			numContainer.insert(std::pair<int, int>(nums[currNum], currNum));
			currArrSize++;
		}
		else // the currNum is present in the subarray so we have to move and delete everything before it
		{
			if (currArrSize > maxArrSize)
				maxArrSize = currArrSize;


			int deleteUpTo = (*numContainer.find(nums[currNum])).second + 1; // find the lower bound of where to delete, a.k.a all elements before it should be gone
			while (startOfSubArrayIndex < deleteUpTo)
			{
				numContainer.erase(nums[startOfSubArrayIndex]);
				currArrSize--;
				startOfSubArrayIndex++;
			} // delete up to must be equal to startofSubarrayIndex
			numContainer.insert(std::pair<int, int>(nums[currNum], currNum));
			currArrSize++;
		}

		currNum++;
	}

	// corner case check for the last sequence to be the largest possible one
	if (currArrSize > maxArrSize)
		maxArrSize = currArrSize;

	std::cout << maxArrSize << '\n';
	delete[] nums;
}