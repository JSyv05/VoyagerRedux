#include "pch.h"
#include "CppUnitTest.h"
#include "../Voyager/plants.h"
#include "../Voyager/planet.h"
#include "../Voyager/rock.h"
#include "../Voyager/npc.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

/*
*  Refactoring rock collection
*    First test - verify can collect the rocks with the new method
*/

namespace VoyagerAutomatedTest
{
	TEST_CLASS(RockCollectionTest)
	{
	public:
		//  Test rock refactoring
		TEST_METHOD(RockRefactoringTest) {
			Logger::WriteMessage("Creating a few planets & show the rocks created\n");
			const int NUM_PLANETS_TO_CREATE = 10;
			Planet planetArray[NUM_PLANETS_TO_CREATE];
			PlanetGenerator generator;
			std::vector<Rock> allGameRocks = createMasterRockList();
			//     Planet(std::string id, std::string name, double distanceAU, Biome biome,int loot, std::array<double, 3>)
			// Planet PlanetGenerator::generatePlanet(int index, const vector<array<double, 3>>& existingCoords) {
			// verify that I can create 5 planets & will see rocks on each planet
			for (int planetNum = 0; planetNum < NUM_PLANETS_TO_CREATE; planetNum++) {
				// generate the planet
				planetArray[planetNum] = generator.generatePlanet(planetNum, { {0,0}, {0,0}, {0,0} });

				Planet p = planetArray[planetNum];   // short cut, tempoary variable to reduce typing and space
				// display the planet & biome
				std::string s = "\n" + p.getName() + "\tBiome: " +
					p.biomeToString(p.getBiome());       // no need for a new line, listRocks() takes care of that
				// create the Rocks on the planet & then display them
				Logger::WriteMessage(s.c_str());
				p.populateRocks(allGameRocks);
				s = p.listRocks() + "\n";
				Logger::WriteMessage(s.c_str());

				// next - grab a rock off of the planet & display the rock, as well as the remaining rocks on the planet
				Rock rock = p.collectRockOnPlanet();
				if (rock.getElementType() != "Generic") {
					s = "Rock collected from the planet " + rock.getName();
				}
				else { // no rocks found on the planet
					s = "NO ROCKS ON THE PLANET TO COLLECT!";
				}
				Logger::WriteMessage(s.c_str());
				s = p.listRocks();
				Logger::WriteMessage(s.c_str());
			}
		}
	};
}
