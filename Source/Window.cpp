#include "Window.h"
#include "Log.h"
#include "Engine.h"

Window::Window() : Module()
{
	window = nullptr;
	context = nullptr;
	name = "window";
}

// Destructor
Window::~Window()
{
}

// Called before render is available
bool Window::Awake()
{
	LOG("Init SDL window & surface");
	bool ret = true;

	if (SDL_Init(SDL_INIT_VIDEO) != true)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);//Use 4
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);//Use 6

		// Create window
		Uint32 flags = 0;
		bool fullscreen = false;
		bool borderless = false;
		bool resizable = false;
		bool fullscreen_window = false;

		// TODO Get the values from the config file
		width = 1280;
		height = 720;
		scale = 1;

		if (fullscreen == true)        flags |= SDL_WINDOW_FULLSCREEN;
		if (borderless == true)        flags |= SDL_WINDOW_BORDERLESS;
		if (resizable == true)         flags |= SDL_WINDOW_RESIZABLE;

		// SDL3: SDL_CreateWindow(title, w, h, flags). Set position separately.
		window = SDL_CreateWindow("Motor test", width, height, SDL_WINDOW_OPENGL);

		if (window == nullptr)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			context = SDL_GL_CreateContext(window);

			if (context == nullptr) {
				LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
				ret = false;
			}
			else {

				SDL_GL_MakeCurrent(window, context);

				SDL_GL_SetSwapInterval(1); 

				if (fullscreen_window == true)
				{
					SDL_SetWindowFullscreenMode(window, nullptr); // use desktop resolution
					SDL_SetWindowFullscreen(window, true);
				}

				SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
				SDL_ShowWindow(window);
			}
		}
	}

	return ret;
}

// Called before quitting
bool Window::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	// Destroy window
	if (context != nullptr)
	{
		SDL_GL_DestroyContext(context);
		context = nullptr;
	}

	if (window != nullptr)
	{
		SDL_DestroyWindow(window);
		window = nullptr;
	}

	// Quit SDL subsystems
	SDL_Quit();
	return true;
}

// Set new window title
void Window::SetTitle(const char* new_title)
{
	//title.create(new_title);
	SDL_SetWindowTitle(window, new_title);
}

void Window::GetWindowSize(int& width, int& height) const
{
	width = this->width;
	height = this->height;
}

int Window::GetScale() const
{
	return scale;
}

SDL_Window* Window::GetWindow() const
{
	return window;
}

void Window::SwapBuffers()
{
	SDL_GL_SwapWindow(window);
}