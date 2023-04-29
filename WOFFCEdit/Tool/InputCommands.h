#pragma once

struct InputCommands
{
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

	float mouseX;
	float mouseY;
};