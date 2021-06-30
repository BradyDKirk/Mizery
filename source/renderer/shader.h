#pragma once

#include <glad/glad.h>

#include <fstream>
#include <vector>

#include "core/math.h"
#include "core/typedefs.h"
#include "utility/log.h"

namespace Mizery {
    
    class Shader
    {
    public:
        Shader(const char* vertexFilepath, const char* fragmentFilepath, const char* geometryFilepath = nullptr);
        
        // Reloads shaders so shaders can be hotswapped
        void reload();
        
        // Use shader
        inline void use() const { glUseProgram(programID); }
        
        // Uniform setting
        inline void setBool(const char* name, bool32 value) const  { glUniform1i(glGetUniformLocation(programID, name), value); }
        inline void setInt(const char* name, int32 value) const    { glUniform1i(glGetUniformLocation(programID, name), value); }
        inline void setFloat(const char* name, real32 value) const { glUniform1f(glGetUniformLocation(programID, name), value); }
        
        inline void setVec2(const char* name, const glm::vec2& value) const { glUniform2fv(glGetUniformLocation(programID, name), 1, &value[0]); }
        inline void setVec3(const char* name, const glm::vec3& value) const { glUniform3fv(glGetUniformLocation(programID, name), 1, &value[0]); }
        inline void setVec4(const char* name, const glm::vec4& value) const { glUniform4fv(glGetUniformLocation(programID, name), 1, &value[0]); }
        
        inline void setMat2(const char* name, const glm::mat2& value) const { glUniformMatrix2fv(glGetUniformLocation(programID, name), 1, GL_FALSE, &value[0][0]); }
        inline void setMat3(const char* name, const glm::mat3& value) const { glUniformMatrix3fv(glGetUniformLocation(programID, name), 1, GL_FALSE, &value[0][0]); }
        inline void setMat4(const char* name, const glm::mat4& value) const { glUniformMatrix4fv(glGetUniformLocation(programID, name), 1, GL_FALSE, &value[0][0]); }
        
    private:
        uint32 programID;
        const char* vertexFilepath;
        const char* fragmentFilepath;
        const char* geometryFilepath;
        
        // Create shader program
        void createProgram();
        
        // Create and load a shader from a SPIR-V binary
        uint32 createShader(GLenum shaderType, const char* shaderFilepath);
        
        // Reads in .spv file
        static std::vector<char> readSPV(const char* filename);
    };
    
}