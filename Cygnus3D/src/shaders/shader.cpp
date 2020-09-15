#pragma once

#include "shader.h"
#include <glm/gtc/type_ptr.hpp>

namespace Cygnus3D {
		
		Shader::Shader(char const *vertexPath, char const *fragmentPath) : m_vertexPath(vertexPath), m_fragmentPath(fragmentPath) {
			shaderID = load();
		}

		Shader::Shader(char const *vertexPath, char const *fragmentPath, char const *geometryPath) : m_vertexPath(vertexPath), m_fragmentPath(fragmentPath), m_geometryPath(geometryPath) {
			shaderID = load();
		}

		Shader::~Shader() {
			glDeleteProgram(shaderID);
		}

		GLuint Shader::load() {

			GLuint program = glCreateProgram();
			GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
			GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);

			std::string vs = filesU::readFiles(m_vertexPath);
			std::string vf = filesU::readFiles(m_fragmentPath);

			const char *vSource = vs.c_str();
			const char *fSource = vf.c_str();

			glShaderSource(vertex, 1, &vSource, NULL);
			glCompileShader(vertex);

			GLint log;
			glGetShaderiv(vertex, GL_COMPILE_STATUS, &log);
			if (!log) {
				GLint length;
				glGetShaderiv(vertex, GL_INFO_LOG_LENGTH, &length);
				std::vector<char> error(length);
				glGetShaderInfoLog(vertex, length, &length, &error[0]);
				std::cout << "PROBLEM WITH VERTEX SHADER! " << std::endl << &error[0] << std::endl;
				return 0;
			}

			glShaderSource(fragment, 1, &fSource, NULL);
			glCompileShader(fragment);

			glGetShaderiv(fragment, GL_COMPILE_STATUS, &log);
			if (!log) {
				GLint length;
				glGetShaderiv(fragment, GL_INFO_LOG_LENGTH, &length);
				std::vector<char> error(length);
				glGetShaderInfoLog(fragment, length, &length, &error[0]);
				std::cout << "PROBLEM WITH FRAGMENT SHADER!" << std::endl << &error[0] << std::endl;
				return 0;
			}

			if (m_geometryPath != nullptr){

				GLuint geometry = glCreateShader(GL_GEOMETRY_SHADER);

				std::string vg = filesU::readFiles(m_geometryPath);
				const char *gSource = vg.c_str();

				glShaderSource(geometry, 1, &gSource, NULL);
				glCompileShader(geometry);

				GLint log;
				glGetShaderiv(geometry, GL_COMPILE_STATUS, &log);
				if (!log) {
					GLint length;
					glGetShaderiv(geometry, GL_INFO_LOG_LENGTH, &length);
					std::vector<char> error(length);
					glGetShaderInfoLog(geometry, length, &length, &error[0]);
					std::cout << "PROBLEM WITH GEOMETRY SHADER! " << std::endl << &error[0] << std::endl;
					return 0;
				}

				glAttachShader(program, geometry);
				glDeleteShader(geometry);
			}

			glAttachShader(program, vertex);
			glAttachShader(program, fragment);
			glLinkProgram(program);
			glValidateProgram(program);

			glDeleteShader(vertex);
			glDeleteShader(fragment);

			return program;

		}

		//void Shader::setGeometryShader(const char *geometryPath) {
		//
		//	GLuint geometry = glCreateShader(GL_GEOMETRY_SHADER);
		//
		//	std::string vg = filesU::readFiles(geometryPath);
		//	const char *gSource = vg.c_str();
		//
		//	glShaderSource(geometry, 1, &gSource, NULL);
		//	glCompileShader(geometry);
		//
		//	GLint log;
		//	glGetShaderiv(geometry, GL_COMPILE_STATUS, &log);
		//	if (!log) {
		//		GLint length;
		//		glGetShaderiv(geometry, GL_INFO_LOG_LENGTH, &length);
		//		std::vector<char> error(length);
		//		glGetShaderInfoLog(geometry, length, &length, &error[0]);
		//		std::cout << "PROBLEM WITH GEOMETRY SHADER! " << std::endl << &error[0] << std::endl;
		//		return ;
		//	}
		//
		//	glAttachShader(shaderID, geometry);
		//	glLinkProgram(shaderID);
		//	glValidateProgram(shaderID);
		//
		//	glDeleteShader(geometry);
		//}

		void Shader::enable() const{
			glUseProgram(shaderID);
		}

		void Shader::disable() const{
			glUseProgram(0);
		}

		GLint Shader::getUniformLocation(GLchar const *name) {
			return glGetUniformLocation(shaderID, name);
		}

		void Shader::setUniform1f(GLchar const *name, float f) {
			glUniform1f(getUniformLocation(name), f);
		}

		void Shader::setUniform1i(GLchar const *name, int i) {
			glUniform1i(getUniformLocation(name), i);
		}

		void Shader::setUniform2f(GLchar const *name, glm::vec2 const &v) {
			glUniform2f(getUniformLocation(name), v.x, v.y);
		}

		void Shader::setUniform3f(GLchar const *name, glm::vec3 const &v) {
			glUniform3f(getUniformLocation(name), v.x, v.y, v.z);
		}

		void Shader::setUniform4f(GLchar const *name, glm::vec4 const &v) {
			glUniform4f(getUniformLocation(name), v.x, v.y, v.z, v.w);
		}

		void Shader::setUniformMat4(GLchar const *name, glm::mat4 const &m) {
			glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &(m[0][0]));
		}

		void Shader::setUniformMat4(GLchar const *name, std::vector<glm::mat4> const &m, size_t const count) {
			glUniformMatrix4fv(getUniformLocation(name), count, GL_FALSE, glm::value_ptr(m[0]));
		}

		
}