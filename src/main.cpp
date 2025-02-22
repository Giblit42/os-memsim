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
                voidPoint = commandV.at(i);
                setVariable(static_cast<uint32_t>(std::stoul(commandV.at(1))), commandV.at(2), stoi(commandV.at(3)), &voidPoint, mmu, page_table, memory);
            }
            //setVariable(static_cast<uint32_t>(std::stoul(commandV.at(1))), commandV.at(2), stoi(commandV.at(3)), &voidPoint, mmu, page_table, memory);
        }else if(commandV.at(0)=="free"){
            freeVariable(static_cast<uint32_t>(std::stoul(commandV.at(1))), commandV.at(2), mmu, page_table);
        }else if(commandV.at(0)== "print"){
            if(commandV.at(1)== "mmu"){
                mmu->print();
            }else if(commandV.at(1)== "page"){
                page_table->print();
            }else if(commandV.at(1) == "process"){
                
            }//need pid and varname to print some stuff after
        }
        else if(commands.at(0) == "exit"){
		exit(0);
	}
        else{
            std::cout << "Command not recognized" << std::endl;
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
    printf("PID %d", mmu->createProcess());
    
}

void allocateVariable(uint32_t pid, std::string var_name, DataType type, uint32_t num_elements, Mmu *mmu, PageTable *page_table)
{
	int page_number = 0;
	
	mmu->allocate(pid, var_name, type, num_elements, mmu, page_table->getSize());
    // TODO: implement this!
    //   - find first free space within a page already allocated to this process that is large enough to fit the new variable
    
    //   - if no hole is large enough, allocate new page(s)
    
    
    
    //   - insert variable into MMU
    
    
    
    //   - print virtual memory address 
	uint32_t virtual_address = page_table->getVirtualAddress(page_number, page_table->getSize());

    printf("Virtual address %d", virtual_address);

}

void setVariable(uint32_t pid, std::string var_name, uint32_t offset, void *value, Mmu *mmu, PageTable *page_table, void *memory)
{
    // TODO: implement this!
    //   - look up physical address for variable based on its virtual address / offset

   
    //git virtual from variable name and pid located in mmu
    std::vector<Variable*> var = mmu->getVariables(pid);
    int i;
    uint32_t virtualAdd;
    int dataTypeSize;
    for(i = 0; i < var.size(); i++)
    {
    	if(var[i]->name == var_name)
    	{
    	    virtualAdd = var[i]->virtual_address;
    	    dataTypeSize = sizeof(var[i]->type);
    	}
    }
    
    
    // then add offset to virtual * datasize(of datatype)
    //create method to get the size of the data type
    
    uint32_t virtualPlusOffset = virtualAdd + offset *dataTypeSize;
    
    //this is will be used to get physical address
    int addressvalue = page_table->getPhysicalAddress(pid, virtualPlusOffset);
    
    //   - insert `value` into `memory` at physical address
    memcpy((uint8_t*)memory + addressvalue, value, dataTypeSize);
    
    //git virtual from variable name and pid located in mmu
    Process *curr = mmu->getProc(pid);
    if(curr == NULL){//checks if the pid exists
        std::cout  << "error: process not found" << std::endl;
        return;
    }
    
    //int phys = page_table->getPhysicalAddress(); somehow et the virtual address then get the physical address
    //then just memcpy based on the data type
    /*
    if(type == DataType::Char){
        memcpt(MemLocal, value, 1);
    }else if(type == DataType::Short){
        memcpy(MemLocal, value, 2);
    }else if(type == DataType::Float || type == DataType::Int){
        memcpy(MemLocal, value, 4);
    }else if(type == DataType::Long || type == DataType:: Double){
        memcpy(MemLocal, value, 8);
    }

    */

    

    //use void memory pointer to copy data into memory
    
    //   * note: this function only handles a single element (i.e. you'll need to call this within a loop when setting
    //           multiple elements of an array)
}

void freeVariable(uint32_t pid, std::string var_name, Mmu *mmu, PageTable *page_table)
{
    // TODO: implement this!
    int page_number = 0;
    std::map<std::string, int>::iterator it;
    std::vector<std::string> keys;
    for(it = page_table->_ptable.begin(); it != page_table->_ptable.end(); it++)
    {
    	keys.push_back(it->first);
    }
    
    std::vector<Variable*> var = mmu->getVariables(pid);
    
    int i;
    for(i = 0; i < var.size(); i++)
    {
    	if(var[i]->name == var_name)
    	{
    		var[i]->name = "<FREE_SPACE>";
    		var[i]->type = FreeSpace;
    		
    		if(var[i]->name == var[i+1]->name)
    		{
    			var[i]->size + var[i]->size;
    		}
    		else if(var[i]->name == var[i-1]->name)
    		{
    			var[i]->size + var[i-1]->size;
    		}
    		else
    		{
    			var[i-1]->size + var[i]->size + var[i+1]->size;
    		}
    		//   - remove entry from MMU
    		page_table->_ptable.erase(var[i]->name);
    	}
    }
    
    
    
    //   - free page if this variable was the only one on a given page
    //loop over all the variables and see how many on each page (do this by checking virtual address)
    
    uint32_t virtual_address = page_table->getVirtualAddress(page_number, page_table->getSize());
    std::string str = std::__cxx11::to_string(virtual_address);
    
    for(it = page_table->_ptable.begin(); it != page_table->_ptable.end(); it++)
    {
        //uint32_t virtual_address = page_table->getVirtualAddress(page_number, page_table->getSize());
        //str = to_string(virtual_address);
    	keys.push_back(it->first);
    	
    	if(keys.at(i) == str)
    	{
    	//delete page from page table
    		page_table->deletePage();
    	}
    	// if no other variables have address on the page then the page should become free
    	if(keys.at(i) != str)
    	{
    		page_table->_ptable.erase(it, page_table->_ptable.end());
    	}
    }
    
    
    
}

void terminateProcess(uint32_t pid, Mmu *mmu, PageTable *page_table)
{
    // TODO: implement this!
    //   - remove process from MMU
    // crete method to erase pid
    mmu->removeVariableFromProcess(pid);
    
    //   - free all pages associated with given process   
    // create method erase all page associated with pid
    mmu->freePage(pid);
}
