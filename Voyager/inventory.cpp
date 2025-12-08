#include "inventory.h"
#include <iostream>
#include <algorithm> 
#include <iomanip>   
#include <sstream>   
#include <string>

// Constructor
Inventory::Inventory(int startCapacity) : maxCapacity(startCapacity) {
}

bool Inventory::addRock(const Rock& rock) {
    if (rocks.size() < maxCapacity) {
        rocks.push_back(rock);
        return true;
    }
    else {
        return false;
    }
}

bool Inventory::upgradeCapacity(int amount, std::string& message) {
    if (amount > 0) {
        maxCapacity += amount;
        message = "Inventory capacity upgraded by " + std::to_string(amount)
            + "! New capacity: " + std::to_string(maxCapacity);
        return true;
    }
    else {
        message = "Invalid upgrade amount.";
        return false;
    }
}

/**
Helper comparison function for sorting.
 */
bool compareRocksByElement(const Rock& a, const Rock& b) {
    return a.getElementType() < b.getElementType();
}

void Inventory::autoSortRocks() {
    sort(rocks.begin(), rocks.end(), compareRocksByElement);
}

std::string Inventory::getDisplayString() const {
    std::ostringstream oss;
    oss << "\n--- INVENTORY (" << rocks.size() << "/" << maxCapacity << ") ---"
        << "\n";
    if (rocks.empty()) {
        oss << "Inventory is empty." << "\n";
        return oss.str();
    }

    const int nameWidth = 20;
    const int typeWidth = 15;
    const int valueWidth = 10;
    const int resourceWidth = 15;

    oss << std::left
        << std::setw(nameWidth) << "Rock Name"
        << std::setw(typeWidth) << "Element"
        << std::setw(valueWidth) << "Value"
        << std::setw(resourceWidth) << "Resource"
        << "\n";
    oss << std::string(nameWidth + typeWidth + valueWidth + resourceWidth, '-')
        << "\n";

    // Use the getter methods from Rock class
    for (const Rock& rock : rocks) {
        oss << std::left
            << std::setw(nameWidth) << rock.getName()
            << std::setw(typeWidth) << rock.getElementType() 
            << std::setw(valueWidth) << rock.getValue()
            << std::setw(resourceWidth) << (std::to_string(rock.getYieldAmount()) + " " + rock.getResourceYielded())
            << "\n";
    }
    oss << "------------------------------------------------------------\n\n";
    return oss.str();
}

Rock Inventory::getRockAtIndex(int index) {
    return rocks[index];
}

std::string Inventory::removeRock(const int& index) {
    std::ostringstream oss;
    try {
        if (index < 0 || index > rocks.size()) {
            throw std::out_of_range("ERR: please input a valid index for the inventory system");
        }
        Rock rock = getRockAtIndex(index);
        rocks.erase(rocks.begin() + index);
        oss << "Removed " << rock.getName();
        return oss.str();
    }
    catch (const std::out_of_range& e) {
        oss << e.what();
        return oss.str(); 
    }    
}

std::string Inventory::inspectRock(const int& index) {

    std::ostringstream oss;
    try {
        if (index - 1 < 0 || index > rocks.size()) {
            throw std::out_of_range("ERR: please input a valid index for the inventory system");
        }
        Rock rock = getRockAtIndex(index - 1);
        rocks.erase(rocks.begin() + index - 1);
        oss << "  - " << rock.getName() << " (" << rock.getElementType() << "). Yields "
            << rock.getYieldAmount() << " " << rock.getResourceYielded() << ".";
        return oss.str();
    }
    catch (const std::out_of_range& e) {
        oss << e.what();
        return oss.str();
    }
}


// --- Getter Implementations ---

int Inventory::getCurrentSize() const {
    return static_cast<int>(rocks.size());
}

int Inventory::getMaxCapacity() const {
    return maxCapacity;
}