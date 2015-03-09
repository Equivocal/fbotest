#include "main.h"

int main() {
	nex::FBO *fbo = new nex::FBO;
	fbo->loop();
	delete fbo;
	return 1;
}