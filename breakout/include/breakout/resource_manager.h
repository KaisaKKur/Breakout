/*!
 * \file resource_manager.h
 * \date 2020/10/30 18:45
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

#ifndef _RESOURCE_MANAGER_H_
#define _RESOURCE_MANAGER_H_

#include <map>
#include <ctime>

#include <filesystem/file_system.h>

#include <sprite/resource_engine.h>

namespace breakout {

enum class PathType {
	SHADER, TEXTURE2D, CHECKPOINT, AUDIO
};

class ResourceManager : public sprite::ResourceEngine {
public:
	static std::vector<std::vector<int>> getCheckpoint(const std::string_view path);
	static void		cacheQuitCheckpointToFile(const unsigned int& checkpoint);
	static void		loadLastQuitCheckpointFromFile(unsigned int& checkpoint);

private:
	static std::vector<std::vector<int>>	loadCheckpointFromFile(const std::string_view path);
	static std::map<PathType, std::map<std::string, std::string>>	initPaths();

public:
	static std::map<PathType, std::map<std::string, std::string>>&	getPaths();
	static std::map<std::string, std::string>&						getPaths(const PathType path_type);

private:
	static std::unique_ptr<filesystem::FileSystem>					_filesystem;
	static std::map<PathType, std::map<std::string, std::string>>	_paths;	
};

}

#endif // !_RESOURCE_MANAGER_H_
