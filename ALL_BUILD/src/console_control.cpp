#include "game2d/console_control.h"

void game2d::ConsoleControl::hideConsole() {
    ::ShowWindow(::GetConsoleWindow(), SW_HIDE);
}

void game2d::ConsoleControl::showConsole() {
    ::ShowWindow(::GetConsoleWindow(), SW_SHOW);
}

bool game2d::ConsoleControl::isConsoleVisible() {
    return ::IsWindowVisible(::GetConsoleWindow()) != FALSE;
}
