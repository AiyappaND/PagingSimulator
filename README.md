## Simulation of Paged Memory Allocation with Page Fault Handling using Clock

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
      - Manually save a page to disk
      - Manually load a page from disk
      - Simulate page faults with overallocation

### Example:
```
Enter choice
 1:Allocate Page 
 2:Free Memory
 3:View physical address
 4:View data at address
 5:Set data at address
 6:Manually save page to disk
 7:Manually load page from disk
 8:Simulate page fault with overallocation
1
Pages allocated: 0	
Enter choice if you wish to continue
 1: Keep Running
 Any other integer to quit
1
Enter choice
 1:Allocate Page 
 2:Free Memory
 3:View physical address
 4:View data at address
 5:Set data at address
 6:Manually save page to disk
 7:Manually load page from disk
 8:Simulate page fault with overallocation
5
Enter page number
0
Enter offset
0
Enter character data: c
Enter choice if you wish to continue
 1: Keep Running
 Any other integer to quit
Invalid choice, please try again: 1
Enter choice
 1:Allocate Page 
 2:Free Memory
 3:View physical address
 4:View data at address
 5:Set data at address
 6:Manually save page to disk
 7:Manually load page from disk
 8:Simulate page fault with overallocation
6
Enter page number
0
Page saved
Enter choice if you wish to continue
 1: Keep Running
 Any other integer to quit
1
Enter choice
 1:Allocate Page 
 2:Free Memory
 3:View physical address
 4:View data at address
 5:Set data at address
 6:Manually save page to disk
 7:Manually load page from disk
 8:Simulate page fault with overallocation
5
Enter page number
0
Enter offset
0
Enter character data: d
Enter choice if you wish to continue
 1: Keep Running
 Any other integer to quit
Invalid choice, please try again: 1
Enter choice
 1:Allocate Page 
 2:Free Memory
 3:View physical address
 4:View data at address
 5:Set data at address
 6:Manually save page to disk
 7:Manually load page from disk
 8:Simulate page fault with overallocation
4
Enter page number
0
Enter offset
0
Data: d
Enter choice if you wish to continue
 1: Keep Running
 Any other integer to quit
1
Enter choice
 1:Allocate Page 
 2:Free Memory
 3:View physical address
 4:View data at address
 5:Set data at address
 6:Manually save page to disk
 7:Manually load page from disk
 8:Simulate page fault with overallocation
7
Enter page number
0
Enter choice if you wish to continue
 1: Keep Running
 Any other integer to quit
1
Enter choice
 1:Allocate Page 
 2:Free Memory
 3:View physical address
 4:View data at address
 5:Set data at address
 6:Manually save page to disk
 7:Manually load page from disk
 8:Simulate page fault with overallocation
4
Enter page number
0
Enter offset
0
Data: c
Enter choice if you wish to continue
 1: Keep Running
 Any other integer to quit
4
```
