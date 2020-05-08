# ECS engine C++ realization

## Folder contnents overview

libs - folder for 3rd party libraries (catch2 - unit testing, json - serialization)

log - log files for engine

src-next - engine folder

## Engine overview

`Entity`-`Component`-`System` approach is designed for more flexible game structure, allowing for functionality-oriented development instead of data-oriented (structure-oriented) approach when using OOP.

This implementation uses 3 main concepts labled in it's name: entity, component and system. I'll try to explain these concepts on the example below.

## Explanation on gophers

### Component

Let's try to create gopher. First thing we need to do is to work out, what this gopher needs to do in our world. In my world, gophers can only move around, grow and ocasionally die. Now let's list all of the data we need for this functionality to work. If we need our gopher to move, he needs to know his position and speed, in order to grow, he needs to know his current height and finally, we need to know gopher's current age and the maximum age average gopher can reach. Such data is called `Component`. Snippet below demonstrates possible implementation of them.

```C
struct Vec2 {
    int x, y;
};

struct ComponentPosition {
    Vec2 pos;
};

struct ComponentSpeed {
    Vec2 speed;
};

struct ComponentAge {
    int age_cur, age_max;
}

struct ComponentSize {
    int height;
};
```

Note: it's vital for the components to be POD, because they will be copied left and right (ideally, they should be in continuous packed array and be able to be copied using memcpy) and need to not lose integrity from this operations. It will become clear later, when other concepts will be introduced.

### Entity

Now we need to somehow keep track of the components someone has. In this implementation, it's achieved like this: when introducing new component, it recieves an id. This approach allows us to represent an entity with just id and signature (bitset) that represents component it has. For example, let ComponentPosition's id be 0, ComponentSpeed's - 1, etc. Thus, each gopher would have sigature <1111> (bits in needed positions are set to 1), and each tree - <1011> (because trees can't move, but sure have position, height and age). In order to keep track of entities and their components, EntityManger and ComponentManager are created. Both of them share same functionality: register new entries, manade id's and instances, remove them. Below are snippets of EntityManager and ComponentManager declaration:

```C
// ====================
// EntityManager
// keeps track of entities and their components(via signatures)

typedef int EntityId;
typedef std::bitset Signature;

class EntityManager {
public:;
  ~EntityManager();

  EntityManager();
  EntityId CreateEntity();

  void RemoveEntity(EntityId entity);

  Signature GetSignature(EntityId entity);

  Signature SetSignature(EntityId entity, Signature signature);

private:
  std::array<Signature, MAX_ENTITIES>
                        abilities_signatures_{}; // shows whenever an component is present for an entity
  std::vector<EntityId> available_ids_{};
  int                   entities_alive_;
};
```

When entity is created, id from `avaliable_ids_` is taken and given to user (when entity is removed, this id is pushed back to the vector). Also, manager stores signatures for every entity in array `abilities_signatures_`, where index in array corresponds to entity's id. Now let's take a look at ComponentManager and how it manages all of the components each entity has.

```C
// ====================
// ComponentManager
// keeps track of components

typedef int ComponentType;

class ComponentManager {
public:
  ComponentManager();

  ~ComponentManager();

  template <typename Component_t>
  ComponentType RegisterComponent();

  template <typename Component_t>
  ComponentType GetComponentType();

  template <typename Component_t>
  void AttachComponent(EntityId entity, Component_t component);

  template <typename Component_t>
  void RemoveComponent(EntityId entity);

  template <typename Component_t>
  Component_t* GetComponent(EntityId entity);

private:
  std::map<std::string, ComponentType>   component_types_{};
  std::map<std::string, IComponentPack*> component_packs_{};

  ComponentType next_{};

  template <typename Component_t>

  template <typename Component_t>
  ComponentPack<Component_t>* GetComponentPack()
};
```

When new component is registered, it's assigned new id (mapping name-id is stored in `component_types_`). Next, new ComponentPack is created for this component. ComponentPack is class derived from IComponentPack that provides functionality every ComponentPack should have:

```C
class IComponentPack {
public:
  virtual void RemoveEntity(EntityId entity) = 0;
  virtual bool Contains(EntityId entity) const = 0;
};
```

But what is ComponentPack itself? Basically, it's storage class for components of same type. Here EntityId is mapped to specific instance of component.

```C
template <typename Component_t>
class ComponentPack : public IComponentPack {
public:
  void AddEntity(EntityId entity, Component_t component)

  void RemoveEntity(EntityId entity) override {...}

  Component_t* GetComponent(EntityId entity);

  bool Contains(EntityId entity) const override {...}


private:
  std::map<EntityId, Component_t> components_; // TODO: replace with Packed Array in order to correspond to the main idea
};
```

Like that, ComponentManager keeps track of every created component and it's affiliation to entity.

Now, let's try to actually make gopher using given tools:

