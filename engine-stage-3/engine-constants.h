#pragma once

#include "engine-types.h"

//====================
// CONSTANTS

std::map<AbilityKind, std::function<void(Entity& origin, Entity& target)>> dict_ability_dispatcher = {
    {Health, [](Entity& origin, Entity& target) -> void {return;}},
    {Kick, [](Entity& origin, Entity& target) -> void {
            if (origin.abilities_.count(Kick)) {                                                                                                           
                double damageAmount = origin.abilities_[Kick].GetStateValue(DamageAmount); 
                Ability targetAbility = target.abilities_[Health]; 

                double hpWas = targetAbility.GetStateValue(HpCurrent);
                double hpNow = hpWas - damageAmount;

                targetAbility.SetStateValue(HpCurrent, hpNow);

                target.abilities_[Health] = targetAbility;
            }                      
        }
    }
};
