
#if defined(VS_BUILD)

layout (location = 0) in vec2 position;

void main() {
	gl_Position = vec4(position, 0.0, 1.0);
}

#elif defined(FS_BUILD)

layout (location = 0) out vec4 outColor;

uniform vec2 screenSize;

uniform sampler2D img;

void main() {
	const vec2 x = gl_FragCoord.xy / screenSize;

	outColor = vec4(texture2D(img, x).rgb, 1.0);
}

#endif
