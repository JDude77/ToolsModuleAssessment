#pragma once
#include "Command.h"
#include "../../Renderer/DisplayObject.h"

class MoveObjectCommand : public Command
{
public:
	MoveObjectCommand(int& selectedObjectID, int movedObjectID, DirectX::SimpleMath::Vector3& objectPosition, DirectX::SimpleMath::Vector3 previousPosition, DirectX::SimpleMath::Vector3 newPosition);
	void Execute() override;
	void Undo() override;

private:
	int& m_selectedObjectID;
	const int m_movedObjectID;
	DirectX::SimpleMath::Vector3* m_objectPositionPointer;
	DirectX::SimpleMath::Vector3 m_previousPosition;
	DirectX::SimpleMath::Vector3 m_newPosition;
};