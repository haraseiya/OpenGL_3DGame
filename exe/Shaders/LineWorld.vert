#version 330

// Uniforms for world transform and view-proj
uniform mat4 uWorld;
uniform mat4 uViewProj;

// Attribute 0 is position.
layout(location = 0) in vec3 inPosition;

void main()
{
	// Convert position to homogeneous coordinates
	vec4 pos = vec4(inPosition, 1.0);

	// Transform to clip space
	gl_Position = pos * uWorld * uViewProj;

}
