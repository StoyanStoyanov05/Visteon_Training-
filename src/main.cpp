#include <glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "tiny_gltf.h"

using namespace std;
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
    std::string gltfFilename = "../examples/gltf/01_triangle/export/triangle.gltf";

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

    GLfloat vertexPositionData[] = {
-0.5f, -0.5f, 0.0f, // positions[0]​
0.5f, -0.5f, 0.0f, // positions[1]​
0.0f, 0.5f, 0.0f, // positions[2]​

};
GLfloat vertexNormalData[] = {
0.0f, 0.0f, 1.0f, // normals[0]​
0.0f, 0.0f, 1.0f, // normals[1]​
0.0f, 0.0f, 1.0f, // normals[2]​

};
GLfloat vertexTexCoordData[] = {
0.25f, 0.25f, // texture coordinates[0]​
0.75f, 0.25f, // texture coordinates[1]​
0.5f, 0.75f // texture coordinates[2]​

};

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

//     uint32_t gltfPositionIndex = model.meshes[0].primitives[0].attributes["POSITION"];
//     uint32_t  gltfBufferIndex = model.bufferViews[gltfPositionIndex].buffer;
//    // size_t gltfBufferData = model.buffers[gltfBufferIndex].data.size();
//     unsigned char* gltfBufferData = model.buffers[gltfBufferIndex].data.data();

    // float* vertices = (float*)gltfBufferData;

    // for(int i = 0; i < 9;i++) 
    // {
    //     std::cout << vertices[i] << std::endl;
    // }


     GLuint vertexPositionBuffer = 0;
     glGenBuffers(1, &vertexPositionBuffer);
     glBindBuffer(GL_ARRAY_BUFFER, vertexPositionBuffer);
     glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositionData), vertexPositionData, GL_STATIC_DRAW);

     GLuint vertexNormalBuffer = 0;
     glGenBuffers(1, &vertexNormalBuffer);
     glBindBuffer(GL_ARRAY_BUFFER, vertexNormalBuffer);
     glBufferData(GL_ARRAY_BUFFER, sizeof(vertexNormalData), vertexNormalData, GL_STATIC_DRAW);

     GLuint vertexTexCoordBuffer = 0;
     glGenBuffers(1, &vertexTexCoordBuffer);
     glBindBuffer(GL_ARRAY_BUFFER, vertexTexCoordBuffer);
     glBufferData(GL_ARRAY_BUFFER, sizeof(vertexTexCoordData), vertexTexCoordData, GL_STATIC_DRAW);

    // glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ARRAY_BUFFER, vertexPositionBuffer);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2 ,3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

    glBindBuffer(GL_ARRAY_BUFFER, vertexNormalBuffer);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1 ,3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0 );

    glBindBuffer(GL_ARRAY_BUFFER, vertexTexCoordBuffer);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0 ,2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0 );
    
    GLuint vertexArrayObject = 0;
    glGenVertexArrays(1,&vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    glBindBuffer(GL_ARRAY_BUFFER, vertexPositionBuffer);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2 ,3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, vertexNormalBuffer);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1 ,3, GL_FLOAT, GL_FALSE, 0, 0 );

    glBindBuffer(GL_ARRAY_BUFFER, vertexTexCoordBuffer);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0 ,2, GL_FLOAT, GL_FALSE, 0, 0 );

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */

    glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(vertexArrayObject);

    glDrawArrays(GL_TRIANGLES, 0, 3);

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