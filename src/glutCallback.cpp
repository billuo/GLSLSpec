#include "Math.hpp"
#include "OpenGL/Common.hpp"
#include "Render.hpp"
#include <bitset>


namespace {

std::bitset<128> KeyPressed;

struct {
    float horizontal;
    float vertical;
} Orientation = {0.0f, 0.0f};

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
        /// @TODO
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
    y = -y; // XXX in glut, window coordinate originates at top left.
    int w = glutGet(GLUT_WINDOW_WIDTH);
    int h = glutGet(GLUT_WINDOW_HEIGHT);
    const float x_degrees(180.0f / w * x);
    const float y_degrees(180.0f / h * y);
    Orientation.horizontal += x_degrees;
    Orientation.vertical += y_degrees;
    if (Orientation.horizontal > 180.0f || Orientation.horizontal <= -180.0f) {
        Orientation.horizontal =
                std::fmod(Orientation.horizontal + 180.0f, 360.0f) - 180.0f;
    }
    const float y_mag_max = 89.0f;
    if (Orientation.vertical > y_mag_max) {
        Orientation.vertical = y_mag_max;
    } else if (Orientation.vertical < -y_mag_max) {
        Orientation.vertical = -y_mag_max;
    }
    LookDir.x = glm::cos(RadianOfDegree(Orientation.vertical)) *
                glm::sin(RadianOfDegree(Orientation.horizontal));
    LookDir.y = glm::sin(RadianOfDegree(Orientation.vertical));
    LookDir.z = glm::cos(RadianOfDegree(Orientation.vertical)) *
                -glm::cos(RadianOfDegree(Orientation.horizontal));
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
            /// @TODO
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
    Render::setProjectionMatrix(glm::perspective(Pi / 2,
                                                 static_cast<float>(w) / h,
                                                 0.01f,
                                                 100.0f));
    glViewport(0, 0, w, h);
}

void
onClose()
{ glutIdleFunc(nullptr); }
