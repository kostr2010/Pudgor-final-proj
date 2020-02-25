//====================
// INCLUDES

#include <vector>
#include <string>
#include <iostream>

#include "classes.h"

//====================
// METHODS IMPLEMENTATION

//====================
// character 

bool operator==(const Ability& left, const Ability& right) {
    return left.name_ == right.name_;
}

void Character::AddAbility(const std::vector<Ability>& abilities_to_add) {
    for (const auto& ability : abilities_to_add)
        abilities_.push_back(ability);
}

Ability Character::GetAbility(const Ability& ability_to_get) const {
    for (auto a : abilities_)
        if (a == ability_to_get)
            return a;

    return {};
}

Character CreateCharacter::CreateWarrior() {
    Character warrior;
    warrior.AddAbility({AbilityStab(10), AbilityHealth(), AbilityMove()});

    return warrior;
}


//====================
// ability 

Ability::Ability(const std::string& name) {
    name_ = name;

    return;
}

AbilityMove::AbilityMove() : Ability("move") {
    return;
}

AbilityHealth::AbilityHealth() : Ability("health") {
    return;
}

AbilityShoot::AbilityShoot(const int damage, const int range) : Ability("shoot") {
    return;
}

AbilityStab::AbilityStab(const int damage) : Ability("stab") {
    return;
}

//====================
// command 

CommandDealDamage::CommandDealDamage(Character& target, int damage) {
    target_ = target;
    damage_ = damage_;

    return;
}

void CommandDealDamage::Execute() {
    Ability ability_health = target_.GetAbility(AbilityHealth());

    if (ability_health == Ability())
        std::cout << "can't damage something that doesn't hae health" << std::endl;

}