SinglyLinkedListNode* deleteNode(SinglyLinkedListNode* llist, int position) {

    SinglyLinkedListNode* toBeDeleted = llist;
    SinglyLinkedListNode* previousElem = llist; // in order it not to be nullptr when dereferencing

    if (llist->next == nullptr) // we have only 1 element
    {
        delete toBeDeleted;
        llist = nullptr;
    }
    else if (position == 0) // we need to move the llist head
    {
        llist = llist->next;
        delete toBeDeleted;
    }
    else
    {
        // take the pointer to the correctPos, if we do it in an outside function we will use ptr to ptr
        for (int i = 0; i < position; i++)
        {
            previousElem = toBeDeleted;
            toBeDeleted = toBeDeleted->next;
        }
        previousElem->next = toBeDeleted->next;
        delete toBeDeleted;
    }

    return llist;
}