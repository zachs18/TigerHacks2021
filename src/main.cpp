#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <math.h>

extern const char vertex_shader_source_start[];
extern const char vertex_shader_source_end[];
//const char vertex_shader_source[] = "#version 140\n"
//"in vec3 aPos; // the position variable has attribute position 0\n"
//"out vec4 vertexColor; // specify a color output to the fragment shader\n"
//"void main(){\n"
//"    gl_Position = vec4(aPos, 1.0); // see how we directly give a vec3 to vec4's constructor\n"
//"    vertexColor = vec4(0.5, 0.0, 0.0, 1.0); // set the output variable to a dark-red color\n"
//"    vertexColor = gl_Position;\n"
//"}\n";

extern const char fragment_shader_source_start[];
extern const char fragment_shader_source_end[];
//const char fragment_shader_source[] = "#version 140\n"
//"out vec4 FragColor;\n"
//"in vec4 vertexColor; // the input variable from the vertex shader (same name and same type)\n"
//"void main(){\n"
//"    FragColor = vertexColor;\n"
//"}\n";

int main(void) {

    /* Initialize the library */
    if (!glfwInit())
        return -1;


    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 1);
//    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);


    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window) {
        printf("Failed to window\n");
        glfwTerminate();
        return -1;
    }

    GLfloat verts[] = {
        0.0, 0.5, 0.0,
        0.5, -0.5, 0,0,
        -0.5, -0.5, 0.0

        -1.0, -1.0, 0.0,
        0.0, -1.0, 0.0,
        -1.0, 0.0, 0.0,
    };

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        /* Problem: glewInit failed, something is seriously wrong. */
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        return -1;
    }


    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    {
        const char *aaa = vertex_shader_source_start;
        GLint length = vertex_shader_source_end - vertex_shader_source_start; // sizeof(vertex_shader_source);
        glShaderSource(vertex_shader, 1, &aaa, &length);
        glCompileShader(vertex_shader);
        GLchar aaaaaa[256];
        GLsizei aaalength = 0;
        glGetShaderInfoLog(vertex_shader, 256, &aaalength, aaaaaa);
        printf("%u: %s\n", aaalength, aaaaaa);
    }
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    {
        const char *aaa = fragment_shader_source_start;
        GLint length = fragment_shader_source_end - fragment_shader_source_start; // sizeof(fragment_shader_source);
        glShaderSource(fragment_shader, 1, &aaa, &length);
        glCompileShader(fragment_shader);
        GLchar aaaaaa[256];
        GLsizei aaalength = 0;
        glGetShaderInfoLog(fragment_shader, 256, &aaalength, aaaaaa);
        printf("%u: %s\n", aaalength, aaaaaa);
    }

    GLuint program = glCreateProgram();
    {
        glAttachShader(program, vertex_shader);
        glAttachShader(program, fragment_shader);
    }


    auto test = [&program](const char *name) {
        printf("%s: %d\n", name, glGetAttribLocation(program, name));
    };
    test("asdf");
    test("vertexColor");
    test("ertexColor");
    test("FragColor");
    glBindAttribLocation(program, 0, "vertexColor");

    test("asdf");
    test("vertexColor");
    test("ertexColor");
    test("FragColor");

    glLinkProgram(program);
    glUseProgram(program);


    test("asdf");
    test("vertexColor");
    test("ertexColor");
    test("FragColor");
    GLuint vboID;

//    printf("%p\n", (void*)glGenBuffers);

    glGenBuffers(1, &vboID);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {

        double x, y;
        glfwGetCursorPos(window, &x, &y);

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        double time = glfwGetTime();
        std::cout << time << "\n\x1b[A" << std::flush;

//        glColor3f(1.0,sin(time/4)/2+0.5,0.0);
        glDrawArrays(GL_TRIANGLES, 0, 3);
//        glColor3f(1.0,sin(time*2)/2+0.5,0.0);
        glDrawArrays(GL_TRIANGLES, 3, 3);

        glFlush();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
        verts[0] = (x - 320) / 320;
        verts[1] = (240 - y) / 240;
        glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    }

    glfwTerminate();
    return 0;
}
