/*!
 * \file brktdef.h
 * \date 2020/11/04 16:42
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

#ifndef _BRKTDEF_H_
#define _BRKTDEF_H_

// setup
// --------------------------------------------------
#ifdef BREAKOUT_DLLEXPORT
 #define BREAKOUT_API __declspec(dllexport)
#else
 #define BREAKOUT_API __declspec(dllimport)
#endif // BREAKOUT_DLLEXPORT
// --------------------------------------------------

// ´°¿Ú¿í¸ß
// ----------------------------
#define	SCREEN_WIDTH	800
#define	SCREEN_HEIGHT	600
// ----------------------------

#endif // !_BRKTDEF_H_
