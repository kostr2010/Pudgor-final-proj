#pragma once

//====================
// ENUMS

enum class AbilityKind {
    CanDie,        // is tough. can be damaged or healed
    CanContain,    // may contain something pickable
    CanBePicked,   // may be picked up
    CanMove,       // ability to move to another tile
    CanPick,       // can pick pickable                    | FIXME
    CanLoot,       // can pick subEntities of CanContain   |
    CanKick,       // ability to impact physical damage to another person
    CanHack,       // can unlock CanBeLocked instance
    CanBeLocked,   // can be locked
    IsMap,         // map special ability
    IsTransparent, // tells if it is possible to see through the object if it is subentity of Tile
    IsWalkable,    // tells if it is possible to be on the same tile with this object
    IsPositioned,  // ability to have coordinates. designated only for tiles
    CanConsume,    // ability to consume potions, food, meds, drugs, stuff
    IsConsumable,  // ability to be consumed
};

enum class AbilityState {
    Range,            // [...]           determines proximity of target, needed to successfully apply the ability
    HpMax,            // [CanDie]        max amount of hp entity has
    HpCurrent,        // [CanDie]        current amount of hp entity has
    ContainCapacity,  // [CanContain]    max capacity of the subentities_ vector
                      // [CanBePicked]
    Movability,       // [CanMove]       determines the movement points
                      // [CanPick]
    SpotToLoot,       // [CanLoot]       index of target.subentities_ vector to loot
    DamageAmount,     // [CanKick]       amount of damage dealt per kick | FIXME separate damage types / sources
    HackLevel,        // [CanHack]       warrior uses his weapon to open locked chests at the price of
                      //                 their durability, rogue uses lockpicks, wizzard uses magic at
                      //                 the cost of the chance that some loot might be destroyed
                      //                 if HackLevel >= LockLevel, then can hack
    LockLevel,        // [CanBeLocked]   lock level [0; n]
    MapWidth,         // [IsMap]         width of the map
    MapHeight,        // [IsMap]         height of the map
                      // [IsTransparent]
    WalkabilityLevel, // [IsWalkable]    0 - no effort to move / 1 - with disadvantage
    PositionX,        // [IsPositioned]  position on x axis
    PositionY,        // [IsPositioned]  position on y axis
                      // [CanConsume]
                      // [IsConsumable]  FIXME need enum for consuming effects, f.ex.: poisonous, restores hp, etc...
};

enum class EntityKind {
    // heroes
    Warrior,
    // enemies
    Mimic,
    // containers
    Chest,
    // items
    Food,
    // misc
    Tile,
    Map,
};

enum class AbilityResult {
    Success,
    ConditionsNotMet,
    Undefined,
    NoDispatcher,
    OriginDoesntHaveAbility,
};