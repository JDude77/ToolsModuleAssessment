#include "DeleteCommand.h"

DeleteCommand::DeleteCommand(std::vector<DisplayObject>& displayList, int& selectedObjectID, const DisplayObject& objectDeleted)
	: m_displayList(displayList), m_selectedObjectID(selectedObjectID), m_objectDeleted(objectDeleted), m_deletedObjectID(selectedObjectID)
{
}//End constructor

void DeleteCommand::Execute()
{
	//Can't delete if nothing is selected
    if(m_selectedObjectID == -1) return;

    //Remove the object from the display list
    m_displayList.erase(m_displayList.begin() + m_selectedObjectID);

    //Set ID to -1 because we just deleted the object that was selected
    m_selectedObjectID = -1;
}//End Delete Execute

void DeleteCommand::Undo()
{
    //Re-set the ID of the selected object
    m_selectedObjectID = m_deletedObjectID;

	//Re-add the object to the display list
    m_displayList.insert(m_displayList.begin() + m_selectedObjectID, m_objectDeleted);
}//End Delete Undo