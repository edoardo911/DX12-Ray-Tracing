#include "utils/header.h"

#include "rendering/Window.h"

using namespace RT;

class Application: public Window
{
public:
	Application(HINSTANCE instance);
	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;
	~Application();

	bool init() override;

	void onResize() override;
	void update() override;
	void draw() override;
};

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
	try
	{
		Application app{ hInstance };
		if(!app.init())
			return EXIT_FAILURE;
		return app.run();
	}
	catch(std::exception e)
	{
		WCHAR buffer[512];
		MultiByteToWideChar(CP_ACP, 0, e.what(), -1, buffer, 512);

		MessageBox(nullptr, buffer, L"Generic Exception", MB_OK);
		return EXIT_FAILURE;
	}

	return 0;
}

Application::Application(HINSTANCE instance): Window(instance) {}

Application::~Application() {}

bool Application::init()
{
	if(!Window::init())
		return false;
	return true;
}

void Application::onResize() {}
void Application::update() {}
void Application::draw() {}