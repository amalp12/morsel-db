// define constants using enum

// #ifndef CONSTANTS_H
// #define CONSTANTS_H
#pragma once

// b plus tree max key size
enum B_PLUS_TREE {
  MAX_KEYS_LEAF = 63,
  MAX_KEYS_INTERNAL = 100,
  MIDDLE_INDEX_INTERNAL = (MAX_KEYS_INTERNAL + 1) / 2,
  MIDDLE_INDEX_LEAF = (MAX_KEYS_LEAF + 1) / 2,
};

// error codes
enum B_PLUS_TREE_ERROR {
  MEMORY_FULL = -2,
  DUPLICATE_KEY = -3,
  KEY_NOT_FOUND = -4,
  INVALID_KEY = -5
};
enum B_PLUS_TREE_NODE_TYPE { LEAF_NODE, INTERNAL_NODE };

// constants for types
enum ColumnType { INTEGER, STRING, FLOAT };

// size in bytes for each type
enum ColumnSize { INTEGER_SIZE = 4, STRING_SIZE = 256, FLOAT_SIZE = 4 };

// success and failure
enum Status { SUCCESS, FAILURE };

// comparison operators
enum ComparisonOperator {
  EQUAL,
  NOT_EQUAL,
  LESS_THAN,
  LESS_THAN_OR_EQUAL,
  GREATER_THAN,
  GREATER_THAN_OR_EQUAL
};

enum FUNCTION_IDENTIFIERS { SELECT_FN_IDENTIFER, JOIN_FN_IDENTIFIER };

// select loop fn arguments

enum SELECT_LOOP_ARG_INDEX {
  SELECT_LOOP_ARG_ATTRIBUTE,
  SELECT_LOOP_ARG_CONDITION,
  SELECT_LOOP_ARG_OPERATOR,
  SELECT_LOOP_ARG_INPUT_TUPLE_STREAM,
  SELECT_LOOP_ARG_OUTPUT_TUPLE_STREAM
};

// #endif