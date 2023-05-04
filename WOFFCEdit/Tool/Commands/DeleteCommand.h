#pragma once
#include "Command.h"
#include <vector>
#include "../../Renderer/DisplayObject.h"

class DeleteCommand : public Command
{
	public:
	DeleteCommand(std::vector<DisplayObject>& displayList, int& selectedObjectID, const DisplayObject& objectDeleted);
	~DeleteCommand() override = default;
	void Execute() override;
	void Undo() override;

private:
	std::vector<DisplayObject>& m_displayList;
	int& m_selectedObjectID;
	int m_deletedObjectID;
	DisplayObject m_objectDeleted;
};