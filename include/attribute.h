#pragma once
#include "constants.h"
#include <string>
struct Attribute {
  std::string name;
  int type;
  int offset;
  int size;
  int attributeIndex;
};

int getAttributeSizeFromType(int type);