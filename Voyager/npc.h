#pragma once
#include <string>
#include <vector>

// Forward declaration from planet.h
class Player;
enum class Biome;


// Types of NPCs
enum class NpcCategory { Person, Monster, Event };

// Base NPC Class
class NPC
{
private:
    std::string name_;
    std::string openingLine_;
    std::string appearance_;
    NpcCategory category_;
    Biome biome_;

public:
    NPC() = default;
    NPC(std::string name, std::string line, std::string appearance, NpcCategory cat, Biome biome);

    std::string getType() const;
    std::string getName() const;
    std::string getOpeningLine() const;
    std::string getAppearance() const;
    Biome getBiome() const;

    std::string shortCard(int index) const;
    std::string talkText() const;
};

// Monster Class (Derived from NPC)
class Monster : public NPC
{
protected:
    int health_;
    int attackPower_;

public:
    Monster(std::string name, std::string line, std::string appearance, NpcCategory cat, Biome biome, int hp, int atk);
    
    //getters 
    int getHealth() const { return health_; }
    int getAttackPower() const { return attackPower_; }

    int dealDamage() const;
    void takeDamage(int dmg);
    bool isDead() const;
    std::string attackPlayer(Player& player);
};

// Returns all NPCs and monsters in the game
const std::vector<NPC>& getAllNPCs();
const std::vector<Monster>& getAllMonsters();

// Picks N NPCs by biome (no repeats)
std::vector<NPC> pickNPCsForBiome(Biome biome, int count);
Monster createMonsterForBiomeAndDifficulty(Biome biome, int difficulty); //difficulty is your lootLevel


// Combat hlper functions

// Alternating turn comber. Monster auto-attacks on odd-numberd turns
// If playerGetsFirstStrike is true, player gets a free attack
void attackSequence(Monster& monster, Player& player, int& turn_counter, bool playerGetsFirstStrike);

// Convenience wrapper - starts a combat with a fresh turn couter
void monsterStartAttack(Monster& monster, Player& player, bool playerGetsFirstStrike);

// Called every time the player input a command while on a planet
// When 'counter' reaches `threshold`, it rests and starts combat.
void inputCounter(int& counter, int threshold, Monster& monster, Player& player, bool& playerGetsFirstStrike);