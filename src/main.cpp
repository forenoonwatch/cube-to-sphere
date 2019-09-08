#include <cstdio>
#include <cstdlib>

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include "rendering/display.hpp"

#include "rendering/render-context.hpp"
#include "rendering/render-target.hpp"

#include "rendering/vertex-array.hpp"
#include "rendering/shader.hpp"

#include "util.hpp"

// initializes an IndexedModel to contain an inverted cube
void initSkyboxCube(IndexedModel&);

// converts a cube map to an equirectangular (sphere) mapped HDR image
void cubeToSphere(RenderContext& context, CubeMap& cubeMap,
		uint32 texWidth, uint32 texHeight, const std::string& fileName);

// converts an equirectangular (sphere) mapped HDR image
// into a set of 6 cube mapped PNGs
void sphereToCube(RenderContext& context, Texture& sphereMap,
		uint32 texLength, const std::string& textureFolder);

int main(int argc, char** argv) {
	if (argc != 6) {
		printf("Illegal number of args: %d\n", argc - 1);

		puts("Cube map to sphere map converter. Enter a source folder containing images");
		puts("of the same file format named top, bottom, front, back, left, and right.");
		puts("Format: CubeToSphere source_folder file_extension destination_file width height");
		puts("Example: CubeToSphere C:/skybox png spheremap.hdr 2400 1200");

		return 0;
	}

	std::string sourceFolder(argv[1]);
	std::string fileExtension(argv[2]);

	uint32 width = std::atoi(argv[4]);
	uint32 height = std::atoi(argv[5]);

	Display display("Cube Map to Sphere Map", 100, 100);
	RenderContext context;

	std::string sides[] = {"right", "left", "top", "bottom", "front", "back"};

	std::vector<std::string> fileNames;
	std::stringstream ss;

	for (uint32 i = 0; i < 6; ++i) {
		ss.str("");
		ss << sourceFolder << "/" << sides[i] << "." << fileExtension;

		fileNames.push_back(ss.str());
	}

	CubeMap skybox(context, &fileNames[0]);

	cubeToSphere(context, skybox, width, height, argv[3]);

	return 0;
}

void cubeToSphere(RenderContext& context, CubeMap& cubeMap,
		uint32 texWidth, uint32 texHeight, const std::string& fileName) {
	Texture buffer(context, texWidth, texHeight, GL_RGBA32F);
	RenderTarget target(context, buffer, GL_COLOR_ATTACHMENT0);

	Sampler sampler(context, GL_LINEAR, GL_LINEAR);

	std::stringstream ss;
	Util::resolveFileLinking(ss, "./src/shaders/cube-to-sphere.glsl", "#include");
	Shader shader(context, ss.str());

	HDRBitmap bmp;

	glViewport(0, 0, texWidth, texHeight);

	shader.setSampler("skybox", cubeMap, sampler, 0);
	context.drawQuad(target, shader);

	bmp.load(buffer);
	bmp.save(fileName);
}

void sphereToCube(RenderContext& context, Texture& sphereMap,
		uint32 texLength, const std::string& textureFolder) {
	Texture buffer(context, texLength, texLength, GL_RGBA);
	RenderTarget target(context, buffer, GL_COLOR_ATTACHMENT0);

	Sampler sampler(context, GL_LINEAR, GL_LINEAR);

	std::stringstream ss;
	Util::resolveFileLinking(ss, "./src/shaders/sphere-skybox.glsl", "#include");
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

	// right
	model.addIndices3i(1, 5, 7);
	model.addIndices3i(7, 3, 1);
}
