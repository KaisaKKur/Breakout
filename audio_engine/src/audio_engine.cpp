#include "audio/audio_engine.h"

audio::AudioEngine::AudioEngine() : _engine(nullptr) {
}

audio::AudioEngine::~AudioEngine() {
	destroy();
}

inline void audio::AudioEngine::init() {
	try {
		if (!(_engine = irrklang::createIrrKlangDevice())) {
			throw std::exception("Failed to init irrklang::ISoundEngine");
		}
	} catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}
}

inline void audio::AudioEngine::destroy() {
	_engine->drop();
}

inline void audio::AudioEngine::play2D(const std::filesystem::path& filepath, const bool loop, const float volume) {
	irrklang::ISound* snd = _engine->play2D(filepath.string().c_str(), loop, false, true);

	if (snd) {
		snd->setVolume(volume);
		snd->drop();
		snd = nullptr;
	}
}

inline void audio::AudioEngine::stopAll() {
	_engine->stopAllSounds();
}
