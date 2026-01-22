#include "art.h"
#include "ship.h"
#include "planet.h"
#include "inventory.h"
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <string>

// Constructor
Ship::Ship()
    : coordinates{ 0.0, 0.0, 0.0 },
    previousCoordinates{ 0.0, 0.0, 0.0 },
    currentPlanet("NULL", "EARTH", 0.0, Biome::Barren, 0, { 0,0,0 }),
    radar(3),
    fuel(100.0),
    maxFuel(100),
    storage(new Inventory(200))
{
}
// Helper functions - Distance and fuel calcutations
static double calcDistanceAU(const std::array<double, 3>& a,
                             const std::array<double, 3>& b) {
    double dx = a[0] - b[0];
    double dy = a[1] - b[1];
    double dz = a[2] - b[2];
    return sqrt(dx * dx + dy * dy + dz * dz) / 10.0; // scale  to AU
}

static double calcFuelCost(double distanceAU, double fuelPerAU)
{
    return distanceAU * fuelPerAU;
}

Inventory* Ship::getShipStorage() {
    return storage;
}

std::string Ship::getStorageContents() {
    return getShipStorage()->getDisplayString();
}

void Ship::addToShipStorage(Inventory& inventory, int index) {
    Rock temp_rock = inventory.getRockAtIndex(index-1);
    getShipStorage()->addRock(temp_rock);
    inventory.removeRock(index-1);
}

// Coord setter and getter
void Ship::setCoordinates(const std::array<double, 3>& coords) {
    coordinates = coords;
}
std::array<double, 3> Ship::getCoordinates() const { return coordinates; }

// Planet - return handle to the actual planet, so can modify it (e.g. removing rocks)

Planet& Ship::getCurrentPlanet() { return currentPlanet; }

// Radar setter and getter
void Ship::setRadar(int r) { radar = r; }
int Ship::getRadar() const { return radar; }

// fuel setter and getter

double Ship::getFuel() const{
    return fuel;
}

double Ship::getMaxFuel() const{
    return maxFuel;
}

void Ship::refuel(double x) {
    if (fuel + x >= maxFuel) {
        fuel = maxFuel;
    }
    else {
        fuel += x;
    }
}
void Ship::subtractFromFuel(double x) {
    if (fuel - x <= 0) {
        fuel = 0;
    }
    else {
        fuel -= x;
    }
}

