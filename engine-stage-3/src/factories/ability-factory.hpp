#pragma once

#include <cstdio>

#include "../main-classes/main-classes.hpp"

class Ability;

//====================
// CLASSES DECLARATION

//====================
// ability factory

class AbilityFactory {
  public:
    static Ability CreateAbilityDie(const size_t hp_max, const size_t hp_current);
    static Ability CreateAbilityContain(const size_t capacity);
    static Ability CreateAbilityBePicked();
    static Ability CreateAbilityMove(const size_t movability);
    static Ability CreateAbilityPick();
    static Ability CreateAbilityLoot(/*default value*/);
    static Ability CreateAbilityKick(const size_t damage);
    static Ability CreateAbilityHack(const size_t hack_level);
    static Ability CreateAbilityBeLocked(const size_t lock_level);
    static Ability CreateAbilityIsMap(const size_t width, const size_t height);
    static Ability CreateAbilityIsTransparent();
    static Ability CreateAbilityIsWalkable(const size_t walkability_level);
    static Ability CreateAbilityIsPositioned(const int pos_x, const int pos_y);
    static Ability CreateAbilityCanConsume();
    static Ability CreateAbilityIsConsumable();

  private:
  protected:
};