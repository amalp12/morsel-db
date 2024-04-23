// define constants using enum

#ifndef CONSTANTS_H
#define CONSTANTS_H

// b plus tree max key size
enum B_PLUS_TREE {
  MAX_B_PLUS_TREE_KEYS = 3, // Maximum number of keys in a node
  MAX_LEAF_NODE_SIZE = 3,
  MAX_INTERNAL_NODE_SIZE = 4
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

#endif