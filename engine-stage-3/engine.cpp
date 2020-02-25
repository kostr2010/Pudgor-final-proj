#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <map>

#include "engine-types.h"

using namespace std;

static Entity createCharacter() {
    Ability ability_health = Ability(Health, {{HpMax, 10}, {HpCurrent, 10}});
    Ability ability_kick = Ability(Kick, {{DamageAmount, 4}});

    return Entity({ {Health, ability_health}, {Kick, ability_kick} });
}

static Entity createChest() {
    // TODO
}

int main() {
    
    Entity ch1 = createCharacter();
    Entity ch2 = createCharacter();
    
    ch1.Apply(Kick, ch2);
    
    cout << ch2;
}
