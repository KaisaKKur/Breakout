#include "game2d/game2d.h"
#include "game2d/console_control.h"

// ���ؿ���̨
// ������ڵ�ַ
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

int main() {
	// setup
	// ���ؿ���̨
	// ����˸Ч��
	//game2d::ConsoleControl::hideConsole();

	// ������Ϸʵ��
	// -----------
	auto game2D = std::make_unique<game2d::Game2D>("Breakout");
	game2D->run();
	// -----------

	return 0;
}
