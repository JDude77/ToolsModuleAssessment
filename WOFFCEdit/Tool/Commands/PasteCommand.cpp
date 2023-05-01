#include "PasteCommand.h"

PasteCommand::PasteCommand(std::vector<DisplayObject>& displayList, const DisplayObject& objectToPaste)
	: m_displayList(displayList), m_objectToPaste(objectToPaste)
{

}//End constructor

void PasteCommand::Execute()
{
	//Can't paste if we don't have anything to paste
    if(m_objectToPaste.m_model == nullptr) return;

    //Slightly offset the position to prevent overlapping
    m_objectToPaste.m_position = DirectX::SimpleMath::Vector3
    (
		m_objectToPaste.m_position.x + 0.1f,
        m_objectToPaste.m_position.y + 0.1f,
        m_objectToPaste.m_position.z + 0.1f
    );

    //Create the new object in the display list
    m_displayList.push_back(m_objectToPaste);

}//End Paste Execute

void PasteCommand::Undo()
{
    //Remove the object from the back of the display list
    m_displayList.pop_back();

    //Reset the object offset
    m_objectToPaste.m_position = DirectX::SimpleMath::Vector3
    (
		m_objectToPaste.m_position.x - 0.1f,
        m_objectToPaste.m_position.y - 0.1f,
        m_objectToPaste.m_position.z - 0.1f
    );
}//End Paste Undo