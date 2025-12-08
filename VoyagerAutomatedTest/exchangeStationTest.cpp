#include "pch.h"
#include "CppUnitTest.h"
#include "../Voyager/exchange.h"
#include "../Voyager/ship.h"
#include "../Voyager/inventory.h"
#include "../Voyager/rock.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace VoyagerautomatedTest {
    TEST_CLASS(ExchangeStationTest) {
        TEST_METHOD(exchange_from_storage_to_loot_point) {
            Logger::WriteMessage("Testing to see if the exchange station will remove the sample from storage and add to the loop point total\n");
            ExchangeStation eStation;
            Ship ship;
            Rock rock("Basalt Shard",
                "A dark, fine-grained volcanic rock.", "Volcanic",
                10, "Iron", 5);
            Inventory* storage = ship.getShipStorage();

            storage->addRock(rock);
            Logger::WriteMessage("Added rock to inventory\n");

            std::string before_removal = storage->getDisplayString();
            Logger::WriteMessage(before_removal.c_str());
            int before_exchange = eStation.getLootPoint();

            eStation.exchangeSampleForLootPoint(storage, 1);
            Logger::WriteMessage("Attempted to exchange sample\n");

            std::string after_removal = storage->getDisplayString();
            Logger::WriteMessage(after_removal.c_str());
            int after_exchange = eStation.getLootPoint();

            Assert::AreNotEqual(before_exchange, after_exchange,
                   L"Item was not exchanged for points");
            Logger::WriteMessage(
                "Item was successfully exchanged for points\n");


            Assert::IsTrue(before_removal != after_removal, L"Item was not removed from storage");
            Logger::WriteMessage("Item was removed from storage\n");

        }
        TEST_METHOD(exchange_loot_point_to_fuel) {
            Logger::WriteMessage("Testing to exchange loot points for fuel\n");
            Ship ship;
            ExchangeStation eStation;
            ship.subtractFromFuel(50);
            eStation.addToLootPoint(15);

            double before_fuel = ship.getFuel();
            int before_point = eStation.getLootPoint();

            ship.setFuel(eStation.exchangeLootPointForFuel(15));

            double after_fuel = ship.getFuel();
            int after_point = eStation.getLootPoint();

            Assert::AreNotEqual(before_point, after_point, L"Points were not spent to fuel the ship");
            Logger::WriteMessage("Points were spent to fuel the ship successfully\n");

            Assert::AreNotEqual(before_fuel, after_fuel, L"Ship was not fueled");
            Logger::WriteMessage("Ship was fueled successfully\n");
        }
        TEST_METHOD(exchange_loot_point_to_health) {
            Logger::WriteMessage("Testing to exchange loot points for health\n");
        }
    };
}