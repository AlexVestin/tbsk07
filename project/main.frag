#version 330
uniform sampler2D texUnit;
uniform vec3  lightSourcesDirPosArr[4];
uniform vec3  lightSourcesColorArr[4];
uniform bool  isDirectional[4];
uniform float specularExponent;
uniform vec3  camPos;
uniform int   lightSourcesNo = 1;
uniform int   shadingMode;

in vec4 col;
in vec2 texCoord;
in vec3 transNormal;
in vec3 fragPosition;
out vec4 out_Color;

void main(void)
{ 
	vec4 particleCol = texture(texUnit, gl_PointCoord);
	out_Color = particleCol;//vec4(1.0, 0.0, 0.0, 1.0);
	// We normalize the normal vector (just in case).
	vec3 norm		  = normalize(transNormal);
	// We have some ambient light in order to simulate general bouncing light
	vec3  amb;	 
	// We have strength factors for our different types of light.
	float specStr = 1.0, ambStr = 0.1, diffStr = 0.5;
	// Then we have some variables that is used during the light calculations.
	vec3  lightDir, viewDir, reflectDir, specular, diffuse;
	float spec, diff;
	vec4 result;

	for(int i = 0; i < lightSourcesNo; i++) {	
	
		// We get the ambient light;
		amb	= ambStr * lightSourcesColorArr[i];

		// We check if the light is directional or not, and calculate the vector accordingly.
		if(isDirectional[i]) {
			lightDir = normalize(lightSourcesDirPosArr[i]);  
		}
		else {
			lightDir = normalize(lightSourcesDirPosArr[i] - fragPosition);
		}

		// Flat shading
		// For a flat shaded surface, we're only interested in the angle between the face normal and the light source.
		if(shadingMode == 0)
		{
			// Instead of using the polygon corners, we get the face normal by calculating two tangents to the surface.
			// We're not using transNormal as it's interpolated between the vertices.
			// Code gathered from: https://gamedev.stackexchange.com/questions/154854/how-do-i-implement-flat-shading-in-glsl
			vec3 xTangent   = dFdx( fragPosition );
			vec3 yTangent   = dFdy( fragPosition );
			vec3 faceNormal = normalize( cross( xTangent, yTangent ) );

			reflectDir = max(dot(lightDir, faceNormal), 0.0) * lightSourcesColorArr[i];
			result	   = result + vec4(amb + reflectDir, 1.0);
			continue;
		}

		// We calculate the diffuse light, which is the dot product between the previous vector and the normal.
		// As we don't want any negative value, we have 0.0 as the lowest possible value.
		diff	= max(dot(norm, lightDir), 0.0);
		diffuse = diffStr * diff * lightSourcesColorArr[i];

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
		specular = specStr * spec * lightSourcesColorArr[i];  

		// Lastly, we combine the ambient, diffuse and specular light and add it to the result.
		result = result + vec4((diffuse + amb + specular), 1.0);
	}
	
	// Before we're done, we multiply the light with the texture.
	//vec4 tex  = texture(texUnit, texCoord);
	//out_Color = result * tex;
	if(length(col) > 0) {
		
	} else {
		out_Color =  particleCol;
	}

	out_Color =  particleCol;

	
	
}
