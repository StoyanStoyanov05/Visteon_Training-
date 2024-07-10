#include <glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "tiny_gltf.h"
#include "basic_types.hpp"

using namespace std;

std::unordered_map<std::string, float> materialUniformFloats;
std::unordered_map<std::string, int> materialUniformInt;
std::unordered_map<std::string, Vector2> materialUniformVector2;
std::unordered_map<std::string, Vector3> materialUniformVector3;
std::unordered_map<std::string, Vector4> materialUniformVector4;


static float getcurrentTime()
{
    return (float)glfwGetTime();
}

static void materialSetPropert(std::string uniformName, float value)
{
    if (materialUniformFloats.find(uniformName) != materialUniformFloats.end())
    {
        materialUniformFloats[uniformName] = value;
    }
}
static void materialSetPropert(std::string uniformName, Vector4 value)
{
    if (materialUniformVector4.find(uniformName) != materialUniformVector4.end())
    {
        materialUniformVector4[uniformName] = value;
    }
}

static void materialSetPropert(std::string uniformName, Vector3 value)
{
    if (materialUniformVector3.find(uniformName) != materialUniformVector3.end())
    {
        materialUniformVector3[uniformName] = value;
    }
}

static void materialSetPropert(std::string uniformName, Vector2 value)
{
    if (materialUniformVector2.find(uniformName) != materialUniformVector2.end())
    {
        materialUniformVector2[uniformName] = value;
    }
}

static void materialSetPropert(std::string uniformName, int value)
{
    if (materialUniformInt.find(uniformName) != materialUniformInt.end())
    {
        materialUniformInt[uniformName] = value;
    }
}

