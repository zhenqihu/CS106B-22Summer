#include "bits.h"
#include "treenode.h"
#include "huffman.h"
#include "map.h"
#include "vector.h"
#include "priorityqueue.h"
#include "strlib.h"
#include "testing/SimpleTest.h"
using namespace std;

/**
 * Given a Queue<Bit> containing the compressed message bits and the encoding tree
 * used to encode those bits, decode the bits back to the original message text.
 *
 * You can assume that tree is a well-formed non-empty encoding tree and
 * messageBits queue contains a valid sequence of encoded bits.
 *
 * Your implementation may change the messageBits queue however you like. There
 * are no requirements about what it should look like after this function
 * returns. The encoding tree should be unchanged.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
string decodeText(EncodingTreeNode* tree, Queue<Bit>& messageBits) {
    string ans = "";
    EncodingTreeNode* ptr = tree;

    while (!messageBits.isEmpty()) {
        while (!ptr->isLeaf()) {
            Bit move = messageBits.dequeue();
            if (move == 0) {
                ptr = ptr->zero;
            } else if (move == 1) {
                ptr = ptr->one;
            }
        }
        ans = ans + ptr->ch;
        ptr = tree;
    }

    return ans;
}

/**
 * Reconstruct encoding tree from flattened form Queue<Bit> and Queue<char>.
 *
 * You can assume that the queues are well-formed and represent
 * a valid encoding tree.
 *
 * Your implementation may change the queue parameters however you like. There
 * are no requirements about what they should look like after this function
 * returns.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
EncodingTreeNode* unflattenTree(Queue<Bit>& treeShape, Queue<char>& treeLeaves) {
    EncodingTreeNode* tree = nullptr;
    Bit b = treeShape.dequeue();
    if (b == 0) {
        char leaf = treeLeaves.dequeue();
        tree = new EncodingTreeNode(leaf);
    } else {
        EncodingTreeNode* left = unflattenTree(treeShape, treeLeaves);
        EncodingTreeNode* right = unflattenTree(treeShape, treeLeaves);
        tree = new EncodingTreeNode(left, right);
    }
    return tree;
}

/**
 * Decompress the given EncodedData and return the original text.
 *
 * You can assume the input data is well-formed and was created by a correct
 * implementation of compress.
 *
 * Your implementation may change the data parameter however you like. There
 * are no requirements about what it should look like after this function
 * returns.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
string decompress(EncodedData& data) {
    EncodingTreeNode* tree = unflattenTree(data.treeShape, data.treeLeaves);
    string text = decodeText(tree, data.messageBits);
    deallocateTree(tree);
    return text;
}

/**
 * Constructs an optimal Huffman coding tree for the given text, using
 * the algorithm described in lecture.
 *
 * Reports an error if the input text does not contain at least
 * two distinct characters.
 *
 * When assembling larger trees out of smaller ones, make sure to set the first
 * tree dequeued from the queue to be the zero subtree of the new tree and the
 * second tree as the one subtree.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
EncodingTreeNode* buildHuffmanTree(string text) {
   /* frequency map */
    Map<char, double> freq;
    for (char c: text) {
        if (freq.containsKey(c)) {
            freq.put(c, freq.get(c) + 1);
        } else {
            freq.put(c, 1);
        }
    }

    /* ensure as least two distinct characters  */
    if (freq.size() < 2) {
        error("less than two distinct characters!");
    }

    /* initialize priority queue */
    PriorityQueue<EncodingTreeNode*> pq;
    for (char c: freq) {
        EncodingTreeNode* leaf = new EncodingTreeNode(c);
        pq.enqueue(leaf, freq.get(c));
    }

    /* apply Huffman algorithm */
    while (pq.size() > 1) {
        double p1 = pq.peekPriority();
        EncodingTreeNode* first = pq.dequeue();
        double p2 = pq.peekPriority();
        EncodingTreeNode* second = pq.dequeue();
        EncodingTreeNode* combine = new EncodingTreeNode(first, second);
        pq.enqueue(combine, p1 + p2);
    }
    EncodingTreeNode* result = pq.dequeue();
    return result;
}

