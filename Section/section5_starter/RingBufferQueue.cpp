#include "RingBufferQueue.h"
#include "testing/SimpleTest.h"

using namespace std;
static int kDefaultCapacity = 4;

/* Constructor */
RBQueue::RBQueue() {
    capacity = kDefaultCapacity;
    queue = new int[capacity];
    head = 0;
    elemNum = 0;
}

/* Destructor */
RBQueue::~RBQueue() {
    delete[] queue;
}

/**
 * TODO: write a method comment
 */
void RBQueue::enqueue(int elem) {
    // The following lines are just here to
    // disable compiler warnings and errors.
    // Delete these lines, then implement
    // this method.
    if (isFull()) {
        doubleCapacity();
    }
    int tail = (head + elemNum) % capacity;
    queue[tail] = elem;
    elemNum += 1;
}

/**
 * TODO: write a method comment
 */
int RBQueue::dequeue() {
    // The following lines are just here to
    // disable compiler warnings and errors.
    // Delete these lines, then implement
    // this method.
    int result = peek();
    head += 1;
    if (head >= capacity) {
        head = head % capacity;
    }
    elemNum -= 1;
    return result;
}

/**
 * TODO: write a method comment
 */
int RBQueue::peek() const {
    // The following lines are just here to
    // disable compiler warnings and errors.
    // Delete these lines, then implement
    // this method.
    if (isEmpty()) {
        error("Empty Queue!");
    } else {
        return queue[head];
    }
}

/**
 * TODO: write a method comment
 */
bool RBQueue::isEmpty() const {
    // The following lines are just here to
    // disable compiler warnings and errors.
    // Delete these lines, then implement
    // this method.
    return elemNum == 0;
}

/**
 * TODO: write a method comment
 */
bool RBQueue::isFull() const {
    // The following lines are just here to
    // disable compiler warnings and errors.
    // Delete these lines, then implement
    // this method.
    return elemNum == capacity;
}

/**
 * TODO: write a method comment
 */
int RBQueue::size() const {
    // The following lines are just here to
    // disable compiler warnings and errors.
    // Delete these lines, then implement
    // this method.
    return elemNum;
}

void RBQueue::doubleCapacity() {
    int* newQueue = new int[capacity * 2];
    for (int i=0; i < elemNum; i++) {
        newQueue[i] = queue[(head + i) % capacity];
    }
    delete[] queue;
    queue = newQueue;
    capacity *= 2;
    head = 0;
}

/**
 * TODO: write a method comment
 */
ostream& operator<<(ostream& out, const RBQueue& queue) {
    // The following lines are just here to
    // disable compiler warnings and errors.
    // Delete these lines, then implement
    // this method.
    (void) queue;
    return out;
}

/* Provided Tests Below This Line */

PROVIDED_TEST("Simple Example From Handout"){
    RBQueue q;
    q.enqueue(1);
    q.enqueue(2);
    q.enqueue(3);
    EXPECT_EQUAL(q.peek(), 1);
    q.enqueue(4);
    q.enqueue(5);
    EXPECT(!q.isFull());
    EXPECT_EQUAL(q.dequeue(), 1);
    EXPECT_EQUAL(q.dequeue(), 2);
    EXPECT_EQUAL(q.peek(), 3);
    q.enqueue(6);
    EXPECT_EQUAL(q.dequeue(), 3);
    EXPECT_EQUAL(q.dequeue(), 4);
    EXPECT_EQUAL(q.dequeue(), 5);
    EXPECT_EQUAL(q.dequeue(), 6);
    EXPECT(q.isEmpty());
}
