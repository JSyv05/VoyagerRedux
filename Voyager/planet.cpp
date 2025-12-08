#include "planet.h"
#include "command.h"
#include "rock.h"
#include <vector>
#include <array>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include<string>

Planet::Planet() = default;

// Planet Class Implementation
Planet::Planet(std::string id, std::string name, double distanceAU, Biome biome,
               int loot, std::array<double, 3> coords)
    : id_(move(id)), name_(move(name)), distanceAU_(distanceAU), biome_(biome),
      lootLevel_(loot), coords_(coords) {}

std::string Planet::quickRow(double fuelPerAU) const {
    std::ostringstream oss;
    oss << "[" << id_ << "] " << name_
        << " | " << biomeToString(biome_)
        << " | Lv " << lootLevel_ << " " << getDifficultyIcon() << " | "
        << std::fixed << std::setprecision(2) << distanceAU_ << " AU"
        << " | Pos: (" << coords_[0] << ", " << coords_[1] << ", " << coords_[2] << ")"
        << " | Fuel Cost: " << travelFuelCost(fuelPerAU);
    return oss.str();
}

std::string Planet::getDifficultyIcon() const {
    std::string icon = "";
    int level = lootLevel_;

    // Cap at 10 stars
    for (int i = 0; i < level; ++i)
    {
        icon += "*";
    }

    return icon;
}

PlanetGenerator::PlanetGenerator() : rng(std::random_device{}()) {}

double Planet::travelFuelCost(double fuelPerAU) const {
    return distanceAU_ * fuelPerAU;
}

std::string Planet::describe() const {
    std::ostringstream oss;
    oss << "\n--- " << name_ << " ---\n";
    oss << "Biome: " << biomeToString(biome_) << "\n";
    oss << "Distance: " << std::fixed << std::setprecision(2) << distanceAU_
        << " AU\n";
    oss << "Loot Level: " << lootLevel_ << " " << getDifficultyIcon() << "\n";
    oss << "Surface Conditions: "
        << (biome_ == Biome::Volcanic ? "Molten terrain and unstable geysers."
            : biome_ == Biome::Ocean ? "Vast seas with strong currents."
            : biome_ == Biome::Forest ? "Dense vegetation and humid climate."
            : biome_ == Biome::Urban ? "Ruins of an advanced civilization."
            : biome_ == Biome::Ice ? "Frozen wastelands under dim sunlight."
            : biome_ == Biome::GasGiant ? "Massive storms and crushing pressure."
            : biome_ == Biome::Desert ? "Endless dunes and scorching heat."
            : "Barren and lifeless terrain.")
        << "\n";
    return oss.str();
}

void Planet::populateRocks(const std::vector<Rock>& allRocksInGame) {
    // Get the planet's biome as a string
    std::string biomeName = Planet::biomeToString(this->biome_);

    // Clear any old rocks
    this->rocksOnPlanet_.clear();

    // Loop through the master list of all rocks
    for (const Rock& rock : allRocksInGame) {

        // If the rock's type matches the planet's biome, add it
        if (rock.getElementType() == biomeName) {
            this->rocksOnPlanet_.push_back(rock);
        }
    }
}

std::string Planet::biomeToString(Biome b) {
    switch (b) {
    case Biome::Desert:
        return "Desert";
    case Biome::Ice:
        return "Ice";
    case Biome::Ocean:
        return "Ocean";
    case Biome::Forest:
        return "Forest";
    case Biome::Volcanic:
        return "Volcanic";
    case Biome::GasGiant:
        return "GasGiant";
    case Biome::Urban:
        return "Urban";
    case Biome::Barren:
        return "Barren";
    }
    return "UNKNOWN";
}

// Plants ToDo: add methods to create the flora on a planet & to display the flora on a planet
        // void populatePlantsOnPlanet();
        // std::string listPlanetsOnPlanet();
void Planet::populatePlantsOnPlanet() {
    if (Planet::biome_ != Biome::Forest) {
        return;   // flora only exists on Forest planets
    }
    else {
        // push a hydrangea on this planet
        plantsOnPlanet_.push_back(Plants(Plants::HYDRANGEA, "A lovely pink hydrangea"));
    }
}

std::string Planet::listPlantsOnPlanet() {
    std::ostringstream oss;
    oss << "\n--- Plants on the planet ---\n";
    if (plantsOnPlanet_.empty()) {
        oss << "No Flora found.\n";
    }
    else {
        for (Plants& plant : plantsOnPlanet_) {
            oss << "  - " << plant.displayPlantDescription() << "\n";
        }
    }
    return oss.str();
}

// collectRockOnPlanet - returns a rock from those on the planet & deletes the rock
//    the rock returned is based on LIFO (ie the last rock added to the planet)
//    currently - only a single rock is created on each planet, so that is the rock returned
Rock Planet::collectRockOnPlanet() {
    if (!rocksOnPlanet_.empty()) {
        Rock rock = rocksOnPlanet_.back();   // grab the rock from the back of the vector
        rocksOnPlanet_.pop_back();           //    delete the item from the planet
        return rock;
    }
    else
        return Rock{};                          // return the null object - no rocks found
}

