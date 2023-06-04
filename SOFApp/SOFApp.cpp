// SOFApp.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "ExampleApp.h"

int main()
{
	ExampleApp *app = new ExampleApp();
	SOF::Window::CreateWindow(1280, 720, "SOF/RenderLib Example", app);
	SOF::Window::SetBackground(glm::vec4(0, 0, 0, 0));
	SOF::Window::Run();

    return 0;
}

