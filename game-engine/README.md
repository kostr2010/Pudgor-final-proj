### ECS engine C++ realization

## Folder contnents overview

libs - folder for 3rd party libraries (catch2 - unit testing, json - serialization)

log - log files for engine

src-next - engine folder

## Engine overview

Entity-component-system approach is designed for more flexible game structure, allowing for functionality-oriented development instead of data-oriented (structure-oriented) approach when using OOP.

This implementation uses 3 main concepts labled in it's name: entity, component and system. I'll try to explain these concepts on the example below.

## Explanation on gophers

# Component

Let's try to create gopher. First thing we need to do is work out, what this gopher needs to do in our world. In my world, gophers can only move around, grow and ocasionally die. Now let's list all of the data we need for this functionality to work. If we need our gopher to move, he needs to know his position and speed, in order to grow, he needs to know his current height and finally, we need to know gopher's current age and the maximum age average gopher can reach. Such data is called Component. Snippet below demonstrates possible implementation of them.

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

# Entity

# System
