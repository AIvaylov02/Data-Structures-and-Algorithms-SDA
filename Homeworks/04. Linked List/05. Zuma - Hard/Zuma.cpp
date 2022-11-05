#include <iostream>
#include <exception>

/*
    So the idea of this task was explained poorly in my humble opinion. I will try to periphrase it shortly:
    Task: Given a sequence(list) of different coloured balls with ballsCount N, complete requests. Each request consists of a ballPosition (P) and a ballColour (C).
    You must shoot balls at that sequence with a constant O(1) time, so you should keep the ptr-s also in an array. By placing a ball after a given position(the placement
    is after position P in the first 'original' array), you should check if it creates a sequence of same coloured balls. If it does not shoot again(we must point out that
    request may be given to the newly put ball, so you should find some way to story its address in some array too). If it does however create a matching order, please remove
    those elements(O(N) complexity) and print the count of elements removed in that turn. By deleting you should glue together the 2 remaining left and right lists and check 
    if it creates a new matching order, if it does continue iterating the process using this logic, if it doesn't - go for the next shot/request. If the lists seize to exist at 
    some point you should accept the request but just print "Game over" and cycle until the end of the programme. At the end of the programme you should print the remaining 
    list if such exists or print '-1' if all balls have been deleted. The logic is like CandyCrush, but here you try to win based on surviving pongs, not by destroying them.
*/

struct Node
{
    int value = -1;
    Node* previous = nullptr;
    Node* next = nullptr;

    Node(int value = -1, Node* previous = nullptr, Node* next = nullptr);
};

Node::Node(int value, Node* previous, Node* next)
{
    this->value = value;
    this->previous = previous;
    this->next = next;
}


class DoubleLinkedList
{
public:
    DoubleLinkedList() = default;

    DoubleLinkedList(const DoubleLinkedList& other);
    DoubleLinkedList& operator=(const DoubleLinkedList& other);
    ~DoubleLinkedList();

    bool isEmpty() const;

    void pop_front();
    void pop_back();

    void push_back(const int element);
    void print() const;

    Node* insertAfter(const int wantedIndex, const int element, Node** collection);

    Node* removeSequence(Node* midIndex, int& sequenceCount, bool& isAtTheCorner);

    Node* head = nullptr;
    Node* tail = nullptr;
 private:

    void free();
    void copyFrom(const DoubleLinkedList& other);

    // Return the next element which is not contained in the wanted interval.
    // Functions which use them should == nullptr check them. They also tell the inclusive interval length without counting the beginning "start" node
    Node* findLeftNode(Node* startElem, int& sequenceCount);
    Node* findRightNode(Node* startElem, int& sequenceCount);
};

bool DoubleLinkedList::isEmpty() const
{
    return head == nullptr;
}


void DoubleLinkedList::pop_front()
{
    //if (isEmpty())
        //throw std::exception("Empty list");
    Node* oldHead = head;

    if (tail == head) // if it has only 1 elem, don't make changes to next and previous ptr-s (1->0)
        head = tail = nullptr;
    else
    {
        head = head->next;
        head->previous = nullptr; // if it has more than 1 elem, change them
    }
    delete oldHead;
}

void DoubleLinkedList::pop_back()
{
    //if (isEmpty())
    //  throw std::exception("Empty list!");
    Node* oldTail = tail;

    if (tail == head) // only 1 element is present
        head = tail = nullptr;
    else
    {
        tail = tail->previous;
        tail->next = nullptr;
    }
    delete oldTail;
}

void DoubleLinkedList::push_back(const int element)
{
    if (isEmpty())
    {
        head = new Node(element);
        tail = head;
    }
    else
    {
        tail->next = new Node(element, tail);
        tail = tail->next;
    }
}
void DoubleLinkedList::print() const
{
    //if (isEmpty())
    //  throw std::exception("Empty list!");

    Node* currElement = head;
    while (currElement)
    {
        std::cout << currElement->value << ' ';
        currElement = currElement->next;
    }
    std::cout << '\n';
}

