#include <iostream>

int TradedValue(const int* arr, int tradingDays);

int main()
{
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(NULL);

	int tradingDays;
	std::cin >> tradingDays;

	// allocate dyn. memory and initialise the values of the days in the int array
	int* tradeValues = new int[tradingDays];
	for (int currDay = 0; currDay < tradingDays; currDay++)
		std::cin >> tradeValues[currDay];

	std::cout << TradedValue(tradeValues, tradingDays);

	delete[] tradeValues; // free up dyn. memory
	return 0;
}

int TradedValue(const int* arr, int tradingDays)
{
	int startIndex = 0;
	int currProfit = 0; 

	for (int i = startIndex; i + 1 < tradingDays; i++)
	{
		if (arr[i] < arr[i + 1]) // if it is time to start trading
		{
			startIndex = i;
			i++;
			
			// while we are still in the array limits and the next price is higher than the current one
			while (i + 1 < tradingDays && arr[i] < arr[i + 1])
				i++;
			currProfit += arr[i] - arr[startIndex];
		}
	}
	return currProfit;
}