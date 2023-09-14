/*
 * Shape.cpp
 *
 *  Created on: Jul 3, 2023
 *      Author: nebk2
 */

#include "Shape.h"

void Shape::moveBy(const Vec2D& deltaOffset){
	for(Vec2D& point : mPoints){
		point = point + deltaOffset;
	}
}


