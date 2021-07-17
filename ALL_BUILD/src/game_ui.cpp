#include "game2d/game_ui.h"

game2d::GameUI::GameUI() : _gui(new gui::GUI()), _window(nullptr), _menu(nullptr), _process(nullptr), _select_checkpoint(nullptr), _result_checkpoint_prompt(nullptr), _main_menu_btns(), _process_btns() {
}

game2d::GameUI::GameUI(const GLFWwindow* window) : _gui(new gui::GUI()), _window(const_cast<GLFWwindow*>(window)), _menu(nullptr), _process(nullptr), _select_checkpoint(nullptr), _result_checkpoint_prompt(nullptr), _main_menu_btns(), _process_btns() {
}

game2d::GameUI::~GameUI() {
	_gui = nullptr;
	_window = nullptr;
	destroyComponent(&_menu);
	destroyComponent(&_process);
	destroyComponent(&_select_checkpoint);
	destroyComponent(&_result_checkpoint_prompt);
	_main_menu_btns.clear();
	_process_btns.clear();
}

inline void game2d::GameUI::initUI() {
	_gui->init();

	// Ìí¼Ó¼àÌýÆ÷
	_gui->getRoot()->subscribeEvent(CEGUI::Window::EventKeyDown, CEGUI::Event::Subscriber(&GameUI::handleRootKeyDown, this));
	_gui->loadScheme("SampleBrowser");

	_gui->setMouseCursor("SampleBrowserSkin/MouseArrow");
	_gui->showMouseCursor();

	setMainMenu();
	loadReserveTextures();
}

inline void game2d::GameUI::updata() {
	losePrompt();
	winPrompt();
}

inline void game2d::GameUI::createProcess() {
	if (!_process) {
		_gui->showMouseCursor();

		glm::vec4 pos_size(0.0f, 0.0f, 0.4f, 0.3f);
		gui::GUI::setWidgetCenter(pos_size);
		_process = reinterpret_cast<CEGUI::FrameWindow*>(
			_gui->createWidget(
				"TaharezLook/FrameWindow", 
				pos_size,
				glm::vec4(0.0f),
				"Process"
			)
		);
		_process->setText("Happy Game");
		_process->setAlpha(0.96f);
		_process->subscribeEvent(CEGUI::FrameWindow::EventCloseClicked, CEGUI::Event::Subscriber(&GameUI::handleProcessCloseButton, this));

		// context
		CEGUI::Window* label = reinterpret_cast<CEGUI::FrameWindow*>(
			gui::GUI::createWidget(
				_process,
				"TaharezLook/Label", 
				glm::vec4(0.0f, 0.3f, 1.0f, 0.3f),
				glm::vec4(0.0f),
				"Process Context"
			)
		);
		if (_breakout_->getState() == breakout::GameState::LOSE) {
			label->setText("You lose that you want to end the game?");
		} else if (_breakout_->getState() == breakout::GameState::WIN) {
			label->setText("You win that you want to try the next checkpoint?");
		} else {
			label->setText("Are you sure you want to end the game?");
		}
		label->setDisabled(true);
		_process->addChild(label);

		// Retry
		_process_btns[ButtonIndex::Process::RETRY] = reinterpret_cast<CEGUI::PushButton*>(CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/Button", "Retry"));
		gui::GUI::setWidgetDestRect(_process_btns[ButtonIndex::Process::RETRY], glm::vec4(0.05f, 0.7f, 0.2f, 0.2f), glm::vec4(0.0f));
		_process_btns[ButtonIndex::Process::RETRY]->setText("Retry");
		_process->addChild(_process_btns[ButtonIndex::Process::RETRY]);
		// °´Å¥¼àÌý
		_process_btns[ButtonIndex::Process::RETRY]->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameUI::handleRetryGameClicked, this));
		
		if (_breakout_->getState() == breakout::GameState::WIN) {
			// Next Checkpoint
			_process_btns[ButtonIndex::Process::NEXT_CHECKPOINT] = reinterpret_cast<CEGUI::PushButton*>(CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/Button", "Next Checkpoint"));
			gui::GUI::setWidgetDestRect(_process_btns[ButtonIndex::Process::NEXT_CHECKPOINT], glm::vec4(0.3f, 0.7f, 0.2f, 0.2f), glm::vec4(0.0f));
			_process_btns[ButtonIndex::Process::NEXT_CHECKPOINT]->setText("Next");
			_process->addChild(_process_btns[ButtonIndex::Process::NEXT_CHECKPOINT]);
			// °´Å¥¼àÌý
			_process_btns[ButtonIndex::Process::NEXT_CHECKPOINT]->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameUI::handleNextGameClicked, this));
		}

		// Back Main Menu
		_process_btns[ButtonIndex::Process::BACK_MAIN_MENU] = reinterpret_cast<CEGUI::PushButton*>(CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/Button", "Back Main Menu"));
		gui::GUI::setWidgetDestRect(_process_btns[ButtonIndex::Process::BACK_MAIN_MENU], glm::vec4(0.55f, 0.7f, 0.4f, 0.2f), glm::vec4(0.0f));
		_process_btns[ButtonIndex::Process::BACK_MAIN_MENU]->setText("Back Main Menu");
		_process->addChild(_process_btns[ButtonIndex::Process::BACK_MAIN_MENU]);
		// °´Å¥¼àÌý
		_process_btns[ButtonIndex::Process::BACK_MAIN_MENU]->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameUI::handleBackMainMenuClicked, this));
	}
}

