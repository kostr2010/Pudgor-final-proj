#pragma once

#include <iostream>

// ====================
// Vec2
// implementation of the simple 2d vector

struct Vec2 {
  int x, y;
};

bool operator==(Vec2 left, Vec2 right) {
  return (left.x == right.x) && (left.y == right.y);
}

void operator+=(Vec2& left, Vec2& right) {
  left.x += right.x;
  left.y += right.y;
}

Vec2 operator+(Vec2& left, Vec2& right) {
  return {left.x + right.x, left.y + right.y};
}

std::ostream& operator<<(std::ostream& os, Vec2 v) {
  return os << "x: " << v.x << " y: " << v.y << std::endl;
}