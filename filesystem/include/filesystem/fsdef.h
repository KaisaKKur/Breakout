/*!
 * \file fsdef.h
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

#ifndef _FSDEF_H_
#define _FSDEF_H_

// setup
// --------------------------------------------------
#ifdef FILESYSTEM_DLLEXPORT
 #define FILESYSTEM_API __declspec(dllexport)
#else
 #define FILESYSTEM_API __declspec(dllimport)
#endif // FILESYSTEM_DLLEXPORT
// --------------------------------------------------

#endif // !_FSDEF_H_
