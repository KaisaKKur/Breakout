/*!
 * \file gui.h
 * \date 2020/11/07 14:34
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

#ifndef _GUI_H_
#define _GUI_H_

// 兼容 c++17
#pragma warning(disable : 4996)
// 排除未初始化变量
#pragma warning(disable : 26495)

#include <iostream>

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/OpenGL/GL3Renderer.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include "guidef.h"

namespace gui {

class GUI_API GUI {
public:
	static CEGUI::OpenGL3Renderer* getRenderder();
	static void destroyRenderer();

	static CEGUI::MouseButton	toCEGUIButton(const int button);
	static CEGUI::Key::Scan		toCEGUIKey(const int glfwKey);

	static void	setWidgetDestRect(CEGUI::Window* widget, const glm::vec4& dest_rect_perc, const glm::vec4& dest_rect_pix);
	static void setWidgetCenter(glm::vec4& convert);
	static CEGUI::Window* createWidget(CEGUI::Window* root, const std::string_view type, const glm::vec4& dest_rect_perc, const glm::vec4& dest_rect_pix, const std::string_view name = "");

public:
	GUI();
	virtual ~GUI();

public:
	inline virtual void init(const std::string_view resource_directory = "datafiles");
	inline virtual void draw();
	inline virtual void destroy();

	inline virtual void	loadScheme(const std::string_view scheme_file);
	inline virtual void	setFont(const std::string_view font_file);
	inline virtual CEGUI::Window*	createWidget(const std::string_view type, const glm::vec4& dest_rect_perc, const glm::vec4& dest_rect_pix, const std::string_view name = "");

	inline virtual void setMouseCursor(const std::string_view image_file);
	inline virtual void showMouseCursor();
	inline virtual void hideMouseCursor();

public:
	inline CEGUI::GUIContext* getContext() const;
	inline CEGUI::Window* getRoot() const;

private:
	static CEGUI::OpenGL3Renderer* _renderer;

private:
	CEGUI::GUIContext*	_context;
	CEGUI::Window*		_root;
};

}

#endif // !_GUI_H_