inline void game2d::GameUI::createSelectCheckpoint() {
	if (!_select_checkpoint) {
		_select_checkpoint = reinterpret_cast<CEGUI::FrameWindow*>(
			_gui->createWidget(
				"TaharezLook/FrameWindow", 
				glm::vec4(0.5f, 0.0f, 0.5f, 1.0f),
				glm::vec4(0.0f),
				"Select Checkpoint"
			)
		);
		_select_checkpoint->setText("Select Checkpoint");
		_select_checkpoint->setAlpha(0.96f);
		_select_checkpoint->subscribeEvent(CEGUI::FrameWindow::EventCloseClicked, CEGUI::Event::Subscriber(&GameUI::handleSelectCheckpointCloseButton, this));
		
		CEGUI::Window* scrollable_pane = reinterpret_cast<CEGUI::ScrollablePane*>(
			gui::GUI::createWidget(
				_select_checkpoint,
				"TaharezLook/ScrollablePane", 
				glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
				glm::vec4(0.0f),
				"Select Checkpoint Scrollable Pane"
			)
		);

		float height = 0.2f;

		// Checkpoint 1
		CEGUI::Window* preview1 = gui::GUI::createWidget(
			scrollable_pane,
			"TaharezLook/StaticImage", 
			glm::vec4(0.05f, 0.05f, 0.425f, height),
			glm::vec4(0.0f),
			"Checkpoint1"
		);
		preview1->setProperty("Image", "Checkpoint1");
		preview1->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&GameUI::handleSelectCheckpoint1, this));

		// Checkpoint 2
		CEGUI::Window* preview2 = gui::GUI::createWidget(
			scrollable_pane,
			"TaharezLook/StaticImage", 
			glm::vec4(0.525f, 0.05, 0.425f, height),
			glm::vec4(0.0f),
			"Checkpoint2"
		);
		preview2->setProperty("Image", "Checkpoint2");
		preview2->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&GameUI::handleSelectCheckpoint2, this));
		
		// Checkpoint 3
		CEGUI::Window* preview3 = gui::GUI::createWidget(
			scrollable_pane,
			"TaharezLook/StaticImage", 
			glm::vec4(0.05f, 0.05f + 1 * (height + 0.05f), 0.425f, height),
			glm::vec4(0.0f),
			"Checkpoint3"
		);
		preview3->setProperty("Image", "Checkpoint3");
		preview3->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&GameUI::handleSelectCheckpoint3, this));
		
		// Checkpoint 4
		CEGUI::Window* preview4 = gui::GUI::createWidget(
			scrollable_pane,
			"TaharezLook/StaticImage", 
			glm::vec4(0.525f, 0.05f + 1 * (height + 0.05f), 0.425f, height),
			glm::vec4(0.0f),
			"Checkpoint4"
		);
		preview4->setProperty("Image", "Checkpoint4");
		preview4->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&GameUI::handleSelectCheckpoint4, this));
		
		// Checkpoint 5
		CEGUI::Window* preview5 = gui::GUI::createWidget(
			scrollable_pane,
			"TaharezLook/StaticImage", 
			glm::vec4(0.05f, 0.05f + 2 * (height + 0.05f), 0.425f, height),
			glm::vec4(0.0f),
			"Checkpoint5"
		);
		preview5->setProperty("Image", "Checkpoint5");
		preview5->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&GameUI::handleSelectCheckpoint5, this));
		
		// Checkpoint 6
		CEGUI::Window* preview6 = gui::GUI::createWidget(
			scrollable_pane,
			"TaharezLook/StaticImage", 
			glm::vec4(0.525f, 0.05f + 2 * (height + 0.05f), 0.425f, height),
			glm::vec4(0.0f),
			"Checkpoint6"
		);
		preview6->setProperty("Image", "Checkpoint6");
		preview6->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&GameUI::handleSelectCheckpoint6, this));
		
		// Checkpoint 7
		CEGUI::Window* preview7 = gui::GUI::createWidget(
			scrollable_pane,
			"TaharezLook/StaticImage", 
			glm::vec4(0.05f, 0.05f + 3 * (height + 0.05f), 0.425f, height),
			glm::vec4(0.0f),
			"Checkpoint7"
		);
		preview7->setProperty("Image", "Checkpoint7");
		preview7->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&GameUI::handleSelectCheckpoint7, this));
		
		// Checkpoint 8
		CEGUI::Window* preview8 = gui::GUI::createWidget(
			scrollable_pane,
			"TaharezLook/StaticImage", 
			glm::vec4(0.525f, 0.05f + 3 * (height + 0.05f), 0.425f, height),
			glm::vec4(0.0f),
			"Checkpoint8"
		);
		preview8->setProperty("Image", "Checkpoint8");
		preview8->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&GameUI::handleSelectCheckpoint8, this));
	}
}

