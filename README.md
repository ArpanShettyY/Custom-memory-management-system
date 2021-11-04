# Custom-memory-management-system
Using C, making my own malloc and free functions using the best fit policy for assigning memory blocks more efficiently.

**1)Allocation policy used:**
                            Best Fit was used. Here out of all the memory blocks the memory space corresponding closest to the requested size is found(the found size is of course larger than the requested size).Also the memory block must be free.

**2)Basic logic used for implementation:**
                                        For every block of memory a bookkeeping is used which contains details of that memory block like its size or if its free,etc. Further using a doubly linked list implementation it is easy to move from one memory block to other and to perform other operations on them.
                                        
                                        Allocation:
                                        Initially a bookkeep is made for the entire block and marked as free. From here the space starts getting allocated. With each space allocated , the remaining space is made into a new free block if possible. If not the current block is allocated more than the requested size.
                                        The new space is selected based on:
                                        1)If its free/empty
                                        2)If it is large enough to allocate required number of bytes
                                        3)If its the smallest block among al the blocks satisfying 1 and 2

                                        Freeing:
                                        For freeing the memory space sent, just the bookkeping details of that space is changed to free/empty. Further the previous and the next block are checked if they are free too. if its so then they are merged. This is done by simply taking the earlier memory block , dissolving rest of the bookkeep and increasing the size of the earlier block to the total of all the memory blocks + their bookkeep.


**3)Explanation of the book-keeping structure:**
                                               Anywhere in the code no memory location is directly accessed. Every access starts from the first bookkeep(head) and from their all the other bookkeep is accessed which in turn lets us access their memory space. The bookkeep is made just before the start address of the allocated space to the user. For easy merging, traversal and deletion, it takes the form of a doubly linked list. The book-keeping structure has 3 parts: 
                                               1)empty: Made of char datatype so that it takes only 1 byte. Its a field to indicate if the memory the bookkeep is pointing to is free or not, 						'1' means its allocated,'2' means its free.
                                               2)size: Of unsigned int data type to represent all possible range of memory required. It gives the size of the memory block its pointing to in 						 bytes.
                                               3)struct bookkeep *prev  :    points to the next block of memory(next bookkeep)
                                               4)struct bookkeep *next  :    points to the previous block of memory(previous bookkeep)

**4)Additional notes:**
                            The bookkeeping structure currently takes 24bytes. It could have been made smaller by omitting some fields like prev and next(and even empty by slight modifications). But since effeciency is not a criteria , I have chosen this structure as it not only gives direct access to adjacent blocks but also helps in merging, traversing and allocating blocks easily. Also traversal is possible without accessing the actual memory block but just their bookkeeps.