```C
// initializing managers
ComponentManager man_c{};
EntityManager    man_e{};

// declaring components
struct Vec2 {
    int x, y;
};

struct ComponentPosition {
    Vec2 pos;
};

struct ComponentSpeed {
    Vec2 speed;
};

struct ComponentAge {
    int age_cur, age_max;
}

struct ComponentSize {
    int height;
};

// registering components (they are given unique and ComponentPack)
man_c.RegisterComponent<ComponentPosition>();
man_c.RegisterComponent<ComponentSpeed>();
man_c.RegisterComponent<ComponentHealth>();
man_c.RegisterComponent<ComponentSize>();

// creating needed components
ComponentHealth   health    = {.age_max = 10, .age_cur = 1};
ComponentSpeed    speed     = {.speed = {0, 0}};
ComponentPosition pos       = {.pos = {0, 0}};
ComponentSize     size      = {.height = 3};

// recieving id of newly born gopher
EntityId gopher = man_e.AddEntity();

// attaching components to gopher's id (new entries in every ComponentPack<ComponentType>'s `components_`)
man_c.AttachComponent(pos, gopher);
man_c.AttachComponent(health, gopher);
man_c.AttachComponent(speed, gopher);
man_c.AttachComponent(size, gopher);

// done. we have created and registered all components, needed for gopher, created an instance "entity" and gave it all properties of a gopher
```

Good. Now we need to do something we have planned from very beginning. We need to teach our gopher how to move, grow and die.

### System

Systems are designed to do exactly that. By definition, `System` is container for entities posessing some attribute/s that provides functionality to operate upon these entities. In other words, it's any functionality that iterates upon a list of entities with a certain signature of components. In this implementation, every `System` should be deriver from base-class `System` in order to work correctly. This class provides minimal requirements to meet definition of `System` given prior:

```C
class Monitor; // forward declaration of class that I will explain further. all that matters now is that it holds pointers for all managers we have

class System {
public:
  std::set<EntityId> entities_{};

  System(Monitor* monitor) {
    monitor_ = monitor;
  }

  virtual std::vector<ComponentType> GetRequiredComponentTypes() = 0;
  virtual void                       RegisterDependentSystems()  = 0;

protected:
  Monitor* monitor_;
};
```

`entities_` set holds all entities that meet system's requirements. GetRequiredComponentTypes returns id's of components that are needed for entity to have functionality system provides. RegisterDependentSystems registers all systems that are used inside this system (this will be explicitly explained in the example below). Let's not talk about Monitor for now, because it's not that important right now.

Similar to Entities and Components, Systems need their own manager that will keep track of registration and provide neede getters/setters:

```C
// ====================
// SystemManager
// registers and manages new systems

class System; // forward - declaration

class SystemManager {
public:
  SystemManager(Monitor* monitor);

  ~SystemManager();

  template <typename System_t>
  System_t* RegisterSystem();

  template <typename System_t>
  System_t* GetSystem();

  template <typename System_t>
  void SetSignature(Signature signature);

  void RemoveEntity(EntityId entity);

  void UpdateEntitySignature(EntityId entity, Signature entity_signature);

  template <typename System_t>
  bool Contains();

  Monitor*                       monitor_;
  std::map<std::string, System*> systems_{};
  std::map<std::string, Signature> system_signatures_{};
};
```

When new system is registered, it's name is taken (from template via typeid) as id and then this system is stored in `systems_` map. Next, we set system's signature. It works same as with entity and its signature. In order to check if entity meets requirements, we need to do bitewise AND upon both entity's and system's signatures. And if `system_signature_ & entity_signature == system_signature`, then entity meets system's requirements. When system's sugnature is set/changed, SystemManager iterates upon every entity and checks if they meet new System's signature. Same goes when entity's signature changes (f.ex, our gopher becomes immortal, thus losing ComponentHealth): SystemManager goes through all systems and checks if new signature meets any of them (or stopped to meet).

Let's write some examples:

```C
// ResponceCode is just enum-class that contains all possible outcomes, such as success, failure, etc.

class SystemHealth : public System {
public:
  ResponseCode ChangeCurrentAge(EntityId entity, int delta) {
    // we assume that entity indeed has this component
    ComponentHealth* comp_health = monitor_->man_c->GetComponent<ComponentHealth>(entity);

    comp_health->age_cur += delta;

    return ResponseCode::Success;
  }

  ResponseCode ChangeMaximumAge(EntityId entity, int delta) {
    ComponentHealth* comp_health = monitor_->man_c->GetComponent<ComponentHealth>(entity);

    comp_health->hp_max += delta;

    return ResponseCode::Success;
  }

  std::vector<ComponentType> GetRequiredComponentTypes() override {
    return {monitor_->man_c->RegisterComponent<ComponentHealth>()};
  }
};
```

Once again, Monitor is just holder for pointers to every one of the managers. Let's talk about it.

### Monitor (a.k.a GOD-CLASS)

