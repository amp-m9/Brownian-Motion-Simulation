#version 330 core
in vec2 TexCoords;
out vec4 color;
uniform sampler2D image;
uniform vec4 spriteColor;
uniform vec4 colour;
uniform bool rect;

void main()
{
    if(!rect)
        color = spriteColor * texture(image, TexCoords);
    else{
        color = colour;
    }

}
