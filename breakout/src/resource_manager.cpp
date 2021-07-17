#include "breakout/resource_manager.h"

namespace breakout {

// ÊµÀý»¯¾²Ì¬±äÁ¿
// -------------
std::unique_ptr<filesystem::FileSystem>		ResourceManager::_filesystem = std::make_unique<filesystem::FileSystem>();
std::map<PathType, std::map<std::string, std::string>>	ResourceManager::_paths = ResourceManager::initPaths();

std::vector<std::vector<int>> ResourceManager::getCheckpoint(const std::string_view path) {
	return loadCheckpointFromFile(path);
}

void ResourceManager::cacheQuitCheckpointToFile(const unsigned int& checkpoint) {
	try {
		std::filesystem::path cache_dir(std::filesystem::current_path().append("cache"));
		if (!std::filesystem::exists(cache_dir)) {
			std::filesystem::create_directory(cache_dir);
		}

		std::filesystem::path log_path(std::filesystem::current_path().append("cache").append("checkpoint.log"));
		std::ofstream ofstrm(log_path);

		std::string log = "checkpoint ";
		std::locale::global(std::locale("en_US.utf8"));
		std::time_t t = std::time(NULL);
		char mbstr[100];
		if (!std::strftime(mbstr, sizeof(mbstr), "%A %c", std::localtime(&t))) {
			throw std::exception("Error while getting localtime!");
		}

		log += std::to_string(checkpoint) + " ";
		log += "last run: ";
		log += mbstr;

		if (ofstrm.good()) {
			ofstrm.seekp(std::ios_base::beg);
			ofstrm << log;
		} else {
			throw std::exception("Error while opening output file!");
		}
	} catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}
}

void ResourceManager::loadLastQuitCheckpointFromFile(unsigned int& checkpoint) {
	try {
		std::filesystem::path log_path(std::filesystem::current_path().append("cache").append("checkpoint.log"));

		if (!std::filesystem::exists(log_path)) {
			throw std::exception("Error while Opening checkpoint.log!");
		}

		std::string discard;
		std::ifstream ifstrm(log_path);

		if (ifstrm.good()) {
			ifstrm >> discard;
			ifstrm >> checkpoint;
		} else {
			throw std::exception("Error while opening output file!");
		}
	} catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}
}

std::vector<std::vector<int>> ResourceManager::loadCheckpointFromFile(const std::string_view path) {
	std::vector<std::vector<int>> tile_data;

	try {
		unsigned int tile_code;
		std::string line;
		std::ifstream fstream(path);

		if (fstream) {
			while (std::getline(fstream, line)) {
				std::istringstream sstream(line);
				std::vector<int> row;
				while (sstream >> tile_code) {
					row.push_back(tile_code);
				}
				tile_data.push_back(row);
			}	
		}
	} catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}		
	
	return tile_data;
}

