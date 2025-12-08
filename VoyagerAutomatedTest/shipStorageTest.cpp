#include "pch.h"
#include "CppUnitTest.h"
#include "../Voyager/ship.h"
#include "../Voyager/inventory.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace VoyagerautomatedTest {
    TEST_CLASS(ShipStorageTest){
        TEST_METHOD(list_storage){
            Logger::WriteMessage("Testing to see if the ship will list out it's contents\n");
            Ship ship;
            std::string not_expected = "";
            std::string expected = ship.getShipStorage()->getDisplayString();
            Assert::IsTrue(not_expected != expected, L"ERR: List did not print properly");
            Logger::WriteMessage("Confirmed that the storage system is able to list its contents");
        }
        TEST_METHOD(move_from_inventory_to_storage) {
            Logger::WriteMessage("Testing to move from one rock from inventory to storage\n");
            Inventory inventory(20);
            Rock rock("Basalt Shard",
                "A dark, fine-grained volcanic rock.", "Volcanic",
                10, "Iron", 5);
            Ship ship;
            std::string before_storage = ship.getStorageContents();
            std::string before_move = inventory.getDisplayString();
            inventory.addRock(rock);
            ship.addToShipStorage(inventory, 1);
            std::string after_storage = ship.getStorageContents();
            std::string after_move = inventory.getDisplayString();
            Assert::IsTrue( before_move != after_move, L"Rock was not removed from inventory");
            Logger::WriteMessage("Rock was removed from the player's inventory\n");
            Assert::IsTrue(before_storage != after_storage, L"Storage was not updated with new rock");
            Logger::WriteMessage("Rock was added to the ship's storage");
        }
    };
}