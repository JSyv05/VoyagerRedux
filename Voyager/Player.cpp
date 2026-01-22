#include "Player.h"
#include "npc.h"

// Forward declaration
class monster;

/**
 * Player class
 *   Currently only supports creation of player objects with a default health
 * future changes will provide the ability to decrease health.
 */

 // constructor - create a player with initial health
Player::Player() = default;
//add two method 
//bool iscrritable
//method to decrease

//gameloop just checks

  // returns the current health
double Player::getPlayerHealth() const{
	return playerHealth;
}

int Player::dealDamage() const
{
	return 10;   //Hard code value we can change this later. needed to test the monster taking damage
}
 

void Player::takeDamage(int dmg)
{
	playerHealth -= dmg;
	if (playerHealth < 0)
		playerHealth = 0;
}

void Player::gainHealth(double heal) {
    if (playerHealth + heal >= maxHealth) {
        playerHealth = maxHealth;
	}
    else {
        playerHealth += heal;
	}
}

bool Player::isDead() const
{
	return playerHealth <= 0;
}

void Player::attackMonster(Monster& monster) const
{
	int dmg = dealDamage(); 
	monster.takeDamage(dmg);
}