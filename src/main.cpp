#include <glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "tiny_gltf.h"

using namespace std;


void loadMaterial(tinygltf::Model& model,std::filesystem::path& gltfDirectory, unsigned int materialId)
{

    // const char* vertexShaderSource =
    //     #include "vertexShader.glsl"
    // ;


    // const char* fragmentShaderSource =
    //     #include "fragmentShader.glsl"
    // ;

    std::filesystem::path vertexShaderPath;
    std::filesystem::path fragmentShadePath;

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
    }


    char* vertexShaderSource = nullptr;
    char* fragmentShaderSource = nullptr;
    std::ifstream vertexShaderFile(vertexShaderPath);
    if(vertexShaderFile.is_open())
    {
        std::string vertexShaderSourceString((std::istreambuf_iterator<char>(vertexShaderFile)), std::istreambuf_iterator<char>());
        vertexShaderSource = new char[vertexShaderSourceString.length() + 1];
        strcpy(vertexShaderSource, vertexShaderSourceString.c_str());
        vertexShaderFile.close();
    }

    std::ifstream fragmentShaderFile(fragmentShadePath);
    if(fragmentShaderFile.is_open())
    {
        std::string fragmentShaderSourceString((std::istreambuf_iterator<char>(fragmentShaderFile)), std::istreambuf_iterator<char>());
        fragmentShaderSource = new char[fragmentShaderSourceString.length() + 1];
        strcpy(fragmentShaderSource, fragmentShaderSourceString.c_str());
        fragmentShaderFile.close();
    }

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader,1 ,&vertexShaderSource, nullptr);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);

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
        return;  
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
        return;  
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
        return;  
    }
        
    glUseProgram(program);
}
int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */

    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);

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
    // std::string gltfFilename = "../examples/gltf/01_triangle/export/triangle.gltf";

    std::string gltfFilename = "../examples/gltf/03_shaders/export/shaders.gltf";
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

    // GLfloat vertices[] = {
    //     0.0f, 0.5f, 0.0f,
    //     0.5f, -0.5f, 0.0f,
    //     -0.5, -0.5f, 0.0f
    
    // }; 
//     GLfloat vertexData[] = {
//     -0.5f, -1.0f, 0.0f, // positions[0]​
//     0.0f, 0.0f, 1.0f, // normals[0]​
//     0.25f, 0.25f, // texture coordinates[0]​
//     0.5f, -0.5f, 0.0f, // positions[1]​
//     0.0f, 0.0f, 1.0f, // normals[1]​
//     0.75f, 0.25f, // texture coordinates[1]​
//     0.0f, 0.5f, 0.0f, // positions[2]​
//     0.0f, 0.0f, 1.0f, // normals[2]​
//     0.5f, 0.75f // texture coordinates[2]​
    

// };

//     GLfloat vertexPositionData[] = {
// -0.5f, -0.5f, 0.0f, // positions[0]​
// 0.5f, -0.5f, 0.0f, // positions[1]​
// 0.0f, 0.5f, 0.0f, // positions[2]​

// };
// GLfloat vertexNormalData[] = {
// 0.0f, 0.0f, 1.0f, // normals[0]​
// 0.0f, 0.0f, 1.0f, // normals[1]​
// 0.0f, 0.0f, 1.0f, // normals[2]​

// };
// GLfloat vertexTexCoordData[] = {
// 0.25f, 0.25f, // texture coordinates[0]​
// 0.75f, 0.25f, // texture coordinates[1]​
// 0.5f, 0.75f // texture coordinates[2]​

// };

