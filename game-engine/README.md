# ECS engine C++ realization

## Folder contnents overview

libs - folder for 3rd party libraries (catch2 - unit testing, json - serialization)

log - log files for engine

src-next - engine folder

## Engine overview

Entity-component-system approach is designed for more flexible game structure, allowing for functionality-oriented development instead of data-oriented (structure-oriented) approach when using OOP.

This implementation uses 3 main concepts labled in it's name: entity, component and system. I'll try to explain these concepts on the example below.

## Explanation on gophers

### Component

Let's try to create gopher. First thing we need to do is to work out, what this gopher needs to do in our world. In my world, gophers can only move around, grow and ocasionally die. Now let's list all of the data we need for this functionality to work. If we need our gopher to move, he needs to know his position and speed, in order to grow, he needs to know his current height and finally, we need to know gopher's current age and the maximum age average gopher can reach. Such data is called Component. Snippet below demonstrates possible implementation of them.

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

struct ComponentHeight {
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
  std::map<const char*, ComponentType>   component_types_{};
  std::map<const char*, IComponentPack*> component_packs_{};

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

### System
