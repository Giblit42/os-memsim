#ifndef __MMU_H_
#define __MMU_H_

#include <iostream>
#include <string>
#include <vector>

enum DataType : uint8_t {FreeSpace, Char, Short, Int, Float, Long, Double};

typedef struct Variable {
    std::string name;
    DataType type;
    uint32_t virtual_address;
    uint32_t size;
} Variable;

typedef struct Process {
    uint32_t pid;
    std::vector<Variable*> variables;
} Process;

class Mmu {
private:
    uint32_t _next_pid;
    uint32_t _max_size;
    std::vector<Process*> _processes;

public:
    Mmu(int memory_size);
    ~Mmu();
    Process* getProc(uint32_t pid);
    uint32_t allocate(uint32_t pid, std::string var_name, DataType type, uint32_t num_elements, Mmu *mmu,  int page_size);
    uint32_t createProcess();
    void addVariableToProcess(uint32_t pid, std::string var_name, DataType type, uint32_t size, uint32_t address);
    void removeVariableFromProcess(uint32_t pid);
    void freePage(uint32_t pid);
    void print();
    std::vector<Variable*>& getVariables(uint32_t pid);
};

#endif // __MMU_H_
