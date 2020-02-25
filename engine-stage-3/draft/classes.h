#pragma once

//====================
// INCLUDES

#include <vector>
#include <string>

//====================
// DATATYPES DECLARATION

//====================
// base classes

class Command {
public:
private:
protected:
};

class Ability {
public:
    Ability(const std::string& name = "");

    friend bool operator==(const Ability& left, const Ability& right);
private:
protected:
    std::string name_;
};

class Character {
public:
    Character() {};

    void    AddAbility(const std::vector<Ability>& abilities_to_add);   // adds certain ability to the object
    Ability GetAbility(const Ability& ability_to_get) const;            // null or ability, if found
private:
protected:
    // ? if going to add Thing class, there should be a stats field
    std::vector<Ability> abilities_;
};  

class Tile {
public:
    Tile();
    // Tile(preset, args...?)   
private:
protected:
    int pos_x_;
    int pos_y_;

    std::vector<Character> characters_; // #TOFIX: unclear how to place multiple instances on the same tile
                                        // possible fix: thing class
};

class Map {
public:
    Map();
    // Map(preset, seed...?)
private:
    std::vector<Tile> tiles_;
    std::vector<Character> friendlies_;
    std::vector<Character> neutrals_;
    std::vector<Character> foes_;
protected:

};

//====================
// characters

class CreateCharacter {
public:
    Character CreateWarrior();
    // ... other characters
private:
protected:
};

//====================
// abilities 

class AbilityMove: public Ability {
public:
    AbilityMove();

    void MoveTo(const int dst_x, const int dst_y);
private:
// protected:
//     int pos_x_;    // #TOFIX: duplication af data with Tile class
//     int pos_y_;
};

class AbilityHealth: public Ability {
public:
    AbilityHealth();

    void HealthSetHealth(int health);
    void HealthSetMaxHealth(int health);
private:
protected:
    int cur_health_;
    int max_health_;
};

class AbilityShoot: public Ability {
public:
    AbilityShoot(const int damage, const int range);

    void Shoot();
    void Reload();
    void ChangeAmmoType();
private:
protected:
    int mag_left_;
    int mag_max_;
};

class AbilityStab: public Ability {
public:
    AbilityStab(int damage);    // ? add size effects, like stun, etc
private:
protected:
};

//====================
// commands

class Command {
public:
private:
protected:
};

class CommandDealDamage: public Command {
public:
    CommandDealDamage(Character& target, int damage);

    void Execute();
private:
protected:
    Character target_;
    int damage_;
};