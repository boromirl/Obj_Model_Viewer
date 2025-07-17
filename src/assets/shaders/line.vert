#version 330 core
layout(location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// flat нужен для передачи переменной 'без интерполяции'
// flat передаёт значение от одной вершины всем фрагментам линии
// без изменения (интерполяции)
flat out vec3 startPos;
out vec3 vertPos;

void main() {
  vec4 pos = projection * view * model * vec4(aPos, 1.0);
  gl_Position = pos;
  vertPos = pos.xyz / pos.w;
  startPos = vertPos;
}