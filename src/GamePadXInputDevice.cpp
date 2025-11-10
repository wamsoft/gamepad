/****************************************************************************/
/*! @file
@brief XInput デバイス

-----------------------------------------------------------------------------
	Copyright (C) 2008 Takenori Imoto. All rights reserved.
	( http://www.kaede-software.com/ )
-----------------------------------------------------------------------------
@author		T.Imoto
@date		2008/01/21
@note

ソースコード形式であれバイナリ形式であれ、変更の有無に関わらず、以下の条件を満
たす限りにおいて、再配布および使用を許可します:

   1. ソースコード形式で再配布する場合、上記著作権表示、本条件書および下記責任
      限定規定を必ず含めてください。
   2. バイナリ形式で再配布する場合、上記著作権表示、本条件書および下記責任限定
      規定を、配布物とともに提供される文書および他の資料に必ず含めてください。

本ソフトウェアは著作権者によって、”現状のまま”提供されるものとします。本ソフ
トウェアについては、明示黙示を問わず、商用品として通常そなえるべき品質をそなえ
ているとの保証も、特定の目的に適合するとの保証を含め、何の保証もなされません。
事由のいかんを問わず、損害発生の原因いかんを問わず、且つ、責任の根拠が契約であ
るか厳格責任であるか (過失その他) 不法行為であるかを問わず、著作権者は仮にその
ような損害が発生する可能性を知らされていたとしても、本ソフトウェアの使用から発
生した直接損害、間接損害、偶発的な損害、特別損害、懲罰的損害または結果損害のい
ずれに対しても (代替品または サービスの提供;使用機会、データまたは利益の損失の
補償; または、業務の中断に対する補償を含め)責任をいっさい負いません。

*****************************************************************************/



#include "GamePadXInputDevice.h"
#include "GamePadDLLLoader.h"

namespace gamepad {

/*
DWORD WINAPI XInputGetState( DWORD dwUserIndex, XINPUT_STATE* pState );
DWORD WINAPI XInputSetState( DWORD dwUserIndex, XINPUT_VIBRATION* pVibration );
DWORD WINAPI XInputGetCapabilities( DWORD dwUserIndex, DWORD dwFlags, XINPUT_CAPABILITIES* pCapabilities );
void WINAPI XInputEnable( BOOL enable );
DWORD WINAPI XInputGetDSoundAudioDeviceGuids( DWORD dwUserIndex, GUID* pDSoundRenderGuid, GUID* pDSoundCaptureGuid );
DWORD WINAPI XInputGetBatteryInformation( DWORD dwUserIndex, BYTE devType, XINPUT_BATTERY_INFORMATION* pBatteryInformation );
DWORD WINAPI XInputGetKeystroke( DWORD dwUserIndex, DWORD dwReserved, PXINPUT_KEYSTROKE pKeystroke );
*/
class xinput
{
	static CDLLLoader			dll_;

	static bool LoadFunctions();
	static bool CheckLoading();

public:
	typedef DWORD (WINAPI *FuncXInputGetState)( DWORD dwUserIndex, XINPUT_STATE* pState );
	typedef DWORD (WINAPI *FuncXInputSetState)( DWORD dwUserIndex, XINPUT_VIBRATION* pVibration );
	typedef DWORD (WINAPI *FuncXInputGetCapabilities)( DWORD dwUserIndex, DWORD dwFlags, XINPUT_CAPABILITIES* pCapabilities );
	typedef void (WINAPI *FuncXInputEnable)( BOOL enable );
// 以下のメソッドは使わない
//	typedef DWORD (WINAPI *FuncXInputGetDSoundAudioDeviceGuids)( DWORD dwUserIndex, GUID* pDSoundRenderGuid, GUID* pDSoundCaptureGuid );
//	typedef DWORD (WINAPI *FuncXInputGetBatteryInformation)( DWORD dwUserIndex, BYTE devType, XINPUT_BATTERY_INFORMATION* pBatteryInformation );
//	typedef DWORD (WINAPI *FuncXInputGetKeystroke)( DWORD dwUserIndex, DWORD dwReserved, PXINPUT_KEYSTROKE pKeystroke );

public:
	static bool Initialize();

