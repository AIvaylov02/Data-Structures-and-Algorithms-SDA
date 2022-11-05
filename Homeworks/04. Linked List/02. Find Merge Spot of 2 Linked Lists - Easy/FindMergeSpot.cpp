#include <string>
#include <fstream>
using namespace std;

string ltrim(const string&);
string rtrim(const string&);

class SinglyLinkedListNode {
public:
    int data;
    SinglyLinkedListNode* next;

    SinglyLinkedListNode(int node_data) {
        this->data = node_data;
        this->next = nullptr;
    }
};

class SinglyLinkedList {
public:
    SinglyLinkedListNode* head;
    SinglyLinkedListNode* tail;

    SinglyLinkedList() {
        this->head = nullptr;
        this->tail = nullptr;
    }

    void insert_node(int node_data) {
        SinglyLinkedListNode* node = new SinglyLinkedListNode(node_data);

        if (!this->head) {
            this->head = node;
        }
        else {
            this->tail->next = node;
        }

        this->tail = node;
    }
};

void print_singly_linked_list(SinglyLinkedListNode* node, string sep, ofstream& fout) {
    while (node) {
        fout << node->data;

        node = node->next;

        if (node) {
            fout << sep;
        }
    }
}

// ---------------------------------------------------------------------------------------------
//                             |
// FUNCTIONS RELATED TO TASK   V

int findListSize(SinglyLinkedListNode* head)
{
    SinglyLinkedListNode* currEl = head;
    int size = 0;
    while (currEl != nullptr)
    {
        size++;
        currEl = currEl->next;
    }
    return size;
}

// This function will guarantee that we will traverse the nodes correctly at a paralel pace
SinglyLinkedListNode* moveHeadToCorrectStart(SinglyLinkedListNode* head, int wantedPositions)
{
    SinglyLinkedListNode* startPos = head;
    for (int i = 0; i < wantedPositions; i++)
        startPos = startPos->next;
    return startPos;
}

SinglyLinkedListNode* findCommonNode(SinglyLinkedListNode* head1Copy, SinglyLinkedListNode* head2Copy)
{
    while (head1Copy != nullptr && head2Copy != nullptr)
    {
        if (head1Copy == head2Copy) // there is a node match
            break;
        head1Copy = head1Copy->next;
        head2Copy = head2Copy->next;
    }

    return head1Copy;
}

// The grand idea is that the 2 linked list will merge together into 1. In other words, the beginning may be different but the end addresses will be the same at some
// point onward. One solution is to use double for to compare the addresses, the other (implemented here) -> traverse the bigger list to a point which the 2 lists sizes will be
// the same. Then with a single for cycle compare the current addresses of both lists nodes.
int findMergeNode(SinglyLinkedListNode* head1, SinglyLinkedListNode* head2) {
    int head1ListSize = findListSize(head1);
    int head2ListSize = findListSize(head2);

    // We make copies in order not to lose the original heads
    SinglyLinkedListNode* head1Copy = head1;
    SinglyLinkedListNode* head2Copy = head2;

    SinglyLinkedListNode* result;
    int listDiff = abs(head1ListSize - head2ListSize);
    if (listDiff == 0)
    {
        result = findCommonNode(head1Copy, head2Copy);
    }
    else if (head1ListSize < head2ListSize)
    {
        head2Copy = moveHeadToCorrectStart(head2Copy, listDiff);
        result = findCommonNode(head1Copy, head2Copy);
    }
    else // list 1 is larger
    {
        head1Copy = moveHeadToCorrectStart(head1Copy, listDiff);
        result = findCommonNode(head1Copy, head2Copy);
    }

    return result->data;
}