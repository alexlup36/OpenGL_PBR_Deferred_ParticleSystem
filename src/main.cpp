#include "GLFramework.h"
#include "CameraMan.h"

int main(void)
{
	int windowWidth = 1600;
	int windowHeight = 900;

	// Get manager
	CameraMan &cameraMan = CameraMan::Instance();

	// Create GLFramework instance
	std::unique_ptr<GLFramework> glFramework = std::make_unique<GLFramework>(windowWidth, windowHeight, cameraMan);
	// Initialize GLFramework
	bool enableMultisampling = true;
	bool enableSRGBFbSupport = true;
	if (glFramework->initialize("GLFramework", enableMultisampling, enableSRGBFbSupport) == false)
		return -1;
	
	// Main loop
	do
	{
		static double previousTime = glfwGetTime();
		double currentTime = glfwGetTime();
		double dt = currentTime - previousTime;

		glFramework->update(dt);
		glFramework->draw(dt);

		previousTime = currentTime;
	} 
	while (!glfwWindowShouldClose(glFramework->window()));
	
	return 0;
}