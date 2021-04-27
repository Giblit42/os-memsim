#include <iostream>
#include <string>
#include <cstring>
#include "mmu.h"
#include "pagetable.h"
#include <stdio.h>
#include <vector>

void printStartMessage(int page_size);
void createProcess(int text_size, int data_size, Mmu *mmu, PageTable *page_table);
void allocateVariable(uint32_t pid, std::string var_name, DataType type, uint32_t num_elements, Mmu *mmu, PageTable *page_table);
void setVariable(uint32_t pid, std::string var_name, uint32_t offset, void *value, Mmu *mmu, PageTable *page_table, void *memory);
void freeVariable(uint32_t pid, std::string var_name, Mmu *mmu, PageTable *page_table);
void terminateProcess(uint32_t pid, Mmu *mmu, PageTable *page_table);
void *memory = malloc(1000);

int main(int argc, char **argv)
{
	std::vector<std::string> commands;
    // Ensure user specified page size as a command line parameter
    if (argc < 2)
    {
        fprintf(stderr, "Error: you must specify the page size\n");
        return 1;
    }
    int text_size = 0;

    // Print opening instuction message
    int page_size = std::stoi(argv[1]);
    printStartMessage(page_size);

    // Create physical 'memory'
    uint32_t mem_size = 67108864;
    void *memory = malloc(mem_size); // 64 MB (64 * 1024 * 1024)

    // Create MMU and Page Table
    Mmu *mmu = new Mmu(mem_size);
    PageTable *page_table = new PageTable(page_size);

    // Prompt loop
    std::string command;
    std::cout << "> ";
    std::getline (std::cin, command);
    while (command != "exit") {
    	char *token = strtok(command, " ");
    	while(token != null)
    	{
    	   commands.push_back(token);
    	}
    	
        // Handle command
        // TODO: implement this!
	if(commands.at(0) == "create"){
		createProcess(commands.at(1), commands.at(2), commands.at(3), commands.at(4));
	}
	if(commands.at(0) == "allocate"){
		allocateVariable(commands.at(1), commands.at(2), commands.at(3), commands.at(4), commands.at(5), commands.at(6));
	}
	if(commands.at(0) == "set"){
		setVariable(commands.at(1), commands.at(2), commands.at(3), commands.at(4), commands.at(5), commands.at(6), commands.at(7));
	}
	if(commands.at(0) == "free"){
		freeVariable(commands.at(1), commands.at(2), commands.at(3), commands.at(4));
	}
	if(commands.at(0) == "terminate"){
		terminateProcess(commands.at(1), commands.at(2), commands.at(3));
	}
	if(commands.at(0) == "print"){
		printStartMessage(commands.at(1));
	}
	if(commands.at(0) == "exit"){
		exit(0);
	}
        // Get next command
        std::cout << "> ";
        std::getline (std::cin, command);
    }

    // Cean up
    free(memory);
    delete mmu;
    delete page_table;

    return 0;
}

void printStartMessage(int page_size)
{
    std::cout << "Welcome to the Memory Allocation Simulator! Using a page size of " << page_size << " bytes." << std:: endl;
    std::cout << "Commands:" << std:: endl;
    std::cout << "  * create <text_size> <data_size> (initializes a new process)" << std:: endl;
    std::cout << "  * allocate <PID> <var_name> <data_type> <number_of_elements> (allocated memory on the heap)" << std:: endl;
    std::cout << "  * set <PID> <var_name> <offset> <value_0> <value_1> <value_2> ... <value_N> (set the value for a variable)" << std:: endl;
    std::cout << "  * free <PID> <var_name> (deallocate memory on the heap that is associated with <var_name>)" << std:: endl;
    std::cout << "  * terminate <PID> (kill the specified process)" << std:: endl;
    std::cout << "  * print <object> (prints data)" << std:: endl;
    std::cout << "    * If <object> is \"mmu\", print the MMU memory table" << std:: endl;
    std::cout << "    * if <object> is \"page\", print the page table" << std:: endl;
    std::cout << "    * if <object> is \"processes\", print a list of PIDs for processes that are still running" << std:: endl;
    std::cout << "    * if <object> is a \"<PID>:<var_name>\", print the value of the variable for that process" << std:: endl;
    std::cout << std::endl;
}

void createProcess(int text_size, int data_size, Mmu *mmu, PageTable *page_table)
{
    // TODO: implement this!
    
    //what is data_size used for? data_size is for globals
    // text size is for text
    //   - create new process in the MMU
    mmu->createProcess();
    //   - allocate new variables for the <TEXT>, <GLOBALS>, and <STACK>
    void *text = malloc(2048 * sizeof(int32_t));
    void *globals = malloc(1024 * sizeof(int32_t));
    void *stack = malloc(65536 * sizeof(int32_t));
    
    //   - print pid
    //I think the pid comes from createProcess
    printf("PID %s", mmu->createProcess());
    
}

void allocateVariable(uint32_t pid, std::string var_name, DataType type, uint32_t num_elements, Mmu *mmu, PageTable *page_table)
{
	int page_number = 0;
	//would the virtual_address be the num_elements?
	// what is var_name used for?
	// what is type used for? helps determine how big of a space you need
	//what is num_elements used for?
	
    // TODO: implement this!
    //   - find first free space within a page already allocated to this process that is large enough to fit the new variable
    pagetable->addEntry(pid, page_number);
    //   - if no hole is large enough, allocate new page(s)
    
    // how do I all ocate new pages? Is it by increasing the page size or adding more pages?
    
    //   - insert variable into MMU
    //whaere do I get the virtual address from?
    mmu->addVariableToProcess(pid, var_name, type, size, pagetable->getPhysicalAddress(pid, num_elements));
    
    //   - print virtual memory address 
    printf("Virtual address %s", virtual_address);

}

void setVariable(uint32_t pid, std::string var_name, uint32_t offset, void *value, Mmu *mmu, PageTable *page_table, void *memory)
{
    // TODO: implement this!
    //   - look up physical address for variable based on its virtual address / offset
    //git virtual from variable name and pid located in mmu
    // then add offset to virtual * datasize(of datatype)
    //this is will be used to get physical address
    
    // do I need to find the virtual offset or can I pass in the offset?
    pagetable->getPhysicalAddress(pid, offset);
    //   - insert `value` into `memory` at physical address
    
    // would we call addEntry here?
    //use void memory pointer to copy data into memory
    
    //   * note: this function only handles a single element (i.e. you'll need to call this within a loop when setting
    //           multiple elements of an array)
}

void freeVariable(uint32_t pid, std::string var_name, Mmu *mmu, PageTable *page_table)
{
    // TODO: implement this!
    //   - remove entry from MMU
    
    // to remove somthing do I need to dereference it?
    //in mmu change variable name to free space type to free space
    // double check if the things before and after are free if so join free space
    var_name = mmu;
    
    //   - free page if this variable was the only one on a given page
    // if no other variables have address on the page then the page should become free
    //page table is a map 
    for(int i = 0; i < page_size; i++)
    {
    //how do I free a page?
    	if(page_table == null)
    	{
    		page_table = null;
    	}
    }
}

void terminateProcess(uint32_t pid, Mmu *mmu, PageTable *page_table)
{
    // TODO: implement this!
    //   - remove process from MMU
    
    // just delete everything associated with the PID
    mmu = pid;
    //   - free all pages associated with given process
    page_table = pid;
}