inline void game2d::GameUI::createResultCheckpointPrompt() {
	if (!_result_checkpoint_prompt) {
		glm::vec4 pos_size(0.0f, 0.0f, 0.4f, 0.3f);
		gui::GUI::setWidgetCenter(pos_size);
		_result_checkpoint_prompt = reinterpret_cast<CEGUI::FrameWindow*>(
			_gui->createWidget(
				"TaharezLook/FrameWindow", 
				pos_size,
				glm::vec4(0.0f),
				"Result Checkpoint Prompt"
			)
		);
		_result_checkpoint_prompt->setText("Happy Game");
		_result_checkpoint_prompt->setAlpha(0.96f);
		_result_checkpoint_prompt->subscribeEvent(CEGUI::FrameWindow::EventCloseClicked, CEGUI::Event::Subscriber(&GameUI::handleResultCheckpointPromptCloseButton, this));

		// context
		CEGUI::Window* label[2];
		label[0] = reinterpret_cast<CEGUI::FrameWindow*>(
			gui::GUI::createWidget(
				_result_checkpoint_prompt,
				"TaharezLook/Label", 
				glm::vec4(0.0f, 0.25f, 1.0f, 0.1f),
				glm::vec4(0.0f),
				"Result Checkpoint Prompt Context 1"
			)
		);
		label[0]->setText("You have completed all checkpoint!");
		label[0]->setDisabled(true);
		_result_checkpoint_prompt->addChild(label[0]);
		
		label[1] = reinterpret_cast<CEGUI::FrameWindow*>(
			gui::GUI::createWidget(
				_result_checkpoint_prompt,
				"TaharezLook/Label", 
				glm::vec4(0.0f, 0.4f, 1.0f, 0.1f),
				glm::vec4(0.0f),
				"Result Checkpoint Prompt Context 2"
			)
		);
		label[1]->setText("You want to go back the main menu?");
		label[1]->setDisabled(true);
		_result_checkpoint_prompt->addChild(label[1]);

		// Retry
		_process_btns[ButtonIndex::Process::RETRY] = reinterpret_cast<CEGUI::PushButton*>(CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/Button", "Result Checkpoint Prompt Retry"));
		gui::GUI::setWidgetDestRect(_process_btns[ButtonIndex::Process::RETRY], glm::vec4(0.05f, 0.7f, 0.2f, 0.2f), glm::vec4(0.0f));
		_process_btns[ButtonIndex::Process::RETRY]->setText("Retry");
		_result_checkpoint_prompt->addChild(_process_btns[ButtonIndex::Process::RETRY]);
		// °´Å¥¼àÌý
		_process_btns[ButtonIndex::Process::RETRY]->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameUI::handleRetryGameClicked, this));

		// Back Main Menu
		_process_btns[ButtonIndex::Process::BACK_MAIN_MENU] = reinterpret_cast<CEGUI::PushButton*>(CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/Button", "Result Checkpoint Prompt Back Main Menu"));
		gui::GUI::setWidgetDestRect(_process_btns[ButtonIndex::Process::BACK_MAIN_MENU], glm::vec4(0.55f, 0.7f, 0.4f, 0.2f), glm::vec4(0.0f));
		_process_btns[ButtonIndex::Process::BACK_MAIN_MENU]->setText("Back Main Menu");
		_result_checkpoint_prompt->addChild(_process_btns[ButtonIndex::Process::BACK_MAIN_MENU]);
		// °´Å¥¼àÌý
		_process_btns[ButtonIndex::Process::BACK_MAIN_MENU]->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameUI::handleBackMainMenuClicked, this));
	}
}

