// define constants using enum

#ifndef CONSTANTS_H
#define CONSTANTS_H

// constants for types
enum ColumnType {
    INTEGER,
    STRING,
    FLOAT
};

// size in bytes for each type
enum ColumnSize {
    INTEGER_SIZE = 4,
    STRING_SIZE = 256,
    FLOAT_SIZE = 4
};

// comparison operators
enum ComparisonOperator {
    EQUAL,
    NOT_EQUAL,
    LESS_THAN,
    LESS_THAN_OR_EQUAL,
    GREATER_THAN,
    GREATER_THAN_OR_EQUAL
};

enum FUNCTION_IDENTIFIERS {
    SELECT_FN_IDENTIFER
};


// select loop fn arguments

enum SELECT_LOOP_ARG_INDEX {
    SELECT_LOOP_ARG_ATTRIBUTE,
    SELECT_LOOP_ARG_CONDITION,
    SELECT_LOOP_ARG_OPERATOR,
    SELECT_LOOP_ARG_INPUT_TUPLE_STREAM,
    SELECT_LOOP_ARG_OUTPUT_TUPLE_STREAM
};


#endif