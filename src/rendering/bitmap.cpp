#include "bitmap.hpp"

#include "util.hpp"

#include "stbi/stb_image.h"
#include "stbi/stb_image_write.h"

#include <cstring>

#define NUM_CHANNELS 4

Bitmap::Bitmap(int32 width, int32 height)
		: width(width)
		, height(height)
		, pixels(new int32[width * height]) {}

Bitmap::Bitmap(int32 width, int32 height, int32* inPixels)
		: width(width)
		, height(height)
		, pixels(new int32[width * height]) {
	std::memcpy(pixels, inPixels, calcPixelsSize());
}

void Bitmap::clear() {
	std::memset(pixels, 0, calcPixelsSize());
}

bool Bitmap::load(const std::string& fileName) {
	int32 texWidth, texHeight, bytesPerPixel;

	uint8* data = stbi_load(fileName.c_str(), &texWidth,
			&texHeight, &bytesPerPixel, 4);

	if (data == nullptr) {
		DEBUG_LOG(LOG_ERROR, "Bitmap Load",
				"Failed to load image file: %s", fileName.c_str());
		return false;
	}

	if (texWidth != width || texHeight != height) {
		width = texWidth;
		height = texHeight;

		delete[] pixels;
		pixels = new int32[width * height];
	}

	std::memcpy(pixels, data, calcPixelsSize());
	stbi_image_free(data);

	return true;
}

bool Bitmap::save(const std::string& fileName) {
	std::string ext = Util::getFileExtension(fileName);

	if (ext.compare("png") != 0) {
		DEBUG_LOG(LOG_ERROR, "Bitmap Save",
				"Failed to save image file %s: %s is not a supported extension",
				fileName.c_str(), ext.c_str());
		return false;
	}

	bool res = stbi_write_png(fileName.c_str(), width, height,
			NUM_CHANNELS, pixels, sizeof(int32));

	if (!res) {
		DEBUG_LOG(LOG_ERROR, "Bitmap Save",
				"Failed to save image file: %s", fileName.c_str());
		return false;
	}

	return true;
}

Bitmap::~Bitmap() {
	delete[] pixels;
}

inline uintptr Bitmap::calcPixelsSize() const {
	return (uintptr)(width * height) * sizeof(int32);
}
