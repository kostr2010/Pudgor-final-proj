#include <iostream>
#include <map>
#include <vector>
#include <functional>
#include <algorithm>

#include "engine-types.h"
#include "engine-constants.h"

//====================
// METHODS IMPLEMENTATION

//====================
// ability

Ability::Ability() {
    return;
}

Ability::Ability(AbilityKind kind, std::map<AbilityStateKey, double> init_state) {
    state_ = init_state;
    kind_ = kind;

    return;
}

void Ability::Recieve(std::map<AbilityStateKey, double> state_delta) {
    for (const auto& e : state_delta)
        state_[e.first] += e.second;

    return;
}

double Ability::GetStateValue(AbilityStateKey name) {
    std::cout << "get " << name << std::endl;

    for (auto& key : state_)
        std::cout << key.second << std::endl;

    return state_[name];
}

void Ability::SetStateValue(AbilityStateKey name, double value) {
    std::cout << "set " << name << " to " << value << std::endl;
    state_[name] = value;

    return;
}

bool operator==(const Ability& left, const Ability& right) {
    return left.kind_ == right.kind_;
}

std::ostream& operator<< (std::ostream& stream, Ability ability) {
    stream << ability.kind_ << ": ";

    for (const auto& s : ability.state_)
        stream << s.second << " ";
        
    return stream;
}

//====================
// entity

Entity::Entity(std::map<AbilityKind, Ability> abilities) {
    abilities_ = abilities;
}

void Entity::Apply(AbilityKind kind, Entity& target) {
    if (abilities_.count(kind) != 0)
        dict_ability_dispatcher[kind](*this, target);
}
    
std::ostream& operator<< (std::ostream& stream, Entity entity) {
    for (const auto& ability : entity.abilities_) {
        stream << ability.first << ": " << ability.second << std::endl; 
    }

    return stream;
}


