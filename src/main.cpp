#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <math.h>
#include <vector>
#include <random>

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

const GLfloat pi = 3.14159;
std::vector<GLfloat> generate_asteroid()
{
    std::random_device dev;
    std::default_random_engine eng(dev());
    std::uniform_int_distribution rng(4, 7);
    int count = rng(eng);
    GLfloat bound = (2.0*pi)*(0.5f/(GLfloat)count);
    std::uniform_real_distribution boundRange(-bound, bound);
    std::uniform_real_distribution scaleRange(0.1, 0.3);

    std::vector<GLfloat> verts;

    for(int i = 0; i < count; ++i)
    {
        GLfloat rads = (2.0*pi)*((GLfloat)i/(GLfloat)count);
        rads += boundRange(eng);
        GLfloat x = std::sin(rads) * scaleRange(eng);
        GLfloat y = std::cos(rads) * scaleRange(eng);
        verts.push_back(x);
        verts.push_back(y);
        verts.push_back(0);
        std::cout << x << " " << y << std::endl;
    }

    return verts;
}

int main(void) {

    /* Initialize the library */
    if (!glfwInit())
        return -1;


    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);


    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window) {
        const char *desc;
        glfwGetError(&desc);
        printf("Failed to create window: %s\n", desc);
        glfwTerminate();
        return -1;
    }

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
        GLint result;
        glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &result);
        if (result == GL_FALSE) {
            GLchar aaaaaa[256];
            GLsizei aaalength = 0;
            glGetShaderInfoLog(vertex_shader, 256, &aaalength, aaaaaa);
            printf("vertex: %u: %s\n", aaalength, aaaaaa);
        }
    }
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    {
        const char *aaa = fragment_shader_source_start;
        GLint length = fragment_shader_source_end - fragment_shader_source_start; // sizeof(fragment_shader_source);
        glShaderSource(fragment_shader, 1, &aaa, &length);
        glCompileShader(fragment_shader);
        GLint result;
        glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &result);
        if (result == GL_FALSE) {
            GLchar aaaaaa[256];
            GLsizei aaalength = 0;
            glGetShaderInfoLog(fragment_shader, 256, &aaalength, aaaaaa);
            printf("fragment: %u: %s\n", aaalength, aaaaaa);
        }
    }

    GLuint program = glCreateProgram();
    {
        glAttachShader(program, vertex_shader);
        glAttachShader(program, fragment_shader);
    }


    auto attrib_location = [&program](const char *name) -> GLint{
        GLint result = glGetAttribLocation(program, name);
        if (result != -1)
            printf("%s: %d\n", name, result);
        else {
            printf("Failed to get attribute location of (%s): %d\n", name, glGetError());
        }
        return result;
    };
    auto uniform_location = [&program](const char *name) -> GLint {
        GLint result = glGetUniformLocation(program, name);
        if (result != -1)
            printf("%s: %d\n", name, result);
        else {
            printf("Failed to get uniform location of (%s): %d\n", name, glGetError());
        }
        return result;
    };
//    attrib_location("asdf"); // GL_INVALID_OPERATION
//    attrib_location("vertexColor"); // GL_INVALID_OPERATION
//    attrib_location("ertexColor"); // GL_INVALID_OPERATION
//    attrib_location("FragColor"); // GL_INVALID_OPERATION
//    {
//        glBindAttribLocation(program, 2, "vertexColor");
//        GLint result = glGetError();
//        if (result != 0) printf("Failed to set attribute location: %d\n", result);
//    }

//    attrib_location("asdf"); // GL_INVALID_OPERATION
//    attrib_location("vertexColor"); // GL_INVALID_OPERATION
//    attrib_location("ertexColor"); // GL_INVALID_OPERATION
//    attrib_location("FragColor"); // GL_INVALID_OPERATION

    glLinkProgram(program);
    {
        GLint result;
        glGetProgramiv(program, GL_LINK_STATUS, &result);
        if (result != GL_TRUE) {
            GLchar aaaaaa[256];
            GLsizei aaalength = 0;
            glGetProgramInfoLog(program, 256, &aaalength, aaaaaa);
            printf("%u: %s\n", aaalength, aaaaaa);
        }
    }
//    {
//        glBindAttribLocation(program, 2, "vertexColor");
//        GLint result = glGetError();
//        if (result != 0) printf("Failed to set attribute location: %d\n", result);
//    }
    glUseProgram(program);
    if (0) {
        GLchar aaaaaa[256];
        GLsizei aaalength = 0;
        glGetProgramInfoLog(program, 256, &aaalength, aaaaaa);
        printf("%u: %s\n", aaalength, aaaaaa);
    }


//    attrib_location("asdf"); // 1282 GL_INVALID_OPERATION (0x502)
//    attrib_location("vertexColor"); // 1282 GL_INVALID_OPERATION (0x502)
//    attrib_location("ertexColor"); // 1282 GL_INVALID_OPERATION (0x502)
    attrib_location("FragColor"); // 1282 GL_INVALID_OPERATION (0x502)
    GLuint vboID;
    GLuint vaoID;

    glGenVertexArrays(1, &vaoID);
    glBindVertexArray(vaoID);

//    printf("%p\n", (void*)glGenBuffers);
    GLfloat verts[] = {
        0.0, 0.5, 0.0,
        0.5, -0.5, 0,0,
        -0.5, -0.5, 0.0

        -1.0, -1.0, 0.0,
        0.0, -1.0, 0.0,
        -1.0, 0.0, 0.0,
    };

    std::vector test_asteroid = generate_asteroid();

    glGenBuffers(1, &vboID);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*test_asteroid.size(), test_asteroid.data(), GL_STATIC_DRAW);

//    glEnableVertexAttribArray(0);
    glEnableVertexArrayAttrib(vaoID, 0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    GLint sin_time_location = uniform_location("sin_time");

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {

        double x, y;
        glfwGetCursorPos(window, &x, &y);

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        double time = glfwGetTime();
        std::cout << time << "\n\x1b[A" << std::flush;

        glUniform1f(sin_time_location, sin(time));

//        glColor3f(1.0,sin(time/4)/2+0.5,0.0);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
//        glColor3f(1.0,sin(time*2)/2+0.5,0.0);
        //glDrawArrays(GL_TRIANGLES, 3, 3);
        glDrawArrays(GL_LINE_LOOP, 0, test_asteroid.size());

        glFlush();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
        /*
        verts[0] = (x - 320) / 320;
        verts[1] = (240 - y) / 240;
        glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
        */
    }

    glfwTerminate();
    return 0;
}