std::map<PathType, std::map<std::string, std::string>> ResourceManager::initPaths() {
	// ¼ÓÃÜ×ÊÔ´
	//_filesystem->initFile("resources");

	// ×ÅÉ«Æ÷Â·¾¶
	std::map<std::string, std::string>* shader_paths = new std::map<std::string, std::string>();
	(*shader_paths)["sprite_vs"]			= _filesystem->getPath("data/shaders/sprite.vert").string();
	(*shader_paths)["sprite_fs"]			= _filesystem->getPath("data/shaders/sprite.frag").string();
	(*shader_paths)["particle_vs"]			= _filesystem->getPath("data/shaders/particle.vert").string();
	(*shader_paths)["particle_fs"]			= _filesystem->getPath("data/shaders/particle.frag").string();
	(*shader_paths)["post-processing_vs"]	= _filesystem->getPath("data/shaders/post-processing.vert").string();
	(*shader_paths)["post-processing_fs"]	= _filesystem->getPath("data/shaders/post-processing.frag").string();
	
	// ÎÆÀíÂ·¾¶
	std::map<std::string, std::string>* texture2d_paths = new std::map<std::string, std::string>();
	// ²Ëµ¥±³¾°
	(*texture2d_paths)["menu_bk"]				= _filesystem->getPath("data/textures/menu/menu_bk.png").string();
	// °å
	(*texture2d_paths)["bat_default"]				= _filesystem->getPath("data/textures/bats/bat_default.png").string();
	(*texture2d_paths)["powerup_speed"]				= _filesystem->getPath("data/textures/bats/powerup_speed.png").string();
	(*texture2d_paths)["powerup_sticky"]			= _filesystem->getPath("data/textures/bats/powerup_sticky.png").string();
	(*texture2d_paths)["powerup_pass-through"]		= _filesystem->getPath("data/textures/bats/powerup_pass-through.png").string();
	(*texture2d_paths)["powerup_pad-size-increase"]	= _filesystem->getPath("data/textures/bats/powerup_pad-size-increase.png").string();
	(*texture2d_paths)["powerup_confuse"]			= _filesystem->getPath("data/textures/bats/powerup_confuse.png").string();
	(*texture2d_paths)["powerup_chaos"]				= _filesystem->getPath("data/textures/bats/powerup_chaos.png").string();
	// Á£×Ó
	(*texture2d_paths)["square"]	= _filesystem->getPath("data/textures/particles/square.png").string();	
	// Çò
	(*texture2d_paths)["ball_crystal"]			= _filesystem->getPath("data/textures/balls/crystal.png").string();
	(*texture2d_paths)["ball_space-invader"]	= _filesystem->getPath("data/textures/balls/space-invader.png").string();
	
	// ¹Ø¿¨ 1
	// -----------------------------------------------------------------------------------------------
	// ±³¾°
	(*texture2d_paths)["1bk1"]	= _filesystem->getPath("data/textures/checkpoints/1/bk1.png").string();
	// ÍßÆ¬
	(*texture2d_paths)["1b1"]	= _filesystem->getPath("data/textures/checkpoints/1/b1.png").string();
	(*texture2d_paths)["1b2"]	= _filesystem->getPath("data/textures/checkpoints/1/b2.png").string();
	(*texture2d_paths)["1b3"]	= _filesystem->getPath("data/textures/checkpoints/1/b3.png").string();
	(*texture2d_paths)["1b4"]	= _filesystem->getPath("data/textures/checkpoints/1/b4.png").string();
	(*texture2d_paths)["1b5"]	= _filesystem->getPath("data/textures/checkpoints/1/b5.png").string();
	(*texture2d_paths)["1b6"]	= _filesystem->getPath("data/textures/checkpoints/1/b6.png").string();
	(*texture2d_paths)["1b7"]	= _filesystem->getPath("data/textures/checkpoints/1/b7.png").string();
	// ×©
	(*texture2d_paths)["1s1"]	= _filesystem->getPath("data/textures/checkpoints/1/s1.png").string();
	// -----------------------------------------------------------------------------------------------

	// ¹Ø¿¨ 2
	// -----------------------------------------------------------------------------------------------
	// ±³¾°
	(*texture2d_paths)["2bk1"]	= _filesystem->getPath("data/textures/checkpoints/2/bk1.jpg").string();
	// ÍßÆ¬
	(*texture2d_paths)["2b1"]	= _filesystem->getPath("data/textures/checkpoints/2/b1.png").string();
	(*texture2d_paths)["2b2"]	= _filesystem->getPath("data/textures/checkpoints/2/b2.png").string();
	(*texture2d_paths)["2b3"]	= _filesystem->getPath("data/textures/checkpoints/2/b3.png").string();
	(*texture2d_paths)["2b4"]	= _filesystem->getPath("data/textures/checkpoints/2/b4.png").string();
	(*texture2d_paths)["2b5"]	= _filesystem->getPath("data/textures/checkpoints/2/b5.png").string();
	(*texture2d_paths)["2b6"]	= _filesystem->getPath("data/textures/checkpoints/2/b6.png").string();
	(*texture2d_paths)["2b7"]	= _filesystem->getPath("data/textures/checkpoints/2/b7.png").string();
	// ×©
	(*texture2d_paths)["2s1"]	= _filesystem->getPath("data/textures/checkpoints/2/s1.png").string();
	// -----------------------------------------------------------------------------------------------
	
	// ¹Ø¿¨ 3
	// -----------------------------------------------------------------------------------------------
	// ±³¾°
	(*texture2d_paths)["3bk1"]	= _filesystem->getPath("data/textures/checkpoints/3/bk1.jpg").string();
	// ÍßÆ¬
	(*texture2d_paths)["3b1"]	= _filesystem->getPath("data/textures/checkpoints/3/b1.png").string();
	(*texture2d_paths)["3b2"]	= _filesystem->getPath("data/textures/checkpoints/3/b2.png").string();
	(*texture2d_paths)["3b3"]	= _filesystem->getPath("data/textures/checkpoints/3/b3.png").string();
	(*texture2d_paths)["3b4"]	= _filesystem->getPath("data/textures/checkpoints/3/b4.png").string();
	(*texture2d_paths)["3b5"]	= _filesystem->getPath("data/textures/checkpoints/3/b5.png").string();
	(*texture2d_paths)["3b6"]	= _filesystem->getPath("data/textures/checkpoints/3/b6.png").string();
	(*texture2d_paths)["3b7"]	= _filesystem->getPath("data/textures/checkpoints/3/b7.png").string();
	// ×©
	(*texture2d_paths)["3s1"]	= _filesystem->getPath("data/textures/checkpoints/3/s1.png").string();
	// -----------------------------------------------------------------------------------------------
	
	// ¹Ø¿¨ 4
	// -----------------------------------------------------------------------------------------------
	// ±³¾°
	(*texture2d_paths)["4bk1"]	= _filesystem->getPath("data/textures/checkpoints/4/bk1.jpg").string();
	// ÍßÆ¬
	(*texture2d_paths)["4b1"]	= _filesystem->getPath("data/textures/checkpoints/4/b1.png").string();
	(*texture2d_paths)["4b2"]	= _filesystem->getPath("data/textures/checkpoints/4/b2.png").string();
	(*texture2d_paths)["4b3"]	= _filesystem->getPath("data/textures/checkpoints/4/b3.png").string();
	(*texture2d_paths)["4b4"]	= _filesystem->getPath("data/textures/checkpoints/4/b4.png").string();
	(*texture2d_paths)["4b5"]	= _filesystem->getPath("data/textures/checkpoints/4/b5.png").string();
	(*texture2d_paths)["4b6"]	= _filesystem->getPath("data/textures/checkpoints/4/b6.png").string();
	(*texture2d_paths)["4b7"]	= _filesystem->getPath("data/textures/checkpoints/4/b7.png").string();
	// ×©
	(*texture2d_paths)["4s1"]	= _filesystem->getPath("data/textures/checkpoints/4/s1.png").string();
	// -----------------------------------------------------------------------------------------------
	
	// ¹Ø¿¨ 5
	// -----------------------------------------------------------------------------------------------
	// ±³¾°
	(*texture2d_paths)["5bk1"]	= _filesystem->getPath("data/textures/checkpoints/5/bk1.jpg").string();
	// ÍßÆ¬
	(*texture2d_paths)["5b1"]	= _filesystem->getPath("data/textures/checkpoints/5/b1.png").string();
	(*texture2d_paths)["5b2"]	= _filesystem->getPath("data/textures/checkpoints/5/b2.png").string();
	(*texture2d_paths)["5b3"]	= _filesystem->getPath("data/textures/checkpoints/5/b3.png").string();
	(*texture2d_paths)["5b4"]	= _filesystem->getPath("data/textures/checkpoints/5/b4.png").string();
	(*texture2d_paths)["5b5"]	= _filesystem->getPath("data/textures/checkpoints/5/b5.png").string();
	(*texture2d_paths)["5b6"]	= _filesystem->getPath("data/textures/checkpoints/5/b6.png").string();
	(*texture2d_paths)["5b7"]	= _filesystem->getPath("data/textures/checkpoints/5/b7.png").string();
	// ×©
	(*texture2d_paths)["5s1"]	= _filesystem->getPath("data/textures/checkpoints/5/s1.png").string();
	// -----------------------------------------------------------------------------------------------
	
	// ¹Ø¿¨ 6
	// -----------------------------------------------------------------------------------------------
	// ±³¾°
	(*texture2d_paths)["6bk1"]	= _filesystem->getPath("data/textures/checkpoints/6/bk1.jpg").string();
	// ÍßÆ¬
	(*texture2d_paths)["6b1"]	= _filesystem->getPath("data/textures/checkpoints/6/b1.png").string();
	(*texture2d_paths)["6b2"]	= _filesystem->getPath("data/textures/checkpoints/6/b2.png").string();
	(*texture2d_paths)["6b3"]	= _filesystem->getPath("data/textures/checkpoints/6/b3.png").string();
	(*texture2d_paths)["6b4"]	= _filesystem->getPath("data/textures/checkpoints/6/b4.png").string();
	(*texture2d_paths)["6b5"]	= _filesystem->getPath("data/textures/checkpoints/6/b5.png").string();
	(*texture2d_paths)["6b6"]	= _filesystem->getPath("data/textures/checkpoints/6/b6.png").string();
	(*texture2d_paths)["6b7"]	= _filesystem->getPath("data/textures/checkpoints/6/b7.png").string();
	// ×©
	(*texture2d_paths)["6s1"]	= _filesystem->getPath("data/textures/checkpoints/6/s1.png").string();
	// -----------------------------------------------------------------------------------------------
	
	// ¹Ø¿¨ 7
	// -----------------------------------------------------------------------------------------------
	// ±³¾°
	(*texture2d_paths)["7bk1"]	= _filesystem->getPath("data/textures/checkpoints/7/bk1.jpg").string();
	// ÍßÆ¬
	(*texture2d_paths)["7b1"]	= _filesystem->getPath("data/textures/checkpoints/7/b1.png").string();
	(*texture2d_paths)["7b2"]	= _filesystem->getPath("data/textures/checkpoints/7/b2.png").string();
	(*texture2d_paths)["7b3"]	= _filesystem->getPath("data/textures/checkpoints/7/b3.png").string();
	(*texture2d_paths)["7b4"]	= _filesystem->getPath("data/textures/checkpoints/7/b4.png").string();
	(*texture2d_paths)["7b5"]	= _filesystem->getPath("data/textures/checkpoints/7/b5.png").string();
	(*texture2d_paths)["7b6"]	= _filesystem->getPath("data/textures/checkpoints/7/b6.png").string();
	(*texture2d_paths)["7b7"]	= _filesystem->getPath("data/textures/checkpoints/7/b7.png").string();
	// ×©
	(*texture2d_paths)["7s1"]	= _filesystem->getPath("data/textures/checkpoints/7/s1.png").string();
	// -----------------------------------------------------------------------------------------------
	
	// ¹Ø¿¨ 8
	// -----------------------------------------------------------------------------------------------
	// ±³¾°
	(*texture2d_paths)["8bk1"]	= _filesystem->getPath("data/textures/checkpoints/8/bk1.jpg").string();
	// ÍßÆ¬
	(*texture2d_paths)["8b1"]	= _filesystem->getPath("data/textures/checkpoints/8/b1.png").string();
	(*texture2d_paths)["8b2"]	= _filesystem->getPath("data/textures/checkpoints/8/b2.png").string();
	(*texture2d_paths)["8b3"]	= _filesystem->getPath("data/textures/checkpoints/8/b3.png").string();
	(*texture2d_paths)["8b4"]	= _filesystem->getPath("data/textures/checkpoints/8/b4.png").string();
	(*texture2d_paths)["8b5"]	= _filesystem->getPath("data/textures/checkpoints/8/b5.png").string();
	(*texture2d_paths)["8b6"]	= _filesystem->getPath("data/textures/checkpoints/8/b6.png").string();
	(*texture2d_paths)["8b7"]	= _filesystem->getPath("data/textures/checkpoints/8/b7.png").string();
	// ×©
	(*texture2d_paths)["8s1"]	= _filesystem->getPath("data/textures/checkpoints/8/s1.png").string();
	// -----------------------------------------------------------------------------------------------
	
	std::map<std::string, std::string>* checkpoint_paths = new std::map<std::string, std::string>();
	// ²Ëµ¥ bricks
	(*checkpoint_paths)["menu"]	= _filesystem->getPath("data/textures/menu/menu").string();
	// µÈ¼¶µØÍ¼Â·¾¶
	(*checkpoint_paths)["1"]	= _filesystem->getPath("data/checkpoints/1.cp").string();
	(*checkpoint_paths)["2"]	= _filesystem->getPath("data/checkpoints/2.cp").string();
	(*checkpoint_paths)["3"]	= _filesystem->getPath("data/checkpoints/3.cp").string();
	(*checkpoint_paths)["4"]	= _filesystem->getPath("data/checkpoints/4.cp").string();
	(*checkpoint_paths)["5"]	= _filesystem->getPath("data/checkpoints/5.cp").string();
	(*checkpoint_paths)["6"]	= _filesystem->getPath("data/checkpoints/6.cp").string();
	(*checkpoint_paths)["7"]	= _filesystem->getPath("data/checkpoints/7.cp").string();
	(*checkpoint_paths)["8"]	= _filesystem->getPath("data/checkpoints/8.cp").string();

	// ÒôÆµÂ·¾¶
	std::map<std::string, std::string>* audio_paths = new std::map<std::string, std::string>();
	// Main Menu
	(*audio_paths)["main_menu"] = _filesystem->getPath("data/audio/menu/main_menu-marcu.mp3").string();
	// ingame
	(*audio_paths)["bleep1"] = _filesystem->getPath("data/audio/ingame/bleep.mp3").string();
	(*audio_paths)["bleep2"] = _filesystem->getPath("data/audio/ingame/bleep.wav").string();
	(*audio_paths)["powerup"] = _filesystem->getPath("data/audio/ingame/powerup.wav").string();
	(*audio_paths)["solid"] = _filesystem->getPath("data/audio/ingame/solid.wav").string();
	(*audio_paths)["win"] = _filesystem->getPath("data/audio/ingame/win-round_end.wav").string();
	(*audio_paths)["lose"] = _filesystem->getPath("data/audio/ingame/lose-death.wav").string();
	// ¹Ø¿¨±³¾°Òô
	(*audio_paths)["cp1"] = _filesystem->getPath("data/audio/ingame/cp1-marcu.mp3").string();
	(*audio_paths)["cp2"] = _filesystem->getPath("data/audio/ingame/cp2-RGA-GT - Match and Tries.mp3").string();
	(*audio_paths)["cp3"] = _filesystem->getPath("data/audio/ingame/cp3-night night.ogg").string();
	(*audio_paths)["cp4"] = _filesystem->getPath("data/audio/ingame/cp4-RGA-GT - American Coffee.mp3").string();
	(*audio_paths)["cp5"] = _filesystem->getPath("data/audio/ingame/cp5-RGA-GT - Club Life.mp3").string();
	(*audio_paths)["cp6"] = _filesystem->getPath("data/audio/ingame/cp6-RGA-GT - Factory Creates New.mp3").string();
	(*audio_paths)["cp7"] = _filesystem->getPath("data/audio/ingame/cp7-RGA-GT - Just Mecho-Style.mp3").string();
	(*audio_paths)["cp8"] = _filesystem->getPath("data/audio/ingame/cp8-RGA-GT - Being Cool Doesn`t Make Me Fool.mp3").string();

	return std::map<PathType, std::map<std::string, std::string>> {
		{ PathType::SHADER,		(*shader_paths) },
		{ PathType::TEXTURE2D,	(*texture2d_paths) },
		{ PathType::CHECKPOINT,		(*checkpoint_paths) },
		{ PathType::AUDIO,		(*audio_paths) }
	};
}

std::map<PathType, std::map<std::string, std::string>>& ResourceManager::getPaths() {
	return _paths;
}

std::map<std::string, std::string>& ResourceManager::getPaths(const PathType path_type) {
	return _paths.at(path_type);
}

}
