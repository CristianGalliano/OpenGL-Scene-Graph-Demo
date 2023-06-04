#version 330

in vec3 vertexPos;

uniform vec3 cameraPos;
uniform mat4 viewProj;

out vec3 texCoord;

void main()
{
	texCoord = normalize(vertexPos);
	vec3 shifted = cameraPos + vertexPos;
	gl_Position = viewProj * vec4(shifted, 1);
}