/*!
 * \file game2d.h
 * \date 2020/10/28 20:37
 *
 * \author Kaisa.K.Kur
 * Contact: ******@***.com
 *
 */

/***************************************************************************
 * Copyright (C) 2020
 *
 * \brief 
 *
 * 
 *
 * \note
 ***************************************************************************/

#ifndef _GAME_2D_H_
#define _GAME_2D_H_

#include <iostream>
#include <string_view>

#include <GL/glew.h>

#include <breakout/instance.h>

#include "game_ui.h"

namespace game2d {

/* ÓÎÏ·ÊµÀý */
extern std::unique_ptr<breakout::Breakout> _breakout_;
/* GUI */
extern GameUI _gui_;

class Game2D {
private:
	static void windowPosCallback(GLFWwindow* window, int xpos, int ypos);
	static void	keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
	static void	framebufferSizeCallback(GLFWwindow* window, int width, int height);
	static void cursorPosCallback(GLFWwindow* window, double x, double y);
	static void mouseButtonCallback(GLFWwindow* window, int button, int state, int mod);
	static void errorCallback(int error, const char* message);

public:
	Game2D(const std::string_view title);
	virtual ~Game2D();

public:
	inline virtual void	run();
	inline virtual void	terminate();

private:
	inline virtual void		glfwInit();
	inline virtual void		init();
	inline virtual void		centerWindow();
	inline virtual void		setupCallbacks();
	inline virtual double	getDeltaTime();
	inline virtual void		render();
	inline virtual void		setIcon(const std::filesystem::path icon_path = "data/textures/icons/breakout_default.png");

public:
	inline virtual GLFWwindow*	getWindow() const;
	inline virtual double		getRuntime() const;

private:
	GLFWwindow*		_window;
	unsigned int	_width;
	unsigned int	_height;
	std::string		_title;
	double			_runtime;
};

}

#endif // !_GAME_2D_H_