// Scans for nerby planets
std::string Ship::getNearbyPlanet(const std::vector<Planet>& planet_vector) {
    const double fuelPerAU = 2.5; 

    //  Build distance list from ship to every planet 
    std::vector<std::pair<double, const Planet*>> planetDistances;
    planetDistances.reserve(planet_vector.size());

    for (const auto& p : planet_vector) {
        double dAU = calcDistanceAU(coordinates, p.getCoordinates());
        planetDistances.push_back({ dAU, &p });
    }

    // Sort by distance (closest first)
    sort(planetDistances.begin(), planetDistances.end(),
        [](const auto& a, const auto& b) { return a.first < b.first; });

    //  Remember last scan for cooldown 
    std::vector<Planet> previousScan = lastScannedPlanets;
    lastScannedPlanets.clear();

    // Compare planets by coordinates (unique in system)
    auto samePlanet = [](const Planet& a, const Planet& b) {
        auto ac = a.getCoordinates();
        auto bc = b.getCoordinates();
        return ac[0] == bc[0] && ac[1] == bc[1] && ac[2] == bc[2];
        };

    auto inPreviousScan = [&](const Planet& p) {
        return any_of(previousScan.begin(), previousScan.end(),
            [&](const Planet& prev) { return samePlanet(p, prev); });
        };

    // Selected planets for this scan: (distance, planet*)
    std::vector<std::pair<double, const Planet*>> selected;

    auto inSelected = [&](const Planet& p) {
        return any_of(selected.begin(), selected.end(),
            [&](const auto& entry) { return samePlanet(p, *entry.second); });
        };

    bool hasTraveledBefore = (currentPlanet.getName() != "EARTH");

    // Always include current planet after first travel 
    if (hasTraveledBefore) {
        for (const auto& entry : planetDistances) {
            const Planet& p = *entry.second;
            if (samePlanet(p, currentPlanet)) {
                selected.push_back(entry);
                lastScannedPlanets.push_back(p);
                break;
            }
        }
    }

    // 1st pass pick planets NOT in previous scan 
    for (const auto& entry : planetDistances)
    {
        if ((int)selected.size() >= radar) { break; }

        const Planet& p = *entry.second;

        if (hasTraveledBefore && samePlanet(p, currentPlanet)) { continue; }  // already added above

        if (inPreviousScan(p)) { continue; } // cooldown: skip planets just seen last scaned
     

        if (inSelected(p)) { continue; } // no duplicates

        selected.push_back(entry);
        lastScannedPlanets.push_back(p);
    }

    // 2nd pass if still not full, allow repeats
    if ((int)selected.size() < radar) {
        for (const auto& entry : planetDistances) {
            if ((int)selected.size() >= radar)
                break;

            const Planet& p = *entry.second;

            if (inSelected(p))
                continue;

            selected.push_back(entry);
            lastScannedPlanets.push_back(p);
        }
    }

    // Build display from selected
    std::ostringstream display;
    display << "--Nearest Planets Detected--\n\n";

    for (size_t i = 0; i < selected.size(); ++i) {
        double distanceFromShipAU = selected[i].first;
        const Planet& p = *selected[i].second;

        bool isCurrent = hasTraveledBefore && samePlanet(p, currentPlanet);

        // quickRow now gets the distance from the ship so it can compute fuel
        display << "(" << (i + 1) << ") "
            << p.quickRow(distanceFromShipAU);

        if (isCurrent)
            display << "  <-- (You Are Here)";

        display << "\n";
    }

    display << "\n(Type 'travel -d 1', 'travel -d 2', or 'travel -d 3' to visit a planet.)";

    return display.str();
}

// Travel to planet, dock on planet, displays name and description
std::string Ship::travelToPlanet(int choice) {
    if (choice > 3 || choice < 1) {
        throw std::out_of_range("ERR: index is out of range");
    }
    Planet destination = lastScannedPlanets[choice - 1];
    const auto& destCoords = destination.getCoordinates();

    // Calculate travel stats
    double distanceAU = calcDistanceAU(coordinates, destCoords);
    double fuelPerAU = 2.5;
    double fuelCost = calcFuelCost(distanceAU, fuelPerAU);

    previousCoordinates = coordinates;
    coordinates = destCoords;
    currentPlanet = destination;
    fuel -= fuelCost;
    std::ostringstream msg;
    msg << "Traveling to " << currentPlanet.getName() << "...\n";
    msg << "Distance: " << std::fixed << std::setprecision(2) << distanceAU << " AU\n";
    msg << "Fuel Used: " << fuelCost << "\n\n";
    msg << "Docking at " << currentPlanet.getName() << ".\n";
    msg << currentPlanet.describe();
    msg << "\n(Type 'exit ship' to step out onto the surface.)";

return msg.str();
}

// Return to ship. Plus undock and hover
std::string Ship::returnToShip() const{
    std::ostringstream msg;
    msg << "You return to your ship and begin pre-flight checks";
    msg << "The ship is now ready for takeoff.\n\n";
    msg << "\n(Type scan -p' to return to scan for nearby planets";

    return msg.str();
}

std::string Ship::shipExit() const {
    std::ostringstream msg;
    msg << "You step out onto the surface of " << currentPlanet.getName()
        << ".\n\n";
    msg << currentPlanet.describe();
    msg << "\nType 'scan -a' to look around";
    msg << "\n(Type 'return to ship' when you are done";

    return msg.str();
}