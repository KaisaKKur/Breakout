/*!
 * \file file_system.h
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

#ifndef _FILE_SYSTEM_H_
#define _FILE_SYSTEM_H_

#include <iostream>
#include <filesystem>

#include "fsdef.h"

namespace filesystem {

class FILESYSTEM_API FileSystem {
public:
	FileSystem();

public:
	inline virtual std::filesystem::path getPath(const std::filesystem::path& path);

private:
	std::unique_ptr<std::filesystem::path>	_path;
};

}

#endif // !_FILE_SYSTEM_H_

