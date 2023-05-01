#pragma once
#include "Command.h"
#include <vector>
#include "../../Renderer/DisplayObject.h"
class PasteCommand : public Command
{
public:
	PasteCommand(std::vector<DisplayObject>& displayList, const DisplayObject& objectToPaste);
	~PasteCommand() override = default;
	void Execute() override;
	void Undo() override;

private:
	std::vector<DisplayObject>& m_displayList;
	DisplayObject m_objectToPaste;
};