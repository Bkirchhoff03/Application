/*
 * BMPImage.h
 *
 *  Created on: Aug 7, 2023
 *      Author: nebk2
 */

#ifndef GRAPHICS_BMPIMAGE_H_
#define GRAPHICS_BMPIMAGE_H_

#include "Color.h"
#include <vector>
#include <string>
#include <stdint.h>

class BMPImage {
public:
	BMPImage();
	bool load(const std::string &path);

	inline const std::vector<Color>& getPixels() const {
		return mPixels;
	}

	inline uint32_t getWidth() const {
		return mWidth;
	}
	inline uint32_t getHeight() const {
		return mHeight;
	}

private:
	std::vector<Color> mPixels;
	uint32_t mWidth;
	uint32_t mHeight;

};

#endif /* GRAPHICS_BMPIMAGE_H_ */
