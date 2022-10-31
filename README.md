## Simulation of Paged Memory Allocation

### Make commands
- `make paging` This command creates the compiled `paging` executable file.

### How to Run
- `make paging` to get executable file.
- `./paging` to run executable
    - Enter inputs to perform supported actions based on prompts
    - List of supported actions:
      - Ask for memory allocation: Returns a list of page numbers which have been allocated
      - Ask to free page: Frees the supplied page number
      - Ask for physical address: Returns a physical address, given a page number and offset
      - Set data at address: Sets character data at a supplied page number and offset
      - View data at address: View character data at a supplied page number and offset

### Example:
```
aiyappa@aiyappas-MacBook-Pro A5 % ./paging
Enter choice
 1:Allocate Pages 
 2:Free Memory
 3:View physical address
 4:View data at address
 5:Set data at address
1
Enter size required in bytes
8034
Pages allocated: 0	1	
Enter choice if you wish to continue
 1: Keep Running
 Any other integer to quit
1
Enter choice
 1:Allocate Pages 
 2:Free Memory
 3:View physical address
 4:View data at address
 5:Set data at address
5
Enter page number
0
Enter offset
24
Enter character data: c
Enter choice if you wish to continue
 1: Keep Running
 Any other integer to quit
Invalid choice, please try again: 1
Enter choice
 1:Allocate Pages 
 2:Free Memory
 3:View physical address
 4:View data at address
 5:Set data at address
4
Enter page number
0
Enter offset
24
Data: c
Enter choice if you wish to continue
 1: Keep Running
 Any other integer to quit
3
```

### Deciding best page size

It is hard to decide on an optimal page size without taking into consideration the different types of overhead that might occur.
Smaller page sizes will mean lesser fragmentation, but will lead to a larger overhead when calculating physical addresses due to the 
larger page table. Similarly, having large pages will ensure that overhead due to address translation is smaller, but has a much higher
chance of fragmentation. The best page size should be decided based on the best compromise between these two approaches and available memory.
Dynamic paging would make sense here, as the memory management system can analyze both overhead and memory fragmentation on a running process and 
then adjust the page sizes on the fly to get the best possible performance with minimal fragmentation. This might be a difficult system to
actually implement considering all the factors that would be involved to make this decision, such as when/at which stage of a process should this be decided, 
will the overhead from dynamic paging be better than just sticking to a fixed page size, will it have any implications on accessing memory etc.