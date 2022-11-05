#include <iostream>
#include <string>
const int DEF_INVALID_INDEX = -1;
const int MAX_CHARS = 62;

class UniqueSymbol
{
public:
	void addCount(int currCharIndex);
	int getIndex() const;

	// operator< will work based on index, thus meaning we will use it to sort by Index -> invalid indexes will be placed first
	bool operator<(const UniqueSymbol& other);
private:
	// in a more advanced build, each Unique symbol could have a char, which will also store the symbol of the film
	int index = DEF_INVALID_INDEX;
	int count = 0;
};

bool IsNumber(char ch);
bool IsCapitalLetter(char ch);
bool IsSmallLetter(char ch);
int ToDecimal(char ch);
void Swap(UniqueSymbol& obj1, UniqueSymbol& obj2);

// function to foreach the string and find the number of each char available, single founded 
// characters will store also their index of founding in the original string
void FillCharsCount(const std::string& input, UniqueSymbol collection[]);

int Partition(UniqueSymbol collection[], int startIndex, int endIndex);
void QuickSort(UniqueSymbol collection[], int startIndex, int endIndex);

void PrintSingleFilms(const UniqueSymbol collection[]);


int main()
{
	// max 62 unique characters -> 26 letters in both lower and upper case and 10 numbers

	std::string input;
	std::cin >> input;

	// 0 - 9, A - Z, a - z
	UniqueSymbol counters[MAX_CHARS];
	FillCharsCount(input, counters); // we have the count of each char, and the index of the single chars -> we can sort by index
	
	QuickSort(counters, 0, MAX_CHARS - 1); // we sort by index (in order not to print them alphabetically a.k.a if input is "xa" it should print "0-1" not "1-0")

	// print the indexes
	PrintSingleFilms(counters);
	return 0;
}



bool IsNumber(char ch)
{
	return ch >= '0' && ch <= '9';
}

bool IsCapitalLetter(char ch)
{
	return ch >= 'A' && ch <= 'Z';
}
bool IsSmallLetter(char ch)
{
	return ch >= 'a' && ch <= 'z';
}

void UniqueSymbol::addCount(int currCharIndex)
{
	count++;
	if (count > 1)
		index = DEF_INVALID_INDEX;
	else // count == 1
		index = currCharIndex;
}

int UniqueSymbol::getIndex() const
{
	return index;
}


bool UniqueSymbol::operator<(const UniqueSymbol& other)
{
	return index < other.index;
}
int ToDecimal(char ch)
{
	const int CAPS_TO_INT = 55;
	const int SMALL_TO_INT = 61;
	if (IsNumber(ch))
		return ch - '0';
	else if (IsCapitalLetter(ch))
		return ch - CAPS_TO_INT;
	// we assume the input is valid
	else
		return ch - SMALL_TO_INT;
}

void Swap(UniqueSymbol& obj1, UniqueSymbol& obj2)
{
	UniqueSymbol temp = obj2;
	obj2 = obj1;
	obj1 = temp;
}

void FillCharsCount(const std::string& input, UniqueSymbol collection[])
{
	int strLen = input.size();
	for (int currStrIndex = 0; currStrIndex < strLen; currStrIndex++)
	{
		char curr = input[currStrIndex];
		int charPlace = ToDecimal(curr);
		collection[charPlace].addCount(currStrIndex);
	}
}

int Partition(UniqueSymbol collection[], int startIndex, int endIndex)
{
	UniqueSymbol pivot = collection[endIndex];

	int pivotPos = startIndex; // we take the right element as pivot and assume it is the most left element
	while (startIndex < endIndex)
	{
		if (collection[startIndex] < pivot) // if pivot is greater than left element, pivot's pos will be put after it
			Swap(collection[pivotPos++], collection[startIndex]);
		startIndex++;
	}
	Swap(collection[pivotPos], collection[endIndex]);
	return pivotPos;
}
void QuickSort(UniqueSymbol collection[], int startIndex, int endIndex)
{
	if (startIndex >= endIndex)
		return;

	int pivot = Partition(collection, startIndex, endIndex);
	QuickSort(collection, startIndex, pivot - 1);
	QuickSort(collection, pivot + 1, endIndex);
}

void PrintSingleFilms(const UniqueSymbol collection[])
{
	for (int i = 0; i < MAX_CHARS; i++)
	{
		int currIndex = collection[i].getIndex();
		if (currIndex == DEF_INVALID_INDEX)
			continue;
		else
			std::cout << currIndex << " ";
	}
}