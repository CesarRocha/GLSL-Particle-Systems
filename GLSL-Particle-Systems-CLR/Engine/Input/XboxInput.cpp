//================================================================
//                     XboxInput.cpp
//================================================================
#include "XboxInput.hpp"

XBOXController::XBOXController(int playerNumber)
{
	_controllerNum = playerNumber - 1;
}

XINPUT_STATE XBOXController::GetState()
{
	ZeroMemory(&_controllerState, sizeof(XINPUT_STATE));

	XInputGetState(_controllerNum, &_controllerState);

	return _controllerState;
}

bool XBOXController::IsConnected()
{
	ZeroMemory(&_controllerState, sizeof(XINPUT_STATE));

	DWORD Result = XInputGetState(_controllerNum, &_controllerState);

	if(Result == ERROR_SUCCESS)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void XBOXController::Vibrate(int leftVal, int rightVal)
{
	XINPUT_VIBRATION Vibration;

	ZeroMemory(&Vibration, sizeof(XINPUT_VIBRATION));

	Vibration.wLeftMotorSpeed = (WORD)leftVal;
	Vibration.wRightMotorSpeed = (WORD)rightVal;

	XInputSetState(_controllerNum, &Vibration);
}