# TwitHandler
Design and implement top trending twitter handlers

Project focus on implementing a system to get the n most popular hash tag appeared on social network i.e. Facebook or Twitter etc.
Project focus on the usage of below Data Structures:

1. Max Fibonacci Heap: A Fibonacci heap is a data structure for priority queue operations, consisting of a collection of heap-ordered trees. It has a better amortized running time than many other priority queue data structures including the binary heap and binomial heap.
In our project, it will be used to keep track of the frequency/occurrence of each hash tags. N most trending/used hash tags can be found out by performing n times remove_max operation on Fibonacci Heap.
Time Complexity:
 
Operation    | Actual | Amortized  |

Insert       |  O(1)  |  O(1)      |

Remove Max   |  O(n)  |  O(log n)  |

Meld         |  O(1)  |  O(1)      |

Increase Key |  O(n)  |  O(1)      |

2. Hash Table: It is used to keep the track of each hash tags. Key of hash table will be hash tag and value will be pointers to node having that hash tag in Max Fibonacci Heap.
Primarily hash table will be used to check whether the incoming hash tags should yield insert or increase occurrence in Fibonacci heap.
For example: If hash tag<key, value> is not present in Fibonacci heap than perform insert (key, value) else increasekey (key, value). Node pointer needed for increase key operation, will be provided from value of hash table.
