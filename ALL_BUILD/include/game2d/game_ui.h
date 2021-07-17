/*!
 * \file game_ui.h
 * \date 2020/11/07 14:34
 *
 * \author Kaisa.K.Kur
 * Contact: ******@***.com
 *
 */
 
/***************************************************************************
 * Copyright (C) 2020
 *
 * \brief 
 *
 * 
 *
 * \note
 ***************************************************************************/

#ifndef _GAME_UI_H_
#define _GAME_UI_H_

#include <GUI/gui.h>

#include <breakout/breakout.h>

namespace game2d {

/* 游戏实例 */
extern std::unique_ptr<breakout::Breakout> _breakout_;

namespace ButtonIndex {

enum class MainMenu {
	NEW_GAME, CONTINUE_GAME, SELECT_CHECKPOINT, QUIT
};

enum class Process {
	RETRY, NEXT_CHECKPOINT, BACK_MAIN_MENU
};

}

class GameUI {
public:
	GameUI();
	GameUI(const GLFWwindow* window);
	virtual ~GameUI();

public:
	inline virtual void initUI();
	inline virtual void updata();
	
private:
	inline virtual void setMainMenu();
	inline virtual void showMainMenu();
	inline virtual void hideMainMenu();
	inline virtual void createProcess();
	inline virtual void createSelectCheckpoint();
	inline virtual void createResultCheckpointPrompt();
	inline virtual void destroyComponent(CEGUI::Window** component);
	inline virtual void destroyComponent(CEGUI::FrameWindow** component);
	inline virtual void losePrompt();
	inline virtual void winPrompt();
	inline virtual void loadReserveTextures();

private:
	inline virtual bool handleRootKeyDown(const CEGUI::EventArgs& args);
	inline virtual bool handleProcessCloseButton(const CEGUI::EventArgs&);
	inline virtual bool handleNewGameClicked(const CEGUI::EventArgs&);
	inline virtual bool handleContinueGameClicked(const CEGUI::EventArgs&);
	inline virtual bool handleSelectCheckpointClicked(const CEGUI::EventArgs&);
	inline virtual bool handleQuitClicked(const CEGUI::EventArgs&);
	inline virtual bool handleRetryGameClicked(const CEGUI::EventArgs&);
	inline virtual bool handleBackMainMenuClicked(const CEGUI::EventArgs&);
	inline virtual bool handleNextGameClicked(const CEGUI::EventArgs&);
	inline virtual bool handleSelectCheckpointCloseButton(const CEGUI::EventArgs&);
	inline virtual bool handleResultCheckpointPromptCloseButton(const CEGUI::EventArgs&);
	inline virtual bool handleSelectCheckpoint1(const CEGUI::EventArgs&);
	inline virtual bool handleSelectCheckpoint2(const CEGUI::EventArgs&);
	inline virtual bool handleSelectCheckpoint3(const CEGUI::EventArgs&);
	inline virtual bool handleSelectCheckpoint4(const CEGUI::EventArgs&);
	inline virtual bool handleSelectCheckpoint5(const CEGUI::EventArgs&);
	inline virtual bool handleSelectCheckpoint6(const CEGUI::EventArgs&);
	inline virtual bool handleSelectCheckpoint7(const CEGUI::EventArgs&);
	inline virtual bool handleSelectCheckpoint8(const CEGUI::EventArgs&);

public:
	inline virtual GameUI& operator=(const GameUI& ui); // 由于需求，这里不位指针重新分配地址
	inline virtual gui::GUI* operator->() const;

public:
	inline virtual gui::GUI* getGUI() const;
	inline virtual GLFWwindow* getWindow() const;
	inline virtual void setWindow(const GLFWwindow* window);
	inline virtual CEGUI::Window* getMenu() const;
	inline virtual CEGUI::FrameWindow* getProcess() const;
	inline virtual CEGUI::FrameWindow* getSelectCheckpoint() const;
	inline virtual CEGUI::FrameWindow* getResultCheckpointPrompt() const;
	inline virtual std::map<ButtonIndex::MainMenu, CEGUI::PushButton*>	getMainMenuButtons() const;
	inline virtual std::map<ButtonIndex::Process, CEGUI::PushButton*>	getProcessButtons() const;

private:
	gui::GUI*			_gui;
	GLFWwindow*			_window; /* 捕获窗口容器 */
	CEGUI::Window*		_menu;
	CEGUI::FrameWindow* _process;
	CEGUI::FrameWindow* _select_checkpoint;
	CEGUI::FrameWindow* _result_checkpoint_prompt;
	std::map<ButtonIndex::MainMenu, CEGUI::PushButton*> _main_menu_btns;
	std::map<ButtonIndex::Process, CEGUI::PushButton*>	_process_btns;
};

}

#endif // !_GAME_UI_H_
