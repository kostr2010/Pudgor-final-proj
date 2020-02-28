#pragma once

#include "engine-types.h"

//====================
// CONSTANTS

std::map<AbilityKind, std::function<void(Entity& origin, Entity& target)>> dict_ability_dispatcher = {
    {CanDie,
     [](Entity& origin, Entity& target) -> void {
         // EMPTY BY DESIGN
         return;
     }},

    {CanContain,
     [](Entity& origin, Entity& target) -> void {
         // EMPTY BY DESIGN
         return;
     }},

    {CanBePicked,
     [](Entity& origin, Entity& target) -> void {
         // EMPTY BY DESIGN
         return;
     }},

    {CanMove, [](Entity& origin, Entity& target) -> void { return; }},

    {CanPick,
     [](Entity& origin, Entity& target) -> void {
         bool origin_can_contain   = origin.abilities_.count(CanContain) != 0;
         bool target_can_be_picked = origin_can_contain && target.abilities_.count(CanBePicked) != 0;
         bool origin_can_fit       = target_can_be_picked && origin.InventoryGetSize() < origin.abilities_[CanContain].GetStateValue(ContainCapacity);

         if (origin_can_contain && target_can_be_picked && origin_can_fit) {
             // #TODO delete picked item from the map
             origin.InventoryAdd({target});
         }

         return;
     }},

    {CanLoot,
     [](Entity& origin, Entity& target) -> void {
         bool target_can_be_looted = target.abilities_.count(CanContain) != 0;
         int  state_locked         = origin.abilities_[CanBeLocked].GetStateValue(LockLevel);
         int  spot_to_loot         = origin.abilities_[CanLoot].GetStateValue(SpotToLoot);

         if (target_can_be_looted && state_locked == 0 && spot_to_loot < target.InventoryGetSize()) {
             // origin.Apply(CanPick, target.subentities_[spot_to_loot]);
             // won't do because Pick deletes object from map, not from
             // conatiner
             Entity loot = target.InventoryGetSubentity(spot_to_loot);

             origin.InventoryAdd({loot});
             target.InventoryRemove(spot_to_loot);
         }

         return;
     }},

    {CanKick,
     [](Entity& origin, Entity& target) -> void {
         if (target.abilities_.count(CanDie) != 0) {
             Ability target_ability = target.abilities_[CanDie];

             int damage_amount = origin.abilities_[CanKick].GetStateValue(DamageAmount);
             int hp_was        = target_ability.GetStateValue(HpCurrent);
             int hp_now        = hp_was - damage_amount;

             target_ability.SetStateValue(HpCurrent, hp_now);
             target.abilities_[CanDie] = target_ability;
         } else {
             std::cout << "it's invincible!" << std::endl;
         }

         return;
     }},

    {CanHack,
     [](Entity& origin, Entity& target) -> void {
         if (target.abilities_.count(CanBeLocked) != 0) {
             int lock_level = target.abilities_[CanBeLocked].GetStateValue(LockLevel);
             int hack_level = origin.abilities_[CanHack].GetStateValue(HackLevel);

             if (hack_level >= lock_level)
                 target.abilities_[CanBeLocked].SetStateValue(LockLevel, 0);
         }

         return;
     }},

    {CanBeLocked,
     [](Entity& origin, Entity& target) -> void {
         // EMPTY BY DESIGN
         return;
     }},
};
