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
    static Ability CreateAbilityDie(size_t hp_max, size_t hp_current);
    static Ability CreateAbilityContain(size_t capacity);
    static Ability CreateAbilityBePicked();
    static Ability CreateAbilityMove(size_t movability);
    static Ability CreateAbilityPick();
    static Ability CreateAbilityLoot(/*default value*/);
    static Ability CreateAbilityKick(size_t damage);
    static Ability CreateAbilityHack(size_t hack_level);
    static Ability CreateAbilityBeLocked(size_t lock_level);
    static Ability CreateAbilityIsMap(size_t width, size_t height);
    static Ability CreateAbilityIsTransparent();
    static Ability CreateAbilityIsWalkable(size_t walkability_level);
    static Ability CreateAbilityIsPositioned(int pos_x, int pos_y);
    static Ability CreateAbilityCanConsume();
    static Ability CreateAbilityIsConsumable();

  private:
  protected:
};