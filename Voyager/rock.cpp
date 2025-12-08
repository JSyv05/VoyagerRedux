#include "rock.h"
#include <iostream>
#include <vector>
#include <string>

// global function to create rocks - bad!!  ToDo - refactor this
// Function implementation for Rock class
std::vector<Rock> createMasterRockList() {
    std::vector<Rock> all_rocks;

    // Rock(name, description, elementType, value, resourceYielded, yieldAmount)
    all_rocks.push_back(Rock("Basalt Shard",
        "A dark, fine-grained volcanic rock.", "Volcanic",
        10, "Iron", 5));
    all_rocks.push_back(Rock("Pumice Stone",
        "A very light and porous volcanic rock.",
        "Volcanic", 5, "Sulfur", 10));
    all_rocks.push_back(Rock("Iron Ore", "A rusty-red rock, heavy with metal.",
        "Metallic", 25, "Iron", 20));
    all_rocks.push_back(Rock("Ice Chunk", "A chunk of frozen, murky water.",
        "Ice", 1, "Water", 10));
    all_rocks.push_back(Rock("Sandstone", "A common sedimentary rock.", "Desert",
        2, "Silicon", 3));
    all_rocks.push_back(Rock("Petrified Wood", "Ancient wood turned to stone.",
        "Forest", 15, "Carbon", 10));
    all_rocks.push_back(Rock("Barren Stone", "A simple, useless rock.", "Barren",
        0, "Gravel", 1));

    return all_rocks;
}

// Constructors
Rock::Rock()
    : name("Unnamed Rock"), description("Just a rock."), elementType("Generic"),
      value(0), resourceYielded("Gravel"), yieldAmount(1) {}

Rock::Rock(const std::string& name, const std::string& description,
           const std::string& elementType, int value,
           const std::string& resourceYielded, int yieldAmount)
    : name(name), description(description), elementType(elementType),
      value(value), resourceYielded(resourceYielded), yieldAmount(yieldAmount) {
}

// Public Methods
void Rock::inspect() const {
    std::cout << "  - " << name << " (" << elementType << "). Yields "
              << yieldAmount << " " << resourceYielded << "." << std::endl;
}

// Getters
std::string Rock::getElementType() const { return elementType; }
std::string Rock::getName() const { return name; }
int Rock::getValue() const { return value; }
std::string Rock::getResourceYielded() const { return resourceYielded; }
int Rock::getYieldAmount() const { return yieldAmount; }
