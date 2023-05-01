#include "CutCommand.h"

CutCommand::CutCommand(std::vector<DisplayObject>& displayList, int& selectedObjectID, const DisplayObject& objectToCopy)
	: m_displayList(displayList), m_selectedObjectID(selectedObjectID), m_cutObjectID(selectedObjectID), m_objectToCopy(objectToCopy)
{
}//End constructor

void CutCommand::Execute()
{
	//Can't cut if nothing is selected
    if(m_selectedObjectID == -1) return;

    m_objectToCopy = m_displayList[m_selectedObjectID];

    //Remove the object from the display list as part of cut
    m_displayList.erase(m_displayList.begin() + m_selectedObjectID);

    //Set ID to -1 because we just cut the object that was selected
    m_selectedObjectID = -1;
}//End Cut Execute

void CutCommand::Undo()
{
    //Get the ID of the selected object back
    m_selectedObjectID = m_cutObjectID;

    //Re-add the object to the display list in its previous location
    m_displayList.insert(m_displayList.begin() + m_selectedObjectID, m_objectToCopy);
}//End Cut Undo