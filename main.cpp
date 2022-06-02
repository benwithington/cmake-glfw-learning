#include <cstdio>
#include <string>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void errorCallback(int error, const char* description);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void windowSizeCallback(GLFWwindow* window, int width, int height);
void windowPosCallback(GLFWwindow* window, int xpos, int ypox);
void windowIconifyCallback(GLFWwindow* window, int iconified);
static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
void cursorEnterCallback(GLFWwindow* window, int entered);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void fileDropCallback(GLFWwindow* window, int count, const char** paths);

void centreWindowOnScreen(GLFWwindow* window);

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* GLFW Version Checks */
    /* Compile-time */
    printf("Compiled against GLFW %i.%i.%i\n", GLFW_VERSION_MAJOR, GLFW_VERSION_MINOR, GLFW_VERSION_REVISION);
    /* Run-time */
    int major, minor, revision;
    glfwGetVersion(&major, &minor, &revision);
    printf("Running against GLFW %i.%i.%i\n", major, minor, revision);

    printf(glfwGetVersionString());

    /* Window Hints */
    glfwWindowHint(GLFW_VISIBLE,    GL_FALSE);
    glfwWindowHint(GLFW_RESIZABLE,  GLFW_FALSE);
    glfwWindowHint(GLFW_DECORATED,  GLFW_FALSE);
    glfwWindowHint(GLFW_FOCUSED,    GLFW_TRUE);
    glfwWindowHint(GLFW_FLOATING,   GLFW_TRUE);
    glfwWindowHint(GLFW_MAXIMIZED,  GLFW_TRUE);
    glfwWindowHint(GLFW_CENTER_CURSOR, GLFW_TRUE);
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

    /* Reset Window Hints */
    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);

    /* Create a windowed mode window and its OpenGL context */
    GLFWmonitor* primary = glfwGetPrimaryMonitor();
    window = glfwCreateWindow(640, 480, "Hello, GLFW!", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Load OpenGL functions with glad*/
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        glfwTerminate();
        return -1;
    } 

    /* Set Callbacks */
    glfwSetErrorCallback(errorCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetWindowSizeCallback(window, windowSizeCallback);
    glfwSetWindowPosCallback(window, windowPosCallback);
    glfwSetWindowIconifyCallback(window, windowIconifyCallback);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetCursorEnterCallback(window, cursorEnterCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetDropCallback(window, fileDropCallback);

    //Tell glfw how big/small the window can get
    glfwSetWindowSizeLimits(window, 200, 200, GLFW_DONT_CARE, GLFW_DONT_CARE);
    //Tell glfw the desired aspect ratio of the window
    glfwSetWindowAspectRatio(window, 16, 9);

    /* Turn on/off vsync */
    glfwSwapInterval(1); //0 = vsync off, 1 = vsync on

    /* Cursor Settings */
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    /* Custom Cursor Objects */
    unsigned char pixels[16 * 16 * 4];
    memset(pixels, 0xAA, sizeof(pixels));

    GLFWimage image;
    image.width = 16;
    image.height = 16;
    image.pixels = pixels;

    GLFWcursor* cursor = glfwCreateCursor(&image, 0, 0);
    glfwSetCursor(window, cursor);

    double currentTime{glfwGetTime()};
    double previousTime{};

    int frameCount{0};

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Time Delta */
        previousTime = currentTime;
        currentTime = glfwGetTime();
        double deltaTime{currentTime - previousTime};

        if(frameCount++ % 5 == 0)
        {
            double fps{1.0 / deltaTime};
            std::string title{"Hello, GLFW!"};
            title += " ";
            title += std::to_string(fps);
            glfwSetWindowTitle(window, title.c_str());
        }

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.3f, 0.2f, 0.45f, 0.5f);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void errorCallback(int error, const char* description)
{
    std::fprintf(stderr, "Error: %s\n", description);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if(key == GLFW_KEY_R && action == GLFW_PRESS)
    {
        glfwSetWindowSize(window, 1280, 720);
    }

    if(key == GLFW_KEY_P && action == GLFW_PRESS)
    {
        centreWindowOnScreen(window);
    }

    if(key == GLFW_KEY_M && action == GLFW_PRESS)
    {
        int iconified = glfwGetWindowAttrib(window, GLFW_ICONIFIED);

        if(iconified)
        {
            glfwRestoreWindow(window);
        } else {
            glfwIconifyWindow(window);
        }
    }

    if(key == GLFW_KEY_A && action == GLFW_PRESS)
    {
        glfwRequestWindowAttention(window);
    }
}

void centreWindowOnScreen(GLFWwindow* window)
{
        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        int centrex = static_cast<int>(mode->width / 2);
        int centrey = static_cast<int>(mode->height / 2);

        int windowWidth{};
        int windowHeight{};
        glfwGetWindowSize(window, &windowWidth, &windowHeight);

        int finalxpos = centrex - static_cast<int>(windowWidth / 2);
        int finalypos = centrey - static_cast<int>(windowHeight / 2);

        glfwSetWindowPos(window, finalxpos, finalypos);
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void windowSizeCallback(GLFWwindow* window, int width, int height)
{
    int w{}, h{};
    glfwGetWindowSize(window, &w, &h);
    printf("New window size is: %i x %i\n", w, h);
}

void windowPosCallback(GLFWwindow* window, int xpos, int ypox)
{
    printf("New Window position is: %i , %i\n", xpos, ypox);
}

void windowIconifyCallback(GLFWwindow* window, int iconified)
{
    if(iconified)
        printf("Cya!\n");
    else
        printf("Welcome back!\n");
}

void cursorEnterCallback(GLFWwindow* window, int entered)
{
    if(entered)
    {
        std::cout << "Mouse has entered the window.\n";
    }
    else
    {
        std::cout << "Mouse has left the window.\n";
    }
}

static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{   
    if(static_cast<int>(xpos) % 10 == 0 || static_cast<int>(ypos) % 10 == 0)
        std::cout << "Cursor Pos, x: " << xpos << ", y: " << ypos << '\n';
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        double x{};
        double y{};
        glfwGetCursorPos(window, &x, &y);

        std::cout << "Cursor Pos on LEFT_CLICK, X: " << x << ", Y: " << y << '\n';
    }
}

void fileDropCallback(GLFWwindow* window, int count, const char** paths)
{
    for(int i = 0; i < count; i++)
    {
        std::cout << "File path " << (i+1) << " " << paths[i] << "\n";
    }
}