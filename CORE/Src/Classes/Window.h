#pragma once
#include <glad/gl.h>
#include <SDL3/SDL.h>


#include <iostream>



#include "Keyboard.h"
#include "Mouse.h"
#include "Camera.h"
#include "Sound.h"


class Window
{
public:
	Window() = default;
	Window(const char* name, const int width, const int height);
	~Window();


	void Event(Camera& camera,float delta);
	void SwapBuffers();
	void CallBufferSize();
	void Clear();


	inline SDL_Window* GetWindow() { return m_window; }
	inline int GetWidth() { return m_width; }
	inline int GetHeight() { return m_height; }

	inline bool GetRun() { return m_run; }

private:
	SDL_Window* m_window;
	SDL_GLContext m_maincontext;

	bool m_run = true;

	Keyboard m_keys;
	Mouse m_mouse;

	Sound m_sound_effects;

	int m_width;
	int m_height;

};

