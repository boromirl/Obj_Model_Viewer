#version 330 core
out vec4 FragColor;

uniform vec4 pointColor = vec4(0.9, 0.9, 0.9, 1.0);
uniform bool isCircle = false;

void main() {
  if (isCircle) {
    vec2 coord = gl_PointCoord - vec2(0.5);
    float dist = length(coord);

    if (dist > 0.5) {
      discard;
    }
  }

  FragColor = pointColor;
}