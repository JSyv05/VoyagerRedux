#pragma once
class Player
{
private:
	double playerHealth = 100;
	double maxHealth = 100;

public:
	Player();  // constructor - create a player with initial health
	double getPlayerHealth() const;   // returns the current health

	//Combat
	int dealDamage() const;
	void takeDamage(int dmg);
    void gainHealth(double heal);
	bool isDead() const;
	void attackMonster(class Monster& monster) const;
};

