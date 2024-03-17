#ifndef WORLD_H_
#define WORLD_H_

#include "cell.h"

typedef struct {
  size_t width;
  size_t height;
  size_t scale;
  cell_t *cells;
} world_t;

#define GET_INDEX(w, x, y) ((size_t)(x) + (w).width * (size_t)(y))

#define INDEX_GET_CELL(w, i) ((w).cells[(i)])
#define GET_CELL(w, x, y) (INDEX_GET_CELL((w), GET_INDEX((w), (x), (y))))

#define INDEX_SET_CELL(w, i, c)                                                \
  do {                                                                         \
    (w).cells[(i)] = (c);                                                      \
  } while (0)
#define SET_CELL(w, x, y, c)                                                   \
  do {                                                                         \
    INDEX_SET_CELL((w), GET_INDEX((x), (y)), (c));                             \
  } while (0)

#define IS_VALID(w, x, y) ((x) < (w).width && (y) < (w).height)
#define IS_EMPTY(w, x, y)                                                      \
  (IS_VALID((w), (x), (y)) && GET_CELL((w), (x), (y)).type == TYPE_EMPTY)

typedef struct {
  size_t dest;
  size_t source;
} cell_move_t;

void move_cell(world_t w, size_t x1, size_t y1, size_t x2, size_t y2);
void commit_cell_updates(world_t w);

#endif // WORLD_H_
