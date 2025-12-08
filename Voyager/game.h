#ifndef GAME_H
#define GAME_H

#include "command.h"
#include "Player.h"
#include <string>
#include <vector>
#include <unordered_map>

/*
The function of the Game class is to manage the overall state and flow of the game.
It handles all text and art output, manages the game loop, and keeps track
of the game state that we are in (on menu, on ship, on planet, etc.).
*/

class Game {
private:
    // Display elements
    std::string art_output;  
    std::string body_output;
    std::string error_output;

    // Gamestate flags
    bool onMenu;
    bool onShip;
    bool onPlanet;
    bool gameOver;
    bool saved;
    bool next;

    Player player;

    /*
    Valid command will keep track of all possible commands in the game. The command parser checkCommand()
    will then check the command and the game state to return each of these enum values
    */

    enum class ValidCommand {
        Attack,
        Collect,
        Credits,
        DropRock,
        DropHelp,
        DropPlant,
        ExchangeFuel,
        ExchangeHealth,
        ExchangeHelp,
        ExchangeSample,
        Exit,
        Fuel,
        Health,
        Help,
        InspectHelp,
        InspectPlant,
        InspectRock,
        Instructions,
        Interact,
        InteractHelp,
        Inventory,
        Load,
        MainMenu,
        Next,
        Quit,
        ReturnToShip,
        Save,
        ScanArea,
        ScanHelp,
        ScanPlanets,
        Sneak,
        Start,
        Storage,
        StorePlant,
        StoreHelp,
        StoreRock,
        TravelDestination,
        TravelHelp,
        TravelPosition,
        UNKNOWN
    };

    std::unordered_map<std::string, std::unordered_map<std::string, ValidCommand>> commandMap = {
        {"attack", {{"", ValidCommand::Attack}}},
        {"collect", {{"", ValidCommand::Collect}}},
        {"credits", {{"", ValidCommand::Credits}}},
        {"drop", {
            {"", ValidCommand::DropHelp},
            {"-r", ValidCommand::DropRock},
            {"--rock", ValidCommand::DropRock},
            {"-p", ValidCommand::DropPlant},
            {"--plant", ValidCommand::DropPlant}}},
        {"exchange",
            {{"", ValidCommand::ExchangeHelp},
             {"-s", ValidCommand::ExchangeSample},
             {"--sample", ValidCommand::ExchangeSample},
             {"-f", ValidCommand::ExchangeFuel}, 
             {"--fuel", ValidCommand::ExchangeFuel},
             {"-hp", ValidCommand::ExchangeHealth},
             {"--health", ValidCommand::ExchangeHealth},
             {"-h", ValidCommand::ExchangeHelp},
             {"--help", ValidCommand::ExchangeHelp}}},
        {"exit", {{"ship", ValidCommand::Exit}}},
        {"fuel", {{"", ValidCommand::Fuel}}},
        {"health", {{"", ValidCommand::Health}}},
        {"help", {{"", ValidCommand::Help}}},
        {"inspect", 
            {{"", ValidCommand::InspectHelp}, 
             {"-r", ValidCommand::InspectRock},
             {"--rock", ValidCommand::InspectRock},
             {"-p", ValidCommand::InspectPlant},
             {"--plant", ValidCommand::InspectPlant},
             {"-h", ValidCommand::InspectHelp},
             {"--help", ValidCommand::InspectHelp}}},
        {"instructions", {{"", ValidCommand::Instructions}}},
        {"interact",
            {{"", ValidCommand::InteractHelp},
             {"-t", ValidCommand::InspectRock},
             {"--target", ValidCommand::InspectRock},
             {"-h", ValidCommand::InteractHelp},
             {"--help", ValidCommand::InteractHelp}}},
        {"main", {{"menu", ValidCommand::MainMenu}}},
        {"next", {{"", ValidCommand::Next}}},
        {"quit", {{"", ValidCommand::Quit}}},
        {"return", {{"to", ValidCommand::ReturnToShip}}},
        {"scan",
            {{"", ValidCommand::ScanHelp},
             {"-a", ValidCommand::ScanArea},
             {"--area", ValidCommand::ScanArea},
             {"-p", ValidCommand::ScanPlanets},
             {"--planet", ValidCommand::ScanPlanets},
             {"-h", ValidCommand::ScanHelp},
             {"--help", ValidCommand::ScanHelp}}},
        {"sneak", {{"", ValidCommand::Sneak}}},
        {"start", {{"game", ValidCommand::Start}}},
        {"storage", {{"", ValidCommand::Storage}}},
        {"store",
            {{"", ValidCommand::StoreHelp},
             {"-p", ValidCommand::StorePlant},
             {"--plant", ValidCommand::StorePlant},
             {"-r", ValidCommand::StoreRock},
             {"--rock", ValidCommand::StoreRock},
             {"-h", ValidCommand::StoreHelp},
             {"--help", ValidCommand::StoreHelp}}},
        {"travel",
         {{"", ValidCommand::TravelHelp},
          {"-p", ValidCommand::TravelPosition},
          {"--position", ValidCommand::TravelPosition},
          {"-d", ValidCommand::TravelDestination},
          {"--destination", ValidCommand::TravelDestination},
          {"-h", ValidCommand::TravelHelp},
          {"--help", ValidCommand::TravelHelp}}}
    };

public:
    Game(); // Default constructor

    /*
    Getters and setters will be utilized a lot in this project.
    Getters are used to set up text and to see the current game state,
    and setters are used to change game states and to change the display.
    */

    void setBodyOutput(const std::string&);
    void setArtOutput(const std::string&);   
    void setErrorOutput(const std::string&);
    void setMenuFlag(const bool&);
    void setShipFlag(const bool&);
    void setPlanetFlag(const bool&);
    void setGameOverFlag(const bool&);
    void setNextFlag(const bool&);
    void setSavedFlag(const bool&);

    std::string getBodyOutput() const;      
    std::string getArtOutput() const;       
    std::string getErrorOutput() const;     
    bool getMenuFlag() const;
    bool getShipFlag() const;
    bool getPlanetFlag() const;
    bool getGameOverFlag() const;
    bool getNextFlag() const;
    bool getSavedFlag() const;

    void saveGame();

    /*
    The check function will check .
    */ 
    ValidCommand getCommand(std::vector<std::string>);

    /*
    These are the UI commands. They will handle clearing and updating
    the display of the game
    */

    void clearScreen() const;
    void displayOutput() const;

    /*
    The game loop will handle all logic. It will initialize the main menu
    and then once in the loop it will display the output and then take in the 
    input. From there it will handle all game logic, and then clear
    */

    void gameLoop();
};

#endif