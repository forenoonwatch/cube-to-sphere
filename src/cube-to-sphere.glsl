#define M_PI 3.1415926535897932384626433832795

#if defined(VS_BUILD)

out vec2 ndc;

layout (location = 0) in vec2 position;

void main() {
	gl_Position = vec4(position, 0.0, 1.0);
	ndc = position;
}

#elif defined(FS_BUILD)

uniform samplerCube skybox;

in vec2 ndc;

layout (location = 0) out vec4 outColor;

void main() {
	const float theta = ndc.x * M_PI;
	const float phi = ndc.y * (0.5 * M_PI);

	const float st = sin(theta);
	const float ct = cos(theta);

	const float sp = sin(phi);
	const float cp = cos(phi);

	const vec3 sampVec = vec3(cp * ct, sp, cp * st);

	outColor = vec4(texture(skybox, sampVec).rgb, 1.0);
}

#endif
