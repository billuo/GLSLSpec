#include "glutCallback.hpp"
#include "Math.hpp"
#include "OpenGL/Common.hpp"
#include "Render.hpp"
#include <bitset>

namespace {

static std::bitset<128> KeyPressed;

static struct {
    float horizontal;
    float vertical;
} ViewAngle = { 0.0f, 0.0f };

static glm::vec3 EyePos = glm::vec3(0.0f, 0.0f, 2.0f);
static glm::vec3 LookDir = glm::vec3(0.0f, 0.0f, -1.0f);
static const glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

static struct {
    int x;
    int y;
} LastGrabbing;

} // namespace

void OnKeyboardUp(unsigned char key, int x, int y) {
    /// @TODO
    switch (key) {
    case 'w':
    case 'a':
    case 's':
    case 'd': KeyPressed[key] = false; break;
    default: break;
    }
}

void OnSpecial(int key, int x, int y) {
    /// @TODO
    switch (key) {
    default: break;
    }
}

void OnMotion(int x, int y) {
    x -= LastGrabbing.x;
    y -= LastGrabbing.y;
    LastGrabbing.x += x;
    LastGrabbing.y += y;
    y = -y; // XXX in glut, window coordinate originates at top left.
    int w = glutGet(GLUT_WINDOW_WIDTH);
    int h = glutGet(GLUT_WINDOW_HEIGHT);
    const float x_degrees(180.0f / w * x);
    const float y_degrees(180.0f / h * y);
    ViewAngle.horizontal += x_degrees;
    ViewAngle.vertical += y_degrees;
    if (ViewAngle.horizontal > 180.0f || ViewAngle.horizontal <= -180.0f) {
        ViewAngle.horizontal = std::fmod(ViewAngle.horizontal + 180.0f, 360.0f) - 180.0f;
    }
    const float y_mag_max = 89.0f;
    if (ViewAngle.vertical > y_mag_max) {
        ViewAngle.vertical = y_mag_max;
    } else if (ViewAngle.vertical < -y_mag_max) {
        ViewAngle.vertical = -y_mag_max;
    }
    LookDir.x = glm::cos(RadianOfDegree(ViewAngle.vertical)) * glm::sin(RadianOfDegree(ViewAngle.horizontal));
    LookDir.y = glm::sin(RadianOfDegree(ViewAngle.vertical));
    LookDir.z = glm::cos(RadianOfDegree(ViewAngle.vertical)) * -glm::cos(RadianOfDegree(ViewAngle.horizontal)); // XXX
}

void OnMouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            LastGrabbing.x = x;
            LastGrabbing.y = y;
        } else if (state == GLUT_UP) {
            /// @TODO
        }
    }
}

void OnKeyboard(unsigned char key, int x, int y) {
    switch (key) {
    case '\\': Render::ToggleAxes(); break;
    case '[': Render::SwitchRasterizationMode(Render::Side::Front); break;
    case ']': Render::SwitchRasterizationMode(Render::Side::Back); break;
    case 'w':
    case 'a':
    case 's':
    case 'd': KeyPressed[key] = true; break;
    case 27: glutLeaveMainLoop(); break;
    default: break;
    }
}

void Every15ms(int current_ms) {
    // callback loop
    static int last_ms = current_ms;
    int delta_ms = current_ms - last_ms;
    glutTimerFunc(15, Every15ms, current_ms + 15);
    last_ms = current_ms;
    // update based on inputs
    if (KeyPressed[static_cast<unsigned char>('w')]) {
        EyePos += LookDir * 0.01f;
    }
    if (KeyPressed[static_cast<unsigned char>('s')]) {
        EyePos -= LookDir * 0.01f;
    }
    if (KeyPressed[static_cast<unsigned char>('a')]) {
        EyePos += glm::normalize(glm::cross(Up, LookDir)) * 0.01f;
    }
    if (KeyPressed[static_cast<unsigned char>('d')]) {
        EyePos -= glm::normalize(glm::cross(Up, LookDir)) * 0.01f;
    }
    // MyModel.SetPos(glm::vec3(glm::sin(current_ms * 0.001f) - 0.5f, 0.0f, glm::cos(current_ms * 0.001f) - 0.5f));
    // MyModel.SetOrientation(
    //         glm::quat_cast(glm::rotate(glm::mat4(1.0f), delta_ms * 0.0005f, glm::vec3(0.0f, 0.0f, 1.0f))) *
    //         MyModel.GetOrientation());
    Render::SetViewMatrix(glm::lookAt(EyePos, EyePos + LookDir, Up));
}

void OnReshape(GLint w, GLint h) {
    Render::SetProjectionMatrix(glm::perspective(Pi / 2, static_cast<float>(w) / h, 0.01f, 100.0f));
    glViewport(0, 0, w, h);
}

void OnClose() { glutIdleFunc(nullptr); }
