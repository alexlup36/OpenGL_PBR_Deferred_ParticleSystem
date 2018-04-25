#include "GLFramework.h"
#include "CameraMan.h"

int main(void)
{
	int windowWidth = 1600;
	int windowHeight = 900;

	/*const int val1 = 0x0000001u;
	const int val2 = 0x0000002u;
	const int val3 = 0x000000Au;
	std::cout << val1 << std::endl;
	std::cout << val2 << std::endl;
	std::cout << val3 << std::endl;*/

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