// GLfloat vertexData[] = { 
//     -0.5f, -0.5f, 0.0f, //
//     0.5, -0.5, 0.0f, //
//     0.0f, 0.5f, 0.0f, //
//     0.0f, 0.0f, 1.0f, //
//     0.0f, 0.0f, 1.0f, //
//     0.0f, 0.0f, 1.0f, //
//     0.25f, 0.25f, //
//     0.75f, 0.25f, // 
//     0.5, 0.75 //
// };

    uint32_t gltfAccessorPositionIndex = model.meshes[0].primitives[0].attributes["POSITION"];
    uint32_t gltfAccessorNormalIndex = model.meshes[0].primitives[0].attributes["NORMAL"];
    uint32_t gltfAccessorTexCoordIndex = model.meshes[0].primitives[0].attributes["TEXCOPED"];

    uint32_t gltfBufferViewPositionIndex = model.accessors[gltfAccessorPositionIndex].bufferView;
    uint32_t gltfBufferViewNormalIndex = model.accessors[gltfAccessorNormalIndex].bufferView;
    uint32_t gltfBufferViewTexCoordIndex = model.accessors[gltfAccessorTexCoordIndex].bufferView;

    uint32_t gltfBufferIndexPosition = model.bufferViews[gltfBufferViewPositionIndex].buffer;
    uint32_t gltfBufferIndexNormal = model.bufferViews[gltfBufferViewNormalIndex].buffer;
    uint32_t gltfBufferIndexTexCoord = model.bufferViews[gltfBufferViewTexCoordIndex].buffer;

    unsigned char* gltfBufferDataPositon = model.buffers[gltfBufferIndexPosition].data.data();
    unsigned char* gltfBufferDataNormal = model.buffers[gltfBufferIndexNormal].data.data();
    unsigned char* gltfBufferDataTexCoord = model.buffers[gltfBufferIndexTexCoord].data.data();

    uint32_t gltfPositionByteOffset = model.bufferViews[gltfBufferViewPositionIndex].byteOffset;
    uint32_t gltfNormalByteOffset = model.bufferViews[gltfBufferViewNormalIndex].byteOffset;
    uint32_t gltfTexCoordByteOffset = model.bufferViews[gltfBufferViewTexCoordIndex].byteOffset;

    uint32_t gltfPostionByteLength = model.bufferViews[gltfBufferViewPositionIndex].byteLength;
    uint32_t gltfNormalByteLength = model.bufferViews[gltfBufferViewNormalIndex].byteLength;
    uint32_t gltfTexCoordByteLength = model.bufferViews[gltfBufferViewTexCoordIndex].byteLength;

    
    // uint32_t  gltfBufferIndex = model.bufferViews[gltfPositionIndex].buffer;
    // unsigned char* gltfBufferData = model.buffers[gltfBufferIndex].data.data();


    // float* vertices = (float*)gltfBufferData;

    // for(int i = 0; i < 9;i++) 
    // {
    //     std::cout << vertices[i] << std::endl;
    // }


     GLuint vertexPositionBuffer = 0;
     glGenBuffers(1, &vertexPositionBuffer);
     glBindBuffer(GL_ARRAY_BUFFER, vertexPositionBuffer);
     glBufferData(GL_ARRAY_BUFFER, gltfPostionByteLength, gltfBufferDataPositon, GL_STATIC_DRAW);

     GLuint vertexNormalBuffer = 0;
     glGenBuffers(1, &vertexNormalBuffer);
     glBindBuffer(GL_ARRAY_BUFFER, vertexNormalBuffer);
     glBufferData(GL_ARRAY_BUFFER, gltfNormalByteLength, gltfBufferDataNormal, GL_STATIC_DRAW);

     GLuint vertexTexCoordBuffer = 0;
     glGenBuffers(1, &vertexTexCoordBuffer);
     glBindBuffer(GL_ARRAY_BUFFER, vertexTexCoordBuffer);
     glBufferData(GL_ARRAY_BUFFER, gltfTexCoordByteLength, gltfBufferDataTexCoord, GL_STATIC_DRAW);


    
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
    loadMaterial(model, shaderpath, 0);

    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glBindVertexArray(0);



    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */

    glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glBindVertexArray(vertexArrayObject1);
    
    glDrawArrays(GL_TRIANGLES, 0, 3);
    // glBindVertexArray(0);
    // glBindVertexArray(vertexArrayObject2);

    // glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
    // glBindVertexArray(vertexArrayObject2);

    // glDrawArrays(GL_TRIANGLES, 0, 3);
    

        // glBegin(GL_TRIANGLES);
        // glVertex2f(-0.5f, -0.5f);
        // glVertex2f( 0.0f, 0.5f);
        // glVertex2f(0.5f, -0.5f);
        // glEnd();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }


    cout<<"check 7" << endl;
    glfwTerminate();
    return 0;
}