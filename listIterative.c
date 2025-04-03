/*  Version that uses pointers to pointers */
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct Node {
    int val;
    struct Node * next;
} Node;

/*  Insert a value into the list

    loc: the address of a next pointer to an element in the list
        (can also think of this as a pointer to the next pointer)
        (if loc is &head, this inserts at the beginning of the list)
    val: the new value to insert

    result: insert a new node with val in it, pointed to by * loc

    Before:

    loc ----------┐
                  ↓
         ┌-----┬------┐   ┌-----┬------┐
         |  x  | next-|-->|  y  |   ---|--┐
         └-----┴------┘   └-----┴------┘  ┴

    After:

    loc ----------┐
                  ↓
         ┌-----┬------┐   ┌-----┬------┐   ┌-----┬------┐
         |  x  | next-|-->| val |   ---|-->|  y  |   ---|--┐
         └-----┴------┘   └-----┴------┘   └-----┴------┘  ┴
*/
void insertNode(Node * * loc, int val) {
    //Allocate the new node
    Node * newNode = malloc(sizeof(Node));
    //Set up the value
    newNode->val = val;
    //The new node points to whatever * nextAddr points to
    newNode->next = * loc;
    //* nextAddr now points to the new node
    * loc = newNode;
}

/*  Remove a value from the list

    loc: the address of a next pointer that _points to the node to remove_
        (can also think of this as the pointer to the pointer)
        (if loc is &head, this removes the first element from the list)

    result: * loc points _past_ the node it used to point to

    Before:

    loc ----------┐
                  ↓
         ┌-----┬------┐   ┌-----┬------┐   ┌-----┬------┐
         |  x  | next-|-->|  y  |   ---|-->|  z  |   ---|--┐
         └-----┴------┘   └-----┴------┘   └-----┴------┘  ┴

    After:

    loc ----------┐
                  ↓
         ┌-----┬------┐   ┌-----┬------┐
         |  x  | next-|-->|  z  |   ---|--┐
         └-----┴------┘   └-----┴------┘  ┴
*/

void removeNode(Node * * loc) {
    //If *loc points to NULL, then there isn't a node to remove
    if (* loc == NULL) return;
    //Save pointer to the thing we're going to delete so we can free it
    Node * toDelete = * loc;
    //* loc now points to * loc's *next* node
    * loc = (* loc)->next;
    //Free the removed node
    free(toDelete);
}

/*  Return whether an element is in the list:

    head: pointer to the node at which to start the search
    key: value to search for

    returns: true if the value exists in the list
*/

bool containsNode(Node * head, int key) {
    // cursor pointing to first node in the list
    Node * cur = head;
    // as long as the cursor hasn't hit the end of the list
    while (cur != NULL) {
        //if we find the value, return true
        if (cur->val == key) return true;
        //move the cursor down the list
        cur = cur->next;
    }
    // if we made it to the end of the list without finding key
    // then the key isn't in the list
    return false;
}

/*  Print out the list

    head: pointer to the node at which to start printing
    fp: file pointer to print to (pass stdout to print to stdout)
*/

void printList(Node * head, FILE * fp) {
    // cursor pointing to first node in the list
    Node * cur = head;
    // as long as the cursor hasn't hit the end of the list
    while (cur != NULL) {
        //print the current value
        fprintf(fp, "%d ", cur->val);
        //move the cursor down the list
        cur = cur->next;
    }
    //print a newline at the end
    fprintf(fp, "\n");
}

/*  Find a node that contains a key 

    loc: the address of the pointer that points to the node where you want to start searching
    val: the value you are looking for

    returns the _address_ of the _pointer_ that points to a node equal to key

    If we're searching for _z_:

    loc ----------┐
                  ↓
         ┌-----┬------┐   ┌-----┬------┐   ┌-----┬------┐
         |  x  | next-|-->|  y  |   ---|-->|  z  |   ---|--┐
         └-----┴------┘   └-----┴------┘   └-----┴------┘  ┴

    loc ----------┐                ┌---- return the address of this
                  ↓                ↓  
         ┌-----┬------┐   ┌-----┬------┐   ┌-----┬------┐
         |  x  | next-|-->|  y  |   ---|-->|  z  |   ---|--┐
         └-----┴------┘   └-----┴------┘   └-----┴------┘  ┴

*/
Node * * findEq(Node * * loc, int key) {
    //While we're not pointing to an empty list
    while ((* loc) != NULL) {
        //if we're pointing to the value we want, return the address of the pointer
        if ((* loc)->val == key) return loc;
        //otherwise, move to the next pointer
        //this is tricky:
        //   1. Get to the pointer to the node itself: * loc
        //   2. Get to the next pointer of the node: (* loc)->next
        //   3. Get the address of that next pointer: &((* loc)->next)
        loc = &((* loc)->next);
    }
    //Return the address of the pointer that points to node == key
    return loc;
}

