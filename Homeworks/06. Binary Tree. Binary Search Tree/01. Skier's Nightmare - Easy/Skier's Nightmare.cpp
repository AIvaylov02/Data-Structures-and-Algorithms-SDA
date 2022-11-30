#include <iostream>

const int DEFAULT_INVALID_VAL = -1;
struct Node
{
	int damageDone = DEFAULT_INVALID_VAL;
	int xCoordinate = DEFAULT_INVALID_VAL;
	Node* leftChild = nullptr;
	Node* rightChild = nullptr;
};

void FillArray(Node* checkPoints, int checkPointsCount);
void CalculateSums(Node* current, long long* skiTrackScores, const int startMidIndex);
void PrintSkiTrackScores(const long long* skiTrackScores, const int SKI_TRACK_SIZE);

int main()
{
	int checkPointsCount;
	std::cin >> checkPointsCount;
	Node* checkPoints = new Node[checkPointsCount]; // create an array for the tree and fill it with values in the next function
	checkPoints[0].xCoordinate = 0;
	FillArray(checkPoints, checkPointsCount);

	const int SKI_TRACK_SIZE = checkPointsCount * 2 - 1;
	long long* skiTrackScores = new long long[checkPointsCount * 2 - 1]{0}; // N*2 - 1, worst case will be to have a list sprung out in one direction, so we will have to be ready
	CalculateSums(&checkPoints[0], skiTrackScores, checkPointsCount - 1); // Mid is always N-1
	PrintSkiTrackScores(skiTrackScores, SKI_TRACK_SIZE);

	delete[] skiTrackScores;
	delete[] checkPoints;
}

// Creates a valid tree, which we could traverse recursively by using its root at position 0. Worth to mention that because every Node has a valid constructor, we could give each
// Node a valid xCoordinate when creating it
void FillArray(Node* checkPoints, int checkPointsCount)
{
	int value, leftChild, rightChild;
	for (int i = 0; i < checkPointsCount; i++)
	{
		std::cin >> value >> leftChild >> rightChild;
		checkPoints[i].damageDone = value;
		if (leftChild != -1) // the childs are nullptr by default, but they are created!, thus we could give them an xCoordinate
		{
			if (leftChild < checkPointsCount) // because there are no X constrains in the task I want to think that this breaks the algorithm
			{
				checkPoints[i].leftChild = &checkPoints[leftChild];
				checkPoints[leftChild].xCoordinate = checkPoints[i].xCoordinate - 1;
			}
		}
		if (rightChild != -1)
		{
			if (rightChild < checkPointsCount)
			{
				checkPoints[i].rightChild = &checkPoints[rightChild];
				checkPoints[rightChild].xCoordinate = checkPoints[i].xCoordinate + 1;
			}
		}
	}
}

void CalculateSums(Node* current, long long* skiTrackScores, const int startMidIndex)
{
	// It makes no difference how we would traverse the tree, we should just keep track of xCoords and damageDone
	if (current != nullptr) // PreOrder implementation -> Root->Left Tree->Right Tree
	{
		int putPosition = startMidIndex + current->xCoordinate; // we need to offset it as it will start at the mid, as the root is "hypothetically" (not when the tree is list)
		// always in the middle, so (0,0) cannot be the start of the array with valid scores, it should be moved generally to the right. We add, because the minus is taken care of
		// by the -x coords, otherwise the list will be reversed at the end, which we don't generally want to traverse backwards
		skiTrackScores[putPosition] += current->damageDone;
		CalculateSums(current->leftChild, skiTrackScores, startMidIndex);
		CalculateSums(current->rightChild, skiTrackScores, startMidIndex);
	}
}

void PrintSkiTrackScores(const long long* skiTrackScores, const int SKI_TRACK_SIZE)
{
	int currIndex = 0;
	while (skiTrackScores[currIndex] == 0) // skip the first 'buffer' empty tracks
		currIndex++;
	while (currIndex < SKI_TRACK_SIZE && skiTrackScores[currIndex] != 0) // only print the valid ski tracks, skip the empty ones after them
	{
		std::cout << skiTrackScores[currIndex] << ' ';
		currIndex++;
	}
}