#version 150
 
in vec3 Colour;
out vec4 outputF;
void main()
{   
    outputF = vec4(Colour,1.0);
}