inline void game2d::GameUI::destroyComponent(CEGUI::Window** component) {
	if (*component) {
		(*component)->destroy();
	}
	*component = nullptr;
}

inline void game2d::GameUI::destroyComponent(CEGUI::FrameWindow** component) {
	if (*component) {
		(*component)->destroy();
	}
	*component = nullptr;
}

inline void game2d::GameUI::losePrompt() {
	if (_breakout_->getState() == breakout::GameState::LOSE) {
		if (!_process) {
			createProcess();
		}
	}
}

inline void game2d::GameUI::winPrompt() {
	if (_breakout_->getState() == breakout::GameState::WIN) {
		if (!_process) {
			createProcess();
		}
	}
}

inline void game2d::GameUI::loadReserveTextures() {
	std::string name;
	for (unsigned short i = 0; i < _breakout_->getCheckpointsSize(); i++) {
		name = std::string("Checkpoint") + std::to_string(i + 1);
		if(!CEGUI::ImageManager::getSingleton().isDefined(name)) {
			CEGUI::ImageManager::getSingleton().addFromImageFile(name, std::string("data/textures/checkpoints/preview/cp") + std::to_string(i + 1) + ".png");
		}
	}
}

inline void game2d::GameUI::setMainMenu() {
	if (!_menu) {
		_menu = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", "Menu");
		_menu->setPosition(CEGUI::UVector2(CEGUI::UDim(0.0f, 0.0f), CEGUI::UDim(0.5f, 0.0f)));
		_menu->setSize(CEGUI::USize(CEGUI::UDim(0.5f, 0.0f), CEGUI::UDim(0.5f, 0.0f)));
		_gui->getRoot()->addChild(_menu);

		float top = 0.3f, apart = 0.15f;

		// New Game
		_main_menu_btns[ButtonIndex::MainMenu::NEW_GAME] = reinterpret_cast<CEGUI::PushButton*>(CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/Button", "New Game"));
		gui::GUI::setWidgetDestRect(_main_menu_btns[ButtonIndex::MainMenu::NEW_GAME], glm::vec4(0.1f, top, 0.35f, 0.1f), glm::vec4(0.0f));
		_main_menu_btns[ButtonIndex::MainMenu::NEW_GAME]->setText("New Game");
		_menu->addChild(_main_menu_btns[ButtonIndex::MainMenu::NEW_GAME]);
		// °´Å¥¼àÌý
		_main_menu_btns[ButtonIndex::MainMenu::NEW_GAME]->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameUI::handleNewGameClicked, this));
		
		// Continue Game
		_main_menu_btns[ButtonIndex::MainMenu::CONTINUE_GAME] = reinterpret_cast<CEGUI::PushButton*>(CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/Button", "Continue Game"));
		gui::GUI::setWidgetDestRect(_main_menu_btns[ButtonIndex::MainMenu::CONTINUE_GAME], glm::vec4(0.1f, top += apart, 0.35f, 0.1f), glm::vec4(0.0f));
		_main_menu_btns[ButtonIndex::MainMenu::CONTINUE_GAME]->setText("Continue Game");
		_menu->addChild(_main_menu_btns[ButtonIndex::MainMenu::CONTINUE_GAME]);
		_main_menu_btns[ButtonIndex::MainMenu::CONTINUE_GAME]->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameUI::handleContinueGameClicked, this));

		// Select Checkpoint
		_main_menu_btns[ButtonIndex::MainMenu::SELECT_CHECKPOINT] = reinterpret_cast<CEGUI::PushButton*>(CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/Button", "Select Checkpoint"));
		gui::GUI::setWidgetDestRect(_main_menu_btns[ButtonIndex::MainMenu::SELECT_CHECKPOINT], glm::vec4(0.1f, top += apart, 0.35f, 0.1f), glm::vec4(0.0f));
		_main_menu_btns[ButtonIndex::MainMenu::SELECT_CHECKPOINT]->setText("Select Checkpoint");
		_menu->addChild(_main_menu_btns[ButtonIndex::MainMenu::SELECT_CHECKPOINT]);
		_main_menu_btns[ButtonIndex::MainMenu::SELECT_CHECKPOINT]->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameUI::handleSelectCheckpointClicked, this));

		// Quit
		_main_menu_btns[ButtonIndex::MainMenu::QUIT] = reinterpret_cast<CEGUI::PushButton*>(CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/Button", "Quit"));
		gui::GUI::setWidgetDestRect(_main_menu_btns[ButtonIndex::MainMenu::QUIT], glm::vec4(0.1f, top += apart, 0.35f, 0.1f), glm::vec4(0.0f));
		_main_menu_btns[ButtonIndex::MainMenu::QUIT]->setText("Quit");
		_menu->addChild(_main_menu_btns[ButtonIndex::MainMenu::QUIT]);
		_main_menu_btns[ButtonIndex::MainMenu::QUIT]->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameUI::handleQuitClicked, this));
	}
}

