#include "listnode.h"
#include "vector.h"
#include "testing/SimpleTest.h"
using namespace std;
void printList(ListNode* front);
void deallocateList(ListNode* front);
ListNode* createList(Vector<int> values);
bool areEquivalent(ListNode* front, Vector<int> v);
void partition (ListNode*& front, ListNode*& less, ListNode*& equal, ListNode*& greater);
ListNode* concatenate (ListNode*left, ListNode* mid, ListNode* right);

/*
 * TODO: Replace this comment with a descriptive function
 * header comment about your implementation of the function.
 */
void quickSort(ListNode*& front) {
    /* do nothing with empty list */
    if (front == nullptr) {
        return;
    }
    /* divide the list into three groups */
    ListNode* less = nullptr;
    ListNode* equal = nullptr;
    ListNode* greater = nullptr;
    partition(front, less, equal, greater);
    /* recursively sort less group and greater group */
    quickSort(less);
    quickSort(greater);
    /* join the three sorted group */
    front = concatenate(less, equal, greater);
}

void partition (ListNode*& front, ListNode*& less, ListNode*& equal, ListNode*& greater) {
    if (front == nullptr) {
        return;
    }
    int pivot = front->data;
    while (front != nullptr) {
        ListNode* temp = front->next;
        if (front->data < pivot) {
            front->next = less;
            less = front;
            front = temp;
        } else if (front->data == pivot) {
            front->next = equal;
            equal = front;
            front = temp;
        } else {
            front->next = greater;
            greater = front;
            front = temp;
        }
    }
}

ListNode* concatenate (ListNode*left, ListNode* mid, ListNode* right) {
    // Concatenate `left` and `mid`
    if (left != nullptr) {
        ListNode* cur = left;
        while (cur->next != nullptr) {
            cur = cur->next;
        }
        cur->next = mid;
    } else {
        left = mid;
    }

    // Concatenate with `right`
    if (left != nullptr) {
        ListNode* cur = left;
        while (cur->next != nullptr) {
            cur = cur->next;
        }
        cur->next = right;
    } else {
        left = right;
    }
    return left;
}

/* * * * * * Test Code Below This Point * * * * * */

/* TODO: Write your own student tests below to test your sorting helper
 * functions and overall sorting algorithm correctness. We have provided
 * the skeleton of some tests below to help you get started with this
 * process.
 */
STUDENT_TEST("Test the `partition` helper function"){
    /* Initialize vector to sequence of values */
    Vector<int> values = {3, 1, 1, 5, 5, 3};
    /* Create equivalent list to use as test input */
    ListNode* list = createList(values);

    /* Partition the given list */
    ListNode* less = nullptr;
    ListNode* equal = nullptr;
    ListNode* greater = nullptr;
    partition(list, less, equal, greater);

    /* Confirm partition list matches vector */
    Vector<int> left = {1, 1};
    EXPECT(areEquivalent(less, left));
    Vector<int> mid = {3, 3};
    EXPECT(areEquivalent(equal, mid));
    Vector<int> right = {5, 5};
    EXPECT(areEquivalent(greater, right));
    EXPECT(list == nullptr);

    /* Avoid memory leaks by deallocating list */
    deallocateList(list);
    deallocateList(less);
    deallocateList(equal);
    deallocateList(greater);
}

STUDENT_TEST("Test the `concatenate` helper function."){
    /* Initialize vector to sequence of values */
    Vector<int> left = {1, 2};
    Vector<int> mid = {};
    Vector<int> right = {3};
    Vector<int> all = {1, 2, 3};
    /* Create equivalent list to use as test input */
    ListNode* less = createList(left);
    ListNode* equal = createList(mid);
    ListNode* greater = createList(right);

    /* Confirm sorted list matches sorted vector */
    ListNode* merge = concatenate(less, equal, greater);
    EXPECT(areEquivalent(merge, all));

    /* Avoid memory leaks by deallocating list */
    deallocateList(merge);
}

STUDENT_TEST("Empty List."){
    /* Initialize vector to sequence of values */
    Vector<int> values = {};
    /* Create equivalent list to use as test input */
    ListNode* list = createList(values);

    /* Sort the list */
    quickSort(list);

    /* Sort the equivalent vector */
    values.sort();

    /* Confirm sorted list matches sorted vector */
    EXPECT(areEquivalent(list, values));

    /* Avoid memory leaks by deallocating list */
    deallocateList(list);
}

STUDENT_TEST("Single-Element List."){
    /* Initialize vector to sequence of values */
    Vector<int> values = {2};
    /* Create equivalent list to use as test input */
    ListNode* list = createList(values);

    /* Sort the list */
    quickSort(list);

    /* Sort the equivalent vector */
    values.sort();

    /* Confirm sorted list matches sorted vector */
    EXPECT(areEquivalent(list, values));

    /* Avoid memory leaks by deallocating list */
    deallocateList(list);
}

STUDENT_TEST("Reverse Sorted List."){
    /* Initialize vector to sequence of values */
    Vector<int> values = {4, 3, 2, 1};
    /* Create equivalent list to use as test input */
    ListNode* list = createList(values);

    /* Sort the list */
    quickSort(list);

    /* Sort the equivalent vector */
    values.sort();

    /* Confirm sorted list matches sorted vector */
    EXPECT(areEquivalent(list, values));

    /* Avoid memory leaks by deallocating list */
    deallocateList(list);
}

