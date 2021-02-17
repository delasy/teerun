#include <duc/testing.h>
#include <stdlib.h>
#include "../src/game-server.h"

void game_server_test_handle_uv_timer (uv_timer_t *timer) {
  game_server_stop(timer->data);
  uv_timer_stop(timer);
}

DUC_TEST(game_server, handle_uv_signal) {
  game_server_t *gs = game_server_new();
  game_server_handle_uv_signal(&gs->signal, SIGINT);
}

DUC_TEST(game_server, handle_uv_timer) {
  game_server_t *gs = game_server_new();
  game_server_handle_uv_timer(&gs->timer);
  game_server_free(gs);
}

DUC_TEST(game_server, handle_wss_close) {
  duc_wss_session_t *session = duc_fake_wss_session_new();

  game_server_handle_wss_close(session);
  duc_fake_wss_session_free(session);
}

DUC_TEST(game_server, handle_wss_message) {
  duc_wss_session_t *session = duc_fake_wss_session_new();
  size_t data_size = 4;
  uint8_t *data = malloc(data_size);

  data[0] = 1;
  data[1] = 2;
  data[2] = 3;
  data[3] = 0;

  game_server_handle_wss_message(session, data, data_size);
  duc_fake_wss_session_free(session);
}

DUC_TEST(game_server, handle_wss_open) {
  duc_wss_session_t *session = duc_fake_wss_session_new();

  game_server_handle_wss_close(session);
  duc_fake_wss_session_free(session);
}

DUC_TEST(game_server, new_and_free) {
  game_server_t *gs = game_server_new();

  DUC_ASSERT_NE(gs, NULL);
  game_server_free(gs);
}

DUC_TEST(game_server, start_restart_stop) {
  game_server_t *gs = game_server_new();
  uv_timer_t timer;

  timer.data = gs;
  uv_timer_init(uv_default_loop(), &timer);

  uv_timer_start(&timer, game_server_test_handle_uv_timer, 0, 500);
  DUC_ASSERT_EQ(game_server_start(gs), 0);

  uv_timer_start(&timer, game_server_test_handle_uv_timer, 0, 500);
  DUC_ASSERT_EQ(game_server_start(gs), 0);

  uv_close((uv_handle_t *) &timer, NULL);
  game_server_free(gs);
}

int32_t main (void) {
  DUC_TEST_RUN(game_server, handle_uv_signal);
  DUC_TEST_RUN(game_server, handle_uv_timer);
  DUC_TEST_RUN(game_server, handle_wss_close);
  DUC_TEST_RUN(game_server, handle_wss_message);
  DUC_TEST_RUN(game_server, handle_wss_open);
  DUC_TEST_RUN(game_server, new_and_free);
  DUC_TEST_RUN(game_server, start_restart_stop);
}
