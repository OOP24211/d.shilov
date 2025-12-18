#include "ConfigParser.h"
#include <algorithm>
#include <iterator>
#include <regex>

std::string trim(const std::string &str) {
    const std::string whitespace = " \t";
    size_t start = str.find_first_not_of(whitespace);
    if (start == std::string::npos) {
        return "";
    }
    size_t end = str.find_last_not_of(whitespace);
    return str.substr(start, end - start + 1);
}

ConverterCommand ConfigParser::parse_line(std::string &line) {
    ConverterCommand command;
    std::stringstream s_stream(line);
    std::string token;
    if (s_stream >> command.name) {
        while (s_stream >> token) {
            command.arguments.push_back(token);
        }
    }
    return command;
}

std::string ConfigParser::reference(std::string &arg) {
    if (arg.empty() || arg[0] != '$') {
        return arg;
    }
    std::string num_str = arg.substr(1);
    int number;
    try {
        number = std::stoi(num_str);
    } catch (const std::invalid_argument &) {
        throw std::invalid_argument("Ссылка '$n' должна содержать число");
    }
    if (number <= 0 || number > input_files_list.size()) {
        throw std::out_of_range("Недопустимая ссылка на входной файл");
    }

    return input_files_list[number - 1];
}

ConfigParser::ConfigParser(const std::string &conf_file, const std::vector<std::string> &input_files) {
    input_files_list = input_files;
    std::ifstream config_file(conf_file);
    if (!config_file.is_open()) {
        throw std::runtime_error("Не удалось открыть файл конфигурации");
    }
    std::string line;
    while (std::getline(config_file, line)) {
        std::string trimmed_line = trim(line);
        if (trimmed_line.empty() || trimmed_line[0] == '#') {
            continue;
        }
        try {
            ConverterCommand command = parse_line(trimmed_line);
            if (!command.name.empty()) {
                commands.push_back(std::move(command));
            }
        } catch (const std::exception &e) {
            throw std::runtime_error("Ошибка в файле конфигурации");
        }
    }
}

std::vector<std::string> ConfigParser::get_command() {
    if (command_read >= commands.size()) {
        return {};
    }
    ConverterCommand current_command = commands[command_read];
    command_read++;
    std::vector<std::string> string_command;
    string_command.push_back(current_command.name);
    for (std::string arg: current_command.arguments) {
        string_command.push_back(reference(arg));
    }
    return string_command;
}
