/**
 * @file main.cpp
 * @brief Program entry point.
 * @author Zhen Luo 461652354@qq.com
 */
#include "Debug.hpp"
#include "OpenGL/Common.hpp"
#include "Render.hpp"
#include "glutCallback.hpp"


int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    // glut window setup
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
    glutInitContextVersion(4, 5);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
    glutInitContextFlags(GLUT_CORE_PROFILE | GLUT_DEBUG);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Sphere");
    // glut setup window specific callbacks
    glutDisplayFunc(Render::render);
    glutReshapeFunc(onReshape);
    glutKeyboardFunc(onKeyboard);
    glutKeyboardUpFunc(onKeyboardUp);
    glutSpecialFunc(onSpecial);
    glutMotionFunc(onMotion);
    glutMouseFunc(onMouse);
    glutCloseFunc(onClose);
    // init glew for OpenGL 4+
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        DEBUG("Failed to init glew:%s", glewGetErrorString(err));
    } else if (GLEW_VERSION_4_5) {
        DEBUG("Found OpenGL 4.5 support");
    }
    // init for draw
    Render::init();
    // glut setup global callbacks
    glutIdleFunc(Render::render);
    glutTimerFunc(15, onTimer, 0);
    // main loop
    glutMainLoop();
}
