#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>

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


void save_page_to_disk(int pageNumber, int pid) {
    FILE *file_local;
    if (pageTable[pageNumber].free == true || pageTable[pageNumber].pid != pid) {
        printf("Unable to save, invalid page number\n");
    }
    else {
        char filename[10];
        snprintf(filename, sizeof filename, "%d_%d", pid, pageNumber);
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

int findNumPagesAllocatedToPid(int pid) {
    int num = 0;
    int i;
    for ( i = 0; i < NUM_PAGES; i++) {
      if (pageTable[i].pid == pid) {
            num = num +1;
        }
   }
   return num;
}

int findPageToFree() {
    int pageToFree = -1;
    int i;
    int currentPid = getpid();
    for ( i = 0; i < NUM_PAGES; i++) {
      if (pageTable[i].clock == false) {
            pageToFree = i;
            break;
        }
        else {
            pageTable[i].clock = false;
        }
   }
   if (pageToFree == -1) {
        for ( i = 0; i < NUM_PAGES; i++) {
          if (pageTable[i].clock == false) {
                pageToFree = i;
                break;
            }
       }
   }

   return pageToFree;

}

// Returns an integer corresponding to the first free page number
int pm_malloc() {
    int i;
    int freePageNumber = -1;

    for ( i = 0; i < NUM_PAGES; i++) {
      if (pageTable[i].free == true) {
            pageTable[i].free = false;
            pageTable[i].pid = getpid();
            freePageNumber = i;
            break;
        }
   }
   if (freePageNumber == -1) {
        freePageNumber = findPageToFree();
        save_page_to_disk(freePageNumber, pageTable[freePageNumber].pid);
        pageTable[i].free = false;
        pageTable[i].pid = getpid();
   }
   return freePageNumber;
}

void delete_local_storage_page(int pid, int pageNumber) {
    char filename[10];
    snprintf(filename, sizeof filename, "%d_%d", pid, pageNumber);
    remove(filename);
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
        delete_local_storage_page(getpid(), pageNumber);
    }
    printf("Page freed %d\n", pageNumber);
}

int load_page_into_memory(int pageNumber, int pid) {
    FILE *myFile;
    char filename[10];
    snprintf(filename, sizeof filename, "%d_%d", pid, pageNumber);
    myFile = fopen(filename, "r");
    char * buffer = &customMemory[pageTable[pageNumber].address];
    if (myFile) {
        if( 1!=fread( buffer , SIZE_OF_PAGE, 1 , myFile) ) {
            printf("Could not read saved page\n");
            fclose(myFile);
            return -1;
        }
        else {
            pageTable[pageNumber].pid = pid;
            pageTable[pageNumber].free = false;
            pageTable[pageNumber].clock = false;
            fclose(myFile);
            return 0;
        }
    }
    else {
        printf("Page does not exist\n");
        return -1;
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
    if (pageTable[pageNumber].pid != getpid()) {
        save_page_to_disk(pageNumber, pageTable[pageNumber].pid);
        if (load_page_into_memory(pageNumber, getpid()) != 0) {
            printf("Invalid page number\n");
            exit(EXIT_FAILURE);
        }
    }
    int physAddress = getPhysicalAddress(pageNumber);
    customMemory[physAddress + offset] = data;
    pageTable[pageNumber].clock = true;
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
    if (pageTable[pageNumber].pid != getpid()) {
        save_page_to_disk(pageNumber, pageTable[pageNumber].pid);
        if (load_page_into_memory(pageNumber, getpid()) != 0) {
            printf("Invalid page number\n");
            exit(EXIT_FAILURE);
        }
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
        printf("Enter choice\n 1:Allocate Page \n 2:Free Memory\n 3:View physical address\n 4:View data at address\n 5:Set data at address\n 6:Manually save page to disk\n 7:Manually load page from disk\n 8:Simulate page fault with overallocation\n");
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
        else if (choice == 7) {
            printf("Enter page number\n");
            int pageToLoad = getUserInputInteger();
            int pid = getpid();
            load_page_into_memory(pageToLoad, pid);
        }
        else if (choice == 8) {
           int child_pages;
           for (child_pages = 0; child_pages < NUM_PAGES; child_pages ++) {
                int pageAllocated = pm_malloc();
                int addressToView = getPhysicalAddress(pageAllocated);
                int offset = 0;
                char child_char = 'c';
                for (offset = 0; offset < SIZE_OF_PAGE; offset ++) {
                    set_data(pageAllocated, offset, child_char);
                }
           }
           printf("Normal allocation done\n");
           for (child_pages = 0; child_pages < 3; child_pages ++) {
                int pageAllocated = pm_malloc();
                int addressToView = getPhysicalAddress(pageAllocated);
                int offset = 0;
                char child_char = 'p';
                for (offset = 0; offset < SIZE_OF_PAGE; offset ++) {
                    set_data(pageAllocated, offset, child_char);
                }
           }
           printf("Extra allocation done\n");
        }

        else {
            printf("Invalid choice\n");
        }
        printf("Enter choice if you wish to continue\n 1: Keep Running\n Any other integer to quit\n");
        keepRunning = getUserInputInteger();
    }
    return 0;
}
