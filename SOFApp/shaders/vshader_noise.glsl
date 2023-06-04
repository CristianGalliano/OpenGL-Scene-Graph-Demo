#version 330

// very simple shader which kust transforms the position

in vec3 vertexPos;
uniform mat4 model;
uniform mat4 viewProj;
out vec3 modelVert;
void main()
{
	//modelVert = vec3(model*vec4(vertexPos,1));
	//gl_Position = viewProj * vec4( modelVert, 1.0f);
	modelVert = vertexPos;
	gl_Position = viewProj * model * vec4( modelVert, 1.0f);
}