/*  Find a pointer to a node that contains a value larger than a key

    loc: the address of the pointer that points to the node where you want to start searching
    key: the value you are looking for

    returns the _address_ of the _pointer_ that points to the first node in the list
    whose val is greater than key. Note: if you want to insert right in front of that
    node, this is the pointer you want to use!

    loc ----------┐
                  ↓
         ┌-----┬------┐   ┌-----┬------┐   ┌-----┬------┐
         |  2  | next-|-->|  4  |   ---|-->|  6  |   ---|--┐
         └-----┴------┘   └-----┴------┘   └-----┴------┘  ┴

    If we're searching for _6_:

    loc ----------┐                ┌---- return the address of this
                  ↓                ↓  
         ┌-----┬------┐   ┌-----┬------┐   ┌-----┬------┐
         |  2  | next-|-->|  4  |   ---|-->|  7  |   ---|--┐
         └-----┴------┘   └-----┴------┘   └-----┴------┘  ┴

    Note: if all the values in the linked list are <= key, this will return the address of
    the last next pointer in the list. If we're searching for _8_

    loc ----------┐    return the address of this---┐
                  ↓                                 ↓
         ┌-----┬------┐   ┌-----┬------┐   ┌-----┬------┐
         |  2  | next-|-->|  4  |   ---|-->|  7  |   ---|--┐
         └-----┴------┘   └-----┴------┘   └-----┴------┘  ┴

*/
Node * * findGt(Node * * loc, int key) {
    //While we're not pointing to an empty list
    while ((* loc) != NULL) {
        //if we're pointing to a value > key, return the address of this pointer
        if ((* loc)->val > key) return loc;
        //otherwise, move to the next pointer
        //this is tricky:
        //   1. Get to the pointer to the node itself: * loc
        //   2. Get to the next pointer of the node: (* loc)->next
        //   3. Get the address of that next pointer: &((* loc)->next)
        loc = &((* loc)->next);
    }
    //Return the address of the pointer that points to node > key
    return loc;
}

/*  Add a new value into a sorted list

    loc: the address of the pointer to the list we want to add into
    key: the key to add to the list

    loc ----------┐
                  ↓
         ┌-----┬------┐   ┌-----┬------┐   ┌-----┬------┐
         |  2  | next-|-->|  4  |   ---|-->|  6  |   ---|--┐
         └-----┴------┘   └-----┴------┘   └-----┴------┘  ┴

    If we're adding 5:

    loc ----------┐
                  ↓
         ┌-----┬------┐   ┌-----┬------┐   ┌-----┬------┐   ┌-----┬------┐
         |  2  | next-|-->|  4  |   ---|-->|  5  |   ---|-->|  6  |   ---|--┐
         └-----┴------┘   └-----┴------┘   └-----┴------┘   └-----┴------┘  ┴
*/

void addNode(Node * * loc, int key) {
    //Find the address of a pointer that points to the node we want to insert in front of:
    Node * * insertLoc = findGt(loc, key);
    //Insert in front of that location:
    insertNode(insertLoc, key);
}

/*  Free a list

    list: address of pointer to list
*/
void freeList(Node * * list) {
    // while list is not empty
    while (* list != NULL) {
        //remove the first element in the list
        removeNode(list);
    }
}


int main(int argc, char * * argv) {
    Node * list1;
    
    // make a list 10 -> 6 -> 3
    insertNode(&list1, 3);
    insertNode(&list1, 6);
    insertNode(&list1, 10);
    
    printList(list1, stdout);
    
    // remove 6
    Node * * toRemove = findEq(&list1, 6);
    removeNode(toRemove);
    printList(list1, stdout);
    
    // insert 12 before 3:
    Node * * wheres3 = findEq(&list1, 3);
    insertNode(wheres3, 12);
    printList(list1, stdout);
    
    //Make a sorted list by building it with insertNode
    Node * list2;
    
    insertNode(&list2, 7);
    insertNode(&list2, 10);
    insertNode(&list2, 4);
    
    printList(list2, stdout);
    
    freeList(&list1);
    freeList(&list2);
    //verify that the lists are empty:
    printList(list1, stdout);
    printList(list2, stdout);
}
