#include "CameraMan.h"

#include <iostream>

CameraMan::CameraMan(void)
	: m_activeCamera(nullptr) { }
CameraMan::~CameraMan(void) { }

// Create camera
void CameraMan::createCamera(const Window& window, 
	const char* camName, 
	float fFOV, 
	float fNearPlane, 
	float fFarPlane)
{
	std::unique_ptr<Camera> newCamera = std::make_unique<Camera>(window, camName, fFOV, fNearPlane, fFarPlane);

	// Set the projection matrix for the camera
	newCamera->setPerspectiveProjection(window.windowWidth, window.windowHeight, fFOV, fNearPlane, fFarPlane);
	newCamera->setOrthographicsProjection(-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 5.0f);

	// If no active camera set it to the new created camera
	if (!m_activeCamera)
		m_activeCamera = newCamera.get();

	m_cameraList.push_back(std::move(newCamera));
}

// Set the active camera
void CameraMan::setActiveCamera(Camera* newCamera)
{
	assert(newCamera != nullptr);
	m_activeCamera = newCamera;
}

// Get camera
Camera *CameraMan::getCamera(const char* camName) const
{
	// Iterator for the list of cameras
	auto camIterator = m_cameraList.begin();

	while (camIterator != m_cameraList.end())
	{
		if (strcmp((*camIterator)->cameraName(), camName) == 0)
			return (*camIterator).get();
		camIterator++;
	}

	std::cout << "Camera " << camName << "not found." << std::endl;
	return nullptr;
}

Camera *CameraMan::getCamera(int index) const
{
	return m_cameraList[index].get();
}

// Update the camera
void CameraMan::update()
{
	if (m_activeCamera)
		m_activeCamera->updateView();
}

void 
CameraMan::updateCamera(const char* camName)
{
	Camera *temp = getCamera(camName);
	if (temp != nullptr)
		temp->updateView();
}

void 
CameraMan::updateCamera(int index)
{
	Camera *temp = getCamera(index);
	if (temp != nullptr)
		temp->updateView();
}
