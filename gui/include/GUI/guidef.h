/*!
 * \file guidef.h
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

#ifndef _GUIDEF_H_
#define _GUIDEF_H_

// setup
// --------------------------------------------------
#ifdef GUI_DLLEXPORT
 #define GUI_API __declspec(dllexport)
#else
 #define GUI_API __declspec(dllimport)
#endif // GUI_DLLEXPORT
// --------------------------------------------------

#endif // !_GUIDEF_H_
