#pragma once
#include "Utilites/KeyCodes.h"
#include "Math/MathTypes.hpp"
#include <bitset>
#include <intsafe.h>
struct _XINPUT_STATE;
namespace CommonUtilities
{
	struct XboxPlayer
	{
		enum ID : uint8_t
		{
			One,
			Two,
			Three,
			Four,
			Count,
			Invalid
		};
	};

	//todo: Vibrations
	class XboxControllerInput
	{
		public:
			XboxControllerInput() = delete;
			explicit XboxControllerInput(XboxPlayer::ID aPlayerID);
			~XboxControllerInput() = default;

			void Refresh();

			bool IsPressed(Xbox::Button aXboxButton) const;
			bool WasPressedThisFrame(Xbox::Button aXboxButton) const;
			bool WasReleasedThisFrame(Xbox::Button aXboxButton) const;

			Vector2f GetLeftStickAxis() const;
			Vector2f GetRightStickAxis() const;

			float GetLeftTriggerAxis() const;
			float GetRightTriggerAxis() const;

			bool IsConnected() const;

			void SetPlayerID(XboxPlayer::ID aPlayerID);
			XboxPlayer::ID GetPlayerID();

			void ResetState();

			void SetVibration(float myLeftMotorValue = 0.0f, float myRightMotorValue = 0.0f);
		private:
			struct XboxControllerState
			{
				std::bitset<Xbox::ButtonsCount> ButtonsState {};
				Vector2f LeftStick { 0,0 };
				Vector2f RightStick { 0,0 };
				uint32_t PacketNumber { 0 };
				float LeftTrigger { 0 };
				float RightTrigger { 0 };
				bool IsConnected { false };
				WORD LeftMotor{ 0 };
				WORD RightMotor{ 0 };
			};

			void UpdateControllerState(const _XINPUT_STATE& aState);
			Vector2f GetStickValueFromXInput(int16_t aStickX, int16_t aStickY, uint16_t aDeadZone) const;
			float GetTriggerValueFromXInput(uint8_t aTriggerValue, uint8_t aTriggerThreshold) const;

			XboxControllerState myPreviousXboxControllerState {};
			XboxControllerState myCurrentXboxControllerState {};
			XboxPlayer::ID myPlayerID { XboxPlayer::Invalid };
	};
}