inline void game2d::GameUI::showMainMenu() {
	_menu->show();
}

inline void game2d::GameUI::hideMainMenu() {
	_menu->hide();
}

inline bool game2d::GameUI::handleRootKeyDown(const CEGUI::EventArgs& args) {
	const CEGUI::KeyEventArgs& key_args = static_cast<const CEGUI::KeyEventArgs&>(args);

	switch (key_args.scancode) {
	case CEGUI::Key::Escape:
		if (_breakout_->getState() == breakout::GameState::MENU) {
			glfwSetWindowShouldClose(const_cast<GLFWwindow*>(_window), GLFW_TRUE);
		} else if (_breakout_->getState() == breakout::GameState::ACTIVE) {
			if (!_process) {
				createProcess();	
			} else {
				destroyComponent(&_process);
			}
		}
	break;

	default:
	return false;
	}

	return true;
}

inline bool game2d::GameUI::handleProcessCloseButton(const CEGUI::EventArgs&) {
	destroyComponent(&_process);
	return true;
}

inline bool game2d::GameUI::handleNewGameClicked(const CEGUI::EventArgs&) {
	destroyComponent(&_select_checkpoint);
	hideMainMenu();
	_gui->hideMouseCursor();
	_breakout_->setState(breakout::GameState::ACTIVE);
	_breakout_->setCheckpoint(0);
	_breakout_->switchBackgroundSound();
	return true;
}

