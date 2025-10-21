#include "utils/bio.hpp"
#include "core/frame.hpp"
#include "utils/globals.hpp"
#include "utils/vec2.hpp"
#include <iostream>
#include <queue>
#include <stdexcept>
#include <string>
#include <optional>
#include <vector>
#include <fstream>

void Bio::error(std::string msg) {
    std::cerr << "Compiling error: " << msg << std::endl;
    return;
}

int Bio::check_type(std::string str) {
    int ascii = str[0]-'0';
    if (ascii >= 0 && ascii <= 9) {
        // Check for number
        try {
            std::stoi(str);
            return INT;
        } catch (std::invalid_argument) {} catch (std::out_of_range) {}
    } else {
        // Check for single char
        if (str.length() == 1) {
            return INT;
        }
    }

    return STRING;
}

std::string Bio::to_lower(std::string str) {
    std::string output_str;
    output_str.resize(str.length());
    for (int j = 0; j < str.length(); j++) output_str[j] = std::tolower(str[j]);
    return output_str;
}

int Bio::to_number(std::string str) {
    if (check_type(str) != INT) return -1;
    int num = 0;

    for (int i = 0; i < str.size(); i++) {
        int offset = 1;
        for (int j = 0; j < str.size()-i-1; j++) offset*=10;
        num+=(str[i]-'0')*offset;
    }

    return num;
}

std::vector<std::vector<int>> Bio::get_keyword_inputs(int val) {
    return KeywordInputs.at(val);
}

std::string Bio::get_keyword_str(int val) {
    return KeywordStrings.at(val);
}

std::optional<int> Bio::get_keyword(std::string name) {
    for (int i = 0; i < KeywordStrings.size(); i++) {
        if (to_lower(KeywordStrings.at(i)) == to_lower(name)) {
            return i;
        }
    }
    return std::nullopt;
}

std::optional<std::queue<std::string>> Bio::read_file(std::string path) {
    std::queue<std::string> output;

    std::ifstream file(path);
    if (!file.is_open()) {
        return std::nullopt;
    }

    std::string line;
    while (std::getline(file, line)) {
        output.push(line);
    }
    
    file.close();
    return output;
}

std::queue<instruction> Bio::lexer(std::queue<std::string> lines) {
    std::queue<instruction> instructions;
    int line_num = 0;
    while (!lines.empty()) {
        line_num++;
        std::string line = lines.front();
        lines.pop();

        if (!line.length()) continue;
        if (line.find(';') != std::string::npos) continue;
        
        std::vector<std::string> tokens;
    
        int last_idx = 0;
        while (true) {
            int idx = line.find(' ',last_idx+1);
            if (idx == std::string::npos) break;
    
            tokens.push_back(to_lower(line.substr(last_idx+(last_idx>0),idx-last_idx-(last_idx>0))));
    
            last_idx = idx;
        }
        tokens.push_back(to_lower(line.substr(last_idx+(last_idx>0))));
    
        std::string name = tokens[0];
        tokens.erase(tokens.begin());
        instructions.push(instruction{name, tokens, line_num});
    }
    return instructions;
}

bool Bio::is_compilable(std::queue<instruction> instructions) {
    Frame reference_frame;
    bool return_val = true;
    while (!instructions.empty()) {
        instruction current_instruction = instructions.front();
        instructions.pop();

        int line_num = current_instruction.line_num;
        std::string name = current_instruction.name;
        std::vector<std::string> args = current_instruction.args;
        int keyword;

        std::optional<int> tmp = get_keyword(name);
        if (tmp.has_value()) {
            keyword = tmp.value();
        } else {
            error("Instruction doesn't exist: line " + std::to_string(line_num));
            return_val = false;
            continue;
        }

        std::vector<std::vector<int>> types = get_keyword_inputs(keyword);

        int optional_args = 0;
        for (std::vector<int> current_types : types) {
            for (int type : current_types) {
                if (type == UNDEFINED) optional_args++;
            }
        }

        if (args.size() < types.size()-optional_args) {
            error("Too few arguments: line " + std::to_string(line_num));
            return_val = false;
            continue;
        } else if (args.size() > types.size()) {
            error("Too many arguments: line " + std::to_string(line_num));
            return_val = false;
            continue;
        }

        for (int i = 0; i < args.size(); i++) {
            std::string arg = args[i];

            if (get_keyword(arg).has_value()) {
                error("Argument cannot contain keyword: line " + std::to_string(line_num));
                return_val = false;
                continue;
            }

            bool in_variables = false;
            int offset = 0;
            for (variable var : variables) {
                if (var.name == arg) {
                    in_variables = true;
                    break;
                }
                offset++;
            }

            int current_type = check_type(arg);

            bool found_type = false;
            for (int type : types[i]) {
                if (type == current_type) {
                    found_type = true;
                    break;
                }
            }

            if (!found_type && !in_variables) {
                error("Wrong type: line " + std::to_string(line_num));
                return_val = false;
                continue;
            }

            /** Instruction specific error catches */
            switch (i) {
                case 0: {
                    switch (keyword) {
                        case SET: {
                            if (in_variables) {
                                error("Already a variable: " + arg + ": line " + std::to_string(line_num));
                                return_val = false;
                            } else {
                                variables.push_back(variable{arg});
                            }
                            break;
                        }
                        case FREE: {
                            if (in_variables) {
                                variables.erase(variables.begin()+offset);
                            } else {
                                error("Cannot free non-existant variable: line " + std::to_string(line_num));
                                return_val = false;
                            }
                            break;
                        }
                        default: 
                            break;
                    }
                }
                case 1: {
                    switch (keyword) {
                        case DIV: {
                            if (get_value(arg) == 0) {
                                error("Cannot divide by zero: line " + std::to_string(line_num));
                                return_val = false;
                            }
                            break;
                        }
                        default:
                            break;
                    }
                }
                default:
                    break;
            }
        }
    }
    if (!variables.empty()) {
		// for (variable var : variables) {
		// 	std::cout << var.name << std::endl;
		// }
        error("Variables not freed");
        return_val = false;
    }
    return return_val;
}

