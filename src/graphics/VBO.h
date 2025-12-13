#ifndef VBO_CLASS_H

#define VBO_CLASS_H

#include <glad/glad.h>

class VBO
{
	public:
		GLuint ID;
		VBO() {} // дефолтный конструктор
		VBO(GLfloat* vertices, GLsizeiptr size);

		void Bind();
		void Unbind();
		void SetData(void* data, GLsizeiptr size);
		void Delete();
};

#endif