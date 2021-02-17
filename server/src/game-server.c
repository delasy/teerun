#include <duc/time.h>
#include <stdlib.h>
#include <string.h>
#include "game-server.h"

void game_server_free (game_server_t *gs) {
  game_server_stop(gs);

  duc_wss_free(gs->wss);
  uv_close((uv_handle_t *) &gs->signal, NULL);
  uv_close((uv_handle_t *) &gs->timer, NULL);
  uv_run(uv_default_loop(), UV_RUN_DEFAULT);
  uv_loop_close(uv_default_loop());

  free(gs);
}

void game_server_handle_uv_signal (uv_signal_t *signal, DUC_UNUSED int32_t signum) {
  game_server_free(signal->data);
}

void game_server_handle_uv_timer (uv_timer_t *timer) {
  game_server_t *gs = timer->data;
  uint64_t now = duc_time_get();
  uint64_t delta = now - gs->previous_tick;
  gs->previous_tick = now;

  printf("RUNNING: %" PRIu64 "\n", delta);
}

void game_server_handle_wss_close (duc_wss_session_t *session) {
  printf("DISCONNECT: %s\n", duc_wss_session_get_host(session));
}

void game_server_handle_wss_message (duc_wss_session_t *session, uint8_t *data, size_t size) {
  printf("MESSAGE(%s): %s\n", duc_wss_session_get_host(session), data);
  duc_wss_send(session, data, size);
  free(data);
}

void game_server_handle_wss_open (duc_wss_session_t *session) {
  printf("CONNECT: %s\n", duc_wss_session_get_host(session));
}

game_server_t *game_server_new (void) {
  game_server_t *gs = malloc(sizeof(game_server_t));

  gs->config.fps = 25;
  strcpy(gs->config.host, "127.0.0.1");
  gs->config.port = 8081;
  gs->previous_tick = 0;
  gs->running = false;
  uv_signal_init(uv_default_loop(), &gs->signal);
  gs->signal.data = gs;
  uv_timer_init(uv_default_loop(), &gs->timer);
  gs->timer.data = gs;

  duc_wss_config_t wss_config = {
    .bound = NULL,
    .host = gs->config.host,
    .loop = uv_default_loop(),
    .onclose = game_server_handle_wss_close,
    .onmessage = game_server_handle_wss_message,
    .onopen = game_server_handle_wss_open,
    .port = gs->config.port
  };

  gs->wss = duc_wss_new(&wss_config);
  return gs;
}

int32_t game_server_start (game_server_t *gs) {
  uint64_t refresh_rate = (uint64_t) (1000) / gs->config.fps;

  gs->previous_tick = duc_time_get();
  gs->running = true;
  uv_signal_start(&gs->signal, game_server_handle_uv_signal, SIGINT);
  uv_timer_start(&gs->timer, game_server_handle_uv_timer, 0, refresh_rate);
  duc_wss_start(gs->wss);

  return uv_run(uv_default_loop(), UV_RUN_DEFAULT);
}

void game_server_stop (game_server_t *gs) {
  if (gs->running) {
    uv_signal_stop(&gs->signal);
    uv_timer_stop(&gs->timer);
    duc_wss_stop(gs->wss);
    uv_stop(uv_default_loop());
    gs->running = false;
  }
}
