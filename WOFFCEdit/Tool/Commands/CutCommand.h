#pragma once
#include <vector>
#include "Command.h"
#include "../../Renderer/DisplayObject.h"

class CutCommand : public Command
{
public:
	CutCommand(std::vector<DisplayObject>& displayList, int& selectedObjectID, const DisplayObject& objectToCopy);
	~CutCommand() override = default;
	void Execute() override;
	void Undo() override;

private:
	std::vector<DisplayObject>& m_displayList;
	int& m_selectedObjectID;
	int m_cutObjectID;
	DisplayObject m_objectToCopy;
};