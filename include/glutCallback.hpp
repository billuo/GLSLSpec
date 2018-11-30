#ifndef GLUTCALLBACK_HPP_ZXY1E9DC
#define GLUTCALLBACK_HPP_ZXY1E9DC
#pragma once

void onKeyboardUp(unsigned char key, int x, int y);

void onSpecial(int key, int x, int y);

void onMotion(int x, int y);

void onMouse(int button, int state, int x, int y);

void onKeyboard(unsigned char key, int x, int y);

void onReshape(int w, int h);

void onClose();

void onTimer(int current_ms);

#endif /* end of include guard: GLUTCALLBACK_HPP_ZXY1E9DC */
