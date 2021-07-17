#include "sprite/shader.h"

namespace sprite {

Shader::Shader() : _id() {
}

inline unsigned int Shader::getId() const {
	return _id;
}

inline Shader& Shader::use() {
	glUseProgram(_id);
	return *this;
}

inline void Shader::compile(const std::string_view vertex_source, const std::string_view fragment_source, const std::string_view geometry_source) {
	// >...
	unsigned int vertex_shader = 0, fragment_shader = 0, geometry_shader = 0;
	const char* shader_source_cchar = nullptr;
	//
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	shader_source_cchar = vertex_source.data();
	glShaderSource(vertex_shader, 1, &shader_source_cchar, nullptr);
	glCompileShader(vertex_shader);
	checkCompileError(vertex_shader, "VERTEX");
	//
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	shader_source_cchar = fragment_source.data();
	glShaderSource(fragment_shader, 1, &shader_source_cchar, nullptr);
	glCompileShader(fragment_shader);
	checkCompileError(fragment_shader, "FRAGMENT");
	//
	if (geometry_source.size() != 0) {
		geometry_shader = glCreateShader(GL_GEOMETRY_SHADER);
		shader_source_cchar = geometry_source.data();
		glShaderSource(geometry_shader, 1, &shader_source_cchar, nullptr);
		glCompileShader(geometry_shader);
		checkCompileError(geometry_shader, "GEOMETRY");
	}

	// 着色器程序
	// ----------
	_id = glCreateProgram();
	glAttachShader(_id, vertex_shader);
	glAttachShader(_id, fragment_shader);
	if (geometry_source.size() != 0) {
		glAttachShader(_id, geometry_shader);
	}
	glLinkProgram(_id);
	checkCompileError(_id, "PROGRAM");
	// ---------------------------------

	// 清理资源
	// -------
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	if (geometry_source.size() != 0) {
		glDeleteShader(geometry_shader);
	}
	// ---------------------------------
}

inline void Shader::setFloat(const std::string_view name, const float value, const bool use_shader) {
	if (use_shader) {
		use();
	}
	glUniform1f(glGetUniformLocation(_id, name.data()), value);
}

inline void Shader::setInteger(const std::string_view name, const int value, const bool use_shader) {
	if (use_shader) {
		use();
	}
	glUniform1i(glGetUniformLocation(_id, name.data()), value);
}

inline void Shader::setVector2f(const std::string_view name, const float x, const float y, const bool use_shader) {
	if (use_shader) {
		use();
	}
	glUniform2f(glGetUniformLocation(_id, name.data()), x, y);
}

inline void Shader::setVector2f(const std::string_view name, const glm::vec2& value, const bool use_shader) {
	if (use_shader) {
		use();
	}
	glUniform2f(glGetUniformLocation(_id, name.data()), value.x, value.y);
}

inline void Shader::setVector3f(const std::string_view name, const float x, const float y, const float z, const bool use_shader) {
	if (use_shader) {
		use();
	}
	glUniform3f(glGetUniformLocation(_id, name.data()), x, y, z);
}

inline void Shader::setVector3f(const std::string_view name, const glm::vec3& value, const bool use_shader) {
	if (use_shader) {
		use();
	}
	glUniform3f(glGetUniformLocation(_id, name.data()), value.x, value.y, value.z);
}

inline void Shader::setVector4f(const std::string_view name, const float x, const float y, const float z, const float w, const bool use_shader) {
	if (use_shader) {
		use();
	}
	glUniform4f(glGetUniformLocation(_id, name.data()), x, y, z, w);
}

inline void Shader::setVector4f(const std::string_view name, const glm::vec4& value, const bool use_shader) {
	if (use_shader) {
		use();
	}
	glUniform4f(glGetUniformLocation(_id, name.data()), value.x, value.y, value.z, value.w);
}

inline void Shader::setMatrix4(const std::string_view name, const glm::mat4& matrix, const bool use_shader) {
	if (use_shader) {
		use();
	}
	glUniformMatrix4fv(glGetUniformLocation(_id, name.data()), 1, false, glm::value_ptr(matrix));
}

inline void Shader::checkCompileError(const unsigned int object, const std::string_view type) const {
	int success;
	std::string info_log;

	try {
		if (type != "PROGRAM") {
			glGetShaderiv(object, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(object, 1024, nullptr, info_log.data());
				std::string error = "| ERROR::SHADER: Compile-time error: Type: ";
				error += type.data() + '\n' + info_log + "\n -- --------------------------------------------------- -- \n";
				throw std::exception(error.c_str());
			}
		} else {
			glGetProgramiv(object, GL_LINK_STATUS, &success);
			if (!success) {
				glGetProgramInfoLog(object, 1024, nullptr, info_log.data());
				std::string error = "| ERROR::SHADER: Link-time error: Type: ";
				error += type.data() + '\n' + info_log + "\n -- --------------------------------------------------- -- \n";
				throw std::exception(error.c_str());
			}
		}
	} catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}
}

}
