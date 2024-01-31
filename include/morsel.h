#include <string>
#include <list>
#include <memory>

class Morsel{

    private:
        std::string subOperator;
        std::list<std::string> argList;
        Morsel * next;
        int size;
        int entrySize;
        void * startPtr;
        void * endPtr;
        void * currentPtr;
    
    public:
        Morsel(int morselSize, int morselEntrySize);
        std::string getSubOperator();
        std::list<std::string> getArgList();
        Morsel* getNext();

        int setSubOperator(std::string  op);
        int setArgList(std::list<std::string>  argumentsList);
        int setNext(Morsel * nextPtr);
        int setCurrentPtr(void * ptr);
        int setNthMorsel(int n, void * ptr);

        void * getStartPtr();
        void * getEndPtr();
        void * getCurrentPtr();
        void * getNthMorsel(int n);
};