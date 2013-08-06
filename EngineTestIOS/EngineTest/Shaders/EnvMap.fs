uniform sampler2D my2DMap;
uniform samplerCube myCubeMap;

uniform bool myCubeReflection;

varying mediump vec3  varReflectDir;

void main()
{
	// select whether to use cube map reflection or 2d reflection	
	if(myCubeReflection)
	{
		gl_FragColor = textureCube(myCubeMap, varReflectDir);
	}
	else 
	{
		gl_FragColor = texture2D(my2DMap, varReflectDir.xy * 0.5 + 0.5);
	}
}