void DoubleLinkedList::free()
{
    Node* currElem = head;
    while (currElem)
    {
        Node* nextElem = currElem->next;
        delete currElem;
        currElem = nextElem;
    }
    head = tail = nullptr;
}

void DoubleLinkedList::copyFrom(const DoubleLinkedList& other)
{
    Node* currElem = other.head;
    while (currElem)
    {
        push_back(currElem->value);
        currElem = currElem->next;
    }
}

DoubleLinkedList::DoubleLinkedList(const DoubleLinkedList& other)
{
    copyFrom(other);
}
DoubleLinkedList& DoubleLinkedList::operator=(const DoubleLinkedList& other)
{
    if (this != &other)
    {
        free();
        copyFrom(other);
    }
    return *this;
}
DoubleLinkedList::~DoubleLinkedList()
{
    free();
}

 //Adds an element at next index (after the oneparsed to the function)
Node* DoubleLinkedList::insertAfter(const int wantedIndex, const int element, Node** collection)
{
    Node* addedNode = collection[wantedIndex];
    addedNode = addedNode->next;

    if (addedNode == nullptr) // End reached, we must insert at the end of the tail, a.k.a push_back
    {
        push_back(element);
        addedNode = tail;
    }
    else
    {
        Node* previousNode = collection[wantedIndex];
        addedNode = new Node(element, previousNode, addedNode);
        addedNode->next->previous = addedNode;
        previousNode->next = addedNode;
    }

    return addedNode;
}

Node* DoubleLinkedList::findLeftNode(Node* startElem, int& sequenceCount)
{
    if (startElem == head) // If we are already at the utmost left point, just break by giving it an outside of limits value
        return nullptr;

    Node* leftElem = startElem->previous;
    while (leftElem != nullptr && leftElem->value == startElem->value)
    {
        sequenceCount++;
        leftElem = leftElem->previous;
    }
    return leftElem; // The index is not included in the sequence -> it is a different number or a nullptr
}

Node* DoubleLinkedList::findRightNode(Node* startElem, int& sequenceCount)
{
    if (startElem == tail)
        return nullptr;

    Node* rightElem = startElem->next;
    while (rightElem != nullptr && rightElem->value == startElem->value)
    {
        sequenceCount++;
        rightElem = rightElem->next;
    }
    return rightElem; // The index is not included in the sequence -> it is a different number or a nullptr
}

const int MIN_SEQUENCE_LEN_NEEDED = 3;
Node* DoubleLinkedList::removeSequence(Node* midIndex, int& sequenceCount, bool& isAtTheCorner)
{
    Node* leftOutsidePoint = findLeftNode(midIndex, sequenceCount);
    Node* rightOutsidePoint = findRightNode(midIndex, sequenceCount);
    sequenceCount++; // compensate for indexes

    if (sequenceCount < MIN_SEQUENCE_LEN_NEEDED) // We won't do anything, just go on onto the next shot
    {
        sequenceCount = 0; // Don't change current score
        return nullptr;
    }

    Node* startingDeletionPoint = nullptr;
    Node* endDeletionPoint = nullptr;

    if (leftOutsidePoint == nullptr || rightOutsidePoint == nullptr)
    {
        isAtTheCorner = true;
        if (leftOutsidePoint == nullptr && rightOutsidePoint == nullptr) // If the whole sequence is full of these wanted numbers
            free();
        else // Only 1 of the two ends the sequence
        {
            if (leftOutsidePoint == nullptr) //Null-> we have ended the whole left sequence or a node has a different value if != null
                startingDeletionPoint = head;
            else
                startingDeletionPoint = leftOutsidePoint->next;

            if (rightOutsidePoint == nullptr)
                endDeletionPoint = tail;
            else
                endDeletionPoint = rightOutsidePoint->previous;

            while (startingDeletionPoint != endDeletionPoint)
            {
                Node* nextNode = startingDeletionPoint->next;
                delete startingDeletionPoint;
                startingDeletionPoint = nextNode;
            }
            delete endDeletionPoint;

            // Re-wire the ptrs
            if (leftOutsidePoint != nullptr)
                leftOutsidePoint->next = rightOutsidePoint;
            else
                head = rightOutsidePoint; // If we have deleted a whole part including extremal inteval, make it's extremal part point to the right valid spot of it

            if (rightOutsidePoint != nullptr)
                rightOutsidePoint->previous = leftOutsidePoint;
            else
                tail = leftOutsidePoint; // If we have deleted a whole part including extremal inteval, make it's extremal part point to the left valid spot of it
        }
    }
    else
    {
        // We don't have nullptr values, a.k.a no extremal cases when a element is at the end of the sequence
        startingDeletionPoint = leftOutsidePoint->next;
        endDeletionPoint = rightOutsidePoint->previous;
        while (startingDeletionPoint != endDeletionPoint) //If the points are in between delete them
        {
            Node* nextNode = startingDeletionPoint->next;
            delete startingDeletionPoint;
            startingDeletionPoint = nextNode;
        }
        delete endDeletionPoint;

        // Re-wire the ptrs
        leftOutsidePoint->next = rightOutsidePoint;
        rightOutsidePoint->previous = leftOutsidePoint;

        if (leftOutsidePoint->value != rightOutsidePoint->value) // Check to see if we are in the middle of a new sequence, if not break the cycle
            isAtTheCorner = true;
    }
    return rightOutsidePoint;
}


