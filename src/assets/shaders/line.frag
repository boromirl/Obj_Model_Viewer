#version 330 core

out vec4 FragColor;
flat in vec3 startPos;  // одинаковая для всех фрагментов линии
in vec3 vertPos;  // уникальная для каждого фрагмента линии

uniform vec2 resolution;
uniform float dashLength;
uniform float gapLength;
uniform bool isDashed;
uniform vec4 lineColor = vec4(1.0, 1.0, 1.0, 1.0);

void main() {
  if (isDashed) {
    // вычисление направления. dir - вектор в экранных координатах от startPos
    // до
    // текущего фрагмента vertPos. Умножение на resolution / 2.0 преобразует
    // координаты из NDC(-1;1) в пиксели
    vec2 dir = (vertPos.xy - startPos.xy) * resolution / 2.0;
    // dist - пройденное расстояние вдоль линии в пикселях
    float dist = length(dir);

    // fract(dist / (dashLength + gapLength)) - делит расстояние на длину
    // цикла (штрих + промежуток) и берет дробную часть. Создаёт повторяющийся
    // паттерн в диапазоне [0,1) Если дробная часть больше отношения штриха к
    // циклу, фрагмент отбрасывается, создавая промежуток
    if (fract(dist / (dashLength + gapLength)) >
        dashLength / (dashLength + gapLength))
      discard;
  }

  FragColor = lineColor;
}