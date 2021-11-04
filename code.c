#include <stdio.h>
#include "assignment_1.h"
#include <stdlib.h>

char *p;    //global character array
int size;   //global character array's size

struct bookkeep
{
    char empty;                 //field to indicate if the memory the bookkeep is pointing to is free or not, '1' means its allocated,'2' means its free
    unsigned int size;          //size of the memory space pointed to by this bookkeep
    struct bookkeep *prev;      //points to the next block of memory(next bookkeep)
    struct bookkeep *next;      //points to the previous block of memory(previous bookkeep)
};

void allocate(int n)//function to allocate memory for the global array p, n number of bytes long.
{
    p=malloc(n);                //getting global memory space of n bytes
    size=n;
    struct bookkeep *head=(struct bookkeep *)p;     //making the first bookkeep. Memory can be allocated further starting from this
    head->empty='2';                                //initially its empty so
    head->size= size-sizeof(struct bookkeep);       //now this bookkeep points to a memory space of the total size(excluding the space for the bookkeep itself) as nothing is allocated yet
    head->prev=NULL;                                //since its the first bookkeep no previous bookkeep
    head->next=NULL;                                //no new memory space allocated yet so no next bookkeep
}

void* mymalloc(int size) //function to allocate a block of size "size" from p
{
    struct bookkeep *temp=(struct bookkeep *)p;     //used to traverse the entire memory space(traverses all the bookkeeps)
    struct bookkeep *best=NULL;                        //used to point to the bookkeep which has the best fit for size bytes
    unsigned int min=~1;                            //used to hold the size of the best fit memory space
    int bksize=sizeof(struct bookkeep);
    
    while(temp!=NULL)
    {
        if(temp->size<min && temp->size>=size && temp->empty=='2')      //best fit checked by:
        {                                                               //1)checking if its free
            min=temp->size;                                             //2)checking if it has atleast size bytes for allocating
            best=temp;                                                     //3)checking if its the smallest memory block satisfying 1 and 2
        }
        temp=temp->next;
    }
    
    if(best==NULL)                                                         //if no suitable memory space is found
    {
        return best;
    }
    else
    {
        best->empty='1';                                                                //since its no longer free
        if(best->size>size+bksize)                                                      //if current block has enough memory space to make another block of free space
        {
            struct bookkeep *newblk=(struct bookkeep *)(((char *)best)+bksize+size);    //bookkeep pointing to the remaining free space after allocation
            newblk->empty='2';
            newblk->size= best->size-bksize-size;                                       //calculating remaining space
            newblk->prev=best;
            newblk->next=best->next;
            best->size=size;
            if(best->next!=NULL)
            {
            best->next->prev=newblk;
            }
            best->next=newblk;
        }
        return (void *)(((char *)best)+bksize);                                         //pointing to the first byte after the bookkeep(from where user can store his data)
    }                                                                                //made char * so as to increment the address by 1(byte size) instead of struct size
}                                                                                    //finally returned as void *

void myfree(void *b) //free the block pointed to by the parameter
{
    int bksize=sizeof(struct bookkeep);
    struct bookkeep *blk=(struct bookkeep *)(((char *)b)-bksize);                    //accessing the bookeep of the corresponding space
    blk->empty='2';                                     
    struct bookkeep *nxtblk=blk->next;                              
    struct bookkeep *prvblk=blk->prev;
    if(nxtblk!=NULL && nxtblk->empty=='2')                                           //checking if the next memory block is also empty so as to merge them to make a single free block
    {
        blk->size=blk->size+bksize+nxtblk->size;
        blk->next=nxtblk->next;
        if(nxtblk->next!=NULL)
        {
            nxtblk->next->prev=blk;
        }
    }
    
    if(prvblk!=NULL && prvblk->empty=='2')                                           //checking if previous block is also free so as to merge them
    {
        prvblk->size=prvblk->size+bksize+blk->size;
        prvblk->next=blk->next;
        if(blk->next!=NULL)
        blk->next->prev=prvblk;
    }
    
}

void print_book() // prints bytes used by the bookkeeping structure
{
    printf("%ld\n",sizeof(struct bookkeep));
}

void display_mem_map() // print the memory array p in the following format
{                        // start_addr   block_in_bytes  encoded_status
    int bksize=sizeof(struct bookkeep);
    struct bookkeep *t=(struct bookkeep *)p;                                        //used to traverse all memory blocks
    struct bookkeep *head=t;                                                        //head has address of first byte of memory bank, used to get relative address
    while(t!=NULL)
    {
        printf("%ld\t%d\t%c\n",(char *)(t)-(char *)(head),bksize,'0');               //prints details of bookkeep
        printf("%ld\t%d\t%c\n",(char *)(t)-(char *)(head)+bksize,t->size,t->empty);  //prints details of memory space pointed by this bookkeep
        t=t->next;                                                                  //using char * so as to get the difference in bytes(instead of size of datatype)
    }
}
