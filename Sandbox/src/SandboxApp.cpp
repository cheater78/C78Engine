#include <C78E.h>

class Sandbox : public C78Engine::Application
{
public:
	Sandbox()
	{

	}
	~Sandbox()
	{

	}


};

C78Engine::Application* C78Engine::CreateApplication()
{
	return new Sandbox();
}