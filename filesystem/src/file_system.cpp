#include "filesystem/file_system.h"

filesystem::FileSystem::FileSystem() : _path(new std::filesystem::path(std::filesystem::current_path())) {
}

inline std::filesystem::path filesystem::FileSystem::getPath(const std::filesystem::path& path) {
	try {
		std::filesystem::path target(*_path);
		target.append(path.string());
		if (!std::filesystem::exists(target)) {
			std::string error("ERROR::FREETYPE: Failed to load ");
			error += target.filename().string();
			throw std::exception(error.c_str());
		}
	} catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}
	return path;
}

//inline void filesystem::FileSystem::loadFile(const std::filesystem::path& directory, std::string& out) {
//	try {
//		std::filesystem::path infilename(*_path);
//		infilename.append(directory.string());
//
//		if (!infilename.empty()) {
//			scrt::FileDecrypt file_decrypt;
//			// 读取文件流
//			std::ifstream in(infilename, std::ios::binary);
//			// 解密文件
//			file_decrypt.decrypt(in, out);
//		}
//	} catch (const std::exception& e) {
//		std::cout << e.what() << std::endl;
//	}
//}
//
//inline void filesystem::FileSystem::initFile(const std::filesystem::path& directory, std::filesystem::path& out) {
//	try {
//		std::filesystem::path current(*_path);
//		current.append(directory.string());
//
//		if (!current.empty()) {
//			std::filesystem::path infilename, outfilename;
//			scrt::FileEncrypt file_extension;
//			auto extension = file_extension.getExtension();
//			std::filesystem::path target;
//			target = *_path;
//			target.append(out.string());	// 创建目标目录
//
//			const auto options = std::filesystem::copy_options::recursive | std::filesystem::copy_options::overwrite_existing;
//			std::filesystem::copy(current, target, options); // 复制文件
//			
//			for(auto& entry : std::filesystem::recursive_directory_iterator(target)) {
//				if (entry.is_regular_file()) {
//					infilename = outfilename = entry.path();
//
//					if (infilename.has_extension()) {
//						auto search = extension.find(infilename.extension().string());
//						if (search != extension.end()) {
//							// 读取文件流
//							std::ifstream infs(infilename, std::ios::binary);
//							// 写入文件流
//							std::ofstream outfs(outfilename.replace_extension(search->second), std::ios::binary | std::ios::out);
//
//							// 加密文件
//							scrt::FileEncrypt file_encrypt;
//							file_encrypt.encrypt(infs, outfs);
//
//							// 清理源文件
//							outfs.close();
//							infs.close();
//							std::filesystem::remove(infilename);
//						} else {
//							continue; // 下一个文件
//						}
//					}
//				}
//			}
//		}
//	} catch (const std::exception& e) {
//		std::cout << e.what() << std::endl;
//	}
//}
