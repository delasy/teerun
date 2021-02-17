#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

static void error_callback (
  __attribute__((unused)) int error,
  const char *description
) {
  fprintf(stderr, "Error: %s\n", description);
}

static void key_callback (
  GLFWwindow *window,
  int key,
  __attribute__((unused)) int scancode,
  int action,
  __attribute__((unused)) int mods
) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
}

const char *vertex_shader =
  "#version 150\n"
  "in vec3 vp;"
  "void main () {"
  "  gl_Position = vec4(vp, 1.0);"
  "}\n";

const char *fragment_shader =
  "#version 150\n"
  "out vec4 frag_colour;"
  "void main () {"
  "  frag_colour = vec4(0.5, 0.0, 0.5, 1.0);"
  "}\n";

float vertices[] = {
  0.f, 0.5f, 0.f,
  -0.5f, -0.5f, 0.f,
  0.5f, -0.5f, 0.f
};

void update_fps_counter (GLFWwindow *window, double *previous_seconds) {
  static int frame_count;
  double current_seconds = glfwGetTime();
  double elapsed_seconds = current_seconds - *previous_seconds;

  if (elapsed_seconds > 0.25) {
    *previous_seconds = current_seconds;
    double fps = (double) frame_count / elapsed_seconds;
    char tmp[128];
    sprintf(tmp, "opengl @ fps: %.2f", fps);
    glfwSetWindowTitle(window, tmp);
    frame_count = 0;
  }

  frame_count++;
}

int main (void) {
  glfwSetErrorCallback(error_callback);

  if (!glfwInit()) {
    return 1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  glfwWindowHint(GLFW_SAMPLES, 4);

  GLFWwindow *window = glfwCreateWindow(1440, 900, "PigRace", NULL, NULL);

  if (!window) {
    glfwTerminate();
    return 1;
  }

  glfwSetKeyCallback(window, key_callback);
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
    fprintf(stderr, "Failed to initialize OpenGL contexdt\n");
    glfwTerminate();
    return 1;
  }

  glfwSwapInterval(1);

  printf("Renderer: %s\n", glGetString(GL_RENDERER));
  printf("OpenGL version supported %s\n", glGetString(GL_VERSION));

  if (GLVersion.major < 2) {
    fprintf(stderr, "Error: Your system doesn't support OpenGL >= 2\n");
    return 1;
  }

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  GLuint vbo = 0;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), vertices, GL_STATIC_DRAW);

  GLuint vao = 0;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

  GLuint vs = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vs, 1, &vertex_shader, NULL);
  glCompileShader(vs);

  GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fs, 1, &fragment_shader, NULL);
  glCompileShader(fs);

  GLuint program = glCreateProgram();
  glAttachShader(program, fs);
  glAttachShader(program, vs);
  glLinkProgram(program);

  double previous_seconds = glfwGetTime();

  while (!glfwWindowShouldClose(window)) {
    update_fps_counter(window, &previous_seconds);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int width, height;

    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    glUseProgram(program);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
