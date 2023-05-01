#pragma once

struct InputCommands
{
	//Movement controls
	bool forward;
	bool back;
	bool right;
	bool left;
	bool up;
	bool down;
	bool rotRight;
	bool rotLeft;

	//Intended to be right-click
	bool activateCameraMovement;
	//Intended to be left-click
	bool mousePickingActive;

	//Mouse position tracking
	float mouseX;
	float mouseY;

	//Copy/paste controls
	bool copy;
	bool cut;
	bool paste;
	bool deleteObject;

	//Undo/redo
	bool undo;
	bool redo;
};