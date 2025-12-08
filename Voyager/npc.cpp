#include "npc.h"
#include "planet.h"
#include "Player.h"
#include <algorithm>
#include <random>
#include <sstream>

// NPC Implementation
NPC::NPC(std::string name, std::string line, std::string appearance, NpcCategory cat, Biome biome)
    : name_(move(name)), openingLine_(move(line)), appearance_(move(appearance)), category_(cat), biome_(biome) {}

std::string NPC::getType() const
{
    switch (category_)
    {
    case NpcCategory::Person: return "Person";
    case NpcCategory::Monster: return "Monster";
    case NpcCategory::Event: return "Event";
    default: return "Unknown";
    }
}

std::string NPC::getName() const { return name_; }
std::string NPC::getOpeningLine() const { return openingLine_; }
std::string NPC::getAppearance() const { return appearance_; }
Biome NPC::getBiome() const { return biome_; }

std::string NPC::shortCard(int index) const {
    return "(" + std::to_string(index) + ") " + name_ + " | " + getType();
}

std::string NPC::talkText() const {
    return name_ + " says: " + openingLine_ + "\nAppearance: " + appearance_ + "\n";
}

// Monster Implementation
Monster::Monster(std::string name, std::string line, std::string appearance,
                 NpcCategory cat, Biome biome, int hp, int atk)
    : NPC(name, line, appearance, NpcCategory::Monster, biome), health_(hp), attackPower_(atk) {}

int Monster::dealDamage() const { return attackPower_; }

void Monster::takeDamage(int dmg)
{
    health_ -= dmg;
    if (health_ < 0)
        health_ = 0;
}

bool Monster::isDead() const
{
    return health_ <= 0;
}

std::string Monster::attackPlayer(Player& player) {
    int dmg = dealDamage();
    player.takeDamage(dmg);

    std::ostringstream ss;
    ss << getName() << " attacks you for " << dmg << " damage!\n"
        << "Your health is now " << player.getPlayerHealth() << ".";
    return ss.str();
}

// Comabt helpers 
// Core attack loop
// - Monster attacks on odd-numbered turhns (turn_counter % 2 != 0)
// - Player attacks on even-numbered turns
// - If playerGetsFirstStrike is true, the player gets one sneak hit before the loop
void attackSequence(Monster& monster, Player& player, int& turn_counter, bool playerGetsFirstStrike)
{
    if (playerGetsFirstStrike && !monster.isDead() && player.getPlayerHealth() > 0)
    {
        player.attackMonster(monster);
        if (monster.isDead()) return;

    }

    while (!monster.isDead() && player.getPlayerHealth() > 0)
    {
        if (turn_counter % 2 != 0)
        {
            monster.attackPlayer(player);
        }
        else
        {
            player.attackMonster(monster);
        }
        ++turn_counter;
    }
}

// Starts a combat with a new turn_counter
void monsterStartAttack(Monster& monster, Player& player, bool playerGetsFirstStrike)
{
    int turn_counter = 0;
    attackSequence(monster, player, turn_counter, playerGetsFirstStrike);
}

void inputCounter(int& counter, int threshold, Monster& monster, Player& player, bool& playerGetsFirstStrike)
{
    ++counter;
    if (counter >= threshold)
    {
        counter = 0;
        monsterStartAttack(monster, player, playerGetsFirstStrike);
    }
}



// NPC DATA (2 per Biome for testing)
const static std::vector<NPC> ALL_NPCS = {
    // Forests noncombat NPCs
    {"Gorm Garnersson",
     "Greetings outlander, I am Gorm of house Garnersson. What brings you to this realm?",
     "7.5 ft tall, draconic features, armored knight with sword and dagger.",
     NpcCategory::Person, Biome::Forest},
    {"Woodrow Burch",
     "Don’t go barking up the wrong tree, pal.",
     "Tree-guy with green leafy hair.",
     NpcCategory::Person, Biome::Forest},

     // Volcanic
     {"Hot Mama",
      "Is it gettin’ hot in here or is it just you?",
      "Long flowing flame hair; eyes like burning coals.",
      NpcCategory::Person, Biome::Volcanic},
     {"Haot Roks",
      "Roks.",
      "8-ft volcanic rock being with glowing cracks.",
      NpcCategory::Monster, Biome::Volcanic},

      // Ocean
      {"Whaley Mamaley",
       "Butter my blowhole, ain’t you a sight for sore eyes!",
       "Whale in overalls with a top hat.",
       NpcCategory::Person, Biome::Ocean},
      {"Marina May",
       "Warm waves welcome you, traveler.",
       "Coral body with pink-to-orange gradient.",
       NpcCategory::Person, Biome::Ocean},

       // Ice
       {"Icicle",
        "I control the snow, ice, and cold.",
        "Blue skin, white hair, blue leather jumpsuit.",
        NpcCategory::Person, Biome::Ice},
       {"Shivers McTimber",
        "There be a cold wind hoistin’ me sails.",
        "Pirate — but blue because he’s cold.",
        NpcCategory::Person, Biome::Ice},

        // Gas Giant
        {"Stormie Nitez",
         "A storm’s a brewin’.",
         "Dark cloud-like hair with lightning streaks.",
         NpcCategory::Event, Biome::GasGiant},
        {"Feebus Fellow",
         "Who the hell are you?",
         "5’2, pink hair, surprisingly nice clothes.",
         NpcCategory::Person, Biome::GasGiant},

         // Barren
         {"P.A.R.E.",
          "... SCANNING ... USER NOT FOUND ... SHOW DRESCO ID.",
          "Rusty humanoid robot barely moving.",
          NpcCategory::Person, Biome::Barren},
         {"Barren John",
          "OoooOooOoOoOoOoOoOoOoo...",
          "Dark silhouette with glowing eyes.",
          NpcCategory::Event, Biome::Barren},

          // Urban
          {"Taxi Jaxi",
           "Need yaself a ride? Take a ride in Taxi Jaxi’s Jaxi Taxi!",
           "1920s-style cabbie with newsboy hat.",
           NpcCategory::Person, Biome::Urban},
          {"Justin Case",
           "Greetings partner… foremost, amazing, genius, expert…",
           "5'8 robot in dirty overcoat; cardboard badge.",
           NpcCategory::Person, Biome::Urban},

           // Desert
           {"Dusty Oasis",
            "Awful hot out there — rest in the shade.",
            "Desert gear; sandy hair with blue streaks.",
            NpcCategory::Person, Biome::Desert},
           {"Jane Finch",
            "Jane Finch. I'm a problem solver.",
            "Cowgirl engineer with belts full of gadgets.",
            NpcCategory::Person, Biome::Desert},
};

