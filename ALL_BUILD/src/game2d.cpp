#include "game2d/game2d.h"

/* ��Ϸʵ�� */
std::unique_ptr<breakout::Breakout>	 game2d::_breakout_(breakout::getInstance());
/* GUI */
game2d::GameUI game2d::_gui_;

void game2d::Game2D::windowPosCallback(GLFWwindow* window, int xpos, int ypos) {
}

void game2d::Game2D::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    /*if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }*/

	// ��Ϸ����
    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS) {
			_breakout_->_getKeys()[key] = true;
        } else if (action == GLFW_RELEASE) {
            _breakout_->_getKeys()[key] = false;
        }
    }

	// GUI ����
	CEGUI::Key::Scan cegui_key = gui::GUI::toCEGUIKey(key);
    if (action == GLFW_PRESS) {
		_gui_->getContext()->injectKeyDown(cegui_key);
    } else {
        _gui_->getContext()->injectKeyUp(cegui_key);
    }
}

void game2d::Game2D::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void game2d::Game2D::cursorPosCallback(GLFWwindow* window, double x, double y) {
	_gui_->getContext()->injectMousePosition(static_cast<float>(x), static_cast<float>(y));
}

void game2d::Game2D::mouseButtonCallback(GLFWwindow* window, int button, int state, int mod) {
	if (state == GLFW_PRESS) {
		_gui_->getContext()->injectMouseButtonDown(gui::GUI::toCEGUIButton(button));
	} else {
		_gui_->getContext()->injectMouseButtonUp(gui::GUI::toCEGUIButton(button));
	}
}

void game2d::Game2D::errorCallback(int error, const char* message) {
	CEGUI::Logger::getSingleton().logEvent(message, CEGUI::Errors);
}

game2d::Game2D::Game2D(const std::string_view title) : _window(nullptr), _width(SCREEN_WIDTH), _height(SCREEN_HEIGHT), _title(title), _runtime(0.0f) {
}

game2d::Game2D::~Game2D() {
	// �رջ�Ϲ���
	glDisable(GL_BLEND);
	// ���ٴ���
	glfwDestroyWindow(_window);
	_window = nullptr;
}

inline void game2d::Game2D::run() {
	init();
	render();
	terminate();
}

inline void game2d::Game2D::terminate() {
	glfwTerminate();
}

inline void game2d::Game2D::glfwInit() {
	try {
		if (!::glfwInit()) {
			throw std::exception("Failed to initialize GLFW");
		}
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif
		glfwWindowHint(GLFW_RESIZABLE, false);
	} catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}
}

inline void game2d::Game2D::init() {
	try {
		glfwInit();

		if (!(_window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, _title.c_str(), nullptr, nullptr))) {
			throw std::exception("Failed to create GLFW Game2D window");
		}

		// ���� OpenGl ������
		glfwMakeContextCurrent(_window);

		// ���д���
		centerWindow();

		// ����ͼ��
		setIcon();

		// Ԥ����
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(_window);
		
		// �����������λ�ڴ����Ϸ�ʱ��������
		glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		
		// ���� VSYNC
		glfwSwapInterval(1);

		// ��������ʱ��
		glfwSetTime(0.0f);

		// ���ط��� OpenGL ��
		glewExperimental = GL_TRUE;
		if(glewInit()) {
			throw std::exception("Failed to init GLEW");
		}

		// ���������Ϣ
		glGetError();

		// ������Ϲ���
		glEnable(GL_BLEND);
	} catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}

	_breakout_->init();
	_gui_.setWindow(_window);
	_gui_.initUI();
	
	// ���ûص�
	setupCallbacks();
}

inline void game2d::Game2D::centerWindow() {
	// ��ȡ���������
	const GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	// ��ȡ GLFW ��Ƶģʽ
	const GLFWvidmode* mode = glfwGetVideoMode(const_cast<GLFWmonitor*>(monitor));
	// ���д���
	glfwSetWindowPos(_window, (mode->width - _width) / 2, (mode->height - _height) / 2);
}

inline void game2d::Game2D::setupCallbacks() {
	// ����ص�
    glfwSetCursorPosCallback(_window, cursorPosCallback);
    glfwSetKeyCallback(_window, keyCallback);
    glfwSetMouseButtonCallback(_window, mouseButtonCallback);
	// λ�øı�ص�
	glfwSetWindowPosCallback(_window, windowPosCallback);
	// ����ص�
	glfwSetErrorCallback(errorCallback);
}

inline void game2d::Game2D::render() {
	float delta_time = 0.0f;

	while (!glfwWindowShouldClose(_window)) {

		delta_time = static_cast<float>(getDeltaTime());
		glfwPollEvents();

		// --------------------------------------------------------------------------
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		_breakout_->render(static_cast<float>(getRuntime()));
		_breakout_->processInput(delta_time);
		_breakout_->update(delta_time);

		_gui_.updata();
		_gui_->draw();
		// --------------------------------------------------------------------------

		glfwSwapBuffers(_window);
	}
}

inline void game2d::Game2D::setIcon(const std::filesystem::path icon_path) {
	try {
		if (!std::filesystem::exists(icon_path)) {
			throw std::exception("Failed to load the icon!");
		}

		std::vector<GLFWimage> images(1);

		int width, height, nr_channels;
		unsigned char* data = stbi_load(icon_path.string().c_str(), &width, &height, &nr_channels, 0);

		if (data) {
			images[0].width = width;
			images[0].height = height;
			images[0].pixels = data;
		}

		glfwSetWindowIcon(_window, 1, images.data()); 
		stbi_image_free(data);
	} catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}
}

inline double game2d::Game2D::getDeltaTime() {
	double current_frame = glfwGetTime();
	double delta_time = current_frame - _runtime;
	_runtime = current_frame;
	return delta_time;
}

inline GLFWwindow* game2d::Game2D::getWindow() const {
	return _window;
}

inline double game2d::Game2D::getRuntime() const {
	return _runtime;
}
