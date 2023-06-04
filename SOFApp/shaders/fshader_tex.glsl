#version 330

in vec4 fragColor;
in vec3 pixelPos;
in vec3 pixelNorm;
in vec2 pixelUV;

out vec4 finalColor;
uniform vec3 lightK; // intensities for ambient, diffuse, specular
uniform float alpha; // specular power
uniform mat4 lightPos; // light position, world space
uniform vec3 cameraPos; // camera position, world space
uniform vec4 ambientColor;
uniform mat4 lightColor; // assume diffuse and specular the same
uniform vec4 attenuation; // 1/d_a**2

uniform sampler2D textureSampler;

void main()
{
	vec3 n = normalize(pixelNorm); // surface normal
    finalColor = vec4(0.0f,0.0f,0.0f,0.0f);
    vec4 materialColor = texture(textureSampler, pixelUV);
    for ( int i = 0; i < 4; i++ )
    {
      vec3 l = normalize(vec3(lightPos[i]) - pixelPos); // light vector
      vec3 e = normalize(cameraPos - pixelPos); // eye vector
      vec3 r = reflect(-l,n); // reflected light vector
      float d = length(vec3(lightPos[i]) - pixelPos);
      float fAttenuation = 1.0f/(1.0f + d*d*attenuation[i]);
      float kdiffuse = max(dot(l,n),0) * lightK.y * fAttenuation;
      float kspecular = pow(max(dot(r,e),0.0f),alpha) * lightK.z * fAttenuation;
      finalColor += materialColor*lightColor[i]*kdiffuse +kspecular*lightColor[i];
    }
    float kambient = lightK.x;
    finalColor += materialColor*ambientColor*kambient;
    finalColor.a = 1.0f;
}