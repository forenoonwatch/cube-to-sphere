#include "hdr-bitmap.hpp"

#include "stbi/stb_image.h"
#include "stbi/stb_image_write.h"

#include <cstring>

#define NUM_CHANNELS 3

HDRBitmap::HDRBitmap(int32 width, int32 height)
		: width(width)
		, height(height)
		, data(new float[NUM_CHANNELS * width * height]) {}

HDRBitmap::HDRBitmap(int32 width, int32 height, float* inData)
		: width(width)
		, height(height)
		, data(new float[NUM_CHANNELS * width * height]) {
	std::memcpy(data, inData, calcDataSize());
}

void HDRBitmap::clear() {
	std::memset(data, 0, calcDataSize());
}

bool HDRBitmap::load(const std::string& fileName) {
	int32 texWidth, texHeight, numComponents;

	stbi_set_flip_vertically_on_load(true);

	float* fileData = stbi_loadf(fileName.c_str(), &texWidth,
			&texHeight, &numComponents, 0);

	stbi_set_flip_vertically_on_load(false);

	if (data == nullptr) {
		DEBUG_LOG(LOG_ERROR, "HDR Bitmap Load",
				"Failed to load HDR image file: %s", fileName.c_str());
		return false;
	}

	if (texWidth != width || texHeight != height) {
		width = texWidth;
		height = texHeight;

		delete[] data;
		data = new float[NUM_CHANNELS * width * height];
	}

	std::memcpy(data, fileData, calcDataSize());
	stbi_image_free(fileData);

	return true;
}

bool HDRBitmap::save(const std::string& fileName) {
	return stbi_write_hdr(fileName.c_str(), width, height,
			NUM_CHANNELS, data);
}

HDRBitmap::~HDRBitmap() {
	delete[] data;
}

inline uintptr HDRBitmap::calcDataSize() const {
	return (uintptr)(NUM_CHANNELS * width * height) * sizeof(float);
}
