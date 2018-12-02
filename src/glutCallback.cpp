#include "Math.hpp"
#include "OpenGL/Common.hpp"
#include "Render.hpp"
#include "Log.hpp"


namespace {

using Degree = Math::Degree;
using Radian = Math::Radian;

std::bitset<128> KeyPressed;

struct {
    Degree horizontal;
    Degree vertical;
} Orientation = {0_deg, 0_deg};

glm::vec3 EyePos = glm::vec3(0.0f, 0.0f, 2.0f);
glm::vec3 LookDir = glm::vec3(0.0f, 0.0f, -1.0f);
const glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

struct {
    int x;
    int y;
} LastGrabbing;
} // namespace

void
onKeyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case '\\':
            Render::toggleAxes();
            break;
        case '[':
            Render::switchRasterizationMode(Render::Side::Front);
            break;
        case ']':
            Render::switchRasterizationMode(Render::Side::Back);
            break;
        case 'W':
        case 'A':
        case 'S':
        case 'D':
        case 'w':
        case 'a':
        case 's':
        case 'd':
            KeyPressed[std::tolower(key)] = true;
            break;
        case 27:
            glutLeaveMainLoop();
            break;
        default:
            break;
    }
}

void
onKeyboardUp(unsigned char key, int x, int y)
{
    switch (key) {
        case 'W':
        case 'A':
        case 'S':
        case 'D':
        case 'w':
        case 'a':
        case 's':
        case 'd':
            KeyPressed[std::tolower(key)] = false;
            break;
        case 'j':
            Render::Shininess *= 2;
            break;
        case 'k':
            Render::Shininess /= 2;
            break;
        case 'r':
            Render::Shininess = 1.0f;
            break;
        default:
            break;
    }
}

void
onSpecial(int key, int x, int y)
{
    switch (key) {
        default:
            break;
    }
}

void
onMotion(int x, int y)
{
    x -= LastGrabbing.x;
    y -= LastGrabbing.y;
    LastGrabbing.x += x;
    LastGrabbing.y += y;
    // XXX in glut, window coordinate origin is located at top left corner.
    // But we want to calculate as if it's at bottom left corner.
    y = -y;
    int w = glutGet(GLUT_WINDOW_WIDTH);
    int h = glutGet(GLUT_WINDOW_HEIGHT);
    Orientation.horizontal += 180_deg / w * x;
    Orientation.vertical += 180_deg / h * y;
    Orientation.horizontal.round_half();
    Orientation.vertical.clamp(89.0f);
    LookDir.x = Orientation.vertical.cos() * Orientation.horizontal.sin();
    LookDir.y = Orientation.vertical.sin();
    LookDir.z = Orientation.vertical.cos() * -Orientation.horizontal.cos();
}

void
onMouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            LastGrabbing.x = x;
            LastGrabbing.y = y;
        } else if (state == GLUT_UP) {
            LastGrabbing.x = -1;
            LastGrabbing.y = -1;
        }
    }
}

void
onTimer(int current_ms)
{
    // callback loop
    static int last_ms = current_ms;
    int delta_ms = current_ms - last_ms;
    glutTimerFunc(15, onTimer, current_ms + 15);
    last_ms = current_ms;
    // update based on inputs
    static float velocity = 1.5f;
    float step = velocity * 15 / 1000;
    if (KeyPressed[static_cast<unsigned char>('w')]) {
        EyePos += LookDir * step;
    }
    if (KeyPressed[static_cast<unsigned char>('s')]) {
        EyePos -= LookDir * step;
    }
    if (KeyPressed[static_cast<unsigned char>('a')]) {
        EyePos += glm::normalize(glm::cross(Up, LookDir)) * step;
    }
    if (KeyPressed[static_cast<unsigned char>('d')]) {
        EyePos -= glm::normalize(glm::cross(Up, LookDir)) * step;
    }
    Render::setViewMatrix(glm::lookAt(EyePos, EyePos + LookDir, Up));
}

void
onReshape(GLint w, GLint h)
{
    Render::setProjectionMatrix(glm::perspective(::Math::Pi / 2, static_cast<float>(w) / h, 0.01f, 100.0f));
    glViewport(0, 0, w, h);
}

void
onClose()
{ glutIdleFunc(nullptr); }
