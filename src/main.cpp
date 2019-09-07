
#include "rendering/display.hpp"
#include "rendering/render-context.hpp"

#include "rendering/render-target.hpp"

#include "rendering/vertex-array.hpp"
#include "rendering/shader.hpp"

#include "rendering/util.hpp"

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include "camera.hpp"

void initQuad(IndexedModel&);
void initSkyboxCube(IndexedModel&);

void sphereToCube(RenderContext& context, Texture& sphereMap,
		uint32 texLength, const std::string& textureFolder);
void cubeToSphere(RenderContext& context, CubeMap& cubeMap,
		uint32 texWidth, uint32 texHeight, const std::string& fileName);

int main() {
	HDRBitmap bmp;
	//bmp.load("./res/Alexs_Apt_2k.hdr");
	bmp.load("./test.hdr");

	//Bitmap pngBMP;
	
	//bmp.save("./test.hdr");

	float aspectRatio = (float)bmp.getWidth() / (float)bmp.getHeight();

	//Display display("Cube Map to Sphere Map", 800, (int32)(800.f / aspectRatio));
	Display display("Cube Map to Sphere Map", 800, 800);

	RenderContext context;

	Texture texture(context, bmp, GL_RGB32F);

	//pngBMP.load(texture);
	//pngBMP.save("./res/test-tex.png");

	//sphereToCube(context, texture, 512, "./res/test-skybox");

	std::string cubeTextures[] = {"./res/skybox/right.jpg",
		"./res/skybox/left.jpg", "./res/skybox/top.jpg",
		"./res/skybox/bottom.jpg", "./res/skybox/front.jpg",
		"./res/skybox/back.jpg"};

	CubeMap skybox(context, cubeTextures);

	Sampler sampler(context, GL_LINEAR, GL_LINEAR);

	IndexedModel quadModel;
	initQuad(quadModel);

	IndexedModel cubeModel;
	initSkyboxCube(cubeModel);

	VertexArray quad(context, quadModel, GL_STATIC_DRAW);
	VertexArray skyboxCube(context, cubeModel, GL_STATIC_DRAW);

	std::stringstream ss;
	Util::resolveFileLinking(ss, "./src/quad-shader.glsl", "#include");
	Shader quadShader(context, ss.str());

	ss.str("");
	Util::resolveFileLinking(ss, "./src/skybox-shader.glsl", "#include");
	Shader skyboxShader(context, ss.str());

	ss.str("");
	Util::resolveFileLinking(ss, "./src/sphere-skybox.glsl", "#include");
	Shader sphereSkyboxShader(context, ss.str());

	RenderTarget screen(context, display.getWidth(), display.getHeight());

	Camera camera(glm::radians(90.f), 1.f, 0.1f, 10.f);

	//quadShader.setVector2f("screenSize", glm::vec2(display.getWidth(), display.getHeight()));

	//camera.rotate(glm::radians(0.f), glm::radians(0.f));
	
	glm::mat4 persp = glm::perspective(glm::radians(90.f), 1.f, 0.1f, 10.f);

	glm::mat4 captureViews[] = {
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};

	int32 counter = 0;
	int32 index = 0;

	glViewport(0, 0, display.getWidth(), display.getHeight());

	while (!display.isCloseRequested()) {
		screen.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//camera.rotate(0.f, 0.005f);
		//camera.update();

		++counter;

		if (counter > 100) {
			counter = 0;
			index = (index + 1) % 6;
		}

		skyboxCube.updateBuffer(1, glm::value_ptr(persp * captureViews[index]),
				sizeof(glm::mat4));

		//skyboxShader.setSampler("skybox", skybox, sampler, 0);
		//context.draw(screen, skyboxShader, skyboxCube, GL_TRIANGLES);

		sphereSkyboxShader.setSampler("skybox", texture, sampler, 0);
		context.draw(screen, sphereSkyboxShader, skyboxCube, GL_TRIANGLES);

		//quadShader.setSampler("img", texture, sampler, 0);
		//context.draw(screen, quadShader, quad, GL_TRIANGLES);

		display.pollEvents();
		display.render();
	}

	return 0;
}

void initQuad(IndexedModel& quadModel) {
	quadModel.allocateElement(2);

	quadModel.addElement2f(0, -1.f, -1.f);
	quadModel.addElement2f(0,  1.f, -1.f);
	quadModel.addElement2f(0, -1.f,  1.f);
	quadModel.addElement2f(0,  1.f,  1.f);

	quadModel.addIndices3i(0, 1, 2);
	quadModel.addIndices3i(1, 3, 2);
}

void initSkyboxCube(IndexedModel& model) {
	model.allocateElement(3); // position
	model.allocateElement(16); // transform

	model.setInstancedElementStartIndex(1);
	
	for (float z = -1.f; z <= 1.f; z += 2.f) {
		for (float y = -1.f; y <= 1.f; y += 2.f) {
			for (float x = -1.f; x <= 1.f; x += 2.f) {
				model.addElement3f(0, x, y, z);
			}
		}
	}

	// back
	model.addIndices3i(0, 1, 2);
	model.addIndices3i(3, 2, 1);

	// front
	model.addIndices3i(6, 5, 4);
	model.addIndices3i(5, 6, 7);

	// bottom
	model.addIndices3i(4, 1, 0);
	model.addIndices3i(1, 4, 5);

	// top
	model.addIndices3i(2, 3, 6);
	model.addIndices3i(7, 6, 3);

	// left
	model.addIndices3i(0, 2, 6);
	model.addIndices3i(6, 4, 0);

	model.addIndices3i(1, 5, 7);
	model.addIndices3i(7, 3, 1);
}

void sphereToCube(RenderContext& context, Texture& sphereMap,
		uint32 texLength, const std::string& textureFolder) {
	Texture buffer(context, texLength, texLength, GL_RGBA);
	RenderTarget target(context, buffer, GL_COLOR_ATTACHMENT0);

	Sampler sampler(context, GL_LINEAR, GL_LINEAR);

	std::stringstream ss;
	Util::resolveFileLinking(ss, "./src/sphere-skybox.glsl", "#include");
	Shader shader(context, ss.str());

	IndexedModel cubeModel;
	initSkyboxCube(cubeModel);

	VertexArray cube(context, cubeModel, GL_STATIC_DRAW);

	Bitmap bmp;

	glm::mat4 persp = glm::perspective(glm::radians(90.f), 1.f, 0.1f, 10.f);

	glm::mat4 captureViews[] = {
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};

	std::string sides[] = {"right", "left", "top", "bottom", "front", "back"};

	shader.setSampler("skybox", sphereMap, sampler, 0);

	glViewport(0, 0, texLength, texLength);

	for (uint32 i = 0; i < 6; ++i) {
		target.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		cube.updateBuffer(1, glm::value_ptr(persp * captureViews[i]), sizeof(glm::mat4));

		context.draw(target, shader, cube, GL_TRIANGLES);

		bmp.load(buffer);
		bmp.save(textureFolder + "/" + sides[i] + ".png");
	}
}
