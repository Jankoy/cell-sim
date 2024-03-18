#include "nob.h"
#include <raylib.h>
#include <raymath.h>

#include "world.h"

static cell_t cells[400 * 300] = {0};

static world_t world = {
    .width = 400,
    .height = 300,
    .scale = 2,
    .cells = cells,
};

bool move_down(size_t x, size_t y) {
  bool down = IS_EMPTY(world, x, y + 1);
  if (down)
    move_cell(world, x, y, x, y + 1);
  return down;
}

bool move_down_side(size_t x, size_t y) {
  bool downLeft = IS_EMPTY(world, x - 1, y + 1);
  bool downRight = IS_EMPTY(world, x + 1, y + 1);

  if (downLeft && downRight) {
    downLeft = GetRandomValue(0, 1) > 0;
    downRight = !downLeft;
  }

  if (downLeft)
    move_cell(world, x, y, x - 1, y + 1);
  else if (downRight)
    move_cell(world, x, y, x + 1, y + 1);

  return downLeft || downRight;
}

bool move_side(size_t x, size_t y) {
  bool left = IS_EMPTY(world, x - 1, y);
  bool right = IS_EMPTY(world, x + 1, y);

  if (left && right) {
    left = GetRandomValue(0, 1) > 0;
    right = !left;
  }

  if (left)
    move_cell(world, x, y, x - 1, y);
  else if (right)
    move_cell(world, x, y, x + 1, y);

  return left || right;
}

void update_cells() {
  for (size_t y = 0; y < world.height; ++y) {
    for (size_t x = 0; x < world.width; ++x) {
      const cell_properties_t properties = GET_CELL(world, x, y).properties;
      if (properties & PROPERTY_MOVE_DOWN)
        if (move_down(x, y))
          continue;
      if (properties & PROPERTY_MOVE_DOWN_SIDE)
        if (move_down_side(x, y))
          continue;
      if (properties & PROPERTY_MOVE_SIDE)
        if (move_side(x, y))
          continue;
    }
  }
}

void update_cell_texture(RenderTexture cell_texture) {
  BeginTextureMode(cell_texture);
  ClearBackground(BLANK);
  for (size_t y = 0; y < world.height; ++y)
    for (size_t x = 0; x < world.width; ++x)
      DrawPixel(x, world.height - y, GET_CELL(world, x, y).color);
  EndTextureMode();
}

static const char *type_name_lookup[] = {
    [TYPE_EMPTY] = "EMPTY",
    [TYPE_STONE] = "STONE",
    [TYPE_SAND] = "SAND",
    [TYPE_WATER] = "WATER",
};

int main(void) {
  SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
  InitWindow(world.width * world.scale, world.height * world.scale, "Cell Sim");
  SetTargetFPS(240);

  RenderTexture cell_texture = LoadRenderTexture(world.width, world.height);

  cell_type_t type = TYPE_SAND;
  cell_t cell = cell_sand();

  while (!WindowShouldClose()) {
    int mouse_wheel = roundf(GetMouseWheelMove());
    if (mouse_wheel != 0) {
      if (type == 0 && mouse_wheel < 0) {
        type = 3;
        ++mouse_wheel;
      }
      type = (type + mouse_wheel) % 4;

      switch (type) {
      case TYPE_EMPTY:
        cell = cell_empty();
        break;
      case TYPE_STONE:
        cell = cell_stone();
        break;
      case TYPE_SAND:
        cell = cell_sand();
        break;
      case TYPE_WATER:
        cell = cell_water();
        break;
      }
    }

    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
      const Vector2 mouse_position = GetMousePosition();
      const size_t grid_x = roundf(mouse_position.x) / world.scale;
      const size_t grid_y = roundf(mouse_position.y) / world.scale;
      for (size_t y = grid_y - 8; y < grid_y + 8; ++y) {
        for (size_t x = grid_x - 8; x < grid_x + 8; ++x) {
          if (!IS_VALID(world, x, y))
            continue;
          if (cell.type == TYPE_EMPTY)
            GET_CELL(world, x, y) = cell;
          else if (IS_EMPTY(world, x, y))
            GET_CELL(world, x, y) = cell;
        }
      }
    }

    update_cells();
    commit_cell_updates(world);
    update_cell_texture(cell_texture);
    BeginDrawing();
    ClearBackground(BLACK);
    DrawTextureEx(cell_texture.texture, Vector2Zero(), 0.0f, world.scale,
                  WHITE);
    DrawText(
        TextFormat("Selected: %s (Scroll to change)", type_name_lookup[type]),
        8 * world.scale, 8 * world.scale, 8 * world.scale, WHITE);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