inline bool game2d::GameUI::handleRetryGameClicked(const CEGUI::EventArgs&) {
	destroyComponent(&_process);
	destroyComponent(&_result_checkpoint_prompt);
	_gui->hideMouseCursor();
	_breakout_->reset();
	_breakout_->setState(breakout::GameState::ACTIVE);
	return true;
}

inline bool game2d::GameUI::handleBackMainMenuClicked(const CEGUI::EventArgs&) {
	destroyComponent(&_process);
	destroyComponent(&_result_checkpoint_prompt);
	_breakout_->reset();
	_breakout_->setState(breakout::GameState::MENU);
	_breakout_->switchBackgroundSound();
	showMainMenu();
	return true;
}

inline bool game2d::GameUI::handleNextGameClicked(const CEGUI::EventArgs&) {
	destroyComponent(&_process);
	_breakout_->setState(breakout::GameState::ACTIVE);
	
	if (_breakout_->getCheckpoint() + 1 < _breakout_->getCheckpointMaxSizelimit()) {
		_gui->hideMouseCursor();
		_breakout_->setCheckpoint(_breakout_->getCheckpoint() + 1);
		_breakout_->reset();
		_breakout_->switchBackgroundSound();
	} else {
		createResultCheckpointPrompt();
	}

	return true;
}

inline bool game2d::GameUI::handleSelectCheckpointCloseButton(const CEGUI::EventArgs&) {
	destroyComponent(&_select_checkpoint);
	return true;
}

inline bool game2d::GameUI::handleResultCheckpointPromptCloseButton(const CEGUI::EventArgs&) {
	std::cout << "Result Checkpoint Prompt" << std::endl;
	destroyComponent(&_result_checkpoint_prompt);
	return true;
}

inline bool game2d::GameUI::handleSelectCheckpoint1(const CEGUI::EventArgs&) {
	hideMainMenu();
	destroyComponent(&_select_checkpoint);
	_gui->hideMouseCursor();
	_breakout_->setState(breakout::GameState::ACTIVE);
	_breakout_->setCheckpoint(0);
	_breakout_->switchBackgroundSound();
	return true;
}

inline bool game2d::GameUI::handleSelectCheckpoint2(const CEGUI::EventArgs&) {
	hideMainMenu();
	destroyComponent(&_select_checkpoint);
	_gui->hideMouseCursor();
	_breakout_->setState(breakout::GameState::ACTIVE);
	_breakout_->setCheckpoint(1);
	_breakout_->switchBackgroundSound();
	return true;
}

inline bool game2d::GameUI::handleSelectCheckpoint3(const CEGUI::EventArgs&) {
	hideMainMenu();
	destroyComponent(&_select_checkpoint);
	_gui->hideMouseCursor();
	_breakout_->setState(breakout::GameState::ACTIVE);
	_breakout_->setCheckpoint(2);
	_breakout_->switchBackgroundSound();
	return true;
}

inline bool game2d::GameUI::handleSelectCheckpoint4(const CEGUI::EventArgs&) {
	hideMainMenu();
	destroyComponent(&_select_checkpoint);
	_gui->hideMouseCursor();
	_breakout_->setState(breakout::GameState::ACTIVE);
	_breakout_->setCheckpoint(3);
	_breakout_->switchBackgroundSound();
	return true;
}

inline bool game2d::GameUI::handleSelectCheckpoint5(const CEGUI::EventArgs&) {
	hideMainMenu();
	destroyComponent(&_select_checkpoint);
	_gui->hideMouseCursor();
	_breakout_->setState(breakout::GameState::ACTIVE);
	_breakout_->setCheckpoint(4);
	_breakout_->switchBackgroundSound();
	return true;
}

inline bool game2d::GameUI::handleSelectCheckpoint6(const CEGUI::EventArgs&) {
	hideMainMenu();
	destroyComponent(&_select_checkpoint);
	_gui->hideMouseCursor();
	_breakout_->setState(breakout::GameState::ACTIVE);
	_breakout_->setCheckpoint(5);
	_breakout_->switchBackgroundSound();
	return true;
}

inline bool game2d::GameUI::handleSelectCheckpoint7(const CEGUI::EventArgs&) {
	hideMainMenu();
	destroyComponent(&_select_checkpoint);
	_gui->hideMouseCursor();
	_breakout_->setState(breakout::GameState::ACTIVE);
	_breakout_->setCheckpoint(6);
	_breakout_->switchBackgroundSound();
	return true;
}

inline bool game2d::GameUI::handleSelectCheckpoint8(const CEGUI::EventArgs&) {
	hideMainMenu();
	destroyComponent(&_select_checkpoint);
	_gui->hideMouseCursor();
	_breakout_->setState(breakout::GameState::ACTIVE);
	_breakout_->setCheckpoint(7);
	_breakout_->switchBackgroundSound();
	return true;
}

inline bool game2d::GameUI::handleContinueGameClicked(const CEGUI::EventArgs&) {
	destroyComponent(&_select_checkpoint);
	hideMainMenu();
	_gui->hideMouseCursor();
	_breakout_->loadLastQuitCheckpoint();
	_breakout_->setState(breakout::GameState::ACTIVE);
	_breakout_->switchBackgroundSound();
	return true;
}

inline bool game2d::GameUI::handleSelectCheckpointClicked(const CEGUI::EventArgs&) {
	if (!_select_checkpoint) {
		createSelectCheckpoint();
	} else {
		destroyComponent(&_select_checkpoint);
	}
	return true;
}

inline bool game2d::GameUI::handleQuitClicked(const CEGUI::EventArgs&) {
	_breakout_->quitCurrentCheckpoint();
	glfwSetWindowShouldClose(const_cast<GLFWwindow*>(_window), GLFW_TRUE);
	return true;
}

inline game2d::GameUI& game2d::GameUI::operator=(const GameUI& ui) {
	if (this == &ui) {
		return *this;
	}

	_gui = ui.getGUI();
	_window = ui.getWindow();
	_menu = ui.getMenu();
	_process = ui.getProcess();
	_select_checkpoint = ui.getSelectCheckpoint();
	_result_checkpoint_prompt = ui.getResultCheckpointPrompt();
	_main_menu_btns = ui.getMainMenuButtons();
	_process_btns = ui.getProcessButtons();

	return *this;
}

inline gui::GUI* game2d::GameUI::operator->() const {
	return _gui;
}

inline gui::GUI* game2d::GameUI::getGUI() const {
	return _gui;
}

inline GLFWwindow* game2d::GameUI::getWindow() const {
	return _window;
}

inline void game2d::GameUI::setWindow(const GLFWwindow* window) {
	_window = const_cast<GLFWwindow*>(window);
}

inline CEGUI::Window* game2d::GameUI::getMenu() const {
	return _menu;
}

inline CEGUI::FrameWindow* game2d::GameUI::getProcess() const {
	return _process;
}

inline CEGUI::FrameWindow* game2d::GameUI::getSelectCheckpoint() const {
	return _select_checkpoint;
}

inline CEGUI::FrameWindow* game2d::GameUI::getResultCheckpointPrompt() const {
	return _result_checkpoint_prompt;
}

inline std::map<game2d::ButtonIndex::MainMenu, CEGUI::PushButton*> game2d::GameUI::getMainMenuButtons() const {
	return _main_menu_btns;
}

inline std::map<game2d::ButtonIndex::Process, CEGUI::PushButton*> game2d::GameUI::getProcessButtons() const {
	return _process_btns;
}
