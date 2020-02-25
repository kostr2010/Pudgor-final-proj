enum Surface {
  Wall,
  FloorStone,
  FloorMetal
}

interface Active<S> {
  prevState: S;
  processor: (worldMap: Map) => void;
}

// contains things
interface Container {
  things: Material[];
}

enum Ability {
  Open,
  Close,
  Hack,
  Kick,
  Shoot
}

enum State {
  LockedEasy,
  LockedMedium,
  LockedHard,
  Broken,
  Burning
}

// material shit
interface Material {
  hp: number;
  walkable: boolean;
  equipable: boolean;
  pickable: boolean;

  interactivities: Ability[];
  states: State[];
}

// active entity
interface Creature {
  str: number;
  dex: number;
  int: number;

  equiped: Material[];
  abilities: Ability[];
}

// deals damage at range
interface Weapon {
  range: number;
  damage: number;
}

// ammo type
enum Ammo {
  Bullet,
  Arrow
}

// consumes ammo to shoot
interface WeaponRanged extends Weapon {
  ammoType: Ammo;
}

type Character = Creature & Container & Material;

type Book = Material;

type Chest = Material & Container;

type Gun = Material & WeaponRanged;

type Door = Material & Active<{}>;

interface Tile {
  surface: Surface;
  entity: any;
}

interface Map {
  tiles: Tile[];
  height: number;
  width: number;
}
