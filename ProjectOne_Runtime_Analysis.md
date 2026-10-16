# Project One: Runtime and Memory Analysis

## Data Structure Evaluation

Project One compared three data structures for the ABCU advising program: a vector, a hash table, and a binary search tree. The program needed to load course data, locate individual courses, and print the complete course list in alphanumeric order.

| Data Structure | Load / Insert | Search | Sorted Course List | Memory | Advantages | Disadvantages |
|---|---|---|---|---|---|---|
| Vector | O(n) to append all courses, assuming amortized O(1) append | O(n) | O(n log n) if the vector must be sorted first | O(n) | Simple structure, low overhead, easy to iterate through | Searching requires a linear scan unless the data is kept sorted; producing a sorted list adds sorting work |
| Hash Table | O(n) average to insert all courses | O(1) average, O(n) worst case | O(n log n) because the entries must be collected and sorted | O(n), with additional bucket/table overhead | Very fast average lookup by course number | Does not maintain alphanumeric order, so extra work is required to print a sorted course list |
| Binary Search Tree | O(n log n) average to insert all courses; O(n²) worst case if the tree becomes unbalanced | O(log n) average, O(n) worst case | O(n) using in-order traversal | O(n), with additional left/right node pointers | Supports searching and naturally produces sorted output through in-order traversal | Performance can degrade to linear behavior when the tree is badly unbalanced |

## Runtime Discussion

A vector is straightforward to load because each course can be appended as it is created. The primary weakness is course lookup. In the worst case, the requested course is at the end of the vector or is not present, so every course must be checked. This gives the search a runtime of O(n). If the vector is not already sorted, producing the required alphanumeric course list also requires a sorting step, normally O(n log n).

A hash table is strong when the most important operation is finding a course by course number. With a good hash function and a reasonable load factor, insertion and lookup are O(1) on average. The tradeoff is that a hash table does not preserve the courses in alphanumeric order. To satisfy the course-list requirement, the entries have to be copied into another structure or their keys collected and sorted, which requires O(n log n) time.

A binary search tree provides a useful compromise for this application. When the tree is reasonably balanced, insertion and lookup are O(log n). More importantly, an in-order traversal prints the courses directly in alphanumeric order in O(n) time because the course number is used as the tree key. The disadvantage is that a basic binary search tree is not guaranteed to stay balanced. In the worst case, insertion and search can degrade to O(n), and loading n ordered items can approach O(n²).

## Memory Discussion

All three structures require O(n) space because each course must be stored once. Their overhead differs. A vector stores its elements contiguously and generally has relatively small overhead, although it may reserve extra capacity. A hash table needs an array of buckets in addition to the course records. A binary search tree stores node links for the left and right child of each course, which also adds overhead. These differences do not change the asymptotic memory requirement of O(n), but they affect the amount of memory used in practice.

## Recommendation

For the advising-program requirements, the binary search tree is a strong choice because the program needs both course lookup and an alphanumerically ordered course list. An in-order traversal provides the sorted list without a separate sorting step, while average search performance is better than a linear vector search. The main limitation is the possibility of an unbalanced tree. In a larger production system, a self-balancing tree such as an AVL or red-black tree would remove much of that risk.

The comparison also showed that there is no single data structure that is best for every operation. The correct choice depends on what the application needs to do most often and whether lookup speed, ordering, memory overhead, or implementation simplicity is the most important requirement.
