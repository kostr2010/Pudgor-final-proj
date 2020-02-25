#pragma once

#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <map>

//#include "engine-constants.h"

//===================
// ENUMS

enum AbilityKind {
    Kick,
    Health,
};

enum AbilityStateKey {
    DamageAmount,
    HpMax,
    HpCurrent,
};

//====================
// CLASSES DECLARATION

class Ability {
public:
    Ability();
    Ability(AbilityKind kind, std::map<AbilityStateKey, double> init_state);

    void   Recieve(std::map<AbilityStateKey, double> state_delta);
    double GetStateValue(AbilityStateKey name);
    void   SetStateValue(AbilityStateKey name, double value);

    friend bool          operator==(const Ability& left, const Ability& right);
    friend std::ostream& operator<< (std::ostream& stream, Ability ability);

    AbilityKind kind_;
private:
    std::map<AbilityStateKey, double> state_;
protected:
};

class Entity {
public:
    Entity(std::map<AbilityKind, Ability> abilities);

    void Apply(AbilityKind kind, Entity& target);

    friend std::ostream& operator<< (std::ostream& stream, Entity entity);

    std::map<AbilityKind, Ability> abilities_;
    std::vector<Entity> subentities_;
private:
protected:
};