See this part: "monitor\_->man_c->RegisterComponent<ComponentHealth>()". It's unpleasant to white this sequrnce of arrows each time we need to get component (and we need it a lot). So we make Monitor not only holder for pointers to managers, but interface of working with them, duplicating needed functions as Monitor's methods:

```C
// ====================
// Monitor
// global manager for all managers

class Monitor {
public:
  // wrappers for managers, so user has to speak only to monitor instead of directly to each one of
  // the managers

  template <typename Component_t>
  ComponentType RegisterComponent();

  template <typename Component_t>
  void AttachComponent(Component_t& component_new, EntityId entity);

  template <typename Component_t>
  void RemoveComponent(EntityId entity);

  EntityId AddEntity();

  void RemoveEntity(EntityId entity);

  template <typename System_t>
  void SetSystemSignature(Signature& signature);

  template <typename System_t>
  void SetSystemSignature(std::vector<ComponentType> components);

  template <typename System_t>
  System_t* RegisterSystem();

  template <typename System_t>
  System_t* GetSystem();

  template <typename Component_t>
  bool HasComponent(EntityId entity);

  template <typename Component_t>
  bool HasNoComponent(EntityId entity);

  template <typename Component_t>
  Component_t* GetComponent(EntityId entity);

  bool CheckIfEntityExists(EntityId entity);

private:
  void UpdateSignature(EntityId entity, Signature& signature);

  ComponentManager man_c{};
  EntityManager    man_e{};
  SystemManager    man_s{this};
};
```

With structure like this, creation of gopher from the scratch will look like this:

```C
// also initializes all managers it holds inside
Monitor monitor();

// declaring components
struct Vec2 {
    int x, y;
};

struct ComponentPosition {
    Vec2 pos;
};

struct ComponentSpeed {
    Vec2 speed;
};

struct ComponentAge {
    int age_cur, age_max;
}

struct ComponentSize {
    int height;
};

// registering components (they are given unique and ComponentPack)
monitor.RegisterComponent<ComponentPosition>();
monitor.RegisterComponent<ComponentSpeed>();
monitor.RegisterComponent<ComponentHealth>();
monitor.RegisterComponent<ComponentSize>();

// creating needed components
ComponentHealth   health    = {.age_max = 10, .age_cur = 1};
ComponentSpeed    speed     = {.speed = {1, 1}};
ComponentPosition pos       = {.pos = {0, 0}};
ComponentSize     size      = {.height = 3};

// recieving id of newly born gopher
EntityId gopher = monitor.AddEntity();

// attaching components to gopher's id (new entries in every ComponentPack<ComponentType>'s `components_`)
monitor.AttachComponent(pos, gopher);
monitor.AttachComponent(health, gopher);
monitor.AttachComponent(speed, gopher);
monitor.AttachComponent(size, gopher);

// now, let's add functionality to our gopher

class SystemHealth : public System {
public:
  ResponseCode ChangeCurrentAge(EntityId entity, int delta) {
    // we assume that entity indeed has this component
    ComponentHealth* comp_health = monitor_->man_c->GetComponent<ComponentHealth>(entity);

    comp_health->age_cur += delta;

    return ResponseCode::Success;
  }

  ResponseCode ChangeMaximumAge(EntityId entity, int delta) {
    ComponentHealth* comp_health = monitor_->man_c->GetComponent<ComponentHealth>(entity);

    comp_health->hp_max += delta;

    return ResponseCode::Success;
  }

  std::vector<ComponentType> GetRequiredComponentTypes() override {
    return {monitor_->man_c->RegisterComponent<ComponentHealth>()};
  }
};

class SystemMovement : public System {
public:
  SystemMovement(Monitor* monitor) : System(monitor) {
  }

  ResponseCode Move(EntityId entity) {
    ComponentSpeed*    comp_speed = monitor_->GetComponent<ComponentSpeed>(entity);
    ComponentPosition* comp_pos   = monitor_->GetComponent<ComponentPosition>(entity);

    comp_pos->pos += comp_speed->speed;

    return ResponseCode::Success;
  }

  void ChangeSpeed(EntityId entity, Vec2 speed_new) {...}

  std::vector<ComponentType> GetRequiredComponentTypes() override {
    return {monitor_->RegisterComponent<ComponentSpeed>(), monitor_->RegisterComponent<ComponentPosition>()};
  }
};

// registering new systems
SystemHealth*   sys_health = monitor->RegisterSystem<SystemHealth>();
SystemMovement* sys_mov    = monitor->RegisterSystem<SystemMovement>();

// now we can make gopher move. let's make so every time gopher moves, it ages. repeat until gopher dies
ComponentHealth* comp_hp = monitor->GetComponent<ComponentHealth>(gopher);

while (comp_hp->age_cur < comp_hp->age_max) {
  sys_mov->Move(gopher);
  sys_health->ChangeCurrentAge(gopher, 1);
}
monitor->RemoveEntity(gopher);
```

Like this, we can easily create any type of entity, thinking only about it's functionality without any concerns about hierarchy of the classes.
