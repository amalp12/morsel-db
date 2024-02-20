#include "lib.h"

bool compareInt(int value1 , int value2 , int operatorType)
{
      switch (operatorType) {
      case EQUAL:
        return value1 ==value2;
      case NOT_EQUAL:
        return value1 != value2;
      case LESS_THAN:
        return value1 < value2;
      case GREATER_THAN:
        return value1 > value2;
      case LESS_THAN_OR_EQUAL:
        return value1 <= value2;
      case GREATER_THAN_OR_EQUAL:
        return value1 >= value2;
      default:
        return false;
  }
}


bool compareStr(char * value1 , char * value2 , int entrySize , int operatorType)
{
  int isEqual = std::memcmp((void *) value1 ,(void *) value2 , (size_t)entrySize);


  switch (operatorType) {
      case EQUAL:
        return isEqual==0;
      case NOT_EQUAL:
        return isEqual != 0;
      case LESS_THAN:
        return isEqual < 0;
      case GREATER_THAN:
        return isEqual > 0;
      case LESS_THAN_OR_EQUAL:
        return isEqual <= 0;
      case GREATER_THAN_OR_EQUAL:
        return isEqual >= 0;
      default:
        return false;


}}