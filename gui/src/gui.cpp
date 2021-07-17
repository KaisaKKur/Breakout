#include "GUI/gui.h"

CEGUI::OpenGL3Renderer* gui::GUI::_renderer = nullptr;

CEGUI::OpenGL3Renderer* gui::GUI::getRenderder() {
    return _renderer;
}

void gui::GUI::destroyRenderer() {
    CEGUI::OpenGL3Renderer::destroy(*_renderer);
    _renderer = nullptr;
}

CEGUI::MouseButton gui::GUI::toCEGUIButton(const int button) {
    switch (button) {
    case GLFW_MOUSE_BUTTON_LEFT: 
        return CEGUI::LeftButton;
        
    case GLFW_MOUSE_BUTTON_MIDDLE:
        return CEGUI::MiddleButton;

    case GLFW_MOUSE_BUTTON_RIGHT:
        return CEGUI::RightButton;

    default:
        return CEGUI::MouseButtonCount;
    }
}

CEGUI::Key::Scan gui::GUI::toCEGUIKey(const int glfwKey) {
    switch (glfwKey) {
    case GLFW_KEY_ESCAPE: return CEGUI::Key::Escape;
    case GLFW_KEY_F1: return CEGUI::Key::F1;
    case GLFW_KEY_F2: return CEGUI::Key::F2;
    case GLFW_KEY_F3: return CEGUI::Key::F3;
    case GLFW_KEY_F4: return CEGUI::Key::F4;
    case GLFW_KEY_F5: return CEGUI::Key::F5;
    case GLFW_KEY_F6: return CEGUI::Key::F6;
    case GLFW_KEY_F7: return CEGUI::Key::F7;
    case GLFW_KEY_F8: return CEGUI::Key::F8;
    case GLFW_KEY_F9: return CEGUI::Key::F9;
    case GLFW_KEY_F10: return CEGUI::Key::F10;
    case GLFW_KEY_F11: return CEGUI::Key::F11;
    case GLFW_KEY_F12: return CEGUI::Key::F12;
    case GLFW_KEY_F13: return CEGUI::Key::F13;
    case GLFW_KEY_F14: return CEGUI::Key::F14;
    case GLFW_KEY_F15: return CEGUI::Key::F15;
    case GLFW_KEY_UP: return CEGUI::Key::ArrowUp;
    case GLFW_KEY_DOWN: return CEGUI::Key::ArrowDown;
    case GLFW_KEY_LEFT: return CEGUI::Key::ArrowLeft;
    case GLFW_KEY_RIGHT: return CEGUI::Key::ArrowRight;
    case GLFW_KEY_LEFT_SHIFT: return CEGUI::Key::LeftShift;
    case GLFW_KEY_RIGHT_SHIFT: return CEGUI::Key::RightShift;
    case GLFW_KEY_LEFT_CONTROL: return CEGUI::Key::LeftControl;
    case GLFW_KEY_RIGHT_CONTROL: return CEGUI::Key::RightControl;
    case GLFW_KEY_LEFT_ALT: return CEGUI::Key::LeftAlt;
    case GLFW_KEY_RIGHT_ALT: return CEGUI::Key::RightAlt;
    case GLFW_KEY_TAB: return CEGUI::Key::Tab;
    case GLFW_KEY_ENTER: return CEGUI::Key::Return;
    case GLFW_KEY_BACKSPACE: return CEGUI::Key::Backspace;
    case GLFW_KEY_INSERT: return CEGUI::Key::Insert;
    case GLFW_KEY_DELETE: return CEGUI::Key::Delete;
    case GLFW_KEY_PAGE_UP: return CEGUI::Key::PageUp;
    case GLFW_KEY_PAGE_DOWN: return CEGUI::Key::PageDown;
    case GLFW_KEY_HOME: return CEGUI::Key::Home;
    case GLFW_KEY_END: return CEGUI::Key::End;
    case GLFW_KEY_KP_ENTER: return CEGUI::Key::NumpadEnter;
    case GLFW_KEY_SPACE: return CEGUI::Key::Space;
    case 'A': return CEGUI::Key::A;
    case 'B': return CEGUI::Key::B;
    case 'C': return CEGUI::Key::C;
    case 'D': return CEGUI::Key::D;
    case 'E': return CEGUI::Key::E;
    case 'F': return CEGUI::Key::F;
    case 'G': return CEGUI::Key::G;
    case 'H': return CEGUI::Key::H;
    case 'I': return CEGUI::Key::I;
    case 'J': return CEGUI::Key::J;
    case 'K': return CEGUI::Key::K;
    case 'L': return CEGUI::Key::L;
    case 'M': return CEGUI::Key::M;
    case 'N': return CEGUI::Key::N;
    case 'O': return CEGUI::Key::O;
    case 'P': return CEGUI::Key::P;
    case 'Q': return CEGUI::Key::Q;
    case 'R': return CEGUI::Key::R;
    case 'S': return CEGUI::Key::S;
    case 'T': return CEGUI::Key::T;
    case 'U': return CEGUI::Key::U;
    case 'V': return CEGUI::Key::V;
    case 'W': return CEGUI::Key::W;
    case 'X': return CEGUI::Key::X;
    case 'Y': return CEGUI::Key::Y;
    case 'Z': return CEGUI::Key::Z;
    default: return CEGUI::Key::Unknown;
    }
}

