#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <core/common.hpp>

#include <stbi/stb_image.h>
#include <stbi/stb_image_write.h>

#define NUM_INPUTS 3

int main(int argc, char** argv) {
	if (argc != 5) {
		fprintf(stderr, "Invalid number of arguments: %d\n", argc - 1);
		fprintf(stderr, "Usage: %s metallic roughness ambient_occlusion output\n",
				argv[0]);
		return 0;
	}

	int32 widths[NUM_INPUTS];
	int32 heights[NUM_INPUTS];
	int32 bpps[NUM_INPUTS];

	uint8* data = nullptr;
	uint8* cache[NUM_INPUTS];

	for (uint32 i = 0; i < NUM_INPUTS; ++i) {
		data = stbi_load(argv[i + 1], &widths[i],
				&heights[i], &bpps[i], 4);

		if (data != nullptr) {
			printf("Loaded input image: %s %dx%d\n", argv[i + 1],
					widths[i], heights[i]);
		}
		else {
			fprintf(stderr, "Failed to load input image %s\n",
					argv[i + 1]);

			for (uint32 j = 0; j < i; ++j) {
				delete[] cache[i];
			}

			return 1;
		}

		cache[i] = new uint8[4 * widths[i] * heights[i]];
		std::memcpy(cache[i], data, widths[i] * heights[i]
				* 4 * sizeof(uint8));
		
		stbi_image_free(data);
	}

	for (uint32 y = 0; y < heights[0]; ++y) {
		for (uint32 x = 0; x < widths[0]; ++x) {
			const uint32 i = 4 * (y * widths[0] + x);
			
			cache[0][i + 1] = cache[1][i + 1];
			cache[0][i + 2] = cache[2][i + 2];
			cache[0][i + 3] = 255;
		}
	}

	bool res = stbi_write_png(argv[argc - 1], widths[0], heights[0],
			4, cache[0], 4 * widths[0]);

	if (res) {
		printf("Wrote combined material: %s %dx%d\n", argv[argc - 1],
				widths[0], heights[0]);
	}
	else {
		fprintf(stderr, "Failed to write material to: %s\n",
				argv[argc - 1]);
	}

	for (uint32 i = 0; i < NUM_INPUTS; ++i) {
		delete[] cache[i];
	}

	return res ? 0 : 1;
}