int Bio::get_variable_value(std::string name) {
    for (variable var : variables) {
        if (var.name == name) {
            return var.value;
        }
    }
    return -1;
}

int Bio::get_value(std::string name) {
	int var_val = get_variable_value(name);
	return var_val == -1 ? to_number(name) : var_val;
}

std::queue<Frame> Bio::compile(std::queue<instruction> instructions) {
    std::queue<Frame> output_frames;
    Frame reference_frame;
    std::queue<Pixel> pixels;

    while (!instructions.empty()) {
        instruction current = instructions.front();
        instructions.pop();
        
        std::vector<std::string> args = current.args;
        int arg_count = args.size();
        std::string name = current.name;
        int keyword = get_keyword(name).value();

        switch (keyword) {
            case RESIZE: {
                Vec2 size;
                switch (arg_count) {
                    case 1:
                        size.x = get_value(args[0]);
                        size.y = size.x;
                        break;
                    default:
                        size.x = get_value(args[0]);
                        size.y = get_value(args[1]);
                        break;
                }
                reference_frame.size = size;
                break;
			}
			case MOVE: {
				Vec2 pos;
                switch (arg_count) {
                    case 1:
                        pos.x = get_value(args[0]);
                        pos.y = pos.x;
                        break;
                    default:
                        pos.x = get_value(args[0]);
                        pos.y = get_value(args[1]);
                        break;
                }
                reference_frame.position = pos;
                break;
			}
			case ZINDEX: {
				reference_frame.zindex = get_value(args[0]);
				break;
			}
			case ADD: {
				for (int i = 0; i < variables.size(); i++) {
					if (variables[i].name == args[0]) {
						variables[i].value += get_value(args[1]);
						break;
					}
				}
				break;
			}
			case SUB: {
				for (int i = 0; i < variables.size(); i++) {
					if (variables[i].name == args[0]) {
						variables[i].value -= get_value(args[1]);
						if (variables[i].value < 0) variables[i].value = 0;
						break;
					}
				}
				break;
			}
            case MUL: {
                for (int i = 0; i < variables.size(); i++) {
					if (variables[i].name == args[0]) {
						variables[i].value *= get_value(args[1]);
						break;
					}
				}
                break;
            }
            case DIV: {
                for (int i = 0; i < variables.size(); i++) {
					if (variables[i].name == args[0]) {
						variables[i].value /= get_value(args[1]);
						break;
					}
				}
                break;
            }
			case SET: {
				variables.push_back(variable{args[0], get_value(args[1])});
				break;
			}
			case FREE: {
				for (int i = 0; i < variables.size(); i++) {
					if (variables[i].name == args[0]) {
						variables.erase(variables.begin()+i);
						break;
					}
				}
				break;
			}
			case DRAW: {
				for (int i = 0; i < get_value(args[0]); i++) {
                    switch (arg_count) {
                        case 1:
                            pixels.push(Pixel{false});
                            break;
                        case 2:
                            pixels.push(Pixel{true, args[1][0], constants::DEFAULT_COLOR});
                            break;
                        default:
                            pixels.push(Pixel{true, args[1][0], get_value(args[2])});
                            break;
                    }
				}
				break;
			}
			case SKIP: {
                int to_skip;
                switch (arg_count) {
                    case 0:
                        to_skip = ( (4 - ( pixels.size() % reference_frame.size.x )) );
                        break;
                    default:
                        to_skip = ( ( get_value(args[0]) * reference_frame.size.x ) + ( (4 - ( pixels.size() % reference_frame.size.x )) ) );
                        break;
                }
				for (int i = 0; i < to_skip; i++) {
					pixels.push(Pixel{false});
				}
				break;
			}
            case QUEUE: {
                int missing = reference_frame.size.x*reference_frame.size.y - pixels.size();
                for (int i = 0; i < missing; i++) {
                    pixels.push(Pixel{false});
                }
                
                reference_frame.pixels = pixels;
                output_frames.push(reference_frame);
                
                while (!pixels.empty()) pixels.pop();
            }

            default:
                break;
        }
    }

	variables.clear();

    return output_frames;
}

std::optional<std::queue<Frame>> Bio::execute(std::string path) {
    std::optional<std::queue<std::string>> lines = read_file(path);
    if (!lines.has_value()) {
        error("Failed to read file: " + path);
        return std::nullopt;
    }

    std::queue<instruction> instructions = lexer(lines.value());
    if (instructions.empty()) {
        error("File empty: " + path);
        return std::nullopt;
    }

    if (!is_compilable(instructions)) {
        error("Couldn't compile file: " + path);
        return std::nullopt;
    }

    return compile(instructions);
}