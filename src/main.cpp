
#include "rendering/display.hpp"
#include "rendering/render-context.hpp"

#include "rendering/render-target.hpp"

#include "rendering/texture.hpp"
#include "rendering/vertex-array.hpp"
#include "rendering/shader.hpp"

#include "rendering/util.hpp"

#include <GLM/glm.hpp>

int main() {
	HDRBitmap bmp;
	bmp.load("./res/Alexs_Apt_Env.hdr");

	float aspectRatio = (float)bmp.getWidth() / (float)bmp.getHeight();

	Display display("Cube Map to Sphere Map", 800, (int32)(800.f / aspectRatio));

	RenderContext context;

	Texture texture(context, bmp, GL_RGB32F);

	Sampler sampler(context, GL_LINEAR, GL_LINEAR);

	IndexedModel quadModel;
	quadModel.allocateElement(2);

	quadModel.addElement2f(0, -1.f, -1.f);
	quadModel.addElement2f(0,  1.f, -1.f);
	quadModel.addElement2f(0, -1.f,  1.f);
	quadModel.addElement2f(0,  1.f,  1.f);

	quadModel.addIndices3i(0, 1, 2);
	quadModel.addIndices3i(1, 3, 2);

	VertexArray quad(context, quadModel, GL_STATIC_DRAW);

	std::stringstream ss;
	Util::resolveFileLinking(ss, "./src/quad-shader.glsl", "#include");
	Shader quadShader(context, ss.str());

	RenderTarget screen(context, display.getWidth(), display.getHeight());

	quadShader.setVector2f("screenSize", glm::vec2(display.getWidth(), display.getHeight()));

	while (!display.isCloseRequested()) {
		screen.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		quadShader.setSampler("img", texture, sampler, 0);
		context.draw(screen, quadShader, quad, GL_TRIANGLES);

		display.pollEvents();
		display.render();
	}

	return 0;
}
