/*!
 * \file 
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

#ifndef _AEDEF_H_
#define _AEDEF_H_

// setup
// --------------------------------------------------
#ifdef AUDIOENGINE_DLLEXPORT
 #define AUDIOENGINE_API __declspec(dllexport)
#else
 #define AUDIOENGINE_API __declspec(dllimport)
#endif // AUDIOENGINE_DLLEXPORT
// --------------------------------------------------

#endif // !_AEDEF_H_
