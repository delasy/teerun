#ifndef SRC_GAME_SERVER_H
#define SRC_GAME_SERVER_H

#include <duc/helpers.h>
#include <duc/wss.h>
#include <uv.h>

typedef struct game_server_s game_server_t;
typedef struct game_server_config_s game_server_config_t;

struct game_server_config_s {
  uint8_t fps;
  char host[32];
  uint16_t port;
};

struct game_server_s {
  game_server_config_t config;
  uint64_t previous_tick;
  bool running;
  uv_signal_t signal;
  uv_timer_t timer;
  duc_wss_t *wss;
};

void game_server_free (game_server_t *gs);
void game_server_handle_uv_signal (uv_signal_t *signal, DUC_UNUSED int32_t signum);
void game_server_handle_uv_timer (uv_timer_t *timer);
void game_server_handle_wss_close (duc_wss_session_t *session);
void game_server_handle_wss_message (duc_wss_session_t *session, uint8_t *data, size_t size);
void game_server_handle_wss_open (duc_wss_session_t *session);
game_server_t *game_server_new (void);
int32_t game_server_start (game_server_t *gs);
void game_server_stop (game_server_t *gs);

#endif
