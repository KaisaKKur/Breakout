/*!
 * \file instance.h
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

#ifndef _INSTANCE_H_
#define _INSTANCE_H_

#include <memory>

#include "breakout.h"

#include "brktdef.h"

namespace breakout {

extern BREAKOUT_API breakout::Breakout* getInstance();

}

#endif // !_INSTANCE_H_
