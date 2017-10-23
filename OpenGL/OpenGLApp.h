#ifndef OPENGLAPP_H
#define OPENGLAPP_H

class OpenGLApp
{
public:
	OpenGLApp();
	~OpenGLApp();

	void Run();

protected:
	virtual void Update(float dt);
	virtual void Draw(float dt);

	virtual int Initialize();
	virtual void SetupScene();
};

#endif // OPENGLAPP_H