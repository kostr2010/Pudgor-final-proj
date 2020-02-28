#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "engine-types.h"

using namespace std;

int main() {
    Entity ch1   = EntityFactory::CreateWarrior();
    Entity ch2   = EntityFactory::CreateWarrior();
    Entity chest = EntityFactory::CreateChest(10, 1);
    Entity food  = EntityFactory::CreateFood();

    chest.InventoryAdd({food, food});
    cout << "chest with food:\n" << chest << endl;
    cout << "chest size:" << chest.InventoryGetSize() << endl;

    ch1.Apply(CanPick, food);
    cout << "inventory size:" << ch1.InventoryGetSize() << endl;

    ch1.Apply(CanLoot, chest);
    cout << "inventory size:" << ch1.InventoryGetSize() << endl;
    cout << "chest size:" << chest.InventoryGetSize() << endl;

    ch1.Apply(CanKick, ch2);

    cout << "ch2: \n" << ch2;
}
