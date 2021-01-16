#version 330 core

in vec2 UV;
in vec3 Position_worldspace;
in vec3 Normal_cameraspace;
in vec3 EyeDirection_cameraspace;
in vec3 LightDirection_cameraspace;

out vec3 color;

uniform sampler2D myTextureSampler;
uniform sampler2D myTextureSamplerOpacity;
uniform mat4 MV;
uniform vec3 myColor;
uniform vec3 LightPosition_worldspace;
uniform bool useTexture;
uniform bool useDepthTexture;
uniform samplerCube shadowMap;

vec3 offsets[20] = vec3[]
(
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

void main(){
	vec3 LightColor = vec3(1,1,1);
	float LightPower = 10.0f;
	
	vec3 MaterialDiffuseColor;
	if(useTexture) {
		vec4 texColor = texture(myTextureSampler, UV);
		if(texColor.a < 0.1)
			discard;
		MaterialDiffuseColor = texture( myTextureSampler, UV ).rgb;
	} else {
		MaterialDiffuseColor = myColor;
	}
	vec3 MaterialAmbientColor = vec3(0.25,0.25,0.25) * MaterialDiffuseColor;
	vec3 MaterialSpecularColor = vec3(0.3,0.3,0.3);

	float distance = length( LightPosition_worldspace - Position_worldspace );

	vec3 n = normalize( Normal_cameraspace );
	vec3 l = normalize( LightDirection_cameraspace );
	float cosTheta = clamp( dot( n,l ), 0,1 );
	vec3 E = normalize(EyeDirection_cameraspace);
	vec3 R = reflect(-l,n);
	float cosAlpha = clamp( dot( E,R ), 0,1 );

	float visibility=1.0;
	if (useDepthTexture) {
        vec3 fragToLight = Position_worldspace - LightPosition_worldspace;
        float currentDepth = length(fragToLight);
        float shadow = 0.0;
        float bias = 0.05;
        int samples = 20;
        float viewDistance = length(EyeDirection_cameraspace - Position_worldspace);
        float diskRadius = (1.0 + (viewDistance / 25.0f)) / 25.0;
        for(int i = 0; i < samples; ++i)
        {
            float closestDepth = texture(shadowMap, fragToLight + offsets[i] * diskRadius).r;
            closestDepth *= 25.0f;
            if(currentDepth - bias > closestDepth)
                shadow += 1.0;
        }
        shadow /= float(samples);    
        visibility = 1.0 - shadow;
	}

	color = 
		MaterialAmbientColor +
		visibility * MaterialDiffuseColor * LightColor * LightPower * cosTheta / (distance*distance) +
		visibility * MaterialSpecularColor * LightColor * LightPower * pow(cosAlpha,5) / (distance*distance);
}