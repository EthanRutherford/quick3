# quick3
implementation of 3-way-partition quicksort to work on stl containers that implement random access iterators. 
The sort makes use of ethreads (see ethread repo) in an attempt to take advantage of multicore processors. 
The algorithm involves picking a pivot using the median of 3 method, then performing a 3-way partition, < pivot, = pivot, and > pivot, then recursively performing the sort on the first and third partion (the = partition by definition not needing to be sorted). 
For small lists, an optimized insertion sort is performed, since insertion sort is fast for small lists.

The sort, even without threads, performs approximately 5-10% faster than std::sort (tested with MinGW compiler) when when compiled with -O3 on lists of size 1,000,000. On an intel i7 (quadcore, dual threaded) processor, the threaded version outperforms std::sort, taking about 1/3 the time.
