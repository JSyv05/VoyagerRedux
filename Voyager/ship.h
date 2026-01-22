#ifndef SHIP_H
#define SHIP_H

#include "planet.h"
#include "inventory.h"
#include <array>
#include <string>
#include <vector>

class Ship {
private:
    std::array<double, 3> coordinates;              // Ship coordinates
    std::array<double, 3> previousCoordinates;      // Ship last coordinates
    std::vector<Planet> lastScannedPlanets;         // The 3 closest planets after scanning.
    Planet currentPlanet;              // The planet the ship is at
    int radar; // Number of planets from scan (default is 3)
    double fuel;
    int maxFuel;
    Inventory* storage;

public:
    Ship();

    Planet& getCurrentPlanet();

    // Coordinates
    void setCoordinates(const std::array<double, 3>& coords);
    std::array<double, 3> getCoordinates() const;

    // Radar
    void setRadar(int r);
    int getRadar() const;

    // Fuel
    void refuel(double);
    void subtractFromFuel(double);
    double getFuel() const;
    double getMaxFuel() const;


    // Ship actions
    std::string getNearbyPlanet(const std::vector<Planet>&);            // Scans for planets
    Inventory* getShipStorage();
    std::string getStorageContents();
    void addToShipStorage(Inventory& inventory, int index);
    std::string travelToPlanet(int choice); // Travel/Dock at chosen planet
    std::string returnToShip() const;               // Returns tp ship
    std::string shipExit() const;                   // Exits ship
};
#endif