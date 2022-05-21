#version 330 core
uniform vec3 colour;
uniform bool colourSet;
out vec4 color;
void main()
{
    color = vec4(colour,1.0);
}
//    color = vec4(spriteColor, 1.0); //* texture(image, TexCoords);
//    float distance = length(pixCoords);
//    if(distance>0.5f){
//        //        color = vec4(1.0,0,0, 1.0);
//        discard;
