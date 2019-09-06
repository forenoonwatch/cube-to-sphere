#pragma once

#include "common.hpp"

#include <string>

class HDRBitmap {
	public:
		HDRBitmap(int32 width = 1, int32 height = 1);
		HDRBitmap(int32 width, int32 height, float* data);
		
		void clear();

		bool load(const std::string& fileName);

		inline int32 getWidth() const { return width; }
		inline int32 getHeight() const { return height; }

		inline float* getData() { return data; }
		inline const float* getData() const { return data; }

		~HDRBitmap();
	private:
		NULL_COPY_AND_ASSIGN(HDRBitmap);

		int32 width;
		int32 height;
		
		float* data;

		uintptr calcDataSize() const;
};

