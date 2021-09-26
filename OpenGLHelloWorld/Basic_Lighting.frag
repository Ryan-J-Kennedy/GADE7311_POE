#version 330 core
out vec4 FragColour;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 lightPos;
uniform vec3 light2Pos;
uniform vec3 lightColour;
uniform vec3 lightColour2;
uniform vec3 objectColour;

void main()
{
//ambiant
float ambientStrength = 0.1;
vec3 amient = ambientStrength * lightColour;

vec3 result = (amient) * objectColour;
FragColour = vec4(result, 1.0);
}