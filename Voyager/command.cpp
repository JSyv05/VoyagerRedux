#include "command.h"

#include<algorithm>
#include<iostream>
#include<sstream>
#include<string>
#include<vector>

/*
The command class will be handling taking in inputs and breaking them up into tokens
Then it will let the game read the command.
*/

/*
setInput will first clear the command buffer of the previous command, 
get the input from the player, and then using a while loop, tokenize and
set the command to all lowercase, making it easier to implement commands
in the game class, and then push the command to a vector.
*/

void Command::setInput() {
    command.clear(); // Empties command vector of previous commands
    std::cout << "> ";             // Input field
    std::getline(std::cin, input); // Take in input from entire line
    std::stringstream untokenizedInput(input); // Convert the input to a stringstream to tokenize input
    std::string token; // Temp variable that will store the individual tokens of the input

    while (untokenizedInput >>token) { // While loop that pushes each token to the end of the list

        std::transform(token.begin(), token.end(), token.begin(),
                           [](unsigned char c) {
                               return std::tolower(c);
                           }); // Transforms the token to lowercase

        command.push_back(token); // Appends a the lowercased token to the end of the list
                                  // tokens for the command parser to read.
        }
}

/*
Returns the tokenized command for the command interpreter
*/

std::vector<std::string> Command::getInput() const{ return command; }