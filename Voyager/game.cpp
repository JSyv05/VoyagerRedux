// User created libraries
#include "art.h"
#include "game.h"
#include "rock.h"
#include "command.h"
#include "menu.h"
#include "help.h"
#include "planet.h"
#include "ship.h"
#include "inventory.h"
#include "exchange.h"
#include "npc.h"

// Standard C++ libraries
#include <array>
#include <iostream>
#include <sstream>
#include <map>
#include <memory>
#include <vector>

std::vector<Rock> createMasterRockList(); // declare rock creation routine, a
                                          // global in the rock class

// Function implementation for Game class

Game::Game()
    : art_output(""), body_output(""), error_output(""), onMenu(false),
    onShip(false), onPlanet(false), gameOver(false), saved(false),
    next(false) {
}

/*
Setters and getters for all outputs and game state flags
*/

void Game::setArtOutput(const std::string& art) { art_output = art; }
void Game::setBodyOutput(const std::string& body) { body_output = body; }
void Game::setErrorOutput(const std::string& error) { error_output = error; }

void Game::setMenuFlag(const bool& flag) { onMenu = flag; }
void Game::setShipFlag(const bool& flag) { onShip = flag; }
void Game::setPlanetFlag(const bool& flag) { onPlanet = flag; }
void Game::setGameOverFlag(const bool& flag) { gameOver = flag; }
void Game::setNextFlag(const bool& flag) { next = flag; }
void Game::setSavedFlag(const bool& flag) { saved = flag; }

std::string Game::getArtOutput() const { return art_output; }
std::string Game::getBodyOutput() const { return body_output; }
std::string Game::getErrorOutput() const { return error_output; }

bool Game::getMenuFlag() const { return onMenu; }
bool Game::getShipFlag() const { return onShip; }
bool Game::getPlanetFlag() const { return onPlanet; }
bool Game::getGameOverFlag() const { return gameOver; }
bool Game::getNextFlag() const { return next; }
bool Game::getSavedFlag() const { return saved; }

Game::ValidCommand Game::getCommand(std::vector<std::string> input) {
    try {
        std::string command = input[0];
        std::string option;
        const auto& inner_command_map = commandMap.at(command);
        if (input.size() == 1) {
            option = "";
        }
        else {
            option = input[1];
        }
        return inner_command_map.at(option);
    } catch (std::out_of_range& e) {
        return ValidCommand::UNKNOWN;
    }
}

/*
This function checks to see what operating system the game is running on, either
Windows, or some Unix-based OS like Linux or MacOS. It will then run the
corresponding command to clear the terminal. A clean and easy way to implement
cross compatibility into our game
*/

