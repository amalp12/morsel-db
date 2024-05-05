#include "static.h"
#include <numa.h>
#include <numaif.h>

// declare static vars
int StaticVars::numCores;
int StaticVars::numNumaNodes;
int StaticVars::maxMorselSize;
std::vector<int> StaticVars::coreNumaMap;

void StaticVars::setNumberOfCores(int cores) {
    numCores = cores;
    coreNumaMap.clear();
    if(numNumaNodes <= 0) {
        return;
    }
    coreNumaMap = std::vector<int>(cores,-1);
    for(int i=0 ; i < cores; i++) {
        coreNumaMap[i] = i % numNumaNodes ;
    }
}

int StaticVars::getNumaID(int core){ 
    if(coreNumaMap.size() == 0)
        return -1;
    return coreNumaMap[core]; 
}

int getNumaIdFromMemoryAddress(void *mem) {
    /*here you should align ptr_to_check to page boundary */
    int status[1];
    int ret_code;
    status[0]=-1;
    ret_code=move_pages(0 /*self memory */, 1, &mem,
        NULL, status, 0);
    return status[0];
}
