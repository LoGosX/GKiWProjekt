#include "Plane.h"


namespace Models {
	Plane plane;

	Plane::Plane()
	{
		vertices = new float[6 * 4]{
			-1.f, -1.f, 0.f, 1.f,
				1.f, -1.f, 0.f, 1.f,
				-1.f, 1.f, 0.f, 1.f,
				-1.f, 1.f, 0.f, 1.f,
				1.f, -1.f, 0.f, 1.f,
				1.f, 1.f, 0.f, 1.f,
		};
		vertexCount = 6;
	}

	void Plane::drawSolid(bool smooth)
	{
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, false, 0, vertices);
		glDrawArrays(GL_TRIANGLES, 0, vertexCount);
		glDisableVertexAttribArray(0);
	}
}


