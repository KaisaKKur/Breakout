#include "game2d/game2d.h"
#include "game2d/console_control.h"

// 隐藏控制台
// 设置入口地址
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

int main() {
	// setup
	// 隐藏控制台
	// 有闪烁效果
	//game2d::ConsoleControl::hideConsole();

	// 运行游戏实例
	// -----------
	auto game2D = std::make_unique<game2d::Game2D>("Breakout");
	game2D->run();
	// -----------

	return 0;
}
