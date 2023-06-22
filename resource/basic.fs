#version 330 core

in vec3 color;
in vec2 texCoord;
out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    // FragColor = vec4(color, 1.0);
    // FragColor = texture(texture2, texCoord) * vec4(color, 1.0);
    FragColor = mix(texture(texture1, texCoord), texture(texture2, texCoord), 0.6);
} 