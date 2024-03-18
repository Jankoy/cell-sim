#include "world.h"

#include "nob.h"

static struct {
  cell_move_t *items;
  size_t count;
  size_t capacity;
} cell_moves = {0};

void move_cell(world_t w, size_t x1, size_t y1, size_t x2, size_t y2) {
  nob_da_append(&cell_moves, ((cell_move_t){.dest = GET_INDEX(w, x2, y2),
                                            .source = GET_INDEX(w, x1, y1)}));
}

static int sort_by_dest(const void *_x, const void *_y) {
  cell_move_t x = *((cell_move_t *)_x);
  cell_move_t y = *((cell_move_t *)_y);
  if (x.dest > y.dest)
    return 1;
  else if (x.dest < y.dest)
    return -1;
  else
    return 0;
}

void commit_cell_updates(world_t w) {
  for (size_t i = 0; i < cell_moves.count; ++i) {
    if (w.cells[cell_moves.items[i].dest].type != TYPE_EMPTY) {
      cell_moves.items[i] = cell_moves.items[cell_moves.count - 1];
      --cell_moves.count;
      --i;
    }
  }

  qsort(cell_moves.items, cell_moves.count, sizeof(cell_move_t), sort_by_dest);

  size_t old_i = 0;

  nob_da_append(&cell_moves, ((cell_move_t){.dest = -1, .source = -1}));

  for (size_t i = 0; i < cell_moves.count; ++i) {
    if (cell_moves.items[i + 1].dest != cell_moves.items[i].dest) {
      size_t rand = old_i + GetRandomValue(0, i - old_i);

      size_t dst = cell_moves.items[rand].dest;
      size_t src = cell_moves.items[rand].source;

      w.cells[dst] = w.cells[src];
      w.cells[src] = cell_empty();

      old_i = i + 1;
    }
  }

  cell_moves.count = 0;
}
