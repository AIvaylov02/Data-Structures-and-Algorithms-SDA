#include <iostream>

bool IsSameLetter(char chA, char chB);

int CalcNeededErases(const char* inputLine, int lineLength);

int main()
{
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(NULL);


	int listsCount; // between 1 and 10, we read the next listsCount lines
	std::cin >> listsCount;
	for (int currList = 0; currList < listsCount; currList++)
	{
		int currentListLength;
		std::cin >> currentListLength; // we have an array of that many elements
		
		char* rawInput = new char[currentListLength+1]; // allocate dynamic memory (plus for '\0')
		std::cin >> rawInput;
		//rawInput[currentListLength] = '\0';

		int neededErases = CalcNeededErases(rawInput, currentListLength);
		std::cout << neededErases << "\n";

		delete[] rawInput; // free up dynamic memory
	}

	return 0;
}

bool IsSameLetter(char chA, char chB)
{
	return chA == chB;
}

int CalcNeededErases(const char* inputLine, int lineLength)
{
	int neededErases = 0;
	for (int currElem = 0; currElem < lineLength - 1; currElem++)
	{
		if (IsSameLetter(inputLine[currElem], inputLine[currElem + 1]))
			neededErases++;

	}
	return neededErases;
}