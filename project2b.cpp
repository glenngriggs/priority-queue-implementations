// Project identifier: 43DE0E0C4C76BFAA6D8C2F5AEAE0518A9C42CF4E

/*
 * Compile this test against your .h files to make sure they compile. We
 * suggest adding to this file or creating your own test cases to test your
 * priority queue implementations more thoroughly. If you do not call a
 * function from here, that template is not instantiated and that function is
 * NOT compiled! So for instance, if you don't add code here to call
 * updatePriorities, that function could later cause compiler errors that you
 * don't even know about.
 *
 * Our makefile will build an executable named testPQ if you type 'make
 * testPQ' or 'make alltests' (without the quotes). This will be a debug
 * executable.
 *
 * Notice that testPairing tests the range-based constructor but main and
 * testPriorityQueue do not. Make sure to test the range-based constructor
 * for other PQ types, and also test the PairingPQ-specific member functions.
 *
 * This is NOT a complete test of your priority queues. You have to add code
 * to do more testing!
 *
 * You do not have to submit this file, but it won't cause problems if you
 * do.
 */

#include <cassert>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "BinaryPQ.hpp"
#include "Eecs281PQ.hpp"
#include "PairingPQ.hpp"
#include "SortedPQ.hpp"
#include "UnorderedPQ.hpp"

// A type for representing priority queue types at runtime
enum class PQType {
    Unordered,
    Sorted,
    Binary,
    Pairing,
};

// These can be pretty-printed :)
std::ostream &operator<<(std::ostream &ost, PQType pqType) {
    switch (pqType) {
    case PQType::Unordered:
        return ost << "Unordered";
    case PQType::Sorted:
        return ost << "Sorted";
    case PQType::Binary:
        return ost << "Binary";
    case PQType::Pairing:
        return ost << "Pairing";
    } // switch

    return ost << "Unknown PQType";
} // operator<<()


// Compares two int const* on the integers they point to
struct IntPtrComp {
    bool operator()(const int *a, const int *b) const {
        return *a < *b;
    } // operator()
}; // IntPtrComp structure


// Test the primitive operations on a priority queue:
// constructor, push, pop, top, size, empty.
template <template <typename...> typename PQ>
void testPrimitiveOperations() {
    std::cout << "Testing primitive priority queue operations..." << std::endl;

    PQ<int> pq {};
    Eecs281PQ<int> &eecsPQ = pq;

    eecsPQ.push(3);
    eecsPQ.push(4);
    assert(eecsPQ.size() == 2);
    assert(eecsPQ.top() == 4);

    eecsPQ.pop();
    assert(eecsPQ.size() == 1);
    assert(eecsPQ.top() == 3);
    assert(not eecsPQ.empty());

    eecsPQ.pop();
    assert(eecsPQ.size() == 0);  // NOLINT: Explicit test for size == 0
    assert(eecsPQ.empty());

    // TODO: Add more testing here!

     // Basic insertions
     eecsPQ.push(3);
     eecsPQ.push(4);
     eecsPQ.push(2);
     eecsPQ.push(5);
     eecsPQ.push(1);
 
     assert(eecsPQ.size() == 5);
     assert(eecsPQ.top() == 5); // Max PQ by default
 
     // Remove all elements one by one
     for (int i = 5; i >= 1; --i) {
         assert(eecsPQ.top() == i);
         eecsPQ.pop();
     }
 
     assert(eecsPQ.empty());
 
     // Test duplicate elements
     eecsPQ.push(10);
     eecsPQ.push(10);
     assert(eecsPQ.size() == 2);
     eecsPQ.pop();
     assert(eecsPQ.top() == 10);
     eecsPQ.pop();
     assert(eecsPQ.empty());
 
    std::cout << "testPrimitiveOperations succeeded!" << std::endl;
} // testPrimitiveOperations()


// Test that the priority queue uses its comparator properly. HiddenData
// can't be compared with operator<, so we use HiddenDataComp{} instead.
template <template <typename...> typename PQ>
void testHiddenData() {
    struct HiddenData {
        int data;
    }; // HiddenData structure

    struct HiddenDataComp {
        bool operator()(const HiddenData &a, const HiddenData &b) const {
            // TODO: Finish this comparator
            (void)a;  // Delete this line when you finish this function
            (void)b;  // Delete this line when you finish this function
            return false;
        } // operator()()
    }; // comparator

    std::cout << "Testing with hidden data..." << std::endl;

    // TODO: Add code here to actually test with the HiddenData type.
    // Consider writing this code in the style of testPrimitiveOperations
    // above.

    std::cout << "testHiddenData succeeded!" << std::endl;
} // testHiddenData()


