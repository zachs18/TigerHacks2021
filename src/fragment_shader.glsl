#version 460
layout (location=2) in vec4 vertexColor;
layout (location=1) in vec4 ertexColor;
out vec4 FragColor;
void main() {
    FragColor = vertexColor;
//    FragColor.x /= 2;
//    FragColor.x += 0.5;
//    FragColor.y /= 2;
//    FragColor.y += 0.5;
//    FragColor = mod(vec4(gl_FragCoord.xy, 0, 0) / 40, 1);
//    FragColor.w = 0;
}
