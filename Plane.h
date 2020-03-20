#pragma once
#include "model.h"

namespace Models {
	class Plane :
		public Model
	{
	public:
		Plane();
		void drawSolid(bool smooth = true) override;
	};

	extern Plane plane;
}


