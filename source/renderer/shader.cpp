#include "shader.h"

namespace Mizery {
    
    Shader::Shader(const char* vertexFilepath, const char* fragmentFilepath, const char* geometryFilepath) : vertexFilepath(vertexFilepath), fragmentFilepath(fragmentFilepath), geometryFilepath(geometryFilepath)
    {
        // Create shader program
        programID = glCreateProgram();
        
        // Load shaders and attach to program
        createProgram();
    }

    void Shader::reload()
    {
        // Unbind shader
        glUseProgram(0);
        
        // Load shaders and attach to program
        createProgram();
    }

    void Shader::createProgram()
    {
        // Load shader binaries
        uint32 vertexShader = createShader(GL_VERTEX_SHADER, vertexFilepath);
        uint32 fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentFilepath);
        uint32 geometryShader;
        
        // Attach shaders to shader program
        glAttachShader(programID, vertexShader);
        glAttachShader(programID, fragmentShader);
        
        // Handle geometry shader
        const bool32 loadGeomShader = geometryFilepath ? true : false;
        if (loadGeomShader)
        {
            geometryShader = createShader(GL_GEOMETRY_SHADER, geometryFilepath);
            glAttachShader(programID, geometryShader);
        }
        
        // Link shader program
        glLinkProgram(programID);
        
        // Cleanup shaders
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        if (loadGeomShader)
        {
            glDeleteShader(geometryShader);
        }
    }

    uint32 Shader::createShader(GLenum shaderType, const char* shaderFilepath)
    {
        // Load SPIR-V binary and create shader
        uint32 shader = glCreateShader(shaderType);
        std::vector<char> shaderBinary = readSPV(shaderFilepath);
        glShaderBinary(1, &shader, GL_SHADER_BINARY_FORMAT_SPIR_V, shaderBinary.data(), shaderBinary.size());
        glSpecializeShader(shader, "main", 0, 0, 0);
        
        // Check for errors
        int32 compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled)
        {
            if (shaderType == GL_VERTEX_SHADER)
            {
                LOG_ERROR("Vertex shader failed to compile");
            }
            else if (shaderType == GL_FRAGMENT_SHADER)
            {
                LOG_ERROR("Fragment shader failed to compile");
            }
            else if (shaderType == GL_GEOMETRY_SHADER)
            {
                LOG_ERROR("Geometry shader failed to compile");
            }
            else
            {
                LOG_ERROR("Unknown shader failed to compile");
            }
            return 0;
        }
        
        return shader;
    }

    std::vector<char> Shader::readSPV(const char* filename)
    {
        // Open file
        std::ifstream file(filename, std::ios::ate | std::ios::binary);

        if (!file.is_open())
        {
            LOG_ERROR("Failed to open file \'{0}\'", filename);
            return std::vector<char>(0);
        }

        // Get file size
        uint64 fileSize = (uint64)file.tellg();
        std::vector<char> buffer(fileSize);

        // Return to beginning of file and read
        file.seekg(0);
        file.read(buffer.data(), fileSize);

        // Close file and return
        file.close();

        return buffer;
    }
    
}