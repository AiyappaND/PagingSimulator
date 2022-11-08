#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#define SIZE_OF_PAGE 4096
#define NUM_PAGES 128

struct pageDirectory
{
    int index;
    int address;
    bool free;
    bool clock;
    int pid;
} pageTable[NUM_PAGES];

char customMemory[SIZE_OF_PAGE * NUM_PAGES];

void initializePageDirectory()
{
  int i;

  for( i = 0; i < NUM_PAGES; i = i + 1 ){
      pageTable[i].index = i;
      pageTable[i].free = true;
      pageTable[i].clock = false;
      pageTable[i].pid = -1;
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

// Returns an integer corresponding to the first free page number
int pm_malloc() {
    int i;
    int freePageNumber = -1;
    for ( i = 0; i < NUM_PAGES; i++) {
      if (pageTable[i].free) {
            pageTable[i].free = false;
            pageTable[i].pid = getpid();
            freePageNumber = i;
            break;
        }
   }
   return freePageNumber;
}

void delete_local_storage_page(int pid, int pageNumber) {

}

void pm_free(int pageNumber) {
    if (pageNumber > NUM_PAGES || pageNumber < 0) {
        printf("Invalid page number\n");
        exit(EXIT_FAILURE);
    }
    else if (pageTable[pageNumber].pid != getpid()) {
       delete_local_storage_page(getpid(), pageNumber);
    }
    else {
        pageTable[pageNumber].free = true;
        pageTable[pageNumber].clock = false;
        pageTable[pageNumber].pid = -1;
    }
    printf("Page freed %d\n", pageNumber);
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



    int physAddress = getPhysicalAddress(pageNumber);
    customMemory[physAddress + offset] = data;
}

void save_page_to_disk(int pageNumber, int pid) {
    FILE *file_local;

    if (pageTable[pageNumber].free == true || pageTable[pageNumber].pid != pid) {
        printf("Unable to save, invalid page number\n");
    }
    else {
        char filename[10];
        snprintf(filename, sizeof filename, "%d_%d\r\n", pid, pageNumber);
        file_local = fopen(filename, "wb"); // wb -write binary
        char * pageToSave = &customMemory[pageTable[pageNumber].address];
        if (file_local != NULL)
        {
            fwrite(pageToSave, SIZE_OF_PAGE, 1, file_local);
            fclose(file_local);
            printf("Page saved\n");
        }
        else
        {
            printf("Failed to save page to disk\n");
            exit(EXIT_FAILURE);
        }
    }

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
        printf("Enter choice\n 1:Allocate Page \n 2:Free Memory\n 3:View physical address\n 4:View data at address\n 5:Set data at address\n 6:Manually save page to disk\n");
        int choice = getUserInputInteger();
        if (choice == 1) {
            int pagesAllocated = pm_malloc();
            printf("Pages allocated: %d\t", pagesAllocated);
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
        else if (choice == 6) {
            printf("Enter page number\n");
            int pageToSave = getUserInputInteger();
            int pid = getpid();
            save_page_to_disk(pageToSave, pid);
        }

        else {
            printf("Invalid choice\n");
        }
        printf("Enter choice if you wish to continue\n 1: Keep Running\n Any other integer to quit\n");
        keepRunning = getUserInputInteger();
    }
    return 0;
}
