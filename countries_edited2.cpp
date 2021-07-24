#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <string>
#include <algorithm>
#include <cstring>
#include <list>
#include <iterator>

namespace TERMINAL {
    enum COMMAND : uint8_t {
        CHANGE_CAPITAL,
        RENAME,
        ABOUT,
        DUMP
    };

    using command_t = COMMAND;

    constexpr char * work_commands[] = { 
        "CHANGE_CAPITAL", "RENAME", "ABOUT", "DUMP"
        };

    bool is_command(std::string received, command_t command) {
        return received == work_commands[command];
    }
}


void parse_commands(const std::vector<std::string>& commands) {
   std::map<std::string, std::string> capitals;

    for (std::string command : commands) {

        if (TERMINAL::is_command(command, TERMINAL::CHANGE_CAPITAL)) {
        std::string country, new_capital;
        std::cin >> country >> new_capital;
        // country = std::next(item, 1);
        // new_capital = std::next(item, 2);
        
            if (capitals.count(country) == 0) {
                std::cout << "Introduce new country " << country 
                << " with capital " << new_capital << std::endl;
        }
            else {
                std::cout << "Country " << country;

            if (new_capital == capitals[country]) {
                std::cout << " hasn't changed its capital " << std::endl;
            }
            else {
                std::cout << " has changed its capital from " 
                << capitals[country] << " to " << new_capital << std::endl;
            }
        }
        capitals[country] = new_capital;
        }
    // Конец чтения CHANGE_CAPITAL

        if (TERMINAL::is_command(command, TERMINAL::RENAME)) {
            std::string old_country_name, new_country_name;
            std::cin >> old_country_name >> new_country_name;

            if (old_country_name == new_country_name || capitals.count(old_country_name) == 0 || capitals.count(new_country_name) == 1)
                std::cout << "Incorrect rename, skip" << std::endl;

            else {
                capitals[new_country_name] = capitals[old_country_name];
                std::cout 
                    << "Country " << old_country_name 
                    << " with capital " << capitals[old_country_name] 
                    << " has been renamed to " << new_country_name 
                    << std::endl;
                capitals.erase(old_country_name);
            }
        }
        // Конец чтения RENAME

        if (TERMINAL::is_command(command, TERMINAL::ABOUT)) {
            std::string country;
            std::cin >> country;

            if (capitals.count(country) == 0)
                std::cout << "Country " << country << " doesn't exist" << std::endl;

            else {
                std::cout << "Country " << country << " has capital " << capitals[country] << std::endl;
            }
        }
        //Конец чтения ABOUT

        if (TERMINAL::is_command(command, TERMINAL::DUMP)) {
            
            if (capitals.empty()) 
                std::cout << "There are no countries in the world";
            
            else {
                for (const auto& i : capitals)
                    std::cout << i.first << "/" << i.second << " ";
            }
            std::cout << std::endl;
        }
        //Конец чтения DUMP

    }
}


std::list<std::string> input_commands(int count_of_commands) {
    std::list<std::string> commands;
    constexpr size_t BUFFER_SIZE = 256;

    for (size_t i = 0; i < count_of_commands; ++i) {
        char command[BUFFER_SIZE];
        memset(command, 0, BUFFER_SIZE);

        size_t j = 0;
        do {
            std::cin >> command[j];
        } while (command[j++] != 'I');

        commands.push_back(std::string(command));
    }
    return commands;
}


int main() {
    int count_of_commands;
    std::cin >> count_of_commands;
    std::map<std::string, std::string> capitals;
    std::vector<std::string> untranslated_command;

    for (std::string item : input_commands(count_of_commands))
        untranslated_command.push_back(item);
        parse_commands(untranslated_command);

    }