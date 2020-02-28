#pragma once

#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>

// to print enums
#define EnumToString(var) #var

//===================
// ENUMS

enum AbilityKind {
    CanDie,      // is tough. can be damaged or healed
    CanContain,  // may contain something pickable
    CanBePicked, // may be picked up
    CanMove,     // ability to move to another tile
    CanPick,     // can pick pickable                    | FIXME
    CanLoot,     // can pick subEntities of CanContain   |
    CanKick,     // ability to impact physical damage to another person
    CanHack,     // can unlock CanBeLocked instance
    CanBeLocked, // can be locked
};

enum AbilityState {
    DamageAmount,    // amount of damage dealt per kick | FIXME separate damage types / sources
    HpMax,           // max amount of hp entity has
    HpCurrent,       // current amount of hp entity has
    ContainCapacity, // max capacity of the subentities_ vector
    SpotToLoot,      // index of target.subentities_ vector to loot
    LockLevel,       // lock level [0; n]
                     // warrior uses his weapon to open locked chests at the price of
                     // their durability, rogue uses lockpicks, wizzard uses magic at
                     // the cost of the chance that some loot might be destroyed
    HackLevel,       // if HackLevel >= LockLevel, then can hack
};

//====================
// CLASSES DECLARATION

class Ability {
  public:
    Ability();
    Ability(AbilityKind kind, std::map<AbilityState, int> init_state);

    int  GetStateValue(AbilityState name);
    void SetStateValue(AbilityState name, int new_value);

    friend bool          operator==(const Ability& left, const Ability& right);
    friend std::ostream& operator<<(std::ostream& stream, Ability ability);

    AbilityKind kind_;

  private:
    std::map<AbilityState, int> state_;

  protected:
};

class Entity {
  public:
    Entity();
    Entity(std::vector<Entity> subentities);
    Entity(std::vector<Ability> abilities);
    Entity(std::vector<Entity> subentities, std::vector<Ability> abilities);

    void SetSubentities(std::vector<Entity>);
    void SetAbilities(std::vector<Ability>);

    void   Apply(AbilityKind kind, Entity& target);
    int    InventoryGetSize();
    void   InventoryAdd(std::vector<Entity> items); //
    void   InventoryRemove(size_t index);
    Entity InventoryGetSubentity(size_t index);

    friend std::ostream& operator<<(std::ostream& stream, Entity entity);

    std::map<AbilityKind, Ability> abilities_;

  private:
    std::vector<Entity> subentities_;

  protected:
};

// class WorldMap {
//   public:
//   private:
//   protected:
// };

// class Tile {
//   public:
//   private:
//   protected:
// };

class EntityFactory {
  public:
    // just a map (main parent entity)
    static Entity CreateMap(size_t width, size_t height);

    // tile (only tile may be a subentity of a map)
    static Entity CreateTile();

    // playable characters
    static Entity CreateWarrior();
    static Entity CreateMage();
    static Entity CreateCustom();

    // objects
    static Entity CreateChest(size_t capacity, size_t lock_lvl);

    // enemies
    static Entity CreateMimic();

    // items
    static Entity CreateFood();

  private:
  protected:
};

class AbilityFactory {
  public:
    static Ability CreateAbilityDie(size_t hp_max, size_t hp_current);
    static Ability CreateAbilityKick(size_t damage);
    static Ability CreateAbilityPick();
    static Ability CreateAbilityLoot();
    static Ability CreateAbilityBePicked();
    static Ability CreateAbilityContain(size_t capacity);
    static Ability CreateAbilityMove();
    static Ability CreateAbilityHack(size_t hack_level);
    static Ability CreateAbilityBeLocked(size_t lock_level);

  private:
  protected:
};
