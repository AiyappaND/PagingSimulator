#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define SIZE_OF_PAGE 4096
#define NUM_PAGES 128

struct pageDirectory
{
    int index;
    int address;
    bool free;
} pageTable[NUM_PAGES];

char customMemory[SIZE_OF_PAGE * NUM_PAGES];

void initializePageDirectory()
{
  int i;

  for( i = 0; i < NUM_PAGES; i = i + 1 ){
      pageTable[i].index = i;
      pageTable[i].free = true;
      pageTable[i].address = i*SIZE_OF_PAGE;
   }
}

int getPhysicalAddress (int pageNumber) {
    if (pageNumber > NUM_PAGES) {
            printf("Invalid Page Memory\n");
            exit(EXIT_FAILURE);
        }
    return pageTable[pageNumber].address;
}

// Returns an array of integers corresponding to free page numbers
int * pm_malloc(int size) {
    if (size <= 0) {
        printf("Positive bytes required\n");
        exit(EXIT_FAILURE);
    }
    int numPages = size/SIZE_OF_PAGE + (size % SIZE_OF_PAGE != 0);
    if (numPages > NUM_PAGES) {
        printf("Insufficient memory\n");
        exit(EXIT_FAILURE);
    }
    int availablePages = 0;
    int i;
    for (i = 0; i < NUM_PAGES; i++) {
        if (pageTable[i].free) {
            availablePages = availablePages + 1;
        }
    }
    if (availablePages < numPages) {
        printf("Insufficient memory\n");
        exit(EXIT_FAILURE);
    }
    else {
        int * availablePageNumbers = malloc (sizeof (int) * availablePages);
        int allocatedPages = 0;
        for ( i = 0; i < NUM_PAGES; i++) {
          if (pageTable[i].free) {
                availablePageNumbers[allocatedPages] = i;
                allocatedPages = allocatedPages + 1;
                pageTable[i].free = false;
            }
          if (allocatedPages == numPages) {
              break;
          }
       }
       return availablePageNumbers;
    }
}

void pm_free(int pageNumber) {
    if (pageNumber > NUM_PAGES || pageNumber < 0) {
        printf("Invalid page number\n");
        exit(EXIT_FAILURE);
    }
    else {
        pageTable[pageNumber].free = true;
        printf("Page freed %d\n", pageNumber);
    }
}

void set_data(int pageNumber, int offset, char data) {
    if (pageNumber > NUM_PAGES || pageNumber < 0) {
        printf("Invalid page number\n");
        exit(EXIT_FAILURE);
    }
    if (offset > SIZE_OF_PAGE || offset < 0) {
        printf("Invalid offset (0-4095)\n");
        exit(EXIT_FAILURE);
    }
    if (pageTable[pageNumber].free == true) {
        printf("Page has not been allocated\n");
        exit(EXIT_FAILURE);
    }
    int physAddress = getPhysicalAddress(pageNumber);
    customMemory[physAddress + offset] = data;
}

char getData(int pageNumber, int offset) {
    if (pageNumber > NUM_PAGES || pageNumber < 0) {
        printf("Invalid page number\n");
        exit(EXIT_FAILURE);
    }
    if (offset > SIZE_OF_PAGE || offset < 0) {
        printf("Invalid offset (0-4095)\n");
        exit(EXIT_FAILURE);
    }
    if (pageTable[pageNumber].free == true) {
        printf("Page has not been allocated\n");
        exit(EXIT_FAILURE);
    }
    int physAddress = getPhysicalAddress(pageNumber);
    char c = customMemory[physAddress + offset];
    return c;
}

int getUserInputInteger() {
    int result;
    char *line = NULL;
    size_t len = 0;
    while (1) {
       if (getline(&line, &len, stdin) == -1) {
           /* eof or error, do whatever is sensible in your case */
           exit(EXIT_FAILURE);
       }
       if (sscanf(line, "%d", &result) != 1) {
           printf("Invalid choice, please try again: ");
           continue;
       }
       break;
    }
    return result;
}

int main()
{
    initializePageDirectory();
    int keepRunning = 1;
    char dataScanned;
    while (keepRunning == 1) {
        printf("Enter choice\n 1:Allocate Pages \n 2:Free Memory\n 3:View physical address\n 4:View data at address\n 5:Set data at address\n");
        int choice = getUserInputInteger();
        if (choice == 1) {
            int size;
            printf("Enter size required in bytes\n");
            size = getUserInputInteger();
            int * pagesAllocated = pm_malloc(size);
            printf("Pages allocated: ");
            int numPages = size/SIZE_OF_PAGE + (size % SIZE_OF_PAGE != 0);
            int i;
            for (i = 0; i < numPages; i++) {
                printf("%d\t", pagesAllocated[i]);
            }
            printf("\n");
        }
        else if (choice == 2) {
            printf("Enter page number to deallocate\n");
            int pageToFree = getUserInputInteger();
            pm_free(pageToFree);

        }
        else if (choice == 3) {
            printf("Enter page number to view start address\n");
            int pageToView = getUserInputInteger();
            int addressToView = getPhysicalAddress(pageToView);
            printf("Physical address: %d\n", addressToView);
        }
        else if (choice == 4) {
            printf("Enter page number\n");
            int pageToView = getUserInputInteger();

            printf("Enter offset\n");
            int offset = getUserInputInteger();

            char dataRead = getData(pageToView, offset);
            printf("Data: %c\n", dataRead);
        }
        else if (choice == 5) {
            printf("Enter page number\n");
            int pageToView = getUserInputInteger();

            printf("Enter offset\n");
            int offset = getUserInputInteger();

            printf("Enter character data: ");
            scanf(" %c", &dataScanned);

            set_data(pageToView, offset, dataScanned);
        }

        else {
            printf("Invalid choice\n");
        }
        printf("Enter choice if you wish to continue\n 1: Keep Running\n Any other integer to quit\n");
        keepRunning = getUserInputInteger();
    }
    return 0;
}
