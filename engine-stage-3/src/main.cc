#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "factories/entity-factory.hpp"
#include "main-classes/main-classes.hpp"

// [+] coordinates -> ability "IsPositioned", only for tiles
// [-] bool AbleToReach(source, origin, range)
// [+] enum AbiltyResult as the dispatcher's & Apply's return value
// [-] revise structure
// [+] enum -> class enum
// [-] test for each ability
// [-] coverage setup
// [-] MAX: add equip (subentities -> map<int, Entity>) (new id === last id + 1)
// [-] MAX: construct a battlefield^c simulation

using namespace std;

int main() {
    Entity world_map = EntityFactory::CreateMapShell();
    EntityFactory::InitMap(&world_map, 10, 10);

    Entity ch1   = EntityFactory::CreateWarrior(world_map.GetSubentity(1));
    Entity ch2   = EntityFactory::CreateWarrior(world_map.GetSubentity(2));
    Entity chest = EntityFactory::CreateChest(world_map.GetSubentity(3), 1);
    Entity food  = EntityFactory::CreateFood(world_map.GetSubentity(4));

    chest.AddSubentity({food, food});
    cout << "chest with food:\n" << chest << endl;
    cout << "chest size:" << chest.GetSubEntitiesCount() << endl;

    ch1.Apply(AbilityKind::CanPick, food);
    cout << "inventory size:" << ch1.GetSubEntitiesCount() << endl;

    ch1.Apply(AbilityKind::CanLoot, chest);
    printf("%s: %d\n", "inventory size", ch1.GetSubEntitiesCount());
    printf("%s: %d\n", "chest size", chest.GetSubEntitiesCount());

    ch1.Apply(AbilityKind::CanKick, ch2);

    cout << "ch2: \n" << ch2;
}