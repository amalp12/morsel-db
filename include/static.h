#pragma once
#include <vector>
// ifndef guard
// #ifndef STATIC_H
// #define STATIC_H
// declare static integers for number of cores and max morsel size

// write a static class to set the number of cores and max morsel size
class StaticVars {
private:
  static int numCores;
  static int maxMorselSize;
  static int numNumaNodes;
  static std::vector<int> coreNumaMap;

public:
  static void setNumberOfCores(int cores); 
  static void setMaxMorselSize(int morselSize) { maxMorselSize = morselSize; }
  static void setNumaNodes(int numa) { numNumaNodes= numa; }
  // getters
  static int getNumberOfCores() { return numCores; }
  static int getMaxMorselSize() { return maxMorselSize; }
  static int getNumaNodes() { return numNumaNodes; }
  static int getNumaID(int core); 
};

int getNumaIdFromMemoryAddress(const void *mem);
// #endif