	static FuncXInputGetState						GetState;
	static FuncXInputSetState						SetState;
	static FuncXInputGetCapabilities				GetCapabilities;
	static FuncXInputEnable							Enable;
// 以下のメソッドは使わない
//	static FuncXInputGetDSoundAudioDeviceGuids		GetDSoundAudioDeviceGuids;
//	static FuncXInputGetBatteryInformation			GetBatteryInformation;
//	static FuncXInputGetKeystroke					GetKeystroke;
};

CDLLLoader xinput::dll_;
xinput::FuncXInputGetState						xinput::GetState = NULL;
xinput::FuncXInputSetState						xinput::SetState = NULL;
xinput::FuncXInputGetCapabilities				xinput::GetCapabilities = NULL;
xinput::FuncXInputEnable						xinput::Enable = NULL;

// 以下のメソッドは使わない
//xinput::FuncXInputGetDSoundAudioDeviceGuids		xinput::GetDSoundAudioDeviceGuids = NULL;
//xinput::FuncXInputGetBatteryInformation			xinput::GetBatteryInformation = NULL;
//xinput::FuncXInputGetKeystroke					xinput::GetKeystroke = NULL;

bool xinput::Initialize()
{
	if( !GetState || !SetState || !GetCapabilities || !Enable ) {
		return LoadFunctions();
	}
	return true;	// 読み込み済み
}
bool xinput::CheckLoading()
{
	if( dll_.IsLoaded() == false ) {
		dll_.Load( "xinput1_3.dll" );
	}
// 古いのも試す？
#if 0
	if( dll_.IsLoaded() == false ) {
		dll_.Load( "xinput1_2.dll" );
	}
	if( dll_.IsLoaded() == false ) {
		dll_.Load( "xinput1_1.dll" );
	}
#endif
	return dll_.IsLoaded();
}
bool xinput::LoadFunctions()
{
	if( !CheckLoading() ) {
		return false;
	}

	GetState = (FuncXInputGetState)dll_.GetProcAddress( "XInputGetState" );
	if( !GetState ) { return false; }

	SetState = (FuncXInputSetState)dll_.GetProcAddress( "XInputSetState" );
	if( !SetState ) { return false; }

	GetCapabilities = (FuncXInputGetCapabilities)dll_.GetProcAddress( "XInputGetCapabilities" );
	if( !GetCapabilities ) { return false; }

	Enable = (FuncXInputEnable)dll_.GetProcAddress( "XInputEnable" );
	if( !Enable ) { return false; }

// 以下のメソッドは使わない
/*
	GetDSoundAudioDeviceGuids = (FuncXInputGetDSoundAudioDeviceGuids)dll_.GetProcAddress( "XInputGetDSoundAudioDeviceGuids" );
	if( !GetDSoundAudioDeviceGuids ) { return false; }

	GetBatteryInformation = (FuncXInputGetBatteryInformation)dll_.GetProcAddress( "XInputGetBatteryInformation" );
	if( !GetBatteryInformation ) { return false; }

	GetKeystroke = (FuncXInputGetKeystroke)dll_.GetProcAddress( "XInputGetKeystroke" );
	if( !GetKeystroke ) { return false; }
*/
	return true;
}
CXInputDevice::CXInputDevice( unsigned long num, CInputDevicePort* port)
 : CInputDeviceBase(port), device_num_(num), is_connected_( true )
{
	ZeroMemory( &state_, sizeof(state_) );
	ZeroMemory( &cur_vibration_, sizeof(cur_vibration_) );
	ZeroMemory( &vibration_, sizeof(vibration_) );

	wchar_t		name_num[2] = {L'\0',L'\0'};
	if( num < 10 ) {
		name_num[0] = L'0' + (wchar_t)num;
	}
	name_ = std::wstring(L"XInput Game Pad ") + std::wstring( (wchar_t*)name_num );
}
void CXInputDevice::Update()
{
	DWORD ret = 0;
	if( is_connected_ ) {
		if( cur_vibration_.wLeftMotorSpeed != vibration_.wLeftMotorSpeed || 
			cur_vibration_.wRightMotorSpeed != vibration_.wRightMotorSpeed ) {
			ret = xinput::SetState( device_num_, &vibration_ );
			cur_vibration_ = vibration_;
			is_connected_ = (ret != ERROR_DEVICE_NOT_CONNECTED);
		}
		if( is_connected_ ) {
			ret = xinput::GetState( device_num_, &state_ );
			is_connected_ = (ret != ERROR_DEVICE_NOT_CONNECTED);
		}
		if( !is_connected_ ) {
			// 抜かれた模様。ゼロクリアしておく
			ZeroMemory( &state_, sizeof(state_) );
			ZeroMemory( &cur_vibration_, sizeof(cur_vibration_) );
		}
	} else {
		// 途中で切断されたので、つながっているかチェック。
		XINPUT_CAPABILITIES	cap;
		ret = xinput::GetCapabilities( device_num_, XINPUT_FLAG_GAMEPAD, &cap );
		is_connected_ = (ret != ERROR_DEVICE_NOT_CONNECTED);
	}
}
unsigned long CXInputDevice::GetKeyState() const
{
	return state_.Gamepad.wButtons;
#if 0
	unsigned long	ret = 0;
	WORD	btn = state_.Gamepad.wButtons;
	if( btn & XINPUT_GAMEPAD_DPAD_UP ) ret |= IDBTN_DPAD_UP;
	if( btn & XINPUT_GAMEPAD_DPAD_DOWN ) ret |= IDBTN_DPAD_DOWN;
	if( btn & XINPUT_GAMEPAD_DPAD_LEFT ) ret |= IDBTN_DPAD_LEFT;
	if( btn & XINPUT_GAMEPAD_DPAD_RIGHT ) ret |= IDBTN_DPAD_RIGHT;
	if( btn & XINPUT_GAMEPAD_START ) ret |= IDBTN_START;
	if( btn & XINPUT_GAMEPAD_BACK ) ret |= IDBTN_BACK;
	if( btn & XINPUT_GAMEPAD_LEFT_THUMB ) ret |= IDBTN_LEFT_THUMB;
	if( btn & XINPUT_GAMEPAD_RIGHT_THUMB ) ret |= IDBTN_RIGHT_THUMB;
	if( btn & XINPUT_GAMEPAD_LEFT_SHOULDER ) ret |= IDBTN_LEFT_SHOULDER;
	if( btn & XINPUT_GAMEPAD_RIGHT_SHOULDER ) ret |= IDBTN_RIGHT_SHOULDER;
	if( btn & XINPUT_GAMEPAD_A ) ret |= IDBTN_A;
	if( btn & XINPUT_GAMEPAD_B ) ret |= IDBTN_B;
	if( btn & XINPUT_GAMEPAD_X ) ret |= IDBTN_X;
	if( btn & XINPUT_GAMEPAD_Y ) ret |= IDBTN_Y;
	return ret;
#endif
}
double CXInputDevice::GetAxisValue( short val, short deadZone )
{
	if( deadZone >= val && val >= (-deadZone) ) {
		return 0.0;
	} else if( val >= 0 ) {
		return (double)(val-deadZone) / (double)(32767-deadZone);
	} else {
		return (double)(val+deadZone) / (double)(32768-deadZone);
	}
}
double CXInputDevice::GetLeftTrigger() const
{
	return state_.Gamepad.bLeftTrigger / 255.0;
}
double CXInputDevice::GetRightTrigger() const
{
	return state_.Gamepad.bRightTrigger / 255.0;
}
double CXInputDevice::GetLeftThumbStickX() const
{
	return GetAxisValue( state_.Gamepad.sThumbLX, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE );
}
double CXInputDevice::GetLeftThumbStickY() const
{
	return GetAxisValue( state_.Gamepad.sThumbLY, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE );
}
double CXInputDevice::GetRightThumbStickX() const
{
	return GetAxisValue( state_.Gamepad.sThumbRX, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE );
}
double CXInputDevice::GetRightThumbStickY() const
{
	return GetAxisValue( state_.Gamepad.sThumbRY, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE );
}
unsigned long CXInputDevice::Sensing() const
{
	unsigned long ret = state_.Gamepad.wButtons;

	short deadZone = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
	if( state_.Gamepad.sThumbLX > deadZone || state_.Gamepad.sThumbLX < (-deadZone) )
		ret |= IDBTN_LEFT_THUMB_AXIS_X;

	if( state_.Gamepad.sThumbLY > deadZone || state_.Gamepad.sThumbLY < (-deadZone) )
		ret |= IDBTN_LEFT_THUMB_AXIS_Y;

	deadZone = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
	if( state_.Gamepad.sThumbRX > deadZone || state_.Gamepad.sThumbRX < (-deadZone) )
		ret |= IDBTN_RIGHT_THUMB_AXIS_X;

	if( state_.Gamepad.sThumbRY > deadZone || state_.Gamepad.sThumbRY < (-deadZone) )
		ret |= IDBTN_RIGHT_THUMB_AXIS_Y;

	if( state_.Gamepad.bLeftTrigger )
		ret |= IDBTN_LEFT_TRIGGER;

	if( state_.Gamepad.bRightTrigger )
		ret |= IDBTN_RIGHT_TRIGGER;

	return ret;
}

// XInput デバイスの初期化を行う
long CXInputDevice::InitializeXInput()
{
	bool	is_xinput_enable = xinput::Initialize();
	if( !is_xinput_enable ) return 0;

	xinput::Enable( TRUE );

	long	ret = 0;
	for( DWORD i = 0; i < 4; ++i ) {
		XINPUT_CAPABILITIES	cap;
		DWORD caps = xinput::GetCapabilities( i, XINPUT_FLAG_GAMEPAD, &cap );
		if( caps == ERROR_SUCCESS ) {
			// connect xinput device
			ret |= 0x01 << i;
		} // else if( ret == ERROR_DEVICE_NOT_CONNECTED ) {}
	}
	if( ret == 0 ) {
		xinput::Enable( FALSE );
	}
	return ret;
}

}; // namespace gamepad


