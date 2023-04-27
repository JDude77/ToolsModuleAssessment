#pragma once

#include "DisplayObject.h"
#include "InputCommands.h"

class Camera 
{
public:
	Camera();
	void Update(InputCommands m_InputCommands);

	float								m_movespeed;
	float								m_camRotRate;
	DirectX::SimpleMath::Vector3		m_camPosition;
	DirectX::SimpleMath::Vector3		m_camOrientation;
	DirectX::SimpleMath::Vector3		m_camLookAt;
	DirectX::SimpleMath::Vector3		m_camLookDirection;
	DirectX::SimpleMath::Vector3		m_camRight;

	float								m_prevMouseX;
	float								m_prevMouseY;
};