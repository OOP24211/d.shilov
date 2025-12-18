#ifndef LAB_3_CONFIGPARSER_H
#define LAB_3_CONFIGPARSER_H


#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <sstream>

struct ConverterCommand {
    std::string name;
    std::vector<std::string> arguments;
};

class ConfigParser {
public:
    ConfigParser(const std::string &config_filename, const std::vector<std::string>& input_files);
    std::vector<std::string> get_command();
    bool is_last_command() const {
        return command_read == commands.size();
    }

private:
    int command_read = 0;
    std::vector<ConverterCommand> commands;
    ConverterCommand parse_line(
    std::string &line);
    std::vector<std::string> input_files_list;
    std::string reference(std::string& arg);
};

#endif //LAB_3_CONFIGPARSER_H
