#include "constants.h"
#include "attribute.h"
int getAttributeSizeFromType(int type) {
  switch (type) {
  case STRING:
    return STRING_SIZE;
  case INTEGER:
    return INTEGER_SIZE;
  case FLOAT:
    return FLOAT_SIZE;
  default:
    return 0;
  }
}



