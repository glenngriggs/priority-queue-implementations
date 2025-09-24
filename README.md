Priority Queue Implementations

Implementations of multiple priority queues sharing a common interface:

- `Eecs281PQ.hpp`: base interface with `push`, `pop`, `top`, `size`, `empty`, and `updatePriorities`. 
- `UnorderedPQ.hpp`: unordered array with linear scan for extreme. 
- `UnorderedFastPQ.hpp`: unordered array that caches the index of the current extreme between `top` and `pop` for speed.  
- `SortedPQ.hpp`: array kept sorted; `top()` is the back; `push` is `O(n)`. 
- `BinaryPQ.hpp`: binary heap over `std::vector` with `fixUp/fixDown`. 
- `PairingPQ.hpp`: pairing heap with `addNode` and `updateElt` support.

## Build & Test
This repo includes a simple test driver:
- `project2b.cpp` compiles the headers and runs sanity tests (primitive ops, `updatePriorities`, and pairing-heap specifics). 

### Example (g++)
```bash
g++ -std=c++20 -O2 -Wall -Wextra -pedantic \
  project2b.cpp -o testPQ
./testPQ
