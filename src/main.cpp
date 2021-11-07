#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <math.h>
#include <vector>
#include <random>
#include <array>
#include <cmath>

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

struct asteroid {
    GLuint buf_id;
    GLuint buf_size;
    GLfloat x, y, rot, x_vel, y_vel;
};

const GLfloat pi = 3.14159;
std::vector<GLfloat> generate_asteroid()
{
    std::random_device dev;
    std::default_random_engine eng(dev());
    std::uniform_int_distribution rng(4, 7);
    int count = rng(eng);
    GLfloat bound = (2.0*pi)*(0.5f/(GLfloat)count);
    std::uniform_real_distribution boundRange(-bound, bound);
    std::uniform_real_distribution scaleRange(0.05, 0.2);

    std::vector<GLfloat> verts;

    for(int i = 0; i < count; ++i)
    {
        GLfloat rads = (2.0*pi)*((GLfloat)i/(GLfloat)count);
        rads += boundRange(eng);
        GLfloat x = std::sin(rads) * scaleRange(eng);
        GLfloat y = std::cos(rads) * scaleRange(eng);
        verts.push_back(x);
        verts.push_back(y);
        //std::cout << x << " " << y << std::endl;
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
    GLFWwindow* window = glfwCreateWindow(640, 640, "Hello World", NULL, NULL);
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
glEnableClientState(GL_VERTEX_ARRAY);
    glGenVertexArrays(1, &vaoID);
    glBindVertexArray(vaoID);

//    printf("%p\n", (void*)glGenBuffers);



    std::array<GLuint, 100> asteroid_buffers;
    std::array<GLuint, 100> asteroid_sizes;

    glGenBuffers(100, asteroid_buffers.data());///FIGURE OUT VBOS AND VAOS BEFORE DOING MORE

    for(int i = 0; i < asteroid_buffers.size(); ++i)
    {
        std::vector test_asteroid = generate_asteroid();
        asteroid_sizes[i] = test_asteroid.size();
    glBindBuffer(GL_ARRAY_BUFFER, asteroid_buffers[i]);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*test_asteroid.size(), test_asteroid.data(), GL_STATIC_DRAW);
    }

    std::array<GLuint, 8> letter_buffers;
    std::array<GLuint, 8> letter_sizes;

    glGenBuffers(8, letter_buffers.data());

    {
        std::vector<GLfloat> letters[] = { // GAME OVER
            { // G
                0, 0, .1, 0,
                .1, 0, .1, -.1,
                .1, -.1, -.1, -.1,
                -.1, -.1, -.1, .1,
                -.1, .1, .1, .1
            },
            { // A
                -.1, -.1, 0, .1,
                0, .1, .1, -.1,
                0, .07, 0, -.07
            },
            { // M
                -.1, -.1, -.1, .1,
                -.1, .1, 0, 0,
                0, 0, .1, .1,
                .1, .1, .1, -.1
            },
            { // E
                -.1, -.1, -.1, .1,
                -.1, .1, .1, .1,
                -.1, 0, .1, 0,
                -.1, -.1, .1, -.1
            },
            { // O
                -.1, -.1, -.1, .1,
                -.1, -.1, .1, -.1,
                .1, .1, -.1, .1,
                .1, .1, .1, -.1
            },
            { // V
                .1, .1, 0, -.1,
                -.1, .1, 0, -.1
            },
            { // E
                -.1, -.1, -.1, .1,
                -.1, .1, .1, .1,
                -.1, 0, .1, 0,
                -.1, -.1, .1, -.1
            },
            { // R
                -.1, -.1, -.1, .1,
                -.1, .1, .1, .1,
                -.1, 0, .1, 0,
                .1, 0, .1, .1,
                -.1, 0, .1, -.1
            }
        };

        for(int i = 0; i < 8; ++i)
        {
            letter_sizes[i] = letters[i].size();
            glBindBuffer(GL_ARRAY_BUFFER, letter_buffers[i]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * letters[i].size(), letters[i].data(), GL_STATIC_DRAW);
        }
    }

    GLuint binOne, binZero;
    glGenBuffers(1, &binOne);
    glGenBuffers(1, &binZero);
    GLuint oneSize, zeroSize;
    {
        GLfloat one[] = {
            0, -.1, 0, .1
        };

        oneSize = 2;
        glBindBuffer(GL_ARRAY_BUFFER, binOne);
        glBufferData(GL_ARRAY_BUFFER, sizeof(one), one, GL_STATIC_DRAW);

        GLfloat zero[] = {
            -.1, -.1,
            .1, -.1,
            .1, .1,
            -.1, .1
        };

        zeroSize = 4;
        glBindBuffer(GL_ARRAY_BUFFER, binZero);
        glBufferData(GL_ARRAY_BUFFER, sizeof(zero), zero, GL_STATIC_DRAW);
    }

    GLuint ship_buffer;
    glGenBuffers(1, &ship_buffer);
    GLuint ship_size = 4;
    {
        GLfloat ship_verts[] = {
            0, 0,
            .04, -.04,
            0, .04,
            -.04, -.04
        };

        glBindBuffer(GL_ARRAY_BUFFER, ship_buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(ship_verts), ship_verts, GL_STATIC_DRAW);
    }

    GLuint laser_buffer;
    GLuint laser_size = 2;
    glGenBuffers(1, &laser_buffer);
    {
        GLfloat laser_verts[] = {
            0, -.02,
            0, .02
        };
        glBindBuffer(GL_ARRAY_BUFFER, laser_buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(laser_verts), laser_verts, GL_STATIC_DRAW);
    }

//    glEnableVertexAttribArray(0);
    glEnableVertexArrayAttrib(vaoID, 0);

    GLint rot = uniform_location("rot");
    GLint pos = uniform_location("pos");

    GLfloat player_rot = 0, x_vel = 0, y_vel = 0, x = 0, y = 0;

    double time = glfwGetTime();
    double old_time = glfwGetTime();

    std::vector<asteroid> lasers;
    std::vector<asteroid> asteroids;

    double laser_timer = 0;

    std::random_device dev;
    std::default_random_engine eng(dev());
    std::uniform_real_distribution asteroid_timer_range(0.1, 0.4);
    std::uniform_real_distribution asteroid_location_range(-2.9, 2.9);
    std::uniform_real_distribution asteroid_rotation_range(-2*pi, 2*pi);
    std::uniform_int_distribution asteroid_buff_id_range(0, 100);

    double asteroid_timer = asteroid_timer_range(eng);

    bool gameover = false;

    unsigned char score = 0;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {

        //double x, y;
        //glfwGetCursorPos(window, &x, &y);

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        old_time = time;
        time = glfwGetTime();
        double deltaTime = time-old_time;
        //std::cout << time << "\n\x1b[A" << std::flush;

        //glUniform1f(rot, time);
        //glUniform2f(pos, sin(time), cos(time));

//        glColor3f(1.0,sin(time/4)/2+0.5,0.0);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
//        glColor3f(1.0,sin(time*2)/2+0.5,0.0);
        //glDrawArrays(GL_TRIANGLES, 3, 3);

/*
        int roid = (int)time % asteroid_buffers.size();

        glBindBuffer(GL_ARRAY_BUFFER, asteroid_buffers[roid]);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

        glDrawArrays(GL_LINE_LOOP, 0, asteroid_sizes[roid]);
*/
        //draw ship
        if(!gameover){
        glUniform2f(pos, x, y);
        glUniform1f(rot, player_rot);
        glBindBuffer(GL_ARRAY_BUFFER, ship_buffer);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glDrawArrays(GL_LINE_LOOP, 0, ship_size);}
        /*
        verts[0] = (x - 320) / 320;
        verts[1] = (240 - y) / 240;
        glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
        */

        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        {
            player_rot += .1;
        }

        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        {
            player_rot -= .1;
        }

        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        {
            x_vel -= sin(player_rot) * 0.01;
            y_vel -= cos(player_rot) * 0.01;
        }

        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        {
            x_vel += sin(player_rot) * 0.01;
            y_vel += cos(player_rot) * 0.01;
        }

        y_vel = std::max(-0.9f, y_vel);
        y_vel = std::min(0.9f, y_vel);
        x_vel = std::max(-0.9f, x_vel);
        x_vel = std::min(0.9f, x_vel);

        x += x_vel * deltaTime;
        y -= y_vel * deltaTime;

        if(x < -1.0)
            x += 2.0;
        if(x > 1.0)
            x -= 2.0;
        if(y < -1.0)
            y += 2.0;
        if(y > 1.0)
            y -= 2.0;

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        {
            laser_timer += deltaTime;
        }

        if(laser_timer >= .3)
        {
            laser_timer -= .3;
            asteroid new_laser;
            new_laser.rot = player_rot;
            new_laser.x = x;
            new_laser.y = y;
            new_laser.x_vel = -sin(player_rot);
            new_laser.y_vel = -cos(player_rot);
            lasers.push_back(new_laser);
        }

        asteroid_timer -= deltaTime;

        if(asteroid_timer < 0.0)
        {
            asteroid_timer += asteroid_timer_range(eng);
            asteroid new_asteroid;
            while(new_asteroid.x > -1 && new_asteroid.x < 1)
                new_asteroid.x = asteroid_location_range(eng);
            while(new_asteroid.y > -1 && new_asteroid.y < 1)
                new_asteroid.y = asteroid_location_range(eng);
            new_asteroid.rot = asteroid_rotation_range(eng);
            new_asteroid.x_vel = sin(new_asteroid.rot) * .3;
            new_asteroid.y_vel = cos(new_asteroid.rot) * .3;
            int id_loc = asteroid_buff_id_range(eng);
            new_asteroid.buf_id = asteroid_buffers[id_loc];
            new_asteroid.buf_size = asteroid_sizes[id_loc];
            asteroids.push_back(new_asteroid);
        }

        for(int i = 0; i < lasers.size(); ++i)
        {
            lasers[i].x += lasers[i].x_vel * deltaTime;
            lasers[i].y -= lasers[i].y_vel * deltaTime;

            glUniform2f(pos, lasers[i].x, lasers[i].y);
            glUniform1f(rot, lasers[i].rot);
            glBindBuffer(GL_ARRAY_BUFFER, laser_buffer);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
            glDrawArrays(GL_LINE_LOOP, 0, laser_size);

            if(lasers[i].x < -2 || lasers[i].x > 2 || lasers[i].y < -2 || lasers[i].y > 2)
            {
                lasers.erase(lasers.begin()+i);
                --i;
            }
        }

        for(int i = 0; i < asteroids.size(); ++i)
        {
            asteroids[i].x += asteroids[i].x_vel * deltaTime;
            asteroids[i].y -= asteroids[i].y_vel * deltaTime;

            glUniform2f(pos, asteroids[i].x, asteroids[i].y);
            glUniform1f(rot, asteroids[i].rot);
            glBindBuffer(GL_ARRAY_BUFFER, asteroids[i].buf_id);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
            glDrawArrays(GL_LINE_LOOP, 0, asteroids[i].buf_size);

            double dist = sqrt(pow(asteroids[i].x - x,2) + pow(asteroids[i].y - y, 2));

            if(dist < 0.12)
            {
                gameover = true;
            }

            for(int i2 = 0; i2 < lasers.size(); ++i2)
            {
                dist = sqrt(pow(asteroids[i].x - lasers[i2].x,2) + pow(asteroids[i].y - lasers[i2].y, 2));
                if(dist < 0.12)
                {
                    score++;
                    lasers.erase(lasers.begin()+i2);
                    asteroids.erase(asteroids.begin()+i);
                    --i;
                    continue;
                }
            }

            if(asteroids[i].x < -3 || asteroids[i].x > 3 || asteroids[i].y < -3 || asteroids[i].y > 3)
            {
                asteroids.erase(asteroids.begin()+i);
                --i;
            }
        }

        for(int i = 0; i < 8; ++i)
        {
            glUniform2f(pos, 0.87 - .25*i, .8);
            glUniform1f(rot, 0);
            if(1<<i & score)
            {
                glBindBuffer(GL_ARRAY_BUFFER, binOne);
                glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
                glDrawArrays(GL_LINES, 0, oneSize);
            }
            else
            {
                glBindBuffer(GL_ARRAY_BUFFER, binZero);
                glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
                glDrawArrays(GL_LINE_LOOP, 0, zeroSize);
            }
        }

        if(gameover)
        for(int i = 0; i < 8; ++i){
            glUniform2f(pos, -0.87 + .25*i, 0);
            glUniform1f(rot, 0);
            glBindBuffer(GL_ARRAY_BUFFER, letter_buffers[i]);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
            glDrawArrays(GL_LINES, 0, letter_sizes[i]);
        }

        glFlush();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
