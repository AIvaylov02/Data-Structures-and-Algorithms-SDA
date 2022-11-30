#include <iostream>
#include <utility>
#include <map>


int main()
{
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(NULL);

	int requests;
	std::cin >> requests;

	int cupcakesCount;
	int* allCupcakes = nullptr;
	for (int processedRequest = 0; processedRequest < requests; processedRequest++)
	{
		std::cin >> cupcakesCount;
		allCupcakes = new int[cupcakesCount];

		int currCake;
		for (int cupcakesInFactory = 0; cupcakesInFactory < cupcakesCount; cupcakesInFactory++)
		{
			std::cin >> currCake;
			allCupcakes[cupcakesInFactory] = currCake;
		}

		std::map<int, int> packageCakes; // it will store the value as key and the index in the array as value
		int currPackageSize, maxPackageSize;
		currPackageSize = maxPackageSize = 0;
		int startOfPackageIndex;

		for (int candyInFactory = 0; candyInFactory < cupcakesCount; candyInFactory++)
		{
			if (packageCakes.empty())
				startOfPackageIndex = candyInFactory;

			if (packageCakes.find(allCupcakes[candyInFactory]) == packageCakes.end()) // the cupcake is not present so we add it to the package
			{
				packageCakes.insert(std::pair<int, int>(allCupcakes[candyInFactory], candyInFactory));
				currPackageSize++;
			}
			else // the curr cupcake is present in the package so we have to move and delete everything before it
			{
				if (currPackageSize > maxPackageSize)
					maxPackageSize = currPackageSize;


				int deleteUpTo = (*packageCakes.find(allCupcakes[candyInFactory])).second + 1; // find the lower bound of where to delete, a.k.a all elements before it should be gone
				while (startOfPackageIndex < deleteUpTo)
				{
					packageCakes.erase(allCupcakes[startOfPackageIndex]);
					currPackageSize--;
					startOfPackageIndex++;
				} // delete up to must be equal to startofpackage
				packageCakes.insert(std::pair<int, int>(allCupcakes[candyInFactory], candyInFactory));
				currPackageSize++;
			}
		}

		// corner case check for the last sequence to be the largest possible one
		if (currPackageSize > maxPackageSize)
			maxPackageSize = currPackageSize;

		std::cout << maxPackageSize << '\n';
		delete[] allCupcakes;
	}
}