STUDENT_TEST("List Contains Duplicates."){
    /* Initialize vector to sequence of values */
    Vector<int> values = {3, 3, 2, 1};
    /* Create equivalent list to use as test input */
    ListNode* list = createList(values);

    /* Sort the list */
    quickSort(list);

    /* Sort the equivalent vector */
    values.sort();

    /* Confirm sorted list matches sorted vector */
    EXPECT(areEquivalent(list, values));

    /* Avoid memory leaks by deallocating list */
    deallocateList(list);
}

STUDENT_TEST("List Contains Positives & Negatives."){
    /* Initialize vector to sequence of values */
    Vector<int> values = {-2, 1, 3, -5};
    /* Create equivalent list to use as test input */
    ListNode* list = createList(values);

    /* Sort the list */
    quickSort(list);

    /* Sort the equivalent vector */
    values.sort();

    /* Confirm sorted list matches sorted vector */
    EXPECT(areEquivalent(list, values));

    /* Avoid memory leaks by deallocating list */
    deallocateList(list);
}

STUDENT_TEST("Very Long List."){
    /* Initialize vector to sequence of values */
    Vector<int> values = {6, 4, 5, 7, 2, 8, 7, 7, 5, 9, 5, 0, 7, 0, 4};
    /* Create equivalent list to use as test input */
    ListNode* list = createList(values);

    /* Sort the list */
    quickSort(list);

    /* Sort the equivalent vector */
    values.sort();

    /* Confirm sorted list matches sorted vector */
    EXPECT(areEquivalent(list, values));

    /* Avoid memory leaks by deallocating list */
    deallocateList(list);
}

/*
 * We have provided this utility function as a convenience. This
 * function traverses the linked list represented by the provided
 * front pointer and prints out the data stored in the list along
 * the way in a convenient format. You may find it to be helpful to
 * use this function as a debugging aid.
 */
void printList(ListNode* front) {
   cout << "{";
   for (ListNode *cur = front; cur != nullptr; cur = cur->next) {
       cout << cur->data;
       if (cur->next != nullptr){
           cout << ", ";
       }
   }
   cout << "}" << endl;
}

/*
 * This utility function deallocates the memory for all the nodes in a
 * given linked list. It can be used to recycle the memory allocated
 * during a test case and avoid memory leaks.
 */
void deallocateList(ListNode* front) {
   while (front != nullptr) {
       ListNode* temp = front->next;
       delete front;
       front = temp;
   }
}

/*
 * This utility function is given a vector of values and constructs
 * an equivalent linked list containing the same values in the same order.
 * It can be used to construct a linked list to use as an input to
 * a function being tested.
 */
ListNode* createList(Vector<int> values) {
   ListNode* result = nullptr;
   for (int i = values.size() - 1; i >= 0; i--) {
       ListNode* temp = new ListNode(values[i], result);
       result = temp;
   }
   return result;
}

/*
 * This utility function compares a linked list and a vector for
 * equivalence and returns true if both contain the same values in the
 * same order. It can be used to confirm a linked list's contents match
 * the expected.
 */
bool areEquivalent(ListNode* front, Vector<int> v) {
   int index = 0;
   while (front != nullptr && index < v.size()) {
       if (front->data != v[index]) {
           return false;
       }
       front = front->next;
       index++;
   }
   if (front == nullptr && index == v.size()) {
       return true;
   } else {
       return false;
   }
}

STUDENT_TEST("This is a skeleton to demonstrate an end-to-end student test."){
    /* Initialize vector to sequence of values */
    Vector<int> values = {1, 2, 3, 4};
    /* Create equivalent list to use as test input */
    ListNode* list = createList(values);

    /* Sort the list */
    quickSort(list);

    /* Sort the equivalent vector */
    values.sort();

    /* Confirm sorted list matches sorted vector */
    EXPECT(areEquivalent(list, values));

    /* Avoid memory leaks by deallocating list */
    deallocateList(list);
}

PROVIDED_TEST("This tests some of the testing helper functions you are recommended to implement."){
    /* Creates a small test list containing the values 1->2->3. */
    ListNode* testList = nullptr;
    testList = new ListNode(3, testList);
    testList = new ListNode(2, testList);
    testList = new ListNode(1, testList);

    /* Test equality comparison function. */
    EXPECT(areEquivalent(testList, {1, 2, 3}));
    EXPECT(!areEquivalent(testList, {1, 2}));
    EXPECT(!areEquivalent(testList, {1, 2, 3, 4}));
    EXPECT(!areEquivalent(testList, {1, 3, 2}));

    /* Test create list function. */
    ListNode* studentList = createList({1, 2, 3, 4});
    printList(studentList);
    ListNode* cur = studentList;
    for (int i = 1; i <= 4; i++){
        EXPECT(cur != nullptr);
        EXPECT_EQUAL(cur->data, i);
        cur = cur->next;
    }

    /* Test deallocate list functions. There should be no memory leaks from this test. */
    deallocateList(studentList);
    deallocateList(testList);
}

/*
 * The provided test case demonstrates a comparsion between the same
 * algorithm (quicksort) operating on a linked list versus a vector.
 */
PROVIDED_TEST("Time linked list quicksort vs vector quicksort") {
    int startSize = 50000;

    for(int n = startSize; n < 10*startSize; n *= 2) {
        Vector<int> v(n);
        ListNode* list = nullptr;

        /* Create linked list and vector with the same random sequence. */
        for (int i = n-1; i >= 0; i--) {
            v[i] = randomInteger(-10000, 10000);
            list = new ListNode(v[i], list);
        }

        /* NOTE: This test does not check correctness of the linked list sort
         * function. It only times the two operations to compare relative speed. */
        TIME_OPERATION(n, quickSort(list));
        TIME_OPERATION(n, v.sort());        /* Standard vector sort operation is backed
                                               with quicksort algorithm. */

        deallocateList(list);
    }
}

