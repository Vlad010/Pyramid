#version 150
 
uniform mat4 viewMatrix, projMatrix, modelMatrix;
 
in vec4 position;
in vec3 colour;
out vec3 Colour;
 
void main()
{
    Colour = colour;
    gl_Position = projMatrix * viewMatrix * modelMatrix * position ;
}