void gui::GUI::setWidgetDestRect(CEGUI::Window* widget, const glm::vec4& dest_rect_perc, const glm::vec4& dest_rect_pix) {
    widget->setPosition(CEGUI::UVector2(CEGUI::UDim(dest_rect_perc.x, dest_rect_pix.x), CEGUI::UDim(dest_rect_perc.y, dest_rect_pix.y)));
    widget->setSize(CEGUI::USize(CEGUI::UDim(dest_rect_perc.z, dest_rect_pix.z), CEGUI::UDim(dest_rect_perc.w, dest_rect_pix.w)));
}

void gui::GUI::setWidgetCenter(glm::vec4& convert) {
    convert.x = (1.0f - convert.z) / 2;
	convert.y = (1.0f - convert.w) / 2;
}

CEGUI::Window* gui::GUI::createWidget(CEGUI::Window* root, const std::string_view type, const glm::vec4& dest_rect_perc, const glm::vec4& dest_rect_pix, const std::string_view name) {
    CEGUI::Window* widget = CEGUI::WindowManager::getSingleton().createWindow(type.data(), name.data());
    root->addChild(widget);
    setWidgetDestRect(widget, dest_rect_perc, dest_rect_pix);
    return widget;
}

gui::GUI::GUI() : _context(nullptr), _root(nullptr) {
}

gui::GUI::~GUI() {
    destroy();
}

inline void gui::GUI::init(const std::string_view resource_directory) {
    try {
        _renderer = &CEGUI::OpenGL3Renderer::bootstrapSystem();
        if (!_renderer) {
		    throw std::exception("Failed to init CEGUI::OpenGL3Renderer");
	    }

        // 加载默认资源
        CEGUI::DefaultResourceProvider* rp = reinterpret_cast<CEGUI::DefaultResourceProvider*>(CEGUI::System::getSingleton().getResourceProvider());
	    rp->setResourceGroupDirectory("imagesets", std::string(resource_directory) + "/imagesets/");
        rp->setResourceGroupDirectory("schemes", std::string(resource_directory) + "/schemes/");
        rp->setResourceGroupDirectory("fonts", std::string(resource_directory) + "/fonts/");
        rp->setResourceGroupDirectory("layouts", std::string(resource_directory) + "/layouts/");
        rp->setResourceGroupDirectory("looknfeels", std::string(resource_directory) + "/looknfeel/");
        rp->setResourceGroupDirectory("lua_scripts", std::string(resource_directory) + "/lua_scripts/");
    
        // 设置默认资源组
        CEGUI::ImageManager::setImagesetDefaultResourceGroup("imagesets");
        CEGUI::Scheme::setDefaultResourceGroup("schemes");
        CEGUI::Font::setDefaultResourceGroup("fonts");
        CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");
        CEGUI::WindowManager::setDefaultResourceGroup("layouts");
        CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts");

        CEGUI::XMLParser* parser = CEGUI::System::getSingleton().getXMLParser();
        if (parser->isPropertyPresent("SchemaDefaultResourceGroup"))
            parser->setProperty("SchemaDefaultResourceGroup", "schemas");

        // 设置 TaharezLook 模式
        // DejaVuSans-10 字体
        CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme", "schemes");
        CEGUI::FontManager::getSingleton().createFromFile("DejaVuSans-10.font");

        // 设置默认字体和光标图像以及工具提示类型
        CEGUI::System::getSingleton().getDefaultGUIContext().setDefaultFont("DejaVuSans-10");
        CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");
        CEGUI::System::getSingleton().getDefaultGUIContext().setDefaultTooltipType("TaharezLook/Tooltip");

        _context = &CEGUI::System::getSingleton().createGUIContext(_renderer->getDefaultRenderTarget());
        _root = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", "root");
        _context->setRootWindow(_root);
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}

inline void gui::GUI::destroy() {
    CEGUI::System::getSingleton().destroyGUIContext(*_context);
    _root->destroy();
    CEGUI::System::getSingleton().destroy();
    _context = nullptr;
    _root = nullptr;
}

inline void gui::GUI::draw() {
    GLboolean blend = false;
	glGetBooleanv(GL_BLEND, &blend);
    
    _renderer->beginRendering();
    _context->draw();
    _renderer->endRendering();

    if (blend) { /* cegui 默认可能会关闭混合功能，保存状态，判断恢复混合功能 */
        glEnable(GL_BLEND);
    }
}

inline void gui::GUI::loadScheme(const std::string_view scheme_file) {
    CEGUI::SchemeManager::getSingleton().createFromFile(std::string(scheme_file.data()) + ".scheme");
}

inline void gui::GUI::setFont(const std::string_view font_file) {
    CEGUI::FontManager::getSingleton().createFromFile(std::string(font_file) + ".font");
    _context->setDefaultFont(font_file.data());
}

inline CEGUI::Window* gui::GUI::createWidget(const std::string_view type, const glm::vec4& dest_rect_perc, const glm::vec4& dest_rect_pix, const std::string_view name) {
    CEGUI::Window* widget = CEGUI::WindowManager::getSingleton().createWindow(type.data(), name.data());
    _root->addChild(widget);
    setWidgetDestRect(widget, dest_rect_perc, dest_rect_pix);
    return widget;
}

inline void gui::GUI::setMouseCursor(const std::string_view image_file) {
    _context->getMouseCursor().setDefaultImage(image_file.data());
}

inline void gui::GUI::showMouseCursor() {
    _context->getMouseCursor().show();
}

inline void gui::GUI::hideMouseCursor() {
    _context->getMouseCursor().hide();
}

inline CEGUI::GUIContext* gui::GUI::getContext() const {
    return _context;
}

inline CEGUI::Window* gui::GUI::getRoot() const {
    return _root;
}
