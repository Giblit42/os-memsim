#include "mmu.h"
#include "pagetable.h"
Mmu::Mmu(int memory_size)
{
    _next_pid = 1024;
    _max_size = memory_size;
}

Mmu::~Mmu()
{
}

uint32_t Mmu::createProcess()
{
    Process *proc = new Process();
    proc->pid = _next_pid;

    Variable *var = new Variable();
    var->name = "<FREE_SPACE>";
    var->type = DataType::FreeSpace;
    var->virtual_address = 0;
    var->size = _max_size;
    proc->variables.push_back(var);

    _processes.push_back(proc);

    _next_pid++;
    return proc->pid;
}

void Mmu::addVariableToProcess(uint32_t pid, std::string var_name, DataType type, uint32_t size, uint32_t address)
{
    int i;
    Process *proc = NULL;
    for (i = 0; i < _processes.size(); i++)
    {
        if (_processes[i]->pid == pid)
        {
            proc = _processes[i];
        }
    }

    Variable *var = new Variable();
    var->name = var_name;
    var->type = type;
    var->virtual_address = address;
    var->size = size;
    if (proc != NULL)
    {
        proc->variables.push_back(var);
    }
}
uint32_t Mmu::allocate(uint32_t pid, std::string var_name, DataType type, uint32_t num_elements, Mmu *mmu, PageTable *page_table, int page_size){
    int pidStart = -1;
    for(int i = 0; i < _processes.size();i++){
        if(_processes[i]->pid == pid){
            pidStart = i;
        }
    }
    //now check if -1 for the pid actually existing 
    if(pidStart == -1){
        std::cout << "error:process not found" <<std::endl;
        return -1;
    }
    //now need to check for var name existing while allocating 
    int i;
    for(i = 0; i < _processes[pidStart]->variables.size(); i++){
        if(_processes[pidStart]->variables[i]->name == var_name){
            std::cout << "error: process not found" << std::endl;
            return -1;
        }
    }
    //now that error checkin is doen 
    int total_size = num_elements;
    int unit_size = 1;
    
    std::vector<Variable*>::iterator table;

    if(type == DataType::Short){
        total_size = total_size * 2;
        unit_size = 2;
    }else if(type == DataType::Int || type == DataType::Float){
        total_size = total_size *4;
        unit_size = 4;
    }else if(type == DataType::Long || type == DataType::Double){
        total_size = total_size *8;
        unit_size = 8;
    }
    int rem_size = total_size;
    int remaining = num_elements;

    //need to loop through our current processes ang get the variable to find free space if no free space is large enough we need to get a new page 
    int page_size = page_table->getSize();
    for(int i = 0; i < _processes[pidStart]->variables.size(); i++){
        
        if(_processes[pidStart]->variables.at(i)->name == "<FREE_SPACE>"){
            if(_processes[pidStart]->variables.at(i)->size < rem_size){
                //take the free space and make it into the stuff, 
                //
            }else if(_processes[pidStart]->variables.at(i)->size >= rem_size){
                table = mmu->getVariables(pidStart).begin() + i;//this doesnt seem to work 

                //if the free space has more than the remaining size then we can allocate all of it and move one
                Variable *oldSpace = _processes[pidStart]->variables.at(i);
                Variable *newSpace = new Variable();
                int start = oldSpace->virtual_address;
                int end = start + rem_size;
                newSpace->size = total_size;
                newSpace->type = type;
                newSpace->name = var_name;
                oldSpace->size = oldSpace->size - total_size;
                //need to replace the old space and insert our new stuff
                //put the new space before the old space


            }
        }
    }
    //gets the total size that needed along with how large each value will be 

    // TODO: implement this!
    //   - find first free space within a page already allocated to this process that is large enough to fit the new variable
    //start this above^

    //   - if no hole is large enough, allocate new page(s)
    //   - insert variable into MMU
    //   - print virtual memory address
    return -1;
}
Process* Mmu::getProc(uint32_t pid){
    for(int i = 0; i< _processes.size(); i++){
        if(_processes[i]->pid == pid){
            return _processes[i];
        }
    }
    return NULL;
}
void Mmu::print()
{
    int i, j;

    std::cout << " PID  | Variable Name | Virtual Addr | Size" << std::endl;
    std::cout << "------+---------------+--------------+------------" << std::endl;
    for (i = 0; i < _processes.size(); i++)
    {
        for (j = 0; j < _processes[i]->variables.size(); j++)
        {
            int curr_pid = _processes[i]->pid;
            std::string variables = _processes[i]->variables[j]->name;
            int virtual_address = _processes[i]->variables[j]->virtual_address;
            int size = _processes[i]->variables[j]->size;
            // TODO: print all variables (excluding <FREE_SPACE> entries)
            printf("%5u | %s | %d | %d", curr_pid, variables, virtual_address, size);
        }
    }
}
std::vector<Variable*>& Mmu::getVariables(uint32_t pid)
{
	int i;
	for (i = 0; i < _processes.size(); i++)
	{
		if(_processes[i]->pid == pid)
		{
			return _processes[i]->variables;
		}
	}
}
void Mmu::removeVariableFromProcess(uint32_t pid)
{
   int i;
   for(i = 0; i < _processes.size(); i++)
   {
   	if(_processes[i]->pid == pid)
   	{
   		_processes.erase(pid);
   	}
   }
}
void Mmu::freePage(uint32_t pid)
{
	int i;
   for(i = 0; i < _processes.size(); i++)
   {
   	if(_processes[i]->pid == pid)
   	{
   		_processes.erase(pid);
   	}
   }
}