Planet PlanetGenerator::generatePlanet(
    int index, const std::vector<std::array<double, 3>>& existingCoords) {
    std::uniform_real_distribution<double> distAU(0.5, 10.0);
    double distance = distAU(rng);

    std::uniform_int_distribution<int> distBiome(0, 7);
    Biome biome = static_cast<Biome>(distBiome(rng));

    std::uniform_int_distribution<int> distLoot(1, 10);
    int lootLevel = distLoot(rng);

    std::uniform_real_distribution<double> distCoord(-50.0, 50.0);

    // Ensure coordinate are unique
    std::array<double, 3> coords;
    bool unique = false;
    while (!unique) {
        coords = { distCoord(rng), distCoord(rng), distCoord(rng) };
        unique = true;

        for (const auto& c : existingCoords) {
            double dx = coords[0] - c[0];
            double dy = coords[1] - c[1];
            double dz = coords[2] - c[2];
            double dist = sqrt(dx * dx + dy * dy + dz * dz);
            if (dist < 5.0) {
                unique = false;
                break;
            }
        }

        
    }
    // Create a name and ID
    std::string name = generateName();
    std::ostringstream id;
    id << "P" << std::setw(3) << std::setfill('0') << index;

    // Plants ToDo: add call to create the flora on a planet
    Planet p(id.str(), name, distance, biome, lootLevel, coords);
    p.populatePlantsOnPlanet();

    return p;
}


std::string PlanetGenerator::generateName() {
    std::vector<std::string> prefixes = { "RX", "M52", "NX", "LX",
                                         "KZ", "ULS", "AD" };
    std::vector<std::string> suffixes = { "-1b",   "-3c", "-Prime", "-Alpha",
                                         "-Vega", "-9",  "-Tau" };
    std::uniform_int_distribution<int> num(10, 999);

    std::string pre = prefixes[rng() % prefixes.size()];
    std::string suf = suffixes[rng() % suffixes.size()];

    std::ostringstream name;
    name << pre << "-" << num(rng) << suf;
    return name.str();
}

std::string Planet::listRocks() const {
    std::ostringstream oss;
    oss << "\n--- Harvestable Rocks ---\n";
    if (this->rocksOnPlanet_.empty()) {
        oss << "No harvestable rocks found.\n";
    }
    else {
        for (const Rock& rock : this->rocksOnPlanet_) {
            // We can't use rock.inspect() because it prints to cout.
            // We'll build the string manually.
            oss << "  - " << rock.getName() << " (" << rock.getElementType()
                << ")\n";
        }
    }
    return oss.str();
}

//NPC-relate functions
void Planet::populateNPCs(int count)
{
    npcs_.clear();
    std::vector<NPC> selected = pickNPCsForBiome(biome_, count);
    npcs_.insert(npcs_.end(), selected.begin(), selected.end());
}

std::string Planet::listNPCs() const {
    std::ostringstream oss;
    oss << "\n--- Local NPCs ---\n";
    if (npcs_.empty())
    {
        oss << "No life forms or settlements detected.\n";
    }
    else
    {
        for (int i = 0; i < (int)npcs_.size(); ++i)
        {
            oss << "  " << npcs_[i].shortCard(i + 1) << "\n";
        }
    }
    return oss.str();
}

std::string Planet::talkToNPC(int index) const {
    if (index < 1 || index >(int)npcs_.size())
        return "There is no one by that number.";
    return npcs_[index - 1].talkText();
}

std::vector<Planet> PlanetSystem::getPlanetList() const { return planetList; }
Planet PlanetSystem::getPlanetAtIndex(int index) const {
    return planetList[index];
}

void PlanetSystem::generatePlanets(int number,
                                   const std::vector<Rock>& allRocks) {
    Planet earth("NULL", "Earth", 0.0, Biome::Barren, 0, {0, 0, 0});
    PlanetGenerator generator;
    planetList.clear();

    std::vector<std::array<double, 3>> fixedCoords =
    {
        {9, 7, -5}, {10, 5, -2}, {20, -4, 6}, {30, 8, -10}, {40, 0, 5},
        {50, 12, -6}, {60, -8, 10}, {70, 4, -15}, {80, -12, 3}, {90, 7, 9},
        {100, -6, -3}, {110, 10, 4}, {120, -5, 12}, {130, 15, -8},
        {140, -10, 0}, {150, 20, 5}, {160, -15, 10}, {170, 25, -12},
        {180, -20, 8}, {190, 30, 0}
    };
    // Build difficulty (2 per level 1-10)
    std::vector<int> difficultyLevels;
    difficultyLevels.reserve(20);

    for (int lvl = 1; lvl <= 10; ++lvl)
    {
        difficultyLevels.push_back(lvl);
        difficultyLevels.push_back(lvl);
    }

    // Make first 3 planets guaranteed low difficulty
    difficultyLevels[0] = 1;
    difficultyLevels[1] = 1;
    difficultyLevels[2] = 2;

    // Shuffle the rest of the 17 level
    std::mt19937 rng(std::random_device{}());
    shuffle(difficultyLevels.begin() + 3, difficultyLevels.end(), rng);

    // Create all the planets
    for (int i = 0; i < number && i < (int)fixedCoords.size(); ++i) {
        // Generate random name, biome, and loot for the planets
        Planet rand = generator.generatePlanet(i + 1, {});

        // Fixed coordinates for this planet
        const auto& coords = fixedCoords[i];
        // Actual distance AU from origin
        double distanceAU = sqrt(coords[0] * coords[0] + coords[1] * coords[1] + coords[2] * coords[2]) / 10.0;
        // Assigned difficulty
        int lootLevel = difficultyLevels[i];

        //create the planets
        Planet p(rand.getId(), rand.getName(), distanceAU,
            rand.getBiome(), lootLevel, coords);


        p.populateRocks(allRocks);
        p.populatePlantsOnPlanet();
        p.populateNPCs(2); // 2 NPCs per planet for now (later scale by size)

        planetList.push_back(p);           // memory management - this 
    }
}