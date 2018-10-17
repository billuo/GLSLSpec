#ifndef GLUTCALLBACK_HPP_ZXY1E9DC
#define GLUTCALLBACK_HPP_ZXY1E9DC
#pragma once

void OnKeyboardUp(unsigned char key, int x, int y);

void OnSpecial(int key, int x, int y);

void OnMotion(int x, int y);

void OnMouse(int button, int state, int x, int y);

void OnKeyboard(unsigned char key, int x, int y);

void OnReshape(int w, int h);

void OnClose();

void Every15ms(int current_ms);

#endif /* end of include guard: GLUTCALLBACK_HPP_ZXY1E9DC */
