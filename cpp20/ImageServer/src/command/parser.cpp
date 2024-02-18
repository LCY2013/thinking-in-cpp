//
// Created by fufeng on 2024/2/18.
//
export module ips.command:parser;

import <string>;
import <map>;
import <vector>;
import <functional>;

import :argument;

namespace ips::command {
    export class Parser {
    public:
        Parser &addArgument(const Argument &argument) {
            _arguments.push_back(argument);

            return *this;
        }

        std::map <std::string, std::string> parsedArguments() {
            return _parsedArguments;
        }

        std::string getParsedArgument(const std::string &name) {
            std::string value = _parsedArguments[name];

            return value;
        }

        template<class T>
        T getParsedArgument(const std::string &name, std::function<T(const std::string &)> converter) {
            std::string value = _parsedArguments[name];
            return converter(value);
        }

    private:
        std::vector <Argument> _arguments;
        std::map <std::string, std::string> _parsedArguments;
    };
}