// Test the last public member function of Eecs281PQ, updatePriorities
template <template <typename...> typename PQ>
void testUpdatePriorities() {
    std::cout << "Testing updatePriorities() more rigorously..." << std::endl;

    std::vector<int> data { 1, 5, 3, 8, 2, 15, 7 };
    PQ<const int *, IntPtrComp> pq {};
    Eecs281PQ<const int *, IntPtrComp> &eecsPQ = pq;

    for (auto &datum : data) {
        eecsPQ.push(&datum);
    }

    // Modify multiple elements and update priorities
    data[2] = 20;  // Originally 3 → now highest priority
    data[5] = 10;  // Originally 15 → now lower than 20

    eecsPQ.updatePriorities();
    
    assert(*eecsPQ.top() == 20); // Ensures highest priority is now correct

    // Expected order after updates
    std::vector<int> expectedOrder {20, 10, 8, 7, 5, 2, 1};
    for (int val : expectedOrder) {
        if(*eecsPQ.top() == val)
        assert(*eecsPQ.top() == val);
        else
        assert(*eecsPQ.top() == val);


        eecsPQ.pop();
    }

    assert(eecsPQ.empty());
    std::cout << "Expanded testUpdatePriorities() succeeded!" << std::endl;
}

void testLargeUpdateElt() {
    std::cout << "Testing PairingPQ with a large update without popping..." << std::endl;

    // Suppose we want to test 100 elements (mimics the 'PES' test).
    const int N = 100;

    PairingPQ<int> pq;
    std::vector<PairingPQ<int>::Node*> nodes(N);

    // Insert N elements; store the Node* so we can update them.
    for (int i = 0; i < N; ++i) {
        // For variety, let's just insert multiples of i or something random.
        nodes[i] = pq.addNode(i * 2); 
    }
    // At this point, top() is i*2 for the largest i (i.e. 198 if N=100).

    // Now let's pick one node and update it to a bigger value
    // that should become the new top if updateElt() is correct.
    // E.g. update the 25th node to 277 (to match the autograder's example).
    const int indexToUpdate = 25;
    const int newValue = 277;
    pq.updateElt(nodes[indexToUpdate], newValue);

    // We do NOT pop anything. Just check top().
    int actualTop = pq.top();
    if (actualTop == newValue) {
        std::cout << "PASS: top() is " << actualTop 
                  << " as expected (updated node at index " << indexToUpdate << ").\n";
    } else {
        std::cout << "FAIL: top() was " << actualTop 
                  << ", but expected " << newValue << "!\n";
    }
}

void testVeryLargeUpdateElt() {
    std::cout << "Testing PairingPQ with 100,000 updates..." << std::endl;

    const int N = 100000;
    PairingPQ<int> pq;
    std::vector<PairingPQ<int>::Node*> nodes(N);

    for (int i = 0; i < N; ++i) {
        nodes[i] = pq.addNode(i); 
    }

    // If the official test sets the new top to 299345, replicate that:
    const int indexToUpdate = 900;  // or whichever
    const int newValue = 299345;
    pq.updateElt(nodes[indexToUpdate], newValue);

    // No pops. Check top().
    int actualTop = pq.top();
    if (actualTop == newValue) {
        std::cout << "PASS: top() is " << actualTop << ", large update test succeeded.\n";
    } else {
        std::cout << "FAIL: top() was " << actualTop 
                  << ", expected " << newValue << "!\n";
    }
}




void testHeapIntegrity() {
    std::cout << "Testing heap integrity for large updates..." << std::endl;

    PairingPQ<int> pq {};
    Eecs281PQ<int> &eecsPQ = pq;

    // Insert 100 elements
    std::vector<int> values(100);
    for (int i = 0; i < 100; ++i) {
        values[i] = i * 3;
        eecsPQ.push(values[i]);
    }

    // Modify a key
    values[25] = 400;
    eecsPQ.push(values[25]); // Originally 75, now highest priority
    eecsPQ.updatePriorities();

    if (eecsPQ.top() == 400) {
        std::cout << "PES Test Passed: top() = " << eecsPQ.top() << "\n";
    } else {
        std::cout << "PES Test Failed: Expected 400, but got " << eecsPQ.top() << "\n";
    }
}

void testUpdateEltPairing() {
    std::cout << "Testing PairingPQ::updateElt..." << std::endl;

    // Create a PairingPQ and add a few items, storing the Node* pointers.
    PairingPQ<int> pairingPQ;
    PairingPQ<int>::Node *n1 = pairingPQ.addNode(10);
    PairingPQ<int>::Node *n2 = pairingPQ.addNode(20);
    PairingPQ<int>::Node *n3 = pairingPQ.addNode(5);
    PairingPQ<int>::Node *n4 = pairingPQ.addNode(15);

    // At this point, top() should be 20 (n2).
    assert(pairingPQ.top() == 20 && "Initial top() should be 20!");

    // Increase the value in n1 (originally 10) to 25.
    // That should now be the maximum, so top() should become 25.
    pairingPQ.updateElt(n1, 25);
    assert(pairingPQ.top() == 25 && "After updateElt(n1,25), top() should be 25!");

    // Increase n4 from 15 → 30.
    // top() should become 30.
    pairingPQ.updateElt(n4, 30);
    assert(pairingPQ.top() == 30 && "After updateElt(n4,30), top() should be 30!");

    // Increase n2 from 20 → 35.
    // top() should become 35.
    pairingPQ.updateElt(n2, 35);
    assert(pairingPQ.top() == 35 && "After updateElt(n2,35), top() should be 35!");

    // Now try decreasing a value (e.g., n3 from 5 to 2) — in a max-heap that shouldn’t bubble up.
    pairingPQ.updateElt(n3, 2);  // This is a lower value; top should remain 35.
    assert(pairingPQ.top() == 35 && "Decreasing priority should not change the top!");

    std::cout << "All updateElt tests passed for PairingPQ!\n";
}



// Test the pairing heap's range-based constructor, copy constructor,
// copy-assignment operator, and destructor
// TODO: Test other operations specific to this PQ type.
void testPairing() {
    std::cout << "Testing Pairing Heap separately..." << std::endl;

    // Start a block of code here, so that when it ends, we have
    // a way to know that the destructors are about to be called,
    // but the function hasn't finished yet.
    {
        const std::vector<int> vec { 1, 0, };

        std::cout << "Calling constructors" << std::endl;

        // Range-based constructor
        PairingPQ<int> pairing1 { vec.cbegin(), vec.cend() };

        // Copy constructor
        PairingPQ<int> pairing2 { pairing1 };

        // Copy-assignment operator
        PairingPQ<int> pairing3 {};
        pairing3 = pairing2;

        // A reference to a PairingPQ<T> is a reference to an Eecs281PQ<T>.
        // Yay for polymorphism! We can therefore write:
        Eecs281PQ<int> &pq1 = pairing1;
        Eecs281PQ<int> &pq2 = pairing2;
        Eecs281PQ<int> &pq3 = pairing3;  // NOLINT: Students may add testing that modifies pq3

        pq1.push(3);
        pq2.pop();
        assert(pq1.size() == 3);
        assert(not pq1.empty());
        assert(pq1.top() == 3);
        pq2.push(pq3.top());
        assert(pq2.top() == pq3.top());

        std::cout << "Basic tests done." << std::endl;

        // TODO: Add more code to test addNode, updateElt, etc.

        // That { above creates a scope, and our pairing heaps will fall out
        // of scope at the matching } below.
        std::cout << "Calling destructors" << std::endl;
    } // block for testing destructors

    std::cout << "testPairing succeeded!" << std::endl;
} // testPairing()


// Run all tests for a particular PQ type.
template <template <typename...> typename PQ>
void testPriorityQueue() {
    testPrimitiveOperations<PQ>();
    testHiddenData<PQ>();
    testUpdatePriorities<PQ>();
} // testPriorityQueue()

// PairingPQ has some extra behavior we need to test in updateElement.
// This template specialization handles that without changing the nice
// uniform interface of testPriorityQueue.
template <>
void testPriorityQueue<PairingPQ>() {
    testPrimitiveOperations<PairingPQ>();
    testHiddenData<PairingPQ>();
    testUpdatePriorities<PairingPQ>();
    testPairing();
    testHeapIntegrity();
    testUpdateEltPairing();
    testLargeUpdateElt();
    testVeryLargeUpdateElt();
} // testPriorityQueue<PairingPQ>()


int main() {
    const std::vector<PQType> types {
        PQType::Unordered,
        PQType::Sorted,
        PQType::Binary,
        PQType::Pairing,
    };

    std::cout << "PQ tester" << std::endl << std::endl;
    int idx { 0 };
    for (const auto &type : types) {
        std::cout << "  " << idx++ << ") " << type << std::endl;
    } // for
    std::cout << std::endl;

    std::cout << "Select one: ";
    uint32_t choice {};
    std::cin >> choice;
    const PQType pqType = types.at(choice);

    std::cout << "Testing the " << pqType << " PQ...";

    // TODO: Add more cases to test other priority queue types.
    switch (pqType) {
    case PQType::Unordered:
        testPriorityQueue<UnorderedPQ>();
        break;

    case PQType::Sorted:
        testPriorityQueue<SortedPQ>();
        break;

    case PQType::Binary:
        testPriorityQueue<BinaryPQ>();
        break;

    case PQType::Pairing:
        testPriorityQueue<PairingPQ>();
        break;

    
    default:
        std::cout << "Unrecognized PQ type " << pqType << " in main.\n"
                  << "You must add tests for all PQ types." << std::endl;
        return 1;
    } // switch

    std::cout << "All tests succeeded!" << std::endl;

    return 0;
} // main()
