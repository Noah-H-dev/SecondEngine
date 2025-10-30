
#ifndef SHADER_H
#define SHADER_H
#include <string>
#include "glm/glm.hpp"

class shader
{
public:
    // the program ID
    unsigned int ID;
    // constructor reads and builds the shader
    shader(const char* vertexPath, const char* fragmentPath);
    // use/activate the shader
    void use() const;
    // utility uniform functions
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setMat4(const std::string& name, const glm::mat4& value) const;
    void setMat3(const std::string& name, const glm::mat3& value) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;
};





#endif