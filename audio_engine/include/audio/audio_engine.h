/*!
 * \file audio_engine.h
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

#ifndef _AUDIO_ENGINE_H_
#define _AUDIO_ENGINE_H_

#include <iostream>
#include <filesystem>

#include <irrKlang/irrKlang.h>

#include "aedef.h"

namespace audio {

class AUDIOENGINE_API AudioEngine {
public:
	AudioEngine();
	virtual ~AudioEngine();

public:
	inline virtual void init();
	inline virtual void destroy();

	inline virtual void play2D(const std::filesystem::path& filepath, const bool loop = false, const float volume = 1.0f);
	inline virtual void stopAll();

private:
	irrklang::ISoundEngine*	_engine;
};


}

#endif // !_AUDIO_ENGINE_H_