/**
 * Given a string and an encoding tree, encode the text using the tree
 * and return a Queue<Bit> of the encoded bit sequence.
 *
 * You can assume tree is a valid non-empty encoding tree and contains an
 * encoding for every character in the text.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
void dfs(EncodingTreeNode* tree, Queue<Bit> q, Map<char, Queue<Bit>>& maps) {
    if (tree == nullptr) {
        error("Empty Tree");
    }
    if (tree->isLeaf()) {
        maps.put(tree->ch, q);
        return;
    }
    Queue<Bit> q_left = q;
    q_left.enqueue(0);
    dfs(tree->zero, q_left, maps);

    Queue<Bit> q_right = q;
    q_right.enqueue(1);
    dfs(tree->one, q_right, maps);
}

Queue<Bit> encodeText(EncodingTreeNode* tree, string text) {
    /* build a map from char to its bit-squence */
    Map<char, Queue<Bit>> codeMap;
    dfs(tree, {}, codeMap);

    /* encode the text using the map */
    Queue<Bit> result;
    for (char c: text) {
        if (codeMap.containsKey(c)) {
            Queue<Bit> code = codeMap.get(c);
            while (!code.isEmpty()) {
                result.enqueue(code.dequeue());
            }
        } else {
            error("Invalid character");
        }
    }
    return result;
}

/**
 * Flatten the given tree into a Queue<Bit> and Queue<char> in the manner
 * specified in the assignment writeup.
 *
 * You can assume the input queues are empty on entry to this function.
 *
 * You can assume tree is a valid well-formed encoding tree.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
void flattenTree(EncodingTreeNode* tree, Queue<Bit>& treeShape, Queue<char>& treeLeaves) {
    if (tree == nullptr) {
        error("Empty tree");
    }

    if (tree->isLeaf()) {
        treeShape.enqueue(0);
        treeLeaves.enqueue(tree->ch);
        return;
    } else {
        // pre-order traversal
        treeShape.enqueue(1);
        flattenTree(tree->zero, treeShape, treeLeaves);
        flattenTree(tree->one, treeShape, treeLeaves);
    }
}

/**
 * Compress the input text using Huffman coding, producing as output
 * an EncodedData containing the encoded message and flattened
 * encoding tree used.
 *
 * Reports an error if the message text does not contain at least
 * two distinct characters.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
EncodedData compress(string messageText) {
    /* result as an `EncodedData` object */
    EncodedData result;
    /* build a optimal encoding tree using Huffman algorithm */
    EncodingTreeNode* encodeTree = buildHuffmanTree(messageText);
    /* save the encoded text */
    result.messageBits = encodeText(encodeTree, messageText);
    /* save the flatten tree & characters */
    flattenTree(encodeTree, result.treeShape, result.treeLeaves);
    /* delete the encoding tree */
    deallocateTree(encodeTree);
    return result;
}

/* * * * * * Testing Helper Functions Below This Point * * * * * */

EncodingTreeNode* createExampleTree() {
    /* Example encoding tree used in multiple test cases:
     *                *
     *              /   \
     *             T     *
     *                  / \
     *                 *   E
     *                / \
     *               R   S
     */
    /* TODO: Implement this utility function needed for testing. */
    /* Leaf Nodes */
    EncodingTreeNode* t = new EncodingTreeNode('T');
    EncodingTreeNode* r = new EncodingTreeNode('R');
    EncodingTreeNode* s = new EncodingTreeNode('S');
    EncodingTreeNode* e = new EncodingTreeNode('E');

    /* Interior Nodes*/
    EncodingTreeNode* i10 = new EncodingTreeNode(r, s);
    EncodingTreeNode* i1 = new EncodingTreeNode(i10, e);

    /* Root Node */
    EncodingTreeNode* root = new EncodingTreeNode(t, i1);
    return root;
}

void deallocateTree(EncodingTreeNode* t) {
    /* TODO: Implement this utility function needed for testing. */
    if (!t->isLeaf()) {
        deallocateTree(t->zero);
        deallocateTree(t->one);
    }
    delete t;
}

bool areEqual(EncodingTreeNode* a, EncodingTreeNode* b) {
    /* Check if any nullptr */
    if (a == nullptr) {
        return b == nullptr;
    } else if (b == nullptr) {
        return false;
    }
    /* Check if any leaf */
    if (a->isLeaf()) {
        return b->isLeaf() && (a->getChar() == b->getChar());
    } else if (b->isLeaf()) {
        return false;
    }
    return areEqual(a->zero, b->zero) && areEqual(a->one, b->one);
}

/* * * * * * Test Cases Below This Point * * * * * */

/* TODO: Write your own student tests. */









/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("decodeText, small example encoding tree") {
    EncodingTreeNode* tree = createExampleTree(); // see diagram above
    EXPECT(tree != nullptr);

    Queue<Bit> messageBits = { 1, 1 }; // E
    EXPECT_EQUAL(decodeText(tree, messageBits), "E");

    messageBits = { 1, 0, 1, 1, 1, 0 }; // SET
    EXPECT_EQUAL(decodeText(tree, messageBits), "SET");

    messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1}; // STREETS
    EXPECT_EQUAL(decodeText(tree, messageBits), "STREETS");

    deallocateTree(tree);
}

