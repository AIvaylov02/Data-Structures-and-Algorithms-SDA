#include <iostream>

const int INVALID_INDEX = -1;

struct Section
{
	Section(int count, int originalIndex);

	void setSortedIndex(int newIndex);
	int getSortedIndex() const;
	int getOriginalIndex() const;

	bool IsSmaller(const Section& other, int comparisonVariant) const; // alternative way will be to represent this in an outside class function which compares two sections
private:
	int count;
	int originalIndex;
	int sortedIndex;
};

void fillCollection(Section** sections, int size);
void freeCollection(Section** sections, int size);

void MergeSort(Section** sections, int startIndex, int endIndex, int regime);
void MergeArrays(Section** sections, int startIndex, int midIndex, int endIndex, int regime);

void setSortedIndexes(Section** sections, int size);
void Swap(Section* left, Section* right);
//void revertElemsToOriginalsPos(Section** sections, int size);

void PrintNewIndexes(const Section* const* sections, int size);

int main()
{
	int sectionsCount;
	std::cin >> sectionsCount;

	Section** sections = new Section * [sectionsCount];
	fillCollection(sections, sectionsCount);

	MergeSort(sections, 0, sectionsCount - 1, 0);
	
	setSortedIndexes(sections, sectionsCount);
	// one way is to use merge sort with condition(a.k.a our comparator will be an extra parameter)
	// an alternative better way is to just linearly plug the array in the right order as we already know each's element position -> this breaks stability
	MergeSort(sections, 0, sectionsCount - 1, 1);

	//revertElemsToOriginalsPos(sections, sectionsCount); -> doesn't work

	PrintNewIndexes(sections, sectionsCount);
	freeCollection(sections, sectionsCount);
	return 0;
}

Section::Section(int count, int originalIndex)
{
	this->count = count;
	this->originalIndex = originalIndex;
	sortedIndex = INVALID_INDEX;
}

void Section::setSortedIndex(int newIndex)
{
	sortedIndex = newIndex;
}

int Section::getSortedIndex() const
{
	return sortedIndex;
}

int Section::getOriginalIndex() const
{
	return originalIndex;
}

bool Section::IsSmaller(const Section& other, int comparisonVariant) const
{
	switch (comparisonVariant)
	{
	case 0: // compare by count
		return count <= other.count;
	case 1: // compare by original Index
		return originalIndex < other.originalIndex;
	default:
		throw std::logic_error("Invalid comparison scale tried!");
	}
}

void fillCollection(Section** sections, int size)
{
		int currSecVotes;
	for (int i = 0; i < size; i++)
	{
		std::cin >> currSecVotes;
		sections[i] = new Section(currSecVotes, i);
	}
}

void freeCollection(Section** sections, int size)
{
	for (int i = 0; i < size; i++)
		delete sections[i];
	delete[] sections;
}

void MergeSort(Section** sections, int startIndex, int endIndex, int regime)
{
	if (startIndex >= endIndex)
		return;

	int midIndex = startIndex + (endIndex - startIndex) / 2;

	MergeSort(sections, startIndex, midIndex, regime);
	MergeSort(sections, midIndex + 1, endIndex, regime);
	MergeArrays(sections, startIndex, midIndex, endIndex, regime);
}

void MergeArrays(Section** sections, int startIndex, int midIndex, int endIndex, int regime)
{
	const int LEFT_SIZE = midIndex - startIndex + 1;
	const int RIGHT_SIZE = endIndex - midIndex;

	Section** leftArr = new Section * [LEFT_SIZE];
	Section** rightArr = new Section * [RIGHT_SIZE];

	for (int i = 0; i < LEFT_SIZE; i++)
		leftArr[i] = new Section(*sections[i+startIndex]);
	for (int i = 0; i < RIGHT_SIZE; i++)
		rightArr[i] = new Section(*sections[i+midIndex+1]);

	int leftIndex, rightIndex;
	leftIndex = rightIndex = 0;

	while (leftIndex < LEFT_SIZE && rightIndex < RIGHT_SIZE)
	{
		if (leftArr[leftIndex]->IsSmaller(*rightArr[rightIndex], regime))
			*sections[startIndex++] = *leftArr[leftIndex++];
		else
			*sections[startIndex++] = *rightArr[rightIndex++];
	}

	while (leftIndex < LEFT_SIZE)
		*sections[startIndex++] = *leftArr[leftIndex++];
	while (rightIndex < RIGHT_SIZE)
		*sections[startIndex++] = *rightArr[rightIndex++];

	freeCollection(leftArr, LEFT_SIZE);
	freeCollection(rightArr, RIGHT_SIZE);
}

void setSortedIndexes(Section** sections, int size)
{
	for (int currIndex = 0; currIndex < size; currIndex++)
		sections[currIndex]->setSortedIndex(currIndex);
}

void Swap(Section* left, Section* right)
{
	Section* temp = left;
	left = right;
	right = temp;
}

/* unstable sort, tends to break and thus is not vialbe for our exercise
void revertElemsToOriginalsPos(Section** sections, int size)
{
	for (int i = 0; i < size; i++)
	{
		int currOGIndex = sections[i]->getOriginalIndex();
		Swap(sections[i], sections[currOGIndex]);
	}
}
*/
void PrintNewIndexes(const Section* const* sections, int size)
{
	for (int i = 0; i < size; i++)
		std::cout << sections[i]->getSortedIndex() << " ";
}