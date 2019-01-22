#ifndef __CAMERAMAN_H__
#define __CAMERAMAN_H__

#include "Camera.h"
#include "Window.h"

#include <vector>
#include <memory>

class CameraMan
{
private:
	CameraMan(void);
	~CameraMan(void);
public:
	
	// Static access function
	static CameraMan& Instance()
	{
		static CameraMan refInstance;
		return refInstance;
	}
	
	void update();

	// Specific -----------------------------------------------
	void updateCamera(const char* camName);
	void updateCamera(int index);

	// Create camera
	void createCamera(const Window& window, 
		const char* camName, 
		float fFOV, 
		float fNearPlane, 
		float fFarPlane);

	// Set the active camera
	void setActiveCamera(Camera* newCamera);
	// Get the active camera
	inline Camera* getActiveCamera() const { assert(m_activeCamera != nullptr); return m_activeCamera; }
	// Get camera
	Camera* getCamera(const char* camName) const;
	Camera* getCamera(int index) const;

	CameraMan(CameraMan const&) = delete;
	void operator=(CameraMan const&) = delete;

private:

	Camera* m_activeCamera;
	std::vector<std::unique_ptr<Camera>> m_cameraList;
};

#endif // _CAMERAMAN_H__