void Game::clearScreen() const {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

/*
This function combines the values for art, body, and error and displays all of
that as one output.
*/

void Game::displayOutput() const {
    std::ostringstream output;
    output << art_output << "\n\n" << body_output << "\n\n" <<
                    error_output;

    if (error_output != "") {
        output << "\n\n";
    }
    std::cout << output.str();
}

//Helper function to throw an error if an argument is shorter than the minimum required

void checkInputSizeLessThanN(const std::vector<std::string>& vector,
                             size_t n) {
    if (vector.size() < n) {
        throw std::out_of_range("ERR: Not enough arguments for command");
    }
}

void checkIndexIsInRange(int index, int size) {
    if (index - 1 < 0 || index - 1 > size) {
        throw std::out_of_range("ERR: Index is out of range");
    }
}
    // Helper function to get art based on planet type

std::string getArtForTravelToPlanet(Biome biome, Art art) {
    switch (biome) {
    case Biome::Desert:
        return art.setArtToDesert();
    case Biome::Ice:
        return art.setArtToIce();   
    case Biome::Ocean:
        return art.setArtToOcean();  
    case Biome::Forest:
        return art.setArtToForest();  
    case Biome::Volcanic:
        return art.setArtToVolcano();
    case Biome::GasGiant:
        return art.setArtToGasGiant();
    case Biome::Urban:
        return art.setArtToCity();
    case Biome::Barren:
        return art.setArtToWasteLand();
    default:
        return "ERR: Surface could not be processed";
    }
}

/*
The game loop is the heart of the game. It will handle all of the displaying of
outputs as well as the logic behind each command.
*/

void Game::gameLoop() { 
    // initializations for initial game objects
    Menu menu;
    Command command;
    Ship ship;
    Art art;
    Help help;
    ExchangeStation exchange;

    PlanetSystem planet_system;
    std::vector<Rock> all_game_rocks = createMasterRockList();

    Inventory inventory(20);  

    int planetCommandCounter = 0; // counter inputs while on a planet
    bool playerIsSneaking = false;  // set by sneak command
    const int MONSTER_ATTACK_THRESHOLD = 5; // how many commands before auto-attack

    // New monster that persits while you're on the planet 
    std::unique_ptr<Monster> active_monster;

    onMenu = true;
    setArtOutput(art.setArtToTitle());
    setBodyOutput(menu.setMenu());

    while (!gameOver) {

        /*
        Display the main menu initially and then take input.
        Contents of the display will be updated in the switch case
        */
        displayOutput();
        command.setInput();

        setErrorOutput("");

        /*
        This is where all of the command logic will be stored. It will go over
        each enum value in ValidCommand and run the corresponding logic behind
        each command
        */
        const auto& input = command.getInput();

        ValidCommand passed_command = getCommand(input);

        if (onMenu) {
             switch (passed_command) {
            case ValidCommand::Credits:
                setBodyOutput(menu.setCredits());
                break;

            case ValidCommand::Instructions:
                setBodyOutput(menu.setInstructions());
                break;

            case ValidCommand::Load:
                break;

            case ValidCommand::MainMenu:
                setBodyOutput(menu.setMenu());
                break;

            case ValidCommand::Next: {
                setMenuFlag(false);
                setNextFlag(false);
                setShipFlag(true);

                std::string output =
                    "The view of space is unlike anything you have seen "
                    "before.\nYou feel a sense of calm wash over you.\n(use "
                    "scan "
                    "-p to scan for nearby planets)";
                setArtOutput(art.setArtToShip());
                setBodyOutput(output);
            } break;

            case ValidCommand::Quit:
                setGameOverFlag(true);
                break;

            case ValidCommand::Start:
                setNextFlag(true);
                setBodyOutput(menu.setIntro());
                setArtOutput(art.setArtToAguila());
                planet_system.generatePlanets(20, all_game_rocks);
                break;
            }
        }

        else if (onShip) {
            switch (passed_command) {
            case ValidCommand::ExchangeFuel:
                try {
                    checkInputSizeLessThanN(input, 3);
                    int points_to_exchange = stoi(input[2]);
                    double refuel =
                        exchange.exchangeLootPointForFuel(points_to_exchange);
                    ship.refuel(refuel);
                    std::ostringstream oss;
                    oss << "Refueled " << refuel
                        << " units. Remaining LP: " << exchange.getLootPoint();
                    setErrorOutput(oss.str());
                } catch (const std::invalid_argument& e) {
                    std::string error = "ERR: Invalid argument";
                    setErrorOutput(error);
                } catch (const std::out_of_range& e) {
                    std::string error = e.what();
                    setErrorOutput(error);
                }
                break;

            case ValidCommand::ExchangeHealth:
                try {
                    checkInputSizeLessThanN(input, 3);
                    int points_to_exchange = stoi(input[2]);
                    double heal =
                        exchange.exchangeLootPointForHealth(points_to_exchange);
                    player.gainHealth(heal);
                    std::ostringstream oss;
                    oss << "Healed " << heal
                        << " hit points. Remaining LP: " << exchange.getLootPoint();
                    setErrorOutput(oss.str());
                } catch (const std::invalid_argument& e) {
                    std::string error = "ERR: Invalid argument";
                    setErrorOutput(error);
                }
                break;

            case ValidCommand::ExchangeHelp:
                setBodyOutput(help.getExchangeHelp());
                break;

            case ValidCommand::Exit:
                setBodyOutput(ship.shipExit());
                setShipFlag(false);
                setPlanetFlag(true);
                break;

            case ValidCommand::Fuel: {
                std::ostringstream oss;
                oss << "Current fuel level: " << ship.getFuel();
                setErrorOutput(oss.str());
            } break;

            case ValidCommand::Health: {
                std::ostringstream oss;
                oss << "Player health: " << player.getPlayerHealth();
                setErrorOutput(oss.str());
            } break;
            case ValidCommand::Help:
                setBodyOutput(help.getGeneralHelp());
                break;

            case ValidCommand::Inventory:
                inventory.autoSortRocks(); // Sort before displaying
                setBodyOutput(inventory.getDisplayString());
                break;

            case ValidCommand::ScanPlanets:
                setBodyOutput(ship.getNearbyPlanet(planet_system.getPlanetList()));
                break;

            case ValidCommand::ScanHelp:
                setBodyOutput(help.getScanHelp());
                break;

            case ValidCommand::Storage:
                setBodyOutput(ship.getStorageContents());
                break;
            case ValidCommand::StoreRock: {
                try {
                    int index = stoi(input[1]);
                    checkIndexIsInRange(index,
                                        inventory.getCurrentSize());
                    ship.addToShipStorage(inventory, index);
                    std::string body_string =
                        ship.getShipStorage()->getDisplayString();
                    setBodyOutput(body_string);
                } catch (const std::out_of_range& e) {
                    setErrorOutput(e.what());
                }
            } break;

            case ValidCommand::TravelDestination:
                try {
                    checkInputSizeLessThanN(input, 3);
                    int index = stoi(input[2]);
                    checkIndexIsInRange(index, 3);
                    setBodyOutput(ship.travelToPlanet(index));
                    setArtOutput(getArtForTravelToPlanet(
                        ship.getCurrentPlanet().getBiome(), art));
                } catch (const std::out_of_range& e) {
                    std::string error = e.what();
                    setErrorOutput(error);
                } catch (const std::invalid_argument& e) {
                    std::ostringstream oss;
                    oss << "ERR: Bad input in argument (" << e.what() << ")";
                    setErrorOutput(oss.str());
                }
                break;

            case ValidCommand::TravelPosition:
                try {
                    checkInputSizeLessThanN(input, 5);
                    double x_pos = stod(input[2]);
                    double y_pos = stod(input[3]);
                    double z_pos = stod(input[4]);
                    std::array<double, 3> position = {x_pos, y_pos, z_pos};
                    ship.setCoordinates(position);
                    std::ostringstream output;
                    output << "set coordinates to (" << x_pos << ", "
                           << y_pos << ", " << z_pos << ")";
                    setBodyOutput(output.str());
                } catch (const std::out_of_range& e) {
                    std::string error = e.what();
                    setErrorOutput(error);
                }
                break;

            case ValidCommand::TravelHelp:
                setBodyOutput(help.getTravelHelp());
                break;
            default:
                std::string error = "ERR: Please enter a valid input";
                setErrorOutput(error);
                break;
            }
        }

        else if (onPlanet) {
            switch (passed_command) {
            case ValidCommand::Attack: {
                // Lazily create a monster for this planet if we don't have one
                // yet
                if (!active_monster) {
                    Planet& active_planet = ship.getCurrentPlanet();
                    Biome biome = active_planet.getBiome();
                    int difficulty =
                        active_planet
                            .getLootLevel(); // lootLevel_ is your difficulty

                    active_monster = std::make_unique<Monster>(
                        createMonsterForBiomeAndDifficulty(biome, difficulty));
                }

                Monster& monster = *active_monster;

                std::ostringstream summary;

                // Player's attack
                int dmg_to_monster = player.dealDamage();
                monster.takeDamage(dmg_to_monster);
                int hpAfter = monster.getHealth();

                summary << "You attack " << monster.getName() << " for "
                        << dmg_to_monster << " damage.\n"
                        << monster.getName() << " HP: " << hpAfter << "\n";

                // Did you kill it with this hit?
                if (monster.isDead()) {
                    summary << "\nYou defeated " << monster.getName() << "!";
                    active_monster
                        .reset(); // no monster until a new one is spawned
                    playerIsSneaking = false; // clear sneak
                    setBodyOutput(summary.str());
                    setErrorOutput("");
                    break;
                }

                // --- Monster counter-attack (unless it was a sneak attack) ---
                if (playerIsSneaking) {
                    // Sneak gives a free hit – no counter this turn
                    summary << monster.getName()
                            << " is caught off guard and can't react!";
                    playerIsSneaking = false; // consume the sneak
                }
                else {
                    // Monster hits back once
                    summary << monster.attackPlayer(player) << "\n";

                    if (player.isDead()) {
                        summary << "You have been defeated by "
                                << monster.getName() << "...";
                        setGameOverFlag(true);
                    }
                }

                setBodyOutput(summary.str());
                setErrorOutput("");
            } break;
        
            case ValidCommand::Collect: {
                Planet& active_planet =
                    ship.getCurrentPlanet(); // will be mutating the rocks on
                                             // the planet in this code

                // Rock Refactor (LV) - deleted code to decide rock to collect
                // based on the biome
                //    Now take a rock from the planet - note - will remove the
                //    rock from the planet after collection If there are no
                //    rocks on the planet, collectRock... will return element
                //    type = "Generic"
                Rock rock = active_planet.collectRockOnPlanet();

                std::string inventory_message;

                if (rock.getElementType() != "Generic") {
                    // Try to add the rock, which fills inventoryMessage
                    if (inventory.addRock(rock)) {
                        inventory_message = "Added " + rock.getName() +
                                            " to inventory.\n(Type 'inventory' "
                                            "to view your inventory)";
                        setBodyOutput(inventory_message);
                        setErrorOutput("");
                    }
                    else { // problem - couldn't add the rock to the inventory,
                           // but rock already deleted
                        // could ToDo possibly add a new planet method to add a
                        // rock and call it to replace the rock note - should
                        // create the method regardless - more intuitive and
                        // scalable
                        inventory_message =
                            "ERR: Inventory is full! Cannot add " +
                            rock.getName() + ".";
                        setBodyOutput("");
                        setErrorOutput(inventory_message);
                    }
                }
                else { // no rocks on the planet to collect
                    setBodyOutput(
                        "You scan the area but find no valuable rocks of "
                        "this planet's type.");
                }
            } break;

            case ValidCommand::Health: {
                std::ostringstream oss;
                oss << "Player health: " << player.getPlayerHealth();
                setErrorOutput(oss.str());
            } break;

            case ValidCommand::Help:
                setBodyOutput(help.getGeneralHelp());
                break;

            case ValidCommand::InspectRock: { // Added curly braces for scope
                if (input.size() < 3) {
                    setErrorOutput("ERR: What rock do you want to inspect? "
                                   "(e.g., inspect rock Basalt Shard)");
                    break;
                }

                int rock_index = stoi(input[3]);

                // inspectRock returns the full string, success or error
                std::string inspect_result = inventory.inspectRock(rock_index);
            } break;

            case ValidCommand::Interact: {
                int npc_index;
                try {
                    npc_index = stoi(input[2]);
                } catch (...) {
                    npc_index = 0;
                }
                std::ostringstream output;
                output << ship.getCurrentPlanet().talkToNPC(npc_index) << "\n";

                setBodyOutput(output.str());
            } break;

            case ValidCommand::Inventory:
                inventory.autoSortRocks(); // Sort before displaying
                setBodyOutput(inventory.getDisplayString());
                break;
            case ValidCommand::ReturnToShip:
                setPlanetFlag(false);
                setShipFlag(true);
                setBodyOutput(ship.returnToShip());
                setArtOutput(art.setArtToShip());
                break;
            case ValidCommand::ScanArea: {
                Planet& active_planet = ship.getCurrentPlanet();
                setBodyOutput(active_planet.describe() +
                              active_planet.listRocks() +
                              active_planet.listPlantsOnPlanet() +
                              active_planet.listNPCs());
                setErrorOutput("Scan complete. Resources listed.\n\n");
            }
                break;

            case ValidCommand::Sneak: {
                playerIsSneaking = true;
                setBodyOutput("You move quietly, preparing a sneak attack...");
                setErrorOutput("");
            } break;

            default:
                std::string error = "ERR: Please enter a valid input";
                setErrorOutput(error);
                break;
            }
        }

        if (passed_command == ValidCommand::UNKNOWN) {
            std::string error = "ERR: Please input a valid command.";
            setErrorOutput(error);
        }

        if (getPlanetFlag()) {
            bool countsTowardsAggro = false;

            switch (passed_command) {
            case ValidCommand::Collect:
            case ValidCommand::ScanArea:
            case ValidCommand::Inventory:
            case ValidCommand::Interact:
            case ValidCommand::Health:
                countsTowardsAggro = true;
                break;
            default:
                break;
            }

            if (countsTowardsAggro) {
                ++planetCommandCounter;
                if (planetCommandCounter >= MONSTER_ATTACK_THRESHOLD) {
                    planetCommandCounter = 0; // reset

                    // Make sure we have a monster for this planet
                    if (!active_monster) {
                        Planet& active_planet = ship.getCurrentPlanet();
                        Biome biome = active_planet.getBiome();
                        int difficulty = active_planet.getLootLevel();

                        active_monster = std::make_unique<Monster>(
                            createMonsterForBiomeAndDifficulty(biome,
                                                               difficulty));
                    }

                    Monster& monster = *active_monster;

                    if (!monster.isDead()) {
                        std::ostringstream oss;
                        // Keep whatever body text we already had this turn
                        oss << getBodyOutput();
                        if (!getBodyOutput().empty())
                            oss << "\n\n";

                        oss << monster.attackPlayer(player) << "\n"
                            << "(Lingering too long has drawn its attention!)";

                        if (player.isDead()) {
                            oss << "\nYou have been slain by "
                                << monster.getName() << "...";
                            setGameOverFlag(true);
                        }

                        setBodyOutput(oss.str());
                    }
                }
            }
        }
        clearScreen(); // Clear screen before start of next loop iteration
    }
}