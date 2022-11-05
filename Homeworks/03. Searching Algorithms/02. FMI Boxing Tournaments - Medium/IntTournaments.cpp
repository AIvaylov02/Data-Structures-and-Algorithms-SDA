#include <iostream>
#include <vector>
#include <algorithm>

// This is the correct solution to the task. Although it doesn't involve doubles, hackerrank loves it. Ohh boy, here we go again...

void FillVector(std::vector<int>& weights, int students);

void MergeSort(std::vector<int>& arr, int startIndex, int endIndex);
void MergeArrays(std::vector<int>& arr, int startIndex, int midIndex, int endIndex);

bool ValidateLimits(int& lower, int& upper);

void PrintBoxersCount(std::vector<int>& weights, int students, const int tournaments);

int main()
{
	// I gave up on writing my own implementation :( ... will use the std::iterator of vector

	std::ios_base::sync_with_stdio(false);
	std::cin.tie(NULL);

	int students, tournaments;
	std::cin >> students >> tournaments;
	std::vector<int> weights;
	FillVector(weights, students);
	// we have to sort weights -> mergeSort seems fine, then we will use index arithmetic
	MergeSort(weights, 0, students - 1);

	PrintBoxersCount(weights, students, tournaments);
}

void FillVector(std::vector<int>& weights, int students)
{
	int currWeight;
	for (int i = 0; i < students; i++)
	{
		std::cin >> currWeight;
		weights.push_back(currWeight);
	}
}

void MergeSort(std::vector<int>& arr, int startIndex, int endIndex)
{
	if (startIndex >= endIndex)
		return;

	int midIndex = (endIndex - startIndex) / 2 + startIndex;
	MergeSort(arr, startIndex, midIndex);
	MergeSort(arr, midIndex + 1, endIndex);
	MergeArrays(arr, startIndex, midIndex, endIndex);
}

void MergeArrays(std::vector<int>& arr, int startIndex, int midIndex, int endIndex)
{
	const int LEFT_SIZE = midIndex - startIndex + 1;
	const int RIGHT_SIZE = endIndex - midIndex;

	std::vector<int>leftArr;
	std::vector<int>rightArr;

	for (int i = 0; i < LEFT_SIZE; i++)
		leftArr.push_back(arr[startIndex + i]);
	for (int i = 0; i < RIGHT_SIZE; i++)
		rightArr.push_back(arr[midIndex + 1 + i]);

	int leftIndex, rightIndex;
	leftIndex = rightIndex = 0;

	while (leftIndex < LEFT_SIZE && rightIndex < RIGHT_SIZE)
	{
		if (rightArr[rightIndex] < leftArr[leftIndex])
			arr[startIndex++] = rightArr[rightIndex++];
		else // leftArr element is less or equal to right -> we want to keep stability
			arr[startIndex++] = leftArr[leftIndex++];
	}

	while (leftIndex < LEFT_SIZE)
		arr[startIndex++] = leftArr[leftIndex++];
	while (rightIndex < RIGHT_SIZE)
		arr[startIndex++] = rightArr[rightIndex++];
}

bool ValidateLimits(int& lower, int& upper)
{
	if (upper < lower)
	{
		std::cout << 0 << "\n";
		return false;
	}
	return true;
}

void PrintBoxersCount(std::vector<int>& weights, int students, const int tournaments)
{
	int minKilos, maxKilos;
	for (int currMatch = 0; currMatch < tournaments; currMatch++)
	{
		std::cin >> minKilos >> maxKilos;
		if (!ValidateLimits(minKilos, maxKilos)) // Maybe we shouldn't try to fix the interval but just check it. If it is not valid, we should continue onto the next category
			continue;

		std::vector<int>::iterator startBound = std::lower_bound(weights.begin(), weights.end(), minKilos);
		int currRoundStartIndex = startBound - weights.begin(); // in order to convert it from iterator to integer we do this arithmetic
		std::vector<int>::iterator endBound = std::upper_bound(weights.begin(), weights.end(), maxKilos);
		int currRoundEndIndex = endBound - weights.begin();

		std::cout << currRoundEndIndex - currRoundStartIndex << "\n"; // we don't add additional 1-s in these solution as the stl methods take care of the indexation problem
	}
}