#version 330

in vec4 fragColor;
in vec3 pixelPos;
in vec3 pixelNorm;
in vec2 pixelUV;
// catch splatUV from vertex shader.
in vec2 splatUV;

out vec4 finalColor;
uniform vec3 lightK; // intensities for ambient, diffuse, specular
uniform float alpha; // specular power
uniform mat4 lightPos; // light position, world space
uniform vec3 cameraPos; // camera position, world space
uniform vec4 ambientColor;
uniform mat4 lightColor; // assume diffuse and specular the same
uniform vec4 attenuation; // 1/d_a**2

// textures for red green and blue values on the splat map.
uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex2;
// texture for splatmap
uniform sampler2D splat_map;

void main()
{
	// surface normal.
	vec3 n = normalize(pixelNorm);
    finalColor = vec4(0.0f,0.0f,0.0f,0.0f);

	// get colour values from textures.
	vec4 t0 = texture(splat_map, splatUV).rgba;
	vec4 t1 = texture(tex0, pixelUV).rgba;
	vec4 t2 = texture(tex1, pixelUV).rgba;
	vec4 t3 = texture(tex2, pixelUV).rgba;

	float total =  t0.r + t0.g + t0.b;
	vec4 materialColor = (t1*t0.r + t2*t0.g + t3*t0.b)/total;

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