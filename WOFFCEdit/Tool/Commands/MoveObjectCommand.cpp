#include "MoveObjectCommand.h"

MoveObjectCommand::MoveObjectCommand(int& selectedObjectID, const int movedObjectID, DirectX::SimpleMath::Vector3& objectPosition, DirectX::SimpleMath::Vector3 previousPosition, DirectX::SimpleMath::Vector3 newPosition)
	: m_selectedObjectID(selectedObjectID), m_movedObjectID(movedObjectID), m_objectPositionPointer(&objectPosition), m_previousPosition(previousPosition), m_newPosition(newPosition)
{
}//End constructor

void MoveObjectCommand::Execute()
{
	//Set the selected ID to be the moved object (done here in case of Redo)
	m_selectedObjectID = m_movedObjectID;

	//Set the position to the new position post-drag
	*m_objectPositionPointer = m_newPosition;
}//End MoveObject Execute

void MoveObjectCommand::Undo()
{
	//Set the position to the old position pre-drag
	*m_objectPositionPointer = m_previousPosition;

	//Set the ID to be the object just moved
	m_selectedObjectID = m_movedObjectID;
}//End MoveObject Undo