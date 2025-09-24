Priority Queue Implementations

Implementations of multiple priority queues sharing a common interface:

- `Eecs281PQ.hpp`: base interface with `push`, `pop`, `top`, `size`, `empty`, and `updatePriorities`. :contentReference[oaicite:0]{index=0}  
- `UnorderedPQ.hpp`: unordered array with linear scan for extreme. :contentReference[oaicite:1]{index=1}  
- `UnorderedFastPQ.hpp`: unordered array that caches the index of the current extreme between `top` and `pop` for speed. :contentReference[oaicite:2]{index=2}  
- `SortedPQ.hpp`: array kept sorted; `top()` is the back; `push` is `O(n)`. :contentReference[oaicite:3]{index=3}  
- `BinaryPQ.hpp`: binary heap over `std::vector` with `fixUp/fixDown`. :contentReference[oaicite:4]{index=4}  
- `PairingPQ.hpp`: pairing heap with `addNode` and `updateElt` support. :contentReference[oaicite:5]{index=5}  

## Build & Test
This repo includes a simple test driver:
- `project2b.cpp` compiles the headers and runs sanity tests (primitive ops, `updatePriorities`, and pairing-heap specifics). :contentReference[oaicite:6]{index=6}

### Example (g++)
```bash
g++ -std=c++20 -O2 -Wall -Wextra -pedantic \
  project2b.cpp -o testPQ
./testPQ
