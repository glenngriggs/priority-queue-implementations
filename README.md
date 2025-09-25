# Priority Queue Implementations

Implementations of multiple priority queues sharing a common interface:

- **`Eecs281PQ.hpp`**:  
  Base interface with `push`, `pop`, `top`, `size`, `empty`, and `updatePriorities`.  
  Provides the foundation for consistent testing and benchmarking across implementations.

- **`UnorderedPQ.hpp`**:  
  Unordered array with linear scan for extreme.  
  - `push`: **O(1)**  
  - `top` / `pop`: **O(n)**

- **`UnorderedFastPQ.hpp`**:  
  Unordered array that caches the index of the current extreme between `top` and `pop` for speed.  
  - Faster than plain `UnorderedPQ` for repeated access.  
  - Useful as a baseline improvement without adding significant complexity.  

- **`SortedPQ.hpp`**:  
  Array kept sorted; `top()` is the back; `push` is **O(n)** due to insertion.  
  - `top` / `pop`: **O(1)**  
  - Best if the workload has far fewer inserts than removes.

- **`BinaryPQ.hpp`**:  
  Binary heap over `std::vector` with `fixUp/fixDown`.  
  - `push` / `pop`: **O(log n)**  
  - `top`: **O(1)**  
  - Standard choice for balanced workloads.

- **`PairingPQ.hpp`**:  
  Pairing heap with `addNode` and `updateElt` support.  
  - Amortized **O(1)** for `push`, **O(log n)** for `pop`.  
  - Supports efficient **decrease-key** (`updateElt`) operations, which are not natively efficient in a binary heap.  
  - More advanced, but very effective in practice for certain workloads.

---

## Build & Test

This repo includes a simple test driver:

- **`project2b.cpp`** compiles the headers and runs sanity tests:  
  - Primitive operations (`push`, `pop`, `top`)  
  - `updatePriorities` (bulk priority adjustment)  
  - Pairing heap–specific operations (`addNode`, `updateElt`)

Compile and run:

```bash
make
./project2b
