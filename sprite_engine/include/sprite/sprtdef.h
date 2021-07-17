/*!
 * \file sprtdef.h
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

#ifndef _SPRTDEF_H_
#define _SPRTDEF_H_

// setup
// --------------------------------------------------
#ifdef SPRITEENGINE_DLLEXPORT
 #define SPRITEENGINE_API __declspec(dllexport)
#else
 #define SPRITEENGINE_API __declspec(dllimport)
#endif // SPRITEENGINE_DLLEXPORT
// --------------------------------------------------

#endif // !_SPRTDEF_H_