PROVIDED_TEST("unflattenTree, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    Queue<Bit>  treeShape  = { 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> treeLeaves = { 'T', 'R', 'S', 'E' };
    EncodingTreeNode* tree = unflattenTree(treeShape, treeLeaves);

    EXPECT(areEqual(tree, reference));

    deallocateTree(tree);
    deallocateTree(reference);
}

PROVIDED_TEST("decompress, small example input") {
    EncodedData data = {
        { 1, 0, 1, 1, 0, 0, 0 }, // treeShape
        { 'T', 'R', 'S', 'E' },  // treeLeaves
        { 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1 } // messageBits
    };

    EXPECT_EQUAL(decompress(data), "TRESS");
}

PROVIDED_TEST("buildHuffmanTree, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    EncodingTreeNode* tree = buildHuffmanTree("STREETTEST");
    EXPECT(areEqual(tree, reference));

    deallocateTree(reference);
    deallocateTree(tree);
}

PROVIDED_TEST("encodeText, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above

    Queue<Bit> messageBits = { 1, 1 }; // E
    EXPECT_EQUAL(encodeText(reference, "E"), messageBits);

    messageBits = { 1, 0, 1, 1, 1, 0 }; // SET
    EXPECT_EQUAL(encodeText(reference, "SET"), messageBits);

    messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1 }; // STREETS
    EXPECT_EQUAL(encodeText(reference, "STREETS"), messageBits);

    deallocateTree(reference);
}

PROVIDED_TEST("flattenTree, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    Queue<Bit>  expectedShape  = { 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> expectedLeaves = { 'T', 'R', 'S', 'E' };

    Queue<Bit>  treeShape;
    Queue<char> treeLeaves;
    flattenTree(reference, treeShape, treeLeaves);

    EXPECT_EQUAL(treeShape,  expectedShape);
    EXPECT_EQUAL(treeLeaves, expectedLeaves);

    deallocateTree(reference);
}

PROVIDED_TEST("compress, small example input") {
    EncodedData data = compress("STREETTEST");
    Queue<Bit>  treeShape   = { 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> treeChars   = { 'T', 'R', 'S', 'E' };
    Queue<Bit>  messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0 };

    EXPECT_EQUAL(data.treeShape, treeShape);
    EXPECT_EQUAL(data.treeLeaves, treeChars);
    EXPECT_EQUAL(data.messageBits, messageBits);
}

PROVIDED_TEST("Test end-to-end compress -> decompress") {
    Vector<string> inputs = {
        "HAPPY HIP HOP",
        "Nana Nana Nana Nana Nana Nana Nana Nana Batman"
        "Research is formalized curiosity. It is poking and prying with a purpose. – Zora Neale Hurston",
    };

    for (string input: inputs) {
        EncodedData data = compress(input);
        string output = decompress(data);

        EXPECT_EQUAL(input, output);
    }
}

/* * * * * Student Tests Below This Point * * * * */
STUDENT_TEST("Test createExampleTree and deallocateTree functions") {
    /* Create example */
    EncodingTreeNode* example = createExampleTree();

    /* Deallocate the example */
    deallocateTree(example);
}

STUDENT_TEST("Test areEqual function") {
    /* Case 1 */
    EncodingTreeNode* leaf = new EncodingTreeNode('A');
    EncodingTreeNode* empty = nullptr;
    EXPECT(!areEqual(leaf, empty));
    /* Case 2 */
    EncodingTreeNode* leaf1 = new EncodingTreeNode('A');
    EncodingTreeNode* leaf2 = new EncodingTreeNode('B');
    EXPECT(areEqual(leaf, leaf1));
    EXPECT(!areEqual(leaf, leaf2));
    /* Case 3 */
    EncodingTreeNode* e1 = createExampleTree();
    EXPECT(!areEqual(leaf, e1));
    /* Case 4*/
    EncodingTreeNode* e2 = createExampleTree();
    EXPECT(areEqual(e1, e2));
    /* Case 5 */
    EXPECT(!areEqual(e1, e1->zero));
    EXPECT(!areEqual(e1, e1->one));

    /* Deallocate */
    deallocateTree(leaf);
    deallocateTree(leaf1);
    deallocateTree(leaf2);
    deallocateTree(e1);
    deallocateTree(e2);
}
