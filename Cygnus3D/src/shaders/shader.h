#pragma once

#include "../utils/filesU.h"
#include <GLEW/glew.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/matrix.hpp>
#include <vector>

namespace Cygnus3D {

		class Shader {

		private: 
			GLuint shaderID;
			const char *m_vertexPath, *m_fragmentPath, *m_geometryPath;

			GLuint load();
			GLint getUniformLocation(GLchar const *name);
		public:
			Shader(char const *vertexPath, char const *fragmentPath);
			Shader(char const *vertexPath, char const *fragmentPath, char const *geometryPath);
			~Shader();


			void enable() const;
			void disable() const;
			//void setGeometryShader(char const *geometryPath);

			void setUniform1f(GLchar const *name, float f);
			void setUniform1i(GLchar const *name, int i);
			void setUniform2f(GLchar const *name, glm::vec2 const &v);
			void setUniform3f(GLchar const *name, glm::vec3 const &v);
			void setUniform4f(GLchar const *name, glm::vec4 const &v);
			void setUniformMat4(GLchar const *name, glm::mat4 const &m);
			void setUniformMat4(GLchar const *name, std::vector<glm::mat4> const &m, size_t count);

		};
}