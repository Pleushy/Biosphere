#pragma once

#include "core/frame.hpp"
#include <map>
#include <queue>
#include <string>
#include <optional>
#include <vector>

/** Permitted  types */
enum Types {
    INT,
    STRING,
    UNDEFINED
};

/** Permitted keywords */
enum Keywords {
    RESIZE,
    MOVE,
    ZINDEX,
    ADD,
    SUB,
    MUL,
    DIV,
    SET,
    FREE,
    DRAW,
    SKIP,
    QUEUE,
};

/** Keywords in string form */
const std::map<int, std::string> KeywordStrings {
    {RESIZE,    "RESIZE"},
    {MOVE,      "MOVE"},
    {ZINDEX,    "ZINDEX"},
    {ADD,       "ADD"},
    {SUB,       "SUB"},
    {MUL,       "MUL"},
    {DIV,       "DIV"},
    {SET,       "SET"},
    {FREE,      "FREE"},
    {DRAW,      "DRAW"},
    {SKIP,      "SKIP"},
    {QUEUE,     "QUEUE"},
};

/** Permitted inputs for keywords */
const std::map<int, std::vector<std::vector<int>>> KeywordInputs {
    {RESIZE,    {{INT},{INT, UNDEFINED}}},
    {MOVE,      {{INT},{INT, UNDEFINED}}},
    {ZINDEX,    {{INT}}},
    {ADD,       {{STRING},{INT}}},
    {SUB,       {{STRING},{INT}}},
    {MUL,       {{STRING},{INT}}},
    {DIV,       {{STRING},{INT}}},
    {SET,       {{STRING},{INT}}},
    {FREE,      {{STRING}}},
    {DRAW,      {{INT},{INT, UNDEFINED},{INT, UNDEFINED}}},
    {SKIP,      {{INT, UNDEFINED}}},
    {QUEUE,     {}},
};

// Instruction data structure
struct instruction {
    std::string name;               // Instruction name/keyword
    std::vector<std::string> args;  // Arguments
    int line_num;                   // Line number where the instruction is
};

// Variable data structure (assuming that every variable can only have one int value)
struct variable {
    std::string name;               // Name of the variable
    int value;                      // Value of the variable
};

/** Compiler for .bio files */
class Bio {
    private:
        // List of declared variables (for checking scope)
        std::vector<variable> variables;

        // Error message
        void error(std::string msg);

        // Converts a string to lowercase
        std::string to_lower(std::string str);

        // Converts a string to a number
        int to_number(std::string str);

        // Returns type of input
        int check_type(std::string str);

        // Returns the value of a variable by name
        int get_variable_value(std::string name);

        // Returns the permitted inputs of keyword
        std::vector<std::vector<int>> get_keyword_inputs(int val);

        // Returns the keyword in string from
        std::string get_keyword_str(int val);

        // Attempts to return keyword from name
        std::optional<int> get_keyword(std::string name);

        // Returns value of argument
        int get_value(std::string name);

        // Attempts to seperate a file into individual lines
        std::optional<std::queue<std::string>> read_file(std::string path);

        // Converts lines into instructions
        std::queue<instruction> lexer(std::queue<std::string> lines);

        // Returns true if the code is compilable
        bool is_compilable(std::queue<instruction> instructions);

        // Compiles and returns a frame
        std::queue<Frame> compile(std::queue<instruction> instructions);
    public:
        // Attempts to return a queue of pixels if the code ran without errors
        std::optional<std::queue<Frame>> execute(std::string path);
};