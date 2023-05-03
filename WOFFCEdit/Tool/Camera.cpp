#include "Camera.h"
#include "../Renderer/pch.h"

using namespace DirectX;
using namespace SimpleMath;

using Microsoft::WRL::ComPtr;

#ifndef PI_LONG
/**
 * \brief Pi to fifty digits
 */
constexpr auto PI_LONG = 3.14159265358979323846264338327950288419716939937510;
#endif

#ifndef PI_SHORT
/**
 * \brief Pi to the first five decimal digits
 */
constexpr auto PI_SHORT = 3.14159;
#endif

Camera::Camera() :
m_camPosition(Vector3::Zero), m_camOrientation(Vector3::Zero), m_camLookAt(Vector3::Zero), m_camLookDirection(Vector3::Zero), m_camRight(Vector3::Zero), m_prevMouseX(0.0f), m_prevMouseY(0.0f)
{
	//Initialise speed values
	m_movespeed = m_normalMovespeed;
	m_camRotRate = 0.5;

	//Initialise camera position values
	m_camPosition.y = 3.7f;
	m_camPosition.z = -3.5f;
}//End default constructor

void Camera::ChangeOrientationWithMouse(const float mouseDeltaX, const float mouseDeltaY)
{
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
}//End ChangeOrientationWithMouse

void Camera::ProcessCameraInput(const InputCommands& inputCommands)
{
	if (inputCommands.forward)	m_camPosition += m_camLookDirection * m_movespeed;
	if (inputCommands.back)		m_camPosition -= m_camLookDirection * m_movespeed;
	if (inputCommands.right)	m_camPosition += m_camRight			* m_movespeed;
	if (inputCommands.left)		m_camPosition -= m_camRight			* m_movespeed;
	if (inputCommands.up)		m_camPosition += Vector3::UnitY		* m_movespeed;
	if (inputCommands.down)		m_camPosition -= Vector3::UnitY		* m_movespeed;
}//End MoveCamera

void Camera::Update(const InputCommands& inputCommands)
{
	//Update the camera move speed based on user input
	m_movespeed = inputCommands.increaseMoveSpeed ? m_fastMovespeed : m_normalMovespeed;

	//Get change in mouse position from previous frame
	const float mouseDeltaX = inputCommands.mouseX - m_prevMouseX;
	const float mouseDeltaY = inputCommands.mouseY - m_prevMouseY;

	//Update previous mouse position
	m_prevMouseX = inputCommands.mouseX;
	m_prevMouseY = inputCommands.mouseY;

	//If we're using the mouse camera control
	if (inputCommands.activateCameraMovement)
	{
		ChangeOrientationWithMouse(mouseDeltaX, mouseDeltaY);
	}//End if

	//Create look direction from Euler angles in m_camOrientation
	m_camLookDirection.x = sin(m_camOrientation.y * PI_SHORT / 180) * cos(m_camOrientation.x * PI_SHORT / 180);
	m_camLookDirection.y = sin(m_camOrientation.x * PI_SHORT / 180);
	m_camLookDirection.z = cos(m_camOrientation.y * PI_SHORT / 180) * cos(m_camOrientation.x * PI_SHORT / 180);
	m_camLookDirection.Normalize();

	//Create right vector using the look direction
	m_camLookDirection.Cross(Vector3::UnitY, m_camRight);

	//Process user input and update camera
	ProcessCameraInput(inputCommands);

	//Update the camera look-at point
	m_camLookAt = m_camPosition + m_camLookDirection;
}//End Update