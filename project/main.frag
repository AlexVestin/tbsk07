#version 150

uniform sampler2D texUnit;

uniform vec3  lightSourcesDirPosArr[4];
uniform vec3  lightSourcesColorArr[4];
uniform bool  isDirectional[4];
uniform float specularExponent;
uniform vec3  camPos;
uniform int   lightSourcesNo = 1;
uniform int   shadingMode;

flat in vec3 flatNormal;

in vec2 texCoord;
in vec3 transNormal;
in vec3 fragPosition;
out vec4 out_Color;

void main(void)
{ 
	// We normalize the normal vector (just in case).
	vec3 norm		  = normalize(transNormal);
	// We have some ambient light in order to simulate general bouncing light
	float ambStr	  = 0.1;
	vec3  amb;	 
	// We have a strength factor to our specular shine.
	float specularStrength = 1.0;
	// Then we have some variables that is used during the light calculations.
	vec3  lightDir, viewDir, reflectDir, specular, diffuse;
	float spec, diff;
	vec4 result;

	for(int i = 0; i < lightSourcesNo; i++) {	
	
		// We get the ambient strength;
		amb	= ambStr * lightSourcesColorArr[i];

		// We check if the light is directional or not, and calculate the vector accordingly.
		if(isDirectional[i]) {
			lightDir = normalize(lightSourcesDirPosArr[i]);  
		}
		else {
			lightDir = normalize(lightSourcesDirPosArr[i] - fragPosition);
		}

		// Flat shading
		// For a flat shaded surface, we're only interested in the angle between the normal and the light source.
		if(shadingMode == 0)
		{
			reflectDir = max(dot(lightDir, normalize(flatNormal)), 0.0) * lightSourcesColorArr[i];
			result	   = result + vec4(amb + reflectDir, 1.0);
			continue;
		}

		// We calculate the diffuse light, which is the dot product between the previous vector and the normal.
		// As we don't want any negative value, we have 0.0 as the lowest possible value.
		diff	= max(dot(norm, lightDir), 0.0);
		diffuse = diff * lightSourcesColorArr[i];

		// Lambert shading.
		// For a Lambertian surface, we assume that the surface scatters the light evenly.
		if(shadingMode == 1)
		{
			result = result + vec4((diffuse + amb), 1.0);
			continue;
		}
			
		// Following is the Phong shading, which adds a specular component.
		// We calculate the vectors needed for the specular light (view- and reflection direction). 
		viewDir	   = normalize(camPos - fragPosition);
		reflectDir = reflect(-lightDir, norm); // LightDir is inverted as the vector must point away from the surface.

		// After that, we calculate the specular component.
		spec	 = pow(max(dot(viewDir, reflectDir), 0.0), specularExponent);
		specular = specularStrength * spec * lightSourcesColorArr[i];  

		// Lastly, we combine the ambient, diffuse and specular light and add it to the result.
		result = result + vec4((diffuse + amb + specular), 1.0);
	}
	
	// Before we're done, we multiply the light with the texture.
	//vec4 tex  = texture(texUnit, texCoord);
	//out_Color = result * tex;

	out_Color = result;
}