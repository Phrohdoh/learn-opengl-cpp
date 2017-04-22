#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

void key_cb(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

const GLchar *vertexShaderSource =
    "#version 330\n"
    "layout (location = 0) in vec3 pos3;"
    "uniform float v;"
    "uniform mat4 matf4;"
    "void main() {"
    "  vec3 pos = pos3;"
    "  pos.x += v;"
    "  gl_Position = matf4 * vec4(pos, 1.0);"
    "}";

const GLchar *fragmentShaderSource = "#version 330 core\n"
                                     "layout (location = 0) out vec4 color; void main() { color = vec4(0.0f, 0.7f, 0.2f, 1.0f); }";

int main()
{
    std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_cb);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to init GLEW" << std::endl;
        return -1;
    }

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    {
        glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
        glCompileShader(vertexShader);

        GLint success;
        GLchar infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);

        if (infoLog[0] != 0)
        {
            if (!success)
            {
                std::cout << "Vertex shader compilation failed!\n"
                          << std::endl;
            }

            std::cout << "Vertex shader info: " << infoLog << std::endl;
        }
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    {
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
        glCompileShader(fragmentShader);

        GLint success;
        GLchar infoLog[512];
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);

        if (infoLog[0] != 0)
        {
            if (!success)
            {
                std::cout << "Fragment shader compilation failed!\n"
                          << std::endl;
            }

            std::cout << "Fragment shader info: " << infoLog << std::endl;
        }
    }

    GLuint shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    {
        GLint success;
        GLchar infoLog[512];
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);

        if (infoLog[0] != 0)
        {
            if (!success)
            {
                std::cout << "Program linking failed!\n"
                          << std::endl;
            }

            std::cout << "Program linking info: " << infoLog << std::endl;
        }
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // NDC (normalized device coordinates) geometry for the sprite/shape to draw
    GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f,
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    GLfloat mat[] = {
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0,
    };

    auto v = -0.5f;

    while (!glfwWindowShouldClose(window))
    {
        v += 0.002;

        if (v > 0.5) {
            v = -0.5;
        }

        glfwPollEvents();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        auto loc_matf4 = glGetUniformLocation(shaderProgram, "matf4");
        glUniformMatrix4fv(loc_matf4, 1, false, (GLfloat const *)&mat);

        auto loc_v = glGetUniformLocation(shaderProgram, "v");
        glUniform1f(loc_v, v);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}