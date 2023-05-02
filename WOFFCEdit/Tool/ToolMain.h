#pragma once

#include <afxext.h>
#include "../Renderer/pch.h"
#include "../Renderer/Game.h"
#include "../SQLITE/sqlite3.h"
#include "SceneObject.h"
#include "InputCommands.h"
#include <vector>

class ToolMain
{
#pragma region Methods
public:
	ToolMain();
	~ToolMain();

	//onAction - These are the interface to MFC
	int				getCurrentSelectionID();								//Returns the selection number of currently selected object so that it can be displayed
	void			onActionInitialise(HWND handle, int width, int height);	//Passes through handle and hieght and width to initialise DirectX renderer and SQL LITE
	void			onActionFocusCamera();
	void			onActionLoad();											//Load the current chunk
	afx_msg	void	onActionSave();											//Save the current chunk
	afx_msg void	onActionSaveTerrain();									//Save chunk geometry
	afx_msg void	onActionUndo();											//Undo an action
	afx_msg void	onActionRedo();											//Redo an undone action

	void	Tick(MSG* msg);
	void	UpdateInput(const MSG* msg);

private:	
	void	onContentAdded();
#pragma endregion

#pragma region Variables
public:
	std::vector<SceneObject>    m_sceneGraph;		//Our SceneGraph storing all the objects in the current chunk
	ChunkObject					m_chunk;			//Our landscape chunk
	int							m_selectedObject;	//ID of current selection

private:
	HWND			m_toolHandle;					//Handle to the window
	Game			m_d3dRenderer;					//Instance of D3D rendering system for our tool
	InputCommands	m_toolInputCommands;			//Input commands that we want to use and possibly pass over to the renderer
	CRect			WindowRECT;						//Window area rectangle
	char			m_keyArray[256];
	sqlite3*		m_databaseConnection;			//SQL database handle

	int m_width;									//Dimensions passed to directX
	int m_height;
	int m_currentChunk;								//The current chunk of the database that we are operating on - dictates loading and saving

	bool m_executeOnce;								//Hard-stop to prevent multiple commands activating when only one is intended
#pragma endregion
};