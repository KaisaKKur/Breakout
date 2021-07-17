#include "sprite/resource_engine.h"

namespace sprite {

// 实例化静态变量
// -------------
std::map<std::string, sprite::Shader>		ResourceEngine::_shaders;
std::map<std::string, sprite::Texture2D>	ResourceEngine::_texture2ds;

sprite::Shader ResourceEngine::loadShader(const std::string_view vertex_shader_path, const std::string_view fragment_shader_path, const std::string_view geometry_shader_path, const std::string_view name) {
	return _shaders[name.data()] = loadShaderFromFile(vertex_shader_path, fragment_shader_path, geometry_shader_path);
}

sprite::Shader& ResourceEngine::_getShader(const std::string_view name) {
	return _shaders[name.data()];
}

sprite::Texture2D ResourceEngine::loadTexture2D(const std::string_view path, const bool alpha, const std::string_view name) {
	return _texture2ds[name.data()] = loadTexture2DFromFile(path, alpha);
}

sprite::Texture2D& ResourceEngine::_getTexture2D(const std::string_view name) {
	return _texture2ds[name.data()];
}

void ResourceEngine::clear() {
	for (auto iter : _shaders) {
		glDeleteProgram(iter.second.getId());
	}
	for (auto iter : _texture2ds) {
		glDeleteTextures(1, &iter.second._getId());
	}
}

sprite::Shader ResourceEngine::loadShaderFromFile(const std::string_view vertex_shader_path, const std::string_view fragment_shader_path, const std::string_view geometry_shader_path) {
	std::string vertex_code, fragment_code, geometry_code;

	// 从 filepath 检索顶点/片段源代码
	// ------------------------------
	try {
		std::ifstream vertex_shader_file(vertex_shader_path.data());
        std::ifstream fragment_shader_file(fragment_shader_path.data());
        std::stringstream vertex_shader_stream, fragment_shader_stream;

		vertex_shader_stream << vertex_shader_file.rdbuf();
        fragment_shader_stream << fragment_shader_file.rdbuf();

		vertex_code = vertex_shader_stream.str();
        fragment_code = fragment_shader_stream.str();

		vertex_shader_file.close();
        fragment_shader_file.close();

		if (geometry_shader_path.size() != 0) {
            std::ifstream geometry_shader_file(geometry_shader_path.data());
            std::stringstream geometry_shader_stream;
            geometry_shader_stream << geometry_shader_file.rdbuf();
            geometry_shader_file.close();
            geometry_code = geometry_shader_stream.str();
        }
	} catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}
	// ------------------------------------

	// 从源代码创建着色器对象
	// ---------------------
	sprite::Shader shader;
	shader.compile(vertex_code, fragment_code, geometry_shader_path.size() != 0 ? geometry_code : NULL_STRING);
	return shader;
	// -----------
}

sprite::Texture2D ResourceEngine::loadTexture2DFromFile(const std::string_view path, const bool alpha) {
	sprite::Texture2D texture2d;

	if (alpha) {
		texture2d.setInternalFormat(GL_RGBA);
		texture2d.setImageFormat(GL_RGBA);
	}

	try {
		//
		// 加载图片
		int width, height, nr_channels;
		unsigned char* data = stbi_load(path.data(), &width, &height, &nr_channels, 0);
		//
		// 生成纹理
		if (data) {
			texture2d.generate(width, height, data);
		} else {
			throw std::exception("Failed to load texture");
		}
		//
		// 清理资源
		stbi_image_free(data);
	} catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}
	
	return texture2d;
}

}
