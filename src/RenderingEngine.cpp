#include "RenderingEngine.h"

SDL_Window*		RenderingEngine::_window;
SDL_GLContext	RenderingEngine::_glContext;
ImGuiIO*		RenderingEngine::_io;
bool			RenderingEngine::_windowOpen;
ImVec4			RenderingEngine::_backgroundColor;
TTF_Font* 		RenderingEngine::_basefont;

void RenderingEngine::setup() {
	RenderingEngine::setupWindow();
	RenderingEngine::setupImGui();
	RenderingEngine::setupFont();
}
void RenderingEngine::stop() {
	RenderingEngine::stopImGui();
	RenderingEngine::stopWindow();
}
void RenderingEngine::preRender() {
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		ImGui_ImplSDL3_ProcessEvent(&event);
		if (event.type == SDL_EVENT_QUIT)
			RenderingEngine::_windowOpen = false;
		if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(RenderingEngine::getWindow()))
			RenderingEngine::_windowOpen = false;
	}
	/* if (SDL_GetWindowFlags(RenderingEngine::getWindow()) & SDL_WINDOW_MINIMIZED)
	{
		SDL_Delay(10);
		continue;
	} */
	// Clear the screen
    ImVec4 clear_color = RenderingEngine::_backgroundColor;
	glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers

    // Enable blending for transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Set up 2D orthographic projection
	glViewport(0, 0, (int)RenderingEngine::_io->DisplaySize.x, (int)RenderingEngine::_io->DisplaySize.y);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, (int)RenderingEngine::_io->DisplaySize.x, (int)RenderingEngine::_io->DisplaySize.y, 0, -1, 1); // Match your window dimensions
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();
}
void RenderingEngine::render() {
	// Rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// Flush OpenGL commands
    glFlush();

	SDL_GL_SwapWindow(RenderingEngine::_window);
}
void RenderingEngine::setupWindow() {
	    // Setup SDL
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD) != 0)
	{
		std::cout << "SDL init error: " << SDL_GetError() << std::endl;
		exit(-1);
	}
	std::cout << "SDL3 successfully initialized" << std::endl;

	// Decide GL+GLSL versions
	#if defined(IMGUI_IMPL_OPENGL_ES2)
		// GL ES 2.0 + GLSL 100
		#define glsl_version "#version 100"
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	#elif defined(__APPLE__)
		// GL 3.2 Core + GLSL 150
		#define glsl_version "#version 150"
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	#else
		// GL 3.0 + GLSL 130
		#define glsl_version "#version 130"
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	#endif

    // From 2.0.18: Enable native IME.
	#ifdef SDL_HINT_IME_SHOW_UI
		SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
	#endif
	// printf(glsl_version);
	std::cout << glsl_version << std::endl;

	// Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    Uint32 window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN | SDL_WINDOW_MAXIMIZED;
	std::cout << "Making window" << std::endl;
    RenderingEngine::_window = SDL_CreateWindow("Dear ImGui SDL3+OpenGL3 example", 1280, 720, window_flags);
    if (RenderingEngine::_window == nullptr)
    {
        std::cout << "Error: SDL_CreateWindow():" << SDL_GetError() << std::endl;
        //return -1;
    }
    RenderingEngine::_windowOpen = true;
	RenderingEngine::_backgroundColor = ImVec4(0.0, 0.0f, 0.0f, 1.0f);

    std::cout << "Made windows successfully" << std::endl;
    SDL_SetWindowPosition(RenderingEngine::_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    RenderingEngine::_glContext = SDL_GL_CreateContext(RenderingEngine::_window);
	SDL_GL_MakeCurrent(RenderingEngine::_window, RenderingEngine::_glContext);
	SDL_GL_SetSwapInterval(1); // Enable vsync
	SDL_ShowWindow(RenderingEngine::_window);
}

void RenderingEngine::setupImGui() {
	std::cout << "Setting up ImGui" << std::endl;
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	RenderingEngine::_io = &ImGui::GetIO();
	RenderingEngine::_io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	RenderingEngine::_io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplSDL3_InitForOpenGL(RenderingEngine::_window, RenderingEngine::_glContext);
	ImGui_ImplOpenGL3_Init(glsl_version);
	std::cout << "Setup ImGui successfully" << std::endl;
}
void RenderingEngine::setupFont() {
	std::cout << "Setting up fonts" << std::endl;
	TTF_Init();
	RenderingEngine::_basefont = TTF_OpenFont("ARIAL.TTF", 24);
	if(RenderingEngine::_basefont == nullptr) {
		std::cout << "Basefont is not found!" << std::endl;
		exit(-1);
	}
}

void RenderingEngine::stopWindow() {
	SDL_GL_DestroyContext(RenderingEngine::_glContext);
	SDL_DestroyWindow(RenderingEngine::_window);
	SDL_Quit();
}
void RenderingEngine::stopImGui() {
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();
}

SDL_Window* RenderingEngine::getWindow() { return RenderingEngine::_window; }
ImGuiIO* RenderingEngine::getIO() { return RenderingEngine::_io; }
bool RenderingEngine::isRunning() { return RenderingEngine::_windowOpen; }

ImVec4& RenderingEngine::getBackgroundColor() { return RenderingEngine::_backgroundColor; }
void RenderingEngine::setBackgroundColor(ImVec4 newbg) { RenderingEngine::_backgroundColor = newbg; }

void RenderingEngine::renderText(std::string message, SDL_Color color, int x, int y, TTF_Font* font) {

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  gluOrtho2D(0, RenderingEngine::_io->DisplaySize.x,0,RenderingEngine::_io->DisplaySize.y);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();

  glDisable(GL_DEPTH_TEST);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  // std::cout << message << std::endl;
  SDL_Surface * sFont = TTF_RenderText_Blended(font, message.c_str(), 255, color);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sFont->w, sFont->h, 0, GL_RGBA,
                GL_UNSIGNED_BYTE, sFont->pixels);

  ImVec4 textSurface;
  textSurface.x = x;
  textSurface.y = y;
  textSurface.z = sFont->w;
  textSurface.w = sFont->h;
  RenderingEngine::debugBound(textSurface);

  glBegin(GL_QUADS);
  {
    glTexCoord2f(0,1); glVertex2f(x, y);
    glTexCoord2f(1,1); glVertex2f(x + sFont->w, y);
    glTexCoord2f(1,0); glVertex2f(x + sFont->w, y + sFont->h);
    glTexCoord2f(0,0); glVertex2f(x, y + sFont->h);
  }
  glEnd();

  glDisable(GL_BLEND);
  glDisable(GL_TEXTURE_2D);
  glEnable(GL_DEPTH_TEST);

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();

  glDeleteTextures(1, &texture);
  SDL_DestroySurface(sFont);
}


void RenderingEngine::debugBound(ImVec4 box) {
	glLineWidth(1);
	glDisable(GL_TEXTURE_2D);
	glPushAttrib(GL_ENABLE_BIT);
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex2f(box.x, box.y);
	glVertex2f(box.x + box.z, box.y);

	glVertex2f(box.x + box.z, box.y);
	glVertex2f(box.x + box.z, box.y + box.w);

	glVertex2f(box.x + box.z, box.y + box.w);
	glVertex2f(box.x, box.y + box.w);

	glVertex2f(box.x, box.y + box.w);
	glVertex2f(box.x, box.y);

	glVertex2f(box.x, box.y);
	glVertex2f(box.x + box.z, box.y + box.w);
	glEnd();
}
