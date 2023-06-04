#version 330

// perlin noise shader 

out vec4 finalColor;
in vec3 modelVert;
uniform sampler2D noiseSampler;
uniform float iNoiseScale;

int perm( int i )
{
	// get the permutation from the permutation texture
	vec4 col = texture( noiseSampler, vec2(i*(1.0f/256.0f), 0.25f) );
	return int(col.x*256);
}

vec3 gradient( int i, int j, int k )
{
	int igrad = perm( perm ( perm (i) + j ) + k );
	// look up the gradients part of the noise texture
	vec4 col = texture( noiseSampler, vec2( igrad*(1.0f/256.0f), 0.75f ));
	return (vec3(-0.5f, -0.5f, -0.5f) + vec3(col)) * 2.0f;
}

float smoothstep( float f0, float f1, float x )
{
	x = max(x, 0.0f);
	x = min(x, 1.0f);
	x = 3*x*x - 2*x*x*x;
	return f0*(1.0f-x) + f1*x;
}

vec3 positionVector( int i, int j, int k, vec3 pos )
{
	return (pos-vec3(i,j,k));
}

float PerlinNoise( vec3 pos, float iScale )
{
	float noiseVals[8];
	pos = (pos + vec3(1000,1000,1000))*iScale; // offset of 1000 is to make sure it is in the positive quadrant
	// get the indices of the cube this vert is in
	int i = int( pos.x );
	int j = int( pos.y );
	int k = int( pos.z );
	// get the noise values at the vertices of the cube
	noiseVals[0] = dot( gradient(i,j,k),       positionVector(i,j,k,pos) );
	noiseVals[1] = dot( gradient(i+1,j,k),     positionVector(i+1,j,k,pos) );
	noiseVals[2] = dot( gradient(i+1,j,k+1),   positionVector(i+1,j,k+1,pos) );
	noiseVals[3] = dot( gradient(i,j,k+1),     positionVector(i,j,k+1,pos) );
	noiseVals[4] = dot( gradient(i,j+1,k),     positionVector(i,j+1,k,pos) );
	noiseVals[5] = dot( gradient(i+1,j+1,k),   positionVector(i+1,j+1,k,pos) );
	noiseVals[6] = dot( gradient(i+1,j+1,k+1), positionVector(i+1,j+1,k+1,pos) );
	noiseVals[7] = dot( gradient(i,j+1,k+1),   positionVector(i,j+1,k+1,pos) );
	// now do the interpolation

	// interpolation parameters tx, ty, tz
	vec3 t = pos - vec3(i,j,k);

	float ft, fb;
	float fy0, fy1;
	// bottom xz plane
	ft = smoothstep( noiseVals[3], noiseVals[2], t.x);
	fb = smoothstep( noiseVals[0], noiseVals[1], t.x);
	fy0 = smoothstep( fb, ft, t.z); 
	// top xz plane
	ft = smoothstep( noiseVals[7], noiseVals[6], t.x);
	fb = smoothstep( noiseVals[4], noiseVals[5], t.x);
	fy1 = smoothstep( fb, ft, t.z); 
	// y interpolation
	return smoothstep(fy0, fy1, t.y);
}

float ScaledNoise( vec3 pos, float iScale )
{
	return (1.0f + PerlinNoise( pos, iScale))*0.5f;
}

float FractalNoise(vec3 pos, float iScale)
{
	float total = 0.0f;
	float amplitude = 1.0f;
	float scale = iScale;

	for(int i = 0; i < 8; i++ )
	{
		total += PerlinNoise(pos, scale) * amplitude;
		amplitude *= 0.7f;
		scale *= 2.0f;
	}
	return (1.0f + total) * 0.5f;
}

float Turbulence(vec3 pos, float iScale)
{
	float total = 0.0f;
	float amplitude = 1.0f;
	float scale = iScale;

	for(int i = 0; i < 8; i++ )
	{
		total += PerlinNoise(pos, scale) * amplitude;
		amplitude *= 0.7f;
		scale *= 2.0f;
	}
	return abs(total);
}

void main()
{
	float noise = ScaledNoise( modelVert, iNoiseScale );
	float x = (1.0f+ sin(modelVert.y*3 + noise*3)) * 0.5f;
	//finalColor = vec4(0.2f, 0.2f, 0.3f, 1.0f) * x + vec4(0.6f, 0.65f, 0.55f, 1.0f)*(1.0f-x);
	finalColor = vec4(x, x, x,1);
}
