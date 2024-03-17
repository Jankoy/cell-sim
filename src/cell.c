#include "cell.h"

cell_t cell_empty() {
  cell_t c = {
      .type = TYPE_EMPTY,
      .properties = PROPERTY_NONE,
      .density = 0,
      .color = BLANK,
  };
  return c;
}

cell_t cell_stone() {
  cell_t c = {
      .type = TYPE_STONE,
      .properties = PROPERTY_NONE,
      .density = -1,
      .color = GRAY,
  };
  return c;
}

cell_t cell_sand() {
  cell_t c = {
      .type = TYPE_SAND,
      .properties = PROPERTY_MOVE_DOWN | PROPERTY_MOVE_DOWN_SIDE,
      .density = 2,
      .color = YELLOW,
  };
  return c;
}

cell_t cell_water() {
  cell_t c = {
      .type = TYPE_WATER,
      .properties = PROPERTY_MOVE_DOWN | PROPERTY_MOVE_SIDE,
      .density = 1,
      .color = BLUE,
  };
  return c;
}