static void materialUpdateProperties(GLuint materialProgram)
{
    for (auto& uniform : materialUniformFloats)
    {
        GLint uniformLocation = glGetUniformLocation(materialProgram, uniform.first.c_str());
        if (uniformLocation != -1)
        {
            glUniform1f(uniformLocation, uniform.second);
        }
        std::cout << "Uniform float: " << uniform.first << " = " << uniform.second << std::endl;
    }
    for (auto& uniform : materialUniformVector4)
    {
        GLint uniformLocation = glGetUniformLocation(materialProgram, uniform.first.c_str());
        if (uniformLocation != -1)
        {
            glUniform4f(uniformLocation, uniform.second.x, uniform.second.y, uniform.second.z, uniform.second.w);
        }
        std::cout << "Uniform float: " << uniform.first << " = " << uniform.second.x << ", " << uniform.second.y << ", " << uniform.second.z << ", " << uniform.second.w << std::endl;
    }
    for (auto& uniform : materialUniformVector3)
    {
        GLint uniformLocation = glGetUniformLocation(materialProgram, uniform.first.c_str());
        if (uniformLocation != -1)
        {
            glUniform3f(uniformLocation, uniform.second.x, uniform.second.y, uniform.second.z);
        }
        std::cout << "Uniform float: " << uniform.first << " = " << uniform.second.x << ", " << uniform.second.y << ", " << uniform.second.z << std::endl;
    }
    for (auto& uniform : materialUniformVector2)
    {
        GLint uniformLocation = glGetUniformLocation(materialProgram, uniform.first.c_str());
        if (uniformLocation != -1)
        {
            glUniform2f(uniformLocation, uniform.second.x, uniform.second.y);
        }
        std::cout << "Uniform float: " << uniform.first << " = " << uniform.second.x << ", " << uniform.second.y << std::endl;
    }
    for (auto& uniform : materialUniformInt)
    {
        GLint uniformLocation = glGetUniformLocation(materialProgram, uniform.first.c_str());
        if (uniformLocation != -1)
        {
            glUniform1i(uniformLocation, uniform.second);
        }
        std::cout << "Uniform int: " << uniform.first << " = " << uniform.second << std::endl;
    }

}
GLuint loadMaterial(tinygltf::Model& model,std::filesystem::path& gltfDirectory, unsigned int materialId)
{

    std::filesystem::path vertexShaderPath;
    std::filesystem::path fragmentShadePath;
     std::string vertexShaderSource;
     std::string fragmentShaderSource;

    const char* defaultVertexShaderSource =
        #include "vertexShader.glsl"
        ;

    const char* defaultFragmentShaderSource =
        #include "fragmentShader.glsl"
        ;

    if (materialId < model.materials.size())
    {
    auto gltfMaterialExtras = model.materials[materialId].extras;
    if (gltfMaterialExtras.Has("shader"))
    {
        auto gltfMaterialShader = gltfMaterialExtras.Get("shader");
        if (gltfMaterialShader.Has("vertex"))
        {
            std::string gltfMaterialShaderVertex = gltfMaterialShader.Get("vertex").Get<std::string>();
            vertexShaderPath = gltfDirectory / gltfMaterialShaderVertex; 
        }
        if (gltfMaterialShader.Has("fragment"))
        {
            std::string gltfMaterialShaderFragment = gltfMaterialShader.Get("fragment").Get<std::string>();
            fragmentShadePath = gltfDirectory / gltfMaterialShaderFragment;
        }
        if (gltfMaterialShader.Has("uniforms"))
        {
            auto gltfUniforms = gltfMaterialShader.Get("uniforms");
            for (int uniformIdx = 0; uniformIdx < gltfUniforms.ArrayLen(); uniformIdx++)
            {
                auto uniform = gltfUniforms.Get(uniformIdx);
                std::string uniformName;
                if (uniform.Has("name"))
                {
                    uniformName = uniform.Get("name").Get<std::string>();
                }
                if (uniform.Has("type"))
                {
                    std::string type = uniform.Get("type").Get<std::string>();
                    auto uniformValue = uniform.Get("value");
                    if (type == "Float")
                    { 
                      double uniformValueFloat = uniformValue.Get(0).Get<double>();  
                      materialUniformFloats[uniformName] = uniformValueFloat;
                      std::cout << "Uniform float: " << uniformName << " = " << uniformValueFloat << std::endl;
                    }
                    else if (type == "Vector4")
                    {
                      double uniformValueFloatX = uniformValue.Get(0).Get<double>();
                      double uniformValueFloatY = uniformValue.Get(1).Get<double>();
                      double uniformValueFloatZ = uniformValue.Get(2).Get<double>();
                      double uniformValueFloatW = uniformValue.Get(3).Get<double>();
                      materialUniformVector4[uniformName] = Vector4(uniformValueFloatX, uniformValueFloatY, uniformValueFloatZ, uniformValueFloatW);
                      std::cout << "Uniform vector4: " << uniformName << " = " << uniformValueFloatX << ", " << uniformValueFloatY << ", " << uniformValueFloatZ << ", " << uniformValueFloatW << std::endl;
                    }
                    else if (type == "Vector3")
                    {
                      double uniformValueFloatX = uniformValue.Get(0).Get<double>();
                      double uniformValueFloatY = uniformValue.Get(1).Get<double>();
                      double uniformValueFloatZ = uniformValue.Get(2).Get<double>();
                      materialUniformVector3[uniformName] = Vector3(uniformValueFloatX, uniformValueFloatY, uniformValueFloatZ);
                      std::cout << "Uniform vector3: " << uniformName << " = " << uniformValueFloatX << ", " << uniformValueFloatY << ", " << uniformValueFloatZ << std::endl;
                    }
                    else if (type == "Vector2")
                    {
                      double uniformValueFloatX = uniformValue.Get(0).Get<double>();
                      double uniformValueFloatY = uniformValue.Get(1).Get<double>();
                      materialUniformVector2[uniformName] = Vector2(uniformValueFloatX, uniformValueFloatY);
                      std::cout << "Uniform vector2: " << uniformName << " = " << uniformValueFloatX << ", " << uniformValueFloatY << std::endl;
                    }
                    else if (type == "Int")
                    {
                      int uniformValueInt = uniformValue.Get(0).Get<int>();
                      materialUniformInt[uniformName] = uniformValueInt;
                      std::cout << "Uniform int: " << uniformName << " = " << uniformValueInt << std::endl;
                    }
                }
            }
        }
    }
    }
    else
    {
        vertexShaderSource = defaultVertexShaderSource;
        fragmentShaderSource = defaultFragmentShaderSource;
    }
    
    std::ifstream vertexShaderFile(vertexShaderPath);
    if(vertexShaderFile.is_open())
    {
        std::string vertexShaderSourceString((std::istreambuf_iterator<char>(vertexShaderFile)), std::istreambuf_iterator<char>());
        vertexShaderSource = vertexShaderSourceString;
        vertexShaderFile.close();
    }

    std::ifstream fragmentShaderFile(fragmentShadePath);
    if(fragmentShaderFile.is_open())
    {
        std::string fragmentShaderSourceString((std::istreambuf_iterator<char>(fragmentShaderFile)), std::istreambuf_iterator<char>());
        fragmentShaderSource = fragmentShaderSourceString;
        fragmentShaderFile.close();
    }

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertexShaderSourcePointer = vertexShaderSource.c_str();
    glShaderSource(vertexShader,1 ,&vertexShaderSourcePointer, nullptr);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragmentShaderSourcePointer = fragmentShaderSource.c_str();
    glShaderSource(fragmentShader, 1, &fragmentShaderSourcePointer, nullptr);

    glCompileShader(vertexShader);
    GLint status;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status); 
    if (status == GL_FALSE)
    {
        GLint infoLogLength;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &infoLogLength);
        std::vector<char> vertexShaderErrorMessage(infoLogLength);
        glGetShaderInfoLog(vertexShader, infoLogLength, nullptr, vertexShaderErrorMessage.data());
        std::cerr << "Failed to compile vertex shader: " << vertexShaderErrorMessage.data() << std::endl;
        return -1;  
    }

    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status); 
    if (status == GL_FALSE)
    {
        GLint infoLogLength;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &infoLogLength);
        std::vector<char> fragmentShaderErrorMessage(infoLogLength);
        glGetShaderInfoLog(fragmentShader, infoLogLength, nullptr, fragmentShaderErrorMessage.data());
        std::cerr << "Failed to compile fragment shader: " << fragmentShaderErrorMessage.data() << std::endl;
        return -1;
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &status); 
    if (status == GL_FALSE)
    {
        GLint infoLogLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
        std::vector<char> programErrorMessage(infoLogLength);
        glGetProgramInfoLog(program, infoLogLength, nullptr, programErrorMessage.data());
        std::cerr << "Failed to link program: " << programErrorMessage.data() << std::endl;
        return -1;
    }
    glUseProgram(program);
    return program;
}
int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */

    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(800, 600, "Monkey", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

        glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
    {
        glfwTerminate();
        return -1;
    }
    //std::string gltfFilename = "../examples/gltf/01_triangle/export/triangle.gltf";
    //std::string gltfFilename = "../examples/gltf/05_suzanne_uniforms/export/suzanne.gltf";
    std::string gltfFilename = "../examples/gltf/06_shadertoy/export/shadertoy.gltf";
    tinygltf::Model model;
    tinygltf::TinyGLTF loader;
    std:string err , warn;
    
   bool succes = loader.LoadASCIIFromFile(&model, nullptr, nullptr, gltfFilename);
    if(!succes){
        std::cerr << "Faled to load glTF file: " << gltfFilename << std::endl;
        std::cerr << "Error" << err << std::endl;
        std::cerr << "Warning" << warn << std::endl;
        return 1;
    }

    uint32_t gltfAccessorPositionIndex = model.meshes[0].primitives[0].attributes["POSITION"];
    uint32_t gltfAccessorNormalIndex = model.meshes[0].primitives[0].attributes["NORMAL"];
    uint32_t gltfAccessorTexCoordIndex = model.meshes[0].primitives[0].attributes["TEXCOORD_0"];
    uint32_t gltfAccessorIndicesIndex = model.meshes[0].primitives[0].indices;

    uint32_t gltfBufferViewPositionIndex = model.accessors[gltfAccessorPositionIndex].bufferView;
    uint32_t gltfBufferViewNormalIndex = model.accessors[gltfAccessorNormalIndex].bufferView;
    uint32_t gltfBufferViewTexCoordIndex = model.accessors[gltfAccessorTexCoordIndex].bufferView;
    uint32_t gltfBufferViewIdeicesIndex = model.accessors[gltfAccessorIndicesIndex].bufferView;

    uint32_t gltfBufferIndexPosition = model.bufferViews[gltfBufferViewPositionIndex].buffer;
    uint32_t gltfBufferIndexNormal = model.bufferViews[gltfBufferViewNormalIndex].buffer;
    uint32_t gltfBufferIndexTexCoord = model.bufferViews[gltfBufferViewTexCoordIndex].buffer;
    uint32_t gltfBufferIndexIdeices = model.bufferViews[gltfBufferViewIdeicesIndex].buffer;

    unsigned char* gltfBufferDataPositon = model.buffers[gltfBufferIndexPosition].data.data();
    unsigned char* gltfBufferDataNormal = model.buffers[gltfBufferIndexNormal].data.data();
    unsigned char* gltfBufferDataTexCoord = model.buffers[gltfBufferIndexTexCoord].data.data();
    unsigned char* gltfBufferDataIdeices = model.buffers[gltfBufferIndexIdeices].data.data();

    uint32_t gltfPositionByteOffset = model.bufferViews[gltfBufferViewPositionIndex].byteOffset;
    uint32_t gltfNormalByteOffset = model.bufferViews[gltfBufferViewNormalIndex].byteOffset;
    uint32_t gltfTexCoordByteOffset = model.bufferViews[gltfBufferViewTexCoordIndex].byteOffset;
    uint32_t gltfIdeicesByteOffset = model.bufferViews[gltfBufferViewIdeicesIndex].byteOffset;

    uint32_t gltfPostionByteLength = model.bufferViews[gltfBufferViewPositionIndex].byteLength;
    uint32_t gltfNormalByteLength = model.bufferViews[gltfBufferViewNormalIndex].byteLength;
    uint32_t gltfTexCoordByteLength = model.bufferViews[gltfBufferViewTexCoordIndex].byteLength;
    uint32_t gltfIndicesByteLength = model.bufferViews[gltfBufferViewIdeicesIndex].byteLength;
    
    // uint32_t  gltfBufferIndex = model.bufferViews[gltfPositionIndex].buffer;
    // unsigned char* gltfBufferData = model.buffers[gltfBufferIndex].data.data();

    GLint indices = gltfIndicesByteLength / sizeof(GLushort);

     GLuint vertexPositionBuffer = 0;
     glGenBuffers(1, &vertexPositionBuffer);
     glBindBuffer(GL_ARRAY_BUFFER, vertexPositionBuffer);
     glBufferData(GL_ARRAY_BUFFER, gltfPostionByteLength, gltfBufferDataPositon + gltfPositionByteOffset, GL_STATIC_DRAW); 

    GLuint IndexBuffer = 0;
     glGenBuffers(1, &IndexBuffer);
     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);
     glBufferData(GL_ELEMENT_ARRAY_BUFFER, gltfIndicesByteLength, gltfBufferDataIdeices + gltfIdeicesByteOffset, GL_STATIC_DRAW);
    
     GLuint vertexNormalBuffer = 0;
     glGenBuffers(1, &vertexNormalBuffer);
     glBindBuffer(GL_ARRAY_BUFFER, vertexNormalBuffer);
     glBufferData(GL_ARRAY_BUFFER, gltfNormalByteLength, gltfBufferDataNormal + gltfNormalByteOffset, GL_STATIC_DRAW);

     GLuint vertexTexCoordBuffer = 0;
     glGenBuffers(1, &vertexTexCoordBuffer);
     glBindBuffer(GL_ARRAY_BUFFER, vertexTexCoordBuffer);
     glBufferData(GL_ARRAY_BUFFER, gltfTexCoordByteLength, gltfBufferDataTexCoord + gltfTexCoordByteOffset, GL_STATIC_DRAW);
    
    GLuint vertexArrayObject1 = 0;
    glGenVertexArrays(1,&vertexArrayObject1);
    glBindVertexArray(vertexArrayObject1);

    glBindBuffer(GL_ARRAY_BUFFER, vertexPositionBuffer);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0 ,3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, vertexNormalBuffer);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1 ,3, GL_FLOAT, GL_FALSE, 0, 0 );

    glBindBuffer(GL_ARRAY_BUFFER, vertexTexCoordBuffer);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2 ,2, GL_FLOAT, GL_FALSE, 0, 0 );

    std::filesystem::path gltfpath = gltfFilename;
    std::filesystem::path shaderpath = gltfpath.parent_path();

    GLuint program = loadMaterial(model, shaderpath, 0);


    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */

    glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT); 
    
    glBindVertexArray(vertexArrayObject1);  
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);
    
    materialSetPropert("iTime", getcurrentTime());

    materialUpdateProperties(program);
    glDrawElements(GL_TRIANGLES, indices, GL_UNSIGNED_SHORT, 0);
    glBindVertexArray(0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }


    std::cout<<"check 7" << endl;
    glfwTerminate();
    return 0;
}