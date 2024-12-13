#ifndef RENDERING_ENGINE
#define RENDERING_ENGINE

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"
#include <iostream>
#include <cstdlib>
#include <SDL3/SDL.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL3/SDL_opengles2.h>
#else
#include <SDL3/SDL_opengl.h>
#endif

#define RENDER_DEBUG

class RenderingEngine {
public:
	static void setup();
	static void stop();

	static void preRender();
	static void render();

	static void setupWindow();
	static void setupImGui();

	static void stopWindow();
	static void stopImGui();

	static SDL_Window* getWindow();
	static ImGuiIO* getIO();
	static bool isRunning();

	static ImVec4& getBackgroundColor();
	static void setBackgroundColor(ImVec4 newbg);

	static bool _windowOpen;
	static ImVec4 _backgroundColor;

	static SDL_Window*		_window;
	static SDL_GLContext	_glContext;

	static ImGuiIO*			_io;

	static void debugBound(ImVec4 box);
};

#endif
