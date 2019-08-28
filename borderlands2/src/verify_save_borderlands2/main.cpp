/**
 * @file        main.cpp
 * @author      David Oberacker (david.oberacker@gmail.com)
 * @brief       Borderlands2 save verify application main file.
 * @details     Implements CLI to verify borderlands2 save files.
 * @version     0.1
 * @date        2019-08-26
 *
 * @copyright   Copyright (c) 2019 David Oberacker
 */

#include <iostream>
#include <string>

#include <cxxopts.hpp>

#include <borderlands2/borderlands2.hpp>

/**
 * @brief Main function of the save verify tool.
 * 
 * @param argc The command line argument count.
 * @param argv The command line argument vector.
 * 
 * @return 0 on success.
 * @return error code on failure.s
 */
int main(int argc, char* argv[]) {

    cxxopts::Options options("Borderlands2SaveEditor", "One line description of MyProgram");
    options.add_options()
            ("h,help", "Show help")
            ("d,debug", "Enable debugging")
            ("f,file", "File name", cxxopts::value<std::string>());


    auto parse_result = options.parse(argc, argv);

    if (parse_result.count("help") > 0) {
        std::cout << options.help();
        return 0;
    }

    std::cout << "Verification started" << std::endl;
    std::string input_path;
    std::cin >> input_path;

    std::cout << "Verifying save file at path: " << input_path << std::endl;
    bool result = D4v3::Borderlands::Borderlands2::verifySave(input_path);
    std::cout << "Verification result: " << std::boolalpha << result << std::endl;



    std::cout << "Verification finished" << std::endl;
    return 0;
}