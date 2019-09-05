
#include "rendering/display.hpp"

#include <GL/glew.h>

int main() {
	Display display("My window", 800, 600);

	while (!display.isCloseRequested()) {
		glBegin(GL_TRIANGLES);

		glVertex2f(-1.f, -1.f);
		glVertex2f( 0.f, 1.f);
		glVertex2f( 1.f, -1.f);

		glEnd();

		display.pollEvents();
		display.render();
	}

	return 0;
}