const std::vector<NPC>& getAllNPCs() { return ALL_NPCS; }

std::vector<NPC> pickNPCsForBiome(Biome biome, int count) {
    std::vector<NPC> pool;
    for (auto& npc : ALL_NPCS)
        if (npc.getBiome() == biome)
            pool.push_back(npc);

    if (pool.empty())
        return {};

    static std::mt19937 rng{std::random_device{}()};
    shuffle(pool.begin(), pool.end(), rng);

    if ((int)pool.size() > count)
        pool.resize(count);

    return pool;
}

// All the monsters objects
static std::vector<Monster> All_MONSTERS = {
    // Forests Monster
    Monster("Tree Monster", "THE FOREST HUNGERS!", "A towering walking tree...", 
            NpcCategory::Monster, Biome::Forest, 40, 10),   // HP = 40 and Attack = 10

    // Volcanic Monster 
    Monster("Lava Serpent", "The ground cracks and molten eyes glare at you...", "A long serpent made of rock and magma.",
            NpcCategory::Monster, Biome::Volcanic, 35, 12),

    // Ocean Monster
    Monster("Leviathan", "THE LEVIATHAN IS COMING...", "A titanic sea serpent rises from the depths, its body dwarfing "
        "everything around it and its roar shaking the ocean itself.", NpcCategory::Monster, Biome::Ocean, 60, 12),

    // Ice Monster 
    Monster("Glacier Wraith", "A chilling whisper echoes across the tundra...", "A translucent spirit wrapped in frost.",
             NpcCategory::Monster, Biome::Ice,30, 8),

   // Gas Giant Monster 
   Monster("Storm Titan", "The clouds tremble... thunder answers.", "A towering figure of swirling clouds and lightning, its body "
           "formed from the raw fury of the gas giant's atmosphere.", NpcCategory::Monster, Biome::GasGiant, 55, 10),

   // Barren Monster 
   Monster("Dust  Wraith", "A hollow whisper drifts across the wasteland...", "A ghostly creature formed from dust and splintered bone, drifting "
           "silently through the barren plains.", NpcCategory::Monster, Biome::Barren, 35, 8),

   // Urban Monster 
    Monster("Living Trash Heap", "Garbage shifts... then stands upright.", "A massive pile of animated refuse, leaking sludge and rattling "
            "with broken machinery, trudges toward you.", NpcCategory::Monster, Biome::Urban, 50, 9),

   // Desert Monster 
    Monster("Colossal Scorpion", "The sands shift... something enormous crawls beneath them.", "A gigantic scorpion bursts from the dunes, its armored plates "
            "glinting beneath the scorching sun.", NpcCategory::Monster, Biome::Desert, 45, 11),
 
};

// Return all templates
const std::vector<Monster>& getAllMonsters() {
    return All_MONSTERS;
}


// Find a template for this biome, or nullptr if none
static const Monster* findTemplateForBiome(Biome biome)
{
    for (const auto& mon : All_MONSTERS)
    {
        if (mon.getBiome() == biome)
            return &mon;
    }
    return nullptr;
}

// Creates a monster instance scaled by difficulty
Monster createMonsterForBiomeAndDifficulty(Biome biome, int difficulty)
{
    const Monster* tmpl = findTemplateForBiome(biome);

    // If no monster for this biome yet, fall back to a generic weak one
    if (!tmpl)
    {
        return Monster(
            "Mysterious Shade",
            "...something stirs in the shadows...",
            "A faint, shifting silhouette.",
            NpcCategory::Monster,
            biome,
            20,  // HP
            5    // attack
        );
    }

    // Base stats from template
    int baseHp = tmpl->getHealth();
    int baseAtk = tmpl->getAttackPower();

    // Difficulty scaling: tweak numbers as you like
    // difficulty is 1–10 from your difficultyLevels / lootLevel_
    int extraHp = (difficulty - 1) * 3;  // +3 HP per level above 1
    int extraAtk = (difficulty - 1) * 1;  // +1 ATK per level above 1

    int scaledHp = baseHp + extraHp;
    int scaledAtk = baseAtk + extraAtk;

    // Build a fresh Monster for this combat
    return Monster(
        tmpl->getName(),
        tmpl->getOpeningLine(),
        tmpl->getAppearance(),
        NpcCategory::Monster,
        biome,
        scaledHp,
        scaledAtk
    );
}

