#include <iostream>
#include <set>

void CompensateIndexOffset(int& cellsCount, int& shipSize, int& shots);
void ProcessGuess(bool& IsSure, int& liarsMove, int currGuessSpot, int currShot, std::set<int>& visitedSpots, int& maxShipsPossible, int originalShipsCount, int shipsSize);
void HasLied(bool& IsSure, int& liarsMove, int currShot, int maxShipsPossible, int originalShipsCount);

int main()
{
	int cellsCount, originalShipsCount, shipsSize;
	std::cin >> cellsCount >> originalShipsCount >> shipsSize;
	int shots;
	std::cin >> shots;
    CompensateIndexOffset(cellsCount, shipsSize, shots);
    int maxShipsPossible = cellsCount / shipsSize; // we divide by floor division, as we cannot split a ship in 2

    std::set<int> visitedSpots; // we will keep track of the spots visited using set -> if somewhere the ship count drops because of guesses, then Mariika has lied
    visitedSpots.insert(0); // in order to have valid iterators we must add our 'invalid' limits to stop the iterator
    visitedSpots.insert(cellsCount);

	int currGuessSpot;
    int liarsMove = -1;
    bool IvanIsSure = false;
	for (int currShot = 1; currShot < shots; currShot++) // position is between 1 and N, not in [0,N-1]
	{
        std::cin >> currGuessSpot;
        ProcessGuess(IvanIsSure, liarsMove, currGuessSpot, currShot, visitedSpots, maxShipsPossible, originalShipsCount, shipsSize);
	}
    std::cout << liarsMove;
	return 0;
}

void CompensateIndexOffset(int& cellsCount, int& shipSize, int& shots)
{
    cellsCount++; // Compensate for the iterator lower_bound offset to the next element and the one missing space inaccuracy (min: spaces = shipsCount - 1)
    shipSize++; // Compensate for the needed empty space
    shots++; // We count shots from 1 to N, not from 0 to N-1
}

void ProcessGuess(bool& IsSure, int& liarsMove, int currGuessSpot, int currShot, std::set<int>& visitedSpots, int& maxShipsPossible, int originalShipsCount, int shipsSize)
{
    if (visitedSpots.find(currGuessSpot) == visitedSpots.end()) // the value must be unique in order to add it to the set and make an educated assumption
    {
        // lower bound return the element which is just greater or equal to the searched one -> since it has not been already added, we can safely get the limits of the new item
        int leftLimit = *(--visitedSpots.lower_bound(currGuessSpot));
        int rightLimit = *visitedSpots.lower_bound(currGuessSpot);
        visitedSpots.insert(currGuessSpot);

        // It may seem strange but these 2 operations may not give the same result always -> by floor dividing in the first row we say if we can plug ships in the subarrays [right-curr] and [curr-left] so we should add them
        maxShipsPossible += (rightLimit - currGuessSpot) / shipsSize + (currGuessSpot - leftLimit) / shipsSize;
        maxShipsPossible -= (rightLimit - leftLimit) / shipsSize; // Tell us if there must be a ship(s) in the area[right-left], so we should subtract it from the total possible
        // If we cannot be sure if she lied to us(the interval is too great), the 2 values should be equal -> if not then most certainly our guess was a ship
        HasLied(IsSure, liarsMove, currShot, maxShipsPossible, originalShipsCount);
    }
}

void HasLied(bool& IsSure, int& liarsMove, int currShot, int maxShipsPossible, int originalShipsCount)
{
    if (maxShipsPossible < originalShipsCount && !IsSure)
    {
        liarsMove = currShot;
        IsSure = true;
    }
}