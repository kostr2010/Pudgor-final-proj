#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "factories/entity-factory.hpp"

// [+] coordinates -> ability "IsPositioned", only for tiles
// [-] bool AbleToReach(source, origin, range)
// [+] enum AbiltyResult as the dispatcher's & Apply's return value
// [+] revise structure
// [+] enum -> class enum
// [-] test for each ability
// [-] coverage setup
// [-] MAX: add equip (subentities -> map<int, Entity>) (new id === last id + 1)
// [-] MAX: construct a battlefield^c simulation

using namespace std;

void PrintRes(const string& ability, AbilityResult res) {
    switch (res) {
    case AbilityResult::Success:
        cout << ability << " successfully applied" << endl;
        break;
    case AbilityResult::Undefined:
        cout << ability << "'s result is undefined" << endl;
        break;
    case AbilityResult::NoDispatcher:
        cout << ability << " has no dispatcher, not applied" << endl;
        break;
    case AbilityResult::ConditionsNotMet:
        cout << ability << " wasn't applied. conditions weren't met" << endl;
        break;
    case AbilityResult::OriginDoesntHaveAbility:
        cout << ability << " isn't avaliable for this entity" << endl;
        break;
    }

    return;
}

int main() {
    /*
    Entity world_map = EntityFactory::CreateMapShell();
    EntityFactory::InitMap(&world_map, 10, 10);
    */
    Entity world_map = EntityFactory::CreateMap(10, 10);

    AbilityResult res = AbilityResult::Success;

    Entity ch1   = EntityFactory::CreateWarrior(world_map.GetSubentity(1));
    Entity ch2   = EntityFactory::CreateWarrior(world_map.GetSubentity(2));
    Entity chest = EntityFactory::CreateChest(world_map.GetSubentity(3), 1);
    Entity food  = EntityFactory::CreateFood(world_map.GetSubentity(4));

    chest.AddSubentity({food, food});

    res = ch1.Apply(AbilityKind::CanPick, food);

    PrintRes("CanPick", res);

    ch1.abilities_[AbilityKind::CanLoot].SetStateValue(AbilityState::SpotToLoot, 0);
    res = ch1.Apply(AbilityKind::CanLoot, chest);

    PrintRes("CanLoot", res);

    res = ch1.Apply(AbilityKind::CanKick, ch2);

    PrintRes("CanKick", res);

    cout << ch1 << endl;
    cout << ch2 << endl;
    cout << chest << endl;
    cout << food << endl;

    return 0;
}