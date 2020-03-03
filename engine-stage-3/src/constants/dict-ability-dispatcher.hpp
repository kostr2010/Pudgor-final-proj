#pragma once

#include "../main-classes/main-classes.hpp"
#include "./enums.hpp"
#include <functional>
#include <map>

std::map<AbilityKind, std::function<AbilityResult(Entity& origin, Entity& target)>> dict_ability_dispatcher = {
    {AbilityKind::CanDie,
     [](Entity& origin, Entity& target) -> AbilityResult {
         // EMPTY BY DESIGN
         return AbilityResult::NoDispatcher;
     }},

    {AbilityKind::CanContain,
     [](Entity& origin, Entity& target) -> AbilityResult {
         // EMPTY BY DESIGN
         return AbilityResult::NoDispatcher;
     }},

    {AbilityKind::CanBePicked,
     [](Entity& origin, Entity& target) -> AbilityResult {
         // EMPTY BY DESIGN
         return AbilityResult::NoDispatcher;
     }},

    {AbilityKind::CanMove,
     [](Entity& origin, Entity& target) -> AbilityResult {
         // FIXME
         return AbilityResult::Success;
     }},

    {AbilityKind::CanPick,
     [](Entity& origin, Entity& target) -> AbilityResult {
         bool can_reach            = origin.CheckIfInRange(target, AbilityKind::CanPick) != -1;
         bool origin_can_contain   = origin.abilities_.count(AbilityKind::CanContain) != 0;
         bool target_can_be_picked = target.abilities_.count(AbilityKind::CanBePicked) != 0;
         bool origin_can_fit       = origin_can_contain &&
                               origin.GetSubEntitiesCount() < origin.abilities_[AbilityKind::CanContain].GetStateValue(AbilityState::ContainCapacity);
         // FIXME visibility check

         if (origin_can_contain && target_can_be_picked && origin_can_fit && can_reach) {
             origin.AddSubentity({target});

             std::vector<Entity> parent_subentites = target.parent_->subentities_;

             int target_index_as_subentity = -1;
             for (int i = 0; i < parent_subentites.size(); i++)
                 if (parent_subentites[i] == target) {
                     target_index_as_subentity = i;
                     break;
                 }

             parent_subentites.erase(parent_subentites.begin() + target_index_as_subentity);

             return AbilityResult::Success;
         } else {
             return AbilityResult::ConditionsNotMet;
         }
     }},

    {AbilityKind::CanLoot,
     [](Entity& origin, Entity& target) -> AbilityResult {
         bool target_can_be_looted = target.abilities_.count(AbilityKind::CanContain) != 0;
         bool target_can_be_locked = target.abilities_.count(AbilityKind::CanBeLocked) != 0;
         bool is_unlocked          = target_can_be_locked && origin.abilities_[AbilityKind::CanBeLocked].GetStateValue(AbilityState::LockLevel) == 0;
         int  spot_to_loot         = origin.abilities_[AbilityKind::CanLoot].GetStateValue(AbilityState::SpotToLoot);
         bool valid_index          = spot_to_loot < target.GetSubEntitiesCount() && spot_to_loot >= 0;
         bool can_reach            = origin.CheckIfInRange(target, AbilityKind::CanLoot) != -1;
         // FIXME visibility check

         if (target_can_be_looted && is_unlocked && valid_index && can_reach) {
             Entity loot = *(target.GetSubentity(spot_to_loot));

             origin.AddSubentity({loot});
             target.RemoveSubentity(spot_to_loot);

             return AbilityResult::Success;
         } else {
             return AbilityResult::ConditionsNotMet;
         }
     }},

    {AbilityKind::CanKick,
     [](Entity& origin, Entity& target) -> AbilityResult {
         bool target_can_die = target.abilities_.count(AbilityKind::CanDie) != 0;
         bool can_reach      = origin.CheckIfInRange(target, AbilityKind::CanKick) != -1;
         // FIXME visibility check

         if (target_can_die && can_reach) {
             Ability target_ability = target.abilities_[AbilityKind::CanDie];

             int damage_amount = origin.abilities_[AbilityKind::CanKick].GetStateValue(AbilityState::DamageAmount);
             int hp_was        = target_ability.GetStateValue(AbilityState::HpCurrent);
             int hp_now        = hp_was - damage_amount;

             target_ability.SetStateValue(AbilityState::HpCurrent, hp_now);
             target.abilities_[AbilityKind::CanDie] = target_ability;

             return AbilityResult::Success;
         } else {
             return AbilityResult::ConditionsNotMet;
         }
     }},

    {AbilityKind::CanHack,
     [](Entity& origin, Entity& target) -> AbilityResult {
         bool target_can_be_hacked = target.abilities_.count(AbilityKind::CanBeLocked) != 0;
         int  hack_level           = origin.abilities_[AbilityKind::CanHack].GetStateValue(AbilityState::HackLevel);
         bool can_hack  = target_can_be_hacked && target.abilities_[AbilityKind::CanBeLocked].GetStateValue(AbilityState::LockLevel) <= hack_level;
         bool can_reach = origin.CheckIfInRange(target, AbilityKind::CanHack) != -1;
         // FIXME visibility check

         if (can_hack & can_reach) {
             target.abilities_[AbilityKind::CanBeLocked].SetStateValue(AbilityState::LockLevel, 0);

             return AbilityResult::Success;
         } else {
             return AbilityResult::ConditionsNotMet;
         }
     }},

    {AbilityKind::CanBeLocked,
     [](Entity& origin, Entity& target) -> AbilityResult {
         // EMPTY BY DESIGN
         return AbilityResult::NoDispatcher;
     }},

    {AbilityKind::IsMap,
     [](Entity& origin, Entity& target) -> AbilityResult {
         // EMPTY BY DESIGN
         return AbilityResult::NoDispatcher;
     }},

    {AbilityKind::IsTransparent,
     [](Entity& origin, Entity& target) -> AbilityResult {
         // EMPTY BY DESIGN
         return AbilityResult::NoDispatcher;
     }},

    {AbilityKind::IsWalkable,
     [](Entity& origin, Entity& target) -> AbilityResult {
         // EMPTY BY DESIGN
         return AbilityResult::NoDispatcher;
     }},

    {AbilityKind::IsPositioned,
     [](Entity& origin, Entity& target) -> AbilityResult {
         // EMPTY BY DESIGN
         return AbilityResult::NoDispatcher;
     }},

    {AbilityKind::CanConsume,
     [](Entity& origin, Entity& target) -> AbilityResult {
         // FIXME
         return AbilityResult::NoDispatcher;
     }},

    {AbilityKind::IsConsumable,
     [](Entity& origin, Entity& target) -> AbilityResult {
         // EMPTY BY DESIGN
         return AbilityResult::NoDispatcher;
     }},
};
