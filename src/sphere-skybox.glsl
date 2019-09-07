
#if defined(VS_BUILD)

out vec3 texCoord0;

layout (location = 0) in vec3 position;
layout (location = 1) in mat4 transform;

void main() {
	const vec4 pos = transform * vec4(position, 1.0);
	
	gl_Position = pos.xyww;
	texCoord0 = position;
}

#elif defined(FS_BUILD)

in vec3 texCoord0;

uniform sampler2D skybox;

layout (location = 0) out vec4 outColor;

const vec2 invAtan = vec2(0.1591, 0.3183);

vec2 calcSphericalUV(vec3 v) {
	const vec2 uv = vec2(atan(v.z, v.x), asin(v.y));

	return fma(uv, invAtan, vec2(0.5));
}

void main() {
	const vec3 skyColor = texture2D(skybox, calcSphericalUV(texCoord0)).rgb;

	outColor = vec4(skyColor, 1.0);
}

#endif
