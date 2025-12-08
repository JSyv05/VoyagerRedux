#include"menu.h"
#include <string>

 std::string Menu::setMenu() const {
    std::string text = 
        "> Start Game\n"
        "> Load Game\n"
        "> Instructions\n"
        "> Credits\n"
        "> Quit\n\n"

        "Please enter your choice."; // Main menu text

    return text;
}
std::string Menu::setInstructions() const {
    std::string text = 
        "the goal of Voyager is to collect as many samples as\n"
        "possible, explore as many planets as possible, and then\n" 
        "return to Earth safely. You will be able to use commands\n"
        "to navigate around the solar system simply by typing in a\n"
        "simple word command.\n"
        "(\"Examine Granite\", \"Trade meat with merchant\", etc.)\n\n"

        "The challenge is resource management. You will have to spend\n"
        "samples to maintain the health of your ship and yourself. You will\n"
        "also lose resources from random events that will occur. You will\n"
        "have to make strategic decisions on what to spend resources on as\n"
        "well as what items you need to survive.\n\n"

        "When it comes to space exploration, there is only one saying:\n\n"
        "\"Every Sacrifice Matters\"\n\n"

        "Type 'back' to return to main menu ";

    return text;
}

std::string Menu::setCredits() const{
    std::string text = "Team Members:\n"
                  "\tAlina Betances\n"
                  "\tAnthony Pinto\n"
                  "\tElias Reeves\n"
                  "\tMatthew Silva\n"
                  "\tJohn Syvertsen\n\n"

                  "Scrum Master:\n"
                  "\tJohn Syvertsen\n\n"

                  "Playtesters (Thank you!)\n"
                  "\tErwin Jonson\n"
                  "\tJosh Klenk\n"
                  "\tMaya Sriram\n\n"
                  
                  "Type back to return to the main menu.";

    return text;
}

std::string Menu::setIntro() const{

    std::string text = 
        "VOYAGER PROGRAM: LOG ALPHA-01\n\n"

        "After decades of preparation, the Voyager Program has finally launched.\n"
        "Our mission: explore the outer systems, gather samples from uncharted\n"
        "worlds, and return home before the fuel cells run dry.\n\n"

        "Each planet brings its own challenge, but every discovery will push\n"
        "humanity one step closer to glory.\n\n"

        "You are going to be the start of Humanities duty to explore the galaxy.\n\n"
        "Remember, Voyager: \"Every Sacrifice Matters\".\n\n"

        "- Grand Overseer of the Legion\n\n"

        "Type 'next' or press Enter to continue...";

    return text;
}