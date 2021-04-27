#include <iostream>
#include <string>
#include <cstring>
#include "mmu.h"
#include "pagetable.h"
#include <stdio.h>
#include <vector>
#include <bits/stdc++.h>
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
        std::vector<std::string> commandV;
    	int i;
        std::string temp = "";
        for(i = 0; i< command.size(); i++){
            if(command.at(i) == ' '){
                commandV.push_back(temp);
                temp = "";
            }else{
                temp = temp + command.at(i);
            }
        }
            	
        // Handle command
        // TODO: implement this!

        if(commandV.at(0)== "create"){
            createProcess(stoi(commandV.at(1)), stoi(commandV.at(2)), mmu, page_table);
        }else if(commandV.at(0)== "allocate"){
            DataType type;

            if(commandV.at(3)=="char"){
                type = Char;
                allocateVariable(stoi(commandV.at(1)), commandV.at(2), type, stoi(commandV.at(4)), mmu, page_table);
            }else if(commandV.at(3)=="short"){
                type = Short;
                allocateVariable(stoi(commandV.at(1)), commandV.at(2), type, stoi(commandV.at(4)), mmu, page_table);
            }else if(commandV.at(3)=="int"){
                type = Int;
                allocateVariable(stoi(commandV.at(1)), commandV.at(2), type, stoi(commandV.at(4)), mmu, page_table);
            }else if(commandV.at(3)=="float"){
                type = Float;
                allocateVariable(stoi(commandV.at(1)), commandV.at(2), type, stoi(commandV.at(4)), mmu, page_table);
            }else if(commandV.at(3)=="long"){
                type = Long;
                allocateVariable(stoi(commandV.at(1)), commandV.at(2), type, stoi(commandV.at(4)), mmu, page_table);
            }else if(commandV.at(3)=="double"){
                type = Double;
                allocateVariable(stoi(commandV.at(1)), commandV.at(2), type, stoi(commandV.at(4)), mmu, page_table);
            }else{
                std::cout << "type not recognized" << std::endl;
            }
        }else if(commandV.at(0)=="set"){
            //set wacky since it can have a bunch of values 
            //void pointer eh?
            std::string voidPoint = "";
            for(int i = 3; i < commandV.size(); i++){
                voidPoint = voidPoint + commandV.at(1);
                if(i != commandV.size()){
                    voidPoint = voidPoint + " ";
                }
            }
            setVariable(static_cast<uint32_t>(std::stoul(commandV.at(1))), commandV.at(2), stoi(commandV.at(3)), &voidPoint, mmu, page_table, memory);
        }else if(commandV.at(0)=="free"){
            freeVariable(static_cast<uint32_t>(std::stoul(commandV.at(1))), commandV.at(2), mmu, page_table);
        }else if(commandV.at(0)== "print"){
            if(commandV.at(1)== "mmu"){
                mmu->print();
            }else if(commandV.at(1)== "page"){
                page_table->print();
            }else if(commandV.at(1) == "process"){
                
            }//need pid and varname to print some stuff after
        }else{
            std::cout << "Command not recognized" << std::endl;
        }


	if(commands.at(0) == "exit"){
		exit(0);
	}
	else{
		printf("error: command not recognized");
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
    
    //   - create new process in the MMU
    mmu->createProcess();
    //   - allocate new variables for the <TEXT>, <GLOBALS>, and <STACK>
    void *text = malloc(2048 * sizeof(text_size));
    void *globals = malloc(1024 * sizeof(data_size));
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
    page_table->addEntry(pid, page_number);
    //   - if no hole is large enough, allocate new page(s)
    
    // how do I all ocate new pages? Is it by increasing the page size or adding more pages?
    
    //   - insert variable into MMU
    //whaere do I get the virtual address from?
    mmu->addVariableToProcess(pid, var_name, type, size, page_table->getPhysicalAddress(pid, num_elements));
    
    //   - print virtual memory address 

    printf("Virtual address %s", virtual_address);


    //printf("Virtual address %s", virtual_address);

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
    
    //in mmu change variable name to free space type to free space
    mmu->var->name = "<FREE_SPACE>";
    
    // double check if the things before and after are free if so join free space
    //?
    for(int i = 0; i < page_size; i++)
    {
    	if(mmu->var->i == mmu->var->i+1)
    	{
    		mmu->var->size->i + mmu->var->size->i+1;
    	}
    	else if(mmu->var->i == mmu->var->i-1)
    	{
    		mmu->var->size->i + mmu->var->size->i-1;
    	}
    	else if(mmu->var->i == mmu->var->i-1 && mmu->var->i == mmu->var->i+1)
    	{
    		mmu->var->size->i + mmu->var->size->i-1 + mmu->var->size->i+1;
    	}
    }
    
    //   - free page if this variable was the only one on a given page
    page_table.clear();
    // if no other variables have address on the page then the page should become free
    
    //page table is a map 
    
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
