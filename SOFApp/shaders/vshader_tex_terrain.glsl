#version 330

// placeholder shader

in vec3 vertexPos;
in vec3 vertexNormal;
in vec2 vertexUV;
in vec2 vertexSplatUV;

uniform mat4 model;
uniform mat4 viewProj;

out vec3 pixelPos;
out vec3 pixelNorm;
out vec2 pixelUV;
out vec2 splatUV;

void main()
{
	pixelUV = vertexUV;
	//set splatUV
	splatUV = vertexSplatUV;
	pixelPos = vec3(model * vec4( vertexPos, 1.0f));
	pixelNorm  = vec3(model * vec4( vertexNormal, 0.0f));
	gl_Position = viewProj * vec4( pixelPos, 1.0f);
}