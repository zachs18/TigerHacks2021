#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <math.h>
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


    GLuint vboID;

//    printf("%p\n", (void*)glGenBuffers);

    glGenBuffers(1, &vboID);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        double time = glfwGetTime();
        std::cout << time << "\n\x1b[A" << std::flush;

        glColor3f(1.0,sin(time/4)/2+0.5,0.0);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glColor3f(1.0,sin(time*2)/2+0.5,0.0);
        glDrawArrays(GL_TRIANGLES, 3, 3);

        glFlush();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
        verts[0] = sin(time/3);
        glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    }

    glfwTerminate();
    return 0;
}
