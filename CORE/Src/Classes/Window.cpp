#include "Window.h"



Window::Window(const char* name, const int width, const int height)
{
	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		std::cerr << "SDL failed to initalize \n";
	}

	m_window = SDL_CreateWindow(name, width, height,SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED);

	if (!m_window)
	{
		std::cerr << "SDL failed to create window\n";
	}

	m_maincontext = SDL_GL_CreateContext(m_window);
	if (!m_maincontext)
	{
		std::cerr << "OpenGL context could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		SDL_DestroyWindow(m_window);
		SDL_Quit();

	}

	// Load OpenGL functions, gladLoadGL returns the loaded version, 0 on error.
    int version = gladLoadGL(SDL_GL_GetProcAddress);
    if (version == 0)
    {
        std::cout << "Failed to initialize OpenGL context" << std::endl;

    }

    // Successfully loaded OpenGL
    std::cout << "Loaded OpenGL " << GLAD_VERSION_MAJOR(version) << "." << GLAD_VERSION_MINOR(version) << std::endl;


	if (SDL_GL_SetSwapInterval(1) < 0)
	{
		std::cout << "Warning: Unable to set VSync! SDL Error: \n" << SDL_GetError()<< std::endl;
	}



	SDL_SetWindowRelativeMouseMode(m_window, true);

	m_sound_effects.LoadSound("Ping", "ping.wav");
}

Window::~Window()
{
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}



void Window::Event(Camera& camera,float delta)
{
	int mousePosX, mousePosY;
	SDL_Event events;
	while (SDL_PollEvent(&events))
	{
		if (events.type == SDL_EVENT_QUIT)
		{
			m_run = false;
			break;
		}
		
		m_keys.Update();
	
		if (m_keys.KeyPressed("Escape"))
		{
			m_run = false;
			break;
		}
	
		if (m_keys.KeyPressed("W")) 
		{
			camera.MoveBack(delta);			
		}
		if (m_keys.KeyPressed("A")) 
		{
			camera.MoveRight(delta);
		}
		if (m_keys.KeyPressed("S")) 
		{
			camera.MoveForward(delta);
		}
		if (m_keys.KeyPressed("D")) 
		{
			camera.MoveLeft(delta);
		}
	
		if (m_keys.KeyPressed("Z"))
		{			
			camera.MoveDown(delta);
		}
	
		if (m_keys.KeyPressed("SPACE"))
		{
			camera.MoveUp(delta);
		}

		if (m_keys.KeyPressed("P"))
		{
			m_sound_effects.PlaySound("Ping");
		}
		
		m_mouse.HandleEvent(events);
		double dx = m_mouse.GetDX();
		double dy = m_mouse.GetDY();
		if (dx != 0 || dy != 0)
		{
			//camera.ProcessMouseMovement(dx, dy,false);			
		}
		
		

	}





}

void Window::SwapBuffers()
{
	SDL_GL_SwapWindow(m_window);
}

void Window::CallBufferSize()
{
	SDL_GetWindowSize(m_window, &m_width, &m_height);
	glViewport(0, 0, m_width, m_height);
}

void Window::Clear()
{
	glClearColor(0.5, 0.2, 0.3, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
