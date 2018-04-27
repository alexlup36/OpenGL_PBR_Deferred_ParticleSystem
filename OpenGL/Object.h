#ifndef OBJECT_H
#define OBJECT_H

#include <memory>

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>

#include "Model.h"
#include "Transform.h"
#include "Shader.h"
#include "CameraMan.h"

template<class T>
class Object
{
public:
	Object(const std::string& filePath);
	~Object();

	virtual void update(double dt);
	virtual void render(const std::unique_ptr<Shader> &shader);

	inline auto &transform() { return m_transform; }

private:
	Model<T> m_model;
	Transform m_transform;

};

template<class T>
Object<T>::Object(const std::string& filePath)
	: m_model(filePath)
{

}

template<class T>
Object<T>::~Object()
{

}

template<class T>
void Object<T>::update(double dt)
{

}

template<class T>
void Object<T>::render(const std::unique_ptr<Shader> &shader)
{
	// Set uniforms
	glm::mat4 model = m_transform.modelMat();
	shader->set<glm::mat4>(ShaderUniform::ModelMat, model);
	shader->set<glm::mat4>(ShaderUniform::NormalMat, glm::transpose(glm::inverse(model)));
	shader->set<glm::mat4>(ShaderUniform::ViewMat, CameraMan::Instance().getActiveCamera()->viewMatrix());
	shader->set<glm::mat4>(ShaderUniform::ProjMat, CameraMan::Instance().getActiveCamera()->projMatrix());
	shader->set<glm::vec3>(ShaderUniform::ViewPos, CameraMan::Instance().getActiveCamera()->viewPos());

	// Render
	m_model.render();
}

#endif // OBJECT_H