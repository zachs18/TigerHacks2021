#version 460
layout (location=0) in vec3 aPos;
layout (location=1) out vec4 asdf;
layout (location=2) out vec4 vertexColor;
uniform float sin_time;
void main() {
    gl_Position = vec4(aPos, 1.0);
    vertexColor = gl_Position;
    vertexColor.x = 0.5 + 0.5 * sin_time * float(gl_VertexID != 0);
    vertexColor.y *= 0.5;
    vertexColor.y += 0.5;
    asdf = vec4(0,0,0,0);
}
