#include <iostream>

// 10^6 input means we can input at worst an algorithm of O(N.sqrt(N)) => we strive toward O(N) then

int* createInputArray(int rouletteSpins);
bool isEven(int num);

// In order to have a better memory allocation, we will first for cycle the big raw array in order to know exactly how 
// much memory to allocate for both the smaller even and odd arrays.
void findOddnessCount(const int* numArray, int count, int& oddCount, int& evenCount);

void createSplitArrays(const int* numArray, int totalCount ,int* oddNumbers, int* evenNumbers);

void printArray(const int* numArray, int count);

// odd = black, even = red
int main()
{
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(NULL);


	int rouletteSpins;
	std::cin >> rouletteSpins;
	// we will accept the roulettes and create an int array of that size
	int* numbersChosen = createInputArray(rouletteSpins);
	
	int blackCount, redCount;
	blackCount = redCount = 0;
	findOddnessCount(numbersChosen, rouletteSpins, blackCount, redCount);

	int* blackNumbers = new int[blackCount];
	int* redNumbers = new int[redCount];
	createSplitArrays(numbersChosen, rouletteSpins, blackNumbers, redNumbers);
	delete[] numbersChosen;

	printArray(redNumbers, redCount);
	delete[] redNumbers;
	printArray(blackNumbers, blackCount);
	delete[] blackNumbers;

	return 0;
}

int* createInputArray(int rouletteSpins)
{
	int* bettedNumbers = new int[rouletteSpins];

	for (int i = 0; i < rouletteSpins; i++)
	{
		int betNumber;
		std::cin >> betNumber;
		bettedNumbers[i] = betNumber;
	}
	return bettedNumbers;
}

bool isEven(int num)
{
	return num % 2 == 0;
}

void findOddnessCount(const int* numArray, int count, int& oddCount, int& evenCount)
{
	for (int i = 0; i < count; i++)
	{
		if (isEven(numArray[i]))
			evenCount++;
		else
			oddCount++;
	}
}

void createSplitArrays(const int* numArray, int totalCount ,int* oddNumbers, int* evenNumbers)
{	
	int oddIndex, evenIndex;
	oddIndex = evenIndex = 0;

	for (int i = 0; i < totalCount; i++)
	{
		int currNum = numArray[i];
		if (isEven(currNum))
		{
			evenNumbers[evenIndex] = currNum;
			evenIndex++;
		}
		else
		{
			oddNumbers[oddIndex] = currNum;
			oddIndex++;
		}
	}
}

void printArray(const int* numArray, int count)
{
	for (int i = 0; i < count; i++)
		std::cout << numArray[i] << "\n";
}