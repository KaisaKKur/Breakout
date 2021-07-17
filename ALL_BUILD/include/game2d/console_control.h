/*!
 * \file console_control.h
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


#ifndef _CONSOLR_CONTROL_H_
#define _CONSOLR_CONTROL_H_

#include <Windows.h>

namespace game2d {

class ConsoleControl {
public:
	static void hideConsole();
	static void showConsole();
	static bool isConsoleVisible();
};


}

#endif // !_CONSOLR_CONTROL_H_