int ShotResult(int ballPosition, int pongsCount, int& putAdditionalIndex, int ballColour, DoubleLinkedList& pongsCollection, Node** originalCollection);
void PrintResult(int result);
void PrintFinalScore(const DoubleLinkedList& pongsCollection);

const int MAX_SHOTS_COUNT = 100'000;
int main()
{
    // Declare and initialise linked list
    int pongsCount;
    std::cin >> pongsCount;
    DoubleLinkedList pongs;
    Node** originalCollection = new Node * [pongsCount + MAX_SHOTS_COUNT];
    int currBall;
    for (int i = 0; i < pongsCount; i++)
    {
        std::cin >> currBall;
        pongs.push_back(currBall);
        originalCollection[i] = pongs.tail;
    }

    // Complete requests
    int shots;
    std::cin >> shots;
    int ballPosition, ballColour;
    int result;
    int putAdditionalIndex = 0;
    for (int i = 0; i < shots; i++)
    {
        std::cin >> ballPosition >> ballColour;
        result = ShotResult(ballPosition, pongsCount, putAdditionalIndex, ballColour, pongs, originalCollection);
        PrintResult(result);
    }

    PrintFinalScore(pongs);
    delete[] originalCollection; // we don't delete the inner ptr-s because they will be deleted by the linked list
}

int ShotResult(int ballPosition, int pongsCount, int& putAdditionalIndex, int ballColour, DoubleLinkedList& pongsCollection, Node** originalCollection)
{
    if (pongsCollection.isEmpty()) // All pongs have been destroyed
        return -1; // Instead of 0, because we may have a case, where we don't remove any pongs

    int currScore = 0;
    int totalResult = 0;
    Node* midPosition = pongsCollection.insertAfter(ballPosition, ballColour, originalCollection);
    originalCollection[pongsCount + putAdditionalIndex] = midPosition;
    putAdditionalIndex++;
    bool isAtTheCorner = false; // Bool value, which will track if we have reached a corner -> because deletion should only happen if we are in the middle somewhere of a sequence
    do
    {
        isAtTheCorner = false;
        currScore = 0;
        midPosition = pongsCollection.removeSequence(midPosition, currScore, isAtTheCorner);
        totalResult += currScore;
    } while (midPosition != nullptr && isAtTheCorner != true);

    return totalResult;
}

void PrintResult(int result)
{
    if (result == -1)
        std::cout << "Game Over";
    else
        std::cout << result;
    std::cout << '\n';
}

void PrintFinalScore(const DoubleLinkedList& pongsCollection)
{
    if (pongsCollection.isEmpty())
        std::cout << -1;
    else
        pongsCollection.print();
}