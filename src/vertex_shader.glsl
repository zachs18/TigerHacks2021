#version 140
in vec3 aPos;
out vec4 asdf;
out vec4 vertexColor;
void main() {
    gl_Position = vec4(aPos, 1.0);
    vertexColor = gl_Position;
    asdf = vec4(0,0,0,0);
    vertexColor.x = 0.2;
}
