#ifndef CELL_H_
#define CELL_H_

#include <raylib.h>
#include <stddef.h>

typedef enum {
  PROPERTY_NONE = 0b00000000,
  PROPERTY_MOVE_DOWN = 0b00000001,
  PROPERTY_MOVE_DOWN_SIDE = 0b00000010,
  PROPERTY_MOVE_SIDE = 0b00000100,
} cell_properties_t;

typedef enum {
  TYPE_EMPTY = 0,
  TYPE_STONE,
  TYPE_SAND,
  TYPE_WATER,
} cell_type_t;

typedef struct {
  cell_type_t type;
  cell_properties_t properties;
  size_t density;
  Color color;
} cell_t;

cell_t cell_empty();
cell_t cell_stone();
cell_t cell_sand();
cell_t cell_water();

#endif // CELL_H_
