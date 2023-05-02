#pragma once
// ReSharper disable once CppUnusedIncludeDirective
#include <d3d11.h>
#include <SimpleMath.h>

#include "InputCommands.h"

class Camera 
{
private:
	const float							m_normalMovespeed  = 0.20f;
	const float							m_fastMovespeed = 0.40f;

	void ChangeOrientationWithMouse(float mouseDeltaX, float mouseDeltaY);
	void ProcessCameraInput(const InputCommands& inputCommands);
public:
	Camera();
	void Update(const InputCommands& inputCommands);

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