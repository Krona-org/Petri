#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
    GLuint ID;

    // Конструктор: создаёт шейдерную программу из файлов
    Shader(const char* vertexFile, const char* fragmentFile);

    // Активировать шейдерную программу
    void Activate();

    // Удалить шейдерную программу
    void Delete();

    // Униформы:
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;

private:
    // Чтение шейдерного файла в строку
    static std::string get_file_contents(const char* filename);
};

#endif