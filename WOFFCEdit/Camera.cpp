#include "pch.h"
#include "Camera.h"

using namespace DirectX;
using namespace SimpleMath;

using Microsoft::WRL::ComPtr;

Camera::Camera() :
m_camPosition(Vector3::Zero), m_camOrientation(Vector3::Zero), m_camLookAt(Vector3::Zero), m_camLookDirection(Vector3::Zero), m_camRight(Vector3::Zero), m_prevMouseX(0.0f), m_prevMouseY(0.0f)
{
	//Initialise speed values
	m_movespeed = 0.20;
	m_camRotRate = 0.5;

	//Initialise camera position values
	m_camPosition.y = 3.7f;
	m_camPosition.z = -3.5f;
}//End default constructor

void Camera::Update(const InputCommands m_InputCommands)
{
	if (m_InputCommands.activateCameraMovement)
	{
		//Get change in mouse position from previous frame
		const float mouseDeltaX = m_InputCommands.mouseX - m_prevMouseX;
		const float mouseDeltaY = m_InputCommands.mouseY - m_prevMouseY;

		//Update previous mouse position
		m_prevMouseX = m_InputCommands.mouseX;
		m_prevMouseY = m_InputCommands.mouseY;

		//Calculate change in orientation of the camera
		m_camOrientation.y -= mouseDeltaX * m_camRotRate;
		m_camOrientation.x -= mouseDeltaY * m_camRotRate;

		//Fix the pitch of the camera to not do flips
		if (m_camOrientation.x > 89.0f)
		{
			m_camOrientation.x = 89.0f;
		}//End if
		if (m_camOrientation.x < -89.0f)
		{
			m_camOrientation.x = -89.0f;
		}//End if
	}//End if

	//Create look direction from Euler angles in m_camOrientation
	m_camLookDirection.x = sin(m_camOrientation.y * 3.1415 / 180) * cos(m_camOrientation.x * 3.1415 / 180);
	m_camLookDirection.y = sin(m_camOrientation.x * 3.1415 / 180);
	m_camLookDirection.z = cos(m_camOrientation.y * 3.1415 / 180) * cos(m_camOrientation.x * 3.1415 / 180);
	m_camLookDirection.Normalize();

	//Create right vector using the look direction
	m_camLookDirection.Cross(Vector3::UnitY, m_camRight);

	//Process user input and update camera
	if (m_InputCommands.forward)
	{
		m_camPosition += m_camLookDirection * m_movespeed;
	}//End if
	if (m_InputCommands.back)
	{
		m_camPosition -= m_camLookDirection * m_movespeed;
	}//End if
	if (m_InputCommands.right)
	{
		m_camPosition += m_camRight * m_movespeed;
	}//End if
	if (m_InputCommands.left)
	{
		m_camPosition -= m_camRight * m_movespeed;
	}//End if
	if (m_InputCommands.up)
	{
		m_camPosition += Vector3::UnitY * m_movespeed;
	}//End if
	if (m_InputCommands.down)
	{
		m_camPosition -= Vector3::UnitY * m_movespeed;
	}//End if

	//Update the camera look-at point
	m_camLookAt = m_camPosition + m_camLookDirection;
}//End Update