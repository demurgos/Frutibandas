#include "window.hpp"

WindowManager::WindowManager(const std::string& title)
{
	alive = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cerr << SDL_GetError() << std::endl;
		std::exit(-1);
	}

	// Get screen size and create main window
	SDL_Rect r;
	if(SDL_GetDisplayBounds(0, &r) != 0)
	{
		std::cerr << SDL_GetError() << std::endl;
		std::exit(-1);
	}

	width = 1050;
	height = 728;

	window = nullptr;

	// OPENGL VERSION
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

	// DOUBLE BUFFER
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// MULTISAMPLING
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

	window = SDL_CreateWindow(
				title.c_str(),
				SDL_WINDOWPOS_CENTERED,
				SDL_WINDOWPOS_CENTERED,
				width,
				height,
				SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS
				);

	if(window == nullptr)
	{
		std::cerr << SDL_GetError() << std::endl;
		SDL_Quit();
		std::exit(-1);
	}

	glContext = SDL_GL_CreateContext(window);

	// Glew init
	glewExperimental = true;
	GLenum err = glewInit();
	if(err != GLEW_OK)
	{
		std::cerr << glewGetErrorString(err) << std::endl;
		SDL_GL_DeleteContext(glContext);
		SDL_DestroyWindow(window);
		SDL_Quit();
		std::exit(-1);
	}

	// Setting OpenGL states
	glViewport(0, 0, width, height);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glClearColor(LIGHT_GREY[0], LIGHT_GREY[1], LIGHT_GREY[2], LIGHT_GREY[3]);
	SDL_GL_SetSwapInterval(1);

	// IMGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForOpenGL(window, glContext);
	ImGui_ImplOpenGL3_Init();
}

WindowManager::~WindowManager()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int WindowManager::getWidth()
{
	return width;
}

int WindowManager::getHeight()
{
	return height;
}

SDL_Window* WindowManager::getWindowPtr()
{
	return window;
}

std::array<int, 3> & WindowManager::getMouseData()
{
	return mouseData;
}

std::bitset<10>& WindowManager::getUserInputs()
{
	return userInputs;
}

char* WindowManager::get_text_input()
{
	return m_textInput;
}

bool& WindowManager::isAlive()
{
	return alive;
}

void WindowManager::checkEvents(bool writing)
{
	event.keyboardState = SDL_GetKeyboardState(nullptr);
	event.mouseButtonBitMask = SDL_GetRelativeMouseState(&mouseData[0], &mouseData[1]);

	while(SDL_PollEvent(&event.e))
	{
		// forward to ImGui
		ImGui_ImplSDL2_ProcessEvent(&event.e);

		if(event.e.type == SDL_QUIT)
		{
			alive = false;
		}

		if(event.e.type == SDL_WINDOWEVENT)
		{
			if(event.e.window.event == SDL_WINDOWEVENT_RESIZED)
			{
				userInputs.set(4);
				width = event.e.window.data1;
				height = event.e.window.data2;
				glViewport(0, 0, width, height);
			}
		}

		if (event.e.type == SDL_MOUSEBUTTONUP)
		{
			userInputs.set(9);
		}

		if(event.e.type == SDL_MOUSEWHEEL)
		{
			userInputs.set(3);
			mouseData[2] = event.e.wheel.y;
		}
		if (writing)
		{
			if (event.e.type == SDL_TEXTINPUT)
			{
				memcpy(m_textInput, event.e.text.text, 32);
			}
			else
			{
				for (int i{ 0 }; i < 32; ++i) {
					m_textInput[i] = '\0';
				}
			}
		}
	}

	if(SDL_BUTTON(event.mouseButtonBitMask) == SDL_BUTTON_LEFT)
	{
		userInputs.set(2);
	}
	
	if(SDL_BUTTON(event.mouseButtonBitMask) == SDL_BUTTON_MIDDLE)
	{
		userInputs.set(1);
	}
	
	if(SDL_BUTTON(event.mouseButtonBitMask) == SDL_BUTTON_RIGHT)
	{
		userInputs.set(0);
	}

	if (event.keyboardState[SDL_GetScancodeFromKey(SDLK_RETURN)])
	{
		userInputs.set(5);
	}

	if (event.keyboardState[SDL_GetScancodeFromKey(SDLK_BACKSPACE)])
	{
		userInputs.set(6);
	}

	if (event.keyboardState[SDL_GetScancodeFromKey(SDLK_RIGHT)])
	{
		userInputs.set(7);
	}

	if (event.keyboardState[SDL_GetScancodeFromKey(SDLK_LEFT)])
	{
		userInputs.set(8);
	}
}

void WindowManager::resetEvents()
{
	userInputs.reset();
}