#include "game-server.h"

int32_t main () {
  game_server_t *gs = game_server_new();
  return game_server_start(gs);
}
