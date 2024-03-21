// ifndef guard
#ifndef STATIC_H
#define STATIC_H
// declare static integers for number of cores and max morsel size


// write a static class to set the number of cores and max morsel size
class StaticVars {
    private:
    static int numCores;
    static int maxMorselSize;

public:
  static void setNumberOfCores(int cores) {
    numCores = cores;
  }
  static void setMaxMorselSize(int morselSize) {
    maxMorselSize = morselSize;

  }
  // getters
    static int getNumberOfCores() {
        return numCores;
    }
    static int getMaxMorselSize() {
        return maxMorselSize;
    }
};
#endif

