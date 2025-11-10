/****************************************************************************/
/*! @file
@brief ゲームパッドポート

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

#define _CRT_SECURE_NO_DEPRECATE

#ifndef _WIN32_DCOM
#define _WIN32_DCOM
#endif

#include <windows.h>
#include <wbemidl.h>
#include <oleauto.h>
#include <objbase.h>
#include <comdef.h>
#include <comutil.h>
#include <atlbase.h>
#include <iostream>
#include <dinput.h>
#include <dinputd.h>

#include "GamePadInputDevicePort.h"
#include "GamePadDLLLoader.h"
#include "GamePadXInputDevice.h"
#include "GamePadLog.h"
#include "GamePadDeviceInspector.h"
#include "GamePadForceFeedbackDevice.h"

#if 0
// dxguid.lib dxerr.lib dinput8.lib comctl32.lib
//#pragma comment(lib, "comsuppw.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput8.lib")
#else

#ifdef DEFINE_GUID
#undef DEFINE_GUID
#endif

#define DEFINE_GUID( name, a, b, c, d, e, f, g, h, i, j, k )	static const GUID name = { a, b, c, { d, e, f, g, h, i, j, k } };

DEFINE_GUID(IID_IDirectInput8A,			0xBF798030,0x483A,0x4DA2,0xAA,0x99,0x5D,0x64,0xED,0x36,0x97,0x00);
DEFINE_GUID(IID_IDirectInput8W,			0xBF798031,0x483A,0x4DA2,0xAA,0x99,0x5D,0x64,0xED,0x36,0x97,0x00);
DEFINE_GUID(IID_IDirectInputJoyConfig8,	0xeb0d7dfa,0x1990,0x4f27,0xb4,0xd6,0xed,0xf2,0xee,0xc4,0xa4,0x4c);
DEFINE_GUID(GUID_XAxis,   0xA36D02E0,0xC9F3,0x11CF,0xBF,0xC7,0x44,0x45,0x53,0x54,0x00,0x00);
DEFINE_GUID(GUID_YAxis,   0xA36D02E1,0xC9F3,0x11CF,0xBF,0xC7,0x44,0x45,0x53,0x54,0x00,0x00);
DEFINE_GUID(GUID_ZAxis,   0xA36D02E2,0xC9F3,0x11CF,0xBF,0xC7,0x44,0x45,0x53,0x54,0x00,0x00);
DEFINE_GUID(GUID_RxAxis,  0xA36D02F4,0xC9F3,0x11CF,0xBF,0xC7,0x44,0x45,0x53,0x54,0x00,0x00);
DEFINE_GUID(GUID_RyAxis,  0xA36D02F5,0xC9F3,0x11CF,0xBF,0xC7,0x44,0x45,0x53,0x54,0x00,0x00);
DEFINE_GUID(GUID_RzAxis,  0xA36D02E3,0xC9F3,0x11CF,0xBF,0xC7,0x44,0x45,0x53,0x54,0x00,0x00);
DEFINE_GUID(GUID_Slider,  0xA36D02E4,0xC9F3,0x11CF,0xBF,0xC7,0x44,0x45,0x53,0x54,0x00,0x00);
DEFINE_GUID(GUID_Button,  0xA36D02F0,0xC9F3,0x11CF,0xBF,0xC7,0x44,0x45,0x53,0x54,0x00,0x00);
DEFINE_GUID(GUID_Key,     0x55728220,0xD33C,0x11CF,0xBF,0xC7,0x44,0x45,0x53,0x54,0x00,0x00);
DEFINE_GUID(GUID_POV,     0xA36D02F2,0xC9F3,0x11CF,0xBF,0xC7,0x44,0x45,0x53,0x54,0x00,0x00);
DEFINE_GUID(GUID_Unknown, 0xA36D02F3,0xC9F3,0x11CF,0xBF,0xC7,0x44,0x45,0x53,0x54,0x00,0x00);

DEFINE_GUID(GUID_ConstantForce, 0x13541C20,0x8E33,0x11D0,0x9A,0xD0,0x00,0xA0,0xC9,0xA0,0x6E,0x35);

#undef DEFINE_GUID

static DIOBJECTDATAFORMAT c_rgodfDIJoy[] = {
	{ &GUID_XAxis, 0, DIDFT_AXIS | DIDFT_ANYINSTANCE | 0x80000000, DIDOI_ASPECTPOSITION },  
	{ &GUID_YAxis, 4, DIDFT_AXIS | DIDFT_ANYINSTANCE | 0x80000000, DIDOI_ASPECTPOSITION },
	{ &GUID_ZAxis, 8, DIDFT_AXIS | DIDFT_ANYINSTANCE | 0x80000000, DIDOI_ASPECTPOSITION },
	{ &GUID_RxAxis, 12, DIDFT_AXIS | DIDFT_ANYINSTANCE | 0x80000000, DIDOI_ASPECTPOSITION },
	{ &GUID_RyAxis, 16, DIDFT_AXIS | DIDFT_ANYINSTANCE | 0x80000000, DIDOI_ASPECTPOSITION },
	{ &GUID_RzAxis, 20, DIDFT_AXIS | DIDFT_ANYINSTANCE | 0x80000000, DIDOI_ASPECTPOSITION },
	{ &GUID_Slider, 24, DIDFT_AXIS | DIDFT_ANYINSTANCE | 0x80000000, DIDOI_ASPECTPOSITION },
	{ &GUID_Slider, 28, DIDFT_AXIS | DIDFT_ANYINSTANCE | 0x80000000, DIDOI_ASPECTPOSITION },
	{ &GUID_POV, 32, DIDFT_POV | DIDFT_ANYINSTANCE | 0x80000000, 0x00000000 },
	{ &GUID_POV, 36, DIDFT_POV | DIDFT_ANYINSTANCE | 0x80000000, 0x00000000 },
	{ &GUID_POV, 40, DIDFT_POV | DIDFT_ANYINSTANCE | 0x80000000, 0x00000000 },
	{ &GUID_POV, 44, DIDFT_POV | DIDFT_ANYINSTANCE | 0x80000000, 0x00000000 },
	{ NULL, 48, DIDFT_BUTTON | DIDFT_ANYINSTANCE | 0x80000000, 0x00000000 },
	{ NULL, 49, DIDFT_BUTTON | DIDFT_ANYINSTANCE | 0x80000000, 0x00000000 },
	{ NULL, 50, DIDFT_BUTTON | DIDFT_ANYINSTANCE | 0x80000000, 0x00000000 },
	{ NULL, 51, DIDFT_BUTTON | DIDFT_ANYINSTANCE | 0x80000000, 0x00000000 },
	{ NULL, 52, DIDFT_BUTTON | DIDFT_ANYINSTANCE | 0x80000000, 0x00000000 },
	{ NULL, 53, DIDFT_BUTTON | DIDFT_ANYINSTANCE | 0x80000000, 0x00000000 },
	{ NULL, 54, DIDFT_BUTTON | DIDFT_ANYINSTANCE | 0x80000000, 0x00000000 },
	{ NULL, 55, DIDFT_BUTTON | DIDFT_ANYINSTANCE | 0x80000000, 0x00000000 },
	{ NULL, 56, DIDFT_BUTTON | DIDFT_ANYINSTANCE | 0x80000000, 0x00000000 },
	{ NULL, 57, DIDFT_BUTTON | DIDFT_ANYINSTANCE | 0x80000000, 0x00000000 },
	{ NULL, 58, DIDFT_BUTTON | DIDFT_ANYINSTANCE | 0x80000000, 0x00000000 },
	{ NULL, 59, DIDFT_BUTTON | DIDFT_ANYINSTANCE | 0x80000000, 0x00000000 },
	{ NULL, 60, DIDFT_BUTTON | DIDFT_ANYINSTANCE | 0x80000000, 0x00000000 },
	{ NULL, 61, DIDFT_BUTTON | DIDFT_ANYINSTANCE | 0x80000000, 0x00000000 },
	{ NULL, 62, DIDFT_BUTTON | DIDFT_ANYINSTANCE | 0x80000000, 0x00000000 },
	{ NULL, 63, DIDFT_BUTTON | DIDFT_ANYINSTANCE | 0x80000000, 0x00000000 },
	{ NULL, 64, DIDFT_BUTTON | DIDFT_ANYINSTANCE | 0x80000000, 0x00000000 },
	{ NULL, 65, DIDFT_BUTTON | DIDFT_ANYINSTANCE | 0x80000000, 0x00000000 },
	{ NULL, 66, DIDFT_BUTTON | DIDFT_ANYINSTANCE | 0x80000000, 0x00000000 },
	{ NULL, 67, DIDFT_BUTTON | DIDFT_ANYINSTANCE | 0x80000000, 0x00000000 },
	{ NULL, 68, DIDFT_BUTTON | DIDFT_ANYINSTANCE | 0x80000000, 0x00000000 },
	{ NULL, 69, DIDFT_BUTTON | DIDFT_ANYINSTANCE | 0x80000000, 0x00000000 },
	{ NULL, 70, DIDFT_BUTTON | DIDFT_ANYINSTANCE | 0x80000000, 0x00000000 },
	{ NULL, 71, DIDFT_BUTTON | DIDFT_ANYINSTANCE | 0x80000000, 0x00000000 },
	{ NULL, 72, DIDFT_BUTTON | DIDFT_ANYINSTANCE | 0x80000000, 0x00000000 },
	{ NULL, 73, DIDFT_BUTTON | DIDFT_ANYINSTANCE | 0x80000000, 0x00000000 },
	{ NULL, 74, DIDFT_BUTTON | DIDFT_ANYINSTANCE | 0x80000000, 0x00000000 },
	{ NULL, 75, DIDFT_BUTTON | DIDFT_ANYINSTANCE | 0x80000000, 0x00000000 },
	{ NULL, 76, DIDFT_BUTTON | DIDFT_ANYINSTANCE | 0x80000000, 0x00000000 },
	{ NULL, 77, DIDFT_BUTTON | DIDFT_ANYINSTANCE | 0x80000000, 0x00000000 },
	{ NULL, 78, DIDFT_BUTTON | DIDFT_ANYINSTANCE | 0x80000000, 0x00000000 },
	{ NULL, 79, DIDFT_BUTTON | DIDFT_ANYINSTANCE | 0x80000000, 0x00000000 }
};
const DIDATAFORMAT c_dfDIJoystick = { 24, 16, DIDF_ABSAXIS, 80, 44, c_rgodfDIJoy};
#endif

namespace gamepad {

#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

CDLLLoader CInputDevicePort::dinput_dll_;

CInputDevicePort::CInputDevicePort()
{
}
CInputDevicePort::~CInputDevicePort()
{
	ClearAllDevice();
}
void CInputDevicePort::ClearAllDevice()
{
	for( std::list<IInputDevice*>::iterator i = controllers_.begin(); i != controllers_.end(); ++i ) {
		delete (*i);
		(*i) = NULL;
	}
	controllers_.clear();
}
IInputDevice* CInputDevicePort::GetController( size_t idx )
{
	if( idx < GetNumberOfEnableControllers() ) {
		size_t cnt = 0;
		for( std::list<IInputDevice*>::iterator i = controllers_.begin(); i != controllers_.end() && cnt <= idx; ++i, ++cnt ) {
			if( cnt == idx ) {
				return (*i);
			}
		}
	}
	return NULL;
}
void CInputDevicePort::InitializeControllers( HWND hWnd )
{
	ClearAllDevice();

	long	xinput_ret = CXInputDevice::InitializeXInput();
	if( xinput_ret ) {
		// どれがつながっているかチェック
		if( xinput_ret & CXInputDevice::UserIndex0 )
			PutTempControllers( new CXInputDevice(0,const_cast<CInputDevicePort*>(this)) );
		if( xinput_ret & CXInputDevice::UserIndex1 )
			PutTempControllers( new CXInputDevice(1,const_cast<CInputDevicePort*>(this)) );
		if( xinput_ret & CXInputDevice::UserIndex2 )
			PutTempControllers( new CXInputDevice(2,const_cast<CInputDevicePort*>(this)) );
		if( xinput_ret & CXInputDevice::UserIndex3 )
			PutTempControllers( new CXInputDevice(3,const_cast<CInputDevicePort*>(this)) );
	}

	// Direct Input DLL の読み込みと DirectInput の生成
	InitializeDirectInput();

	if( xinput_ret ) {
		// XInput デバイスがつながっている時は、DirectInput の検索で
		// XInputが引っかからないようにデバイスを検索し、列挙時チェックする
		FindXInputDevice();
	}

	// コントロールパネル - ゲームコントローラで優先デバイスに設定されているものの GUID を取得しておく
	GetPreferredDevice();

	// DirectInput デバイスを検索してリストに入れる
	FindDirectInputDevice();

	InitializeAllDevice( hWnd );
}
void CInputDevicePort::InitializeAllDevice( HWND hWnd )
{
	for( std::list<IInputDevice*>::iterator i = controllers_.begin(); i != controllers_.end();  ) {
		bool	ret = (*i)->Initialize( hWnd );
		std::list<IInputDevice*>::iterator cur = i;
		++i;
		if( !ret ) {
			// デバイスの初期化に失敗。失敗したデバイスはリストから削除
			delete (*cur);
			controllers_.erase( cur );
		}
	}
}
void CInputDevicePort::InitializeDirectInput()
{
	if( direct_input_.p ) {
		Log( L"Already Initizelized DirectInput." );
		return;
	}

	if( dinput_dll_.IsLoaded() == false ) {
#ifdef _DEBUG
		dinput_dll_.Load( "dinput8d.dll" );
#else
		dinput_dll_.Load( "dinput8.dll" );
#endif
	}

	if( dinput_dll_.IsLoaded() == false ) {
		Log( L"Cannot load dinput8.dll." );
		return;
	}

	typedef HRESULT (WINAPI *FuncDirectInput8Create)(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID *ppvOut, LPUNKNOWN punkOuter);

	FuncDirectInput8Create	pDirectInputCreate = NULL;
	pDirectInputCreate = (FuncDirectInput8Create)dinput_dll_.GetProcAddress( "DirectInput8Create" );
	if( !pDirectInputCreate ) {
		Log( L"Cannot get API." );
		return;
	}

	HRESULT		hr;
	if( FAILED( hr = pDirectInputCreate( GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&direct_input_, NULL ) ) ) {
		Log( L"Failed to Create DirectInput.", hr );
		return;
	}
}
void CInputDevicePort::FindDirectInputDevice()
{
	if( !direct_input_ ) {
		Log( L"Could not create DirectInput." );
		return;
	}

	HRESULT     hr;
	if( FAILED( hr = direct_input_->EnumDevices( DI8DEVCLASS_GAMECTRL, EnumDevicesCallback, (void*)this, DIEDFL_ATTACHEDONLY ) ) ) {
		Log( L"Failed to Call EnumDevices.", hr );
		return;
	}
}
BOOL CALLBACK CInputDevicePort::EnumDevicesCallback( const DIDEVICEINSTANCE* pInst, VOID* pContext )
{
	return reinterpret_cast<CInputDevicePort*>(pContext)->EnumDevicesCallback(pInst) ? DIENUM_CONTINUE : DIENUM_STOP;
}
bool CInputDevicePort::EnumDevicesCallback( const DIDEVICEINSTANCE* pInst )
{
	if( !direct_input_ ) {
		return false;
	}

	// XInput Device はここでは加えない
	if( IsXInputDevice( pInst->guidProduct ) ) {
		return true;
	}

	// 優先デバイスかどうかの判定
	bool	is_preferred_device = ( is_valid_preferred_device_guid_ && IsEqualGUID( pInst->guidInstance, preferred_device_guid_ ) );

	HRESULT					hr;
	CComPtr<IDirectInputDevice8>	pDevice;
	if( SUCCEEDED(hr = direct_input_->CreateDevice( pInst->guidInstance, &pDevice, NULL )) ) {
		IInputDevice*	device = NULL;
		if( CGamePadDeviceInspector::IsForceFeedbackDevice( pDevice ) ) {
			device = new CForceFeedbackDevice( pDevice, const_cast<CInputDevicePort*>(this) );
		} else {
			device = new CDirectInputDevice( pDevice, const_cast<CInputDevicePort*>(this) );
		}

		if( device ) {
			if( !is_preferred_device ) {
				PutTempControllers( device );
			} else {
				// 優先デバイスはリストの先頭に入れる
				PutTempControllersFront( device );
			}
		}
	}
	return true;
}
bool CInputDevicePort::CreateDirectInputDevice( GUID& instGuid, IDirectInputDevice8** device )
{
	HRESULT hr;
	if( FAILED(hr = direct_input_->CreateDevice( instGuid, device, NULL )) ) {
		Log( L"Failed to Call QueryInterface.", hr );
		return false;
	}
	return true;
}
/*
int CInputDevicePort::GetEmptyPort()
{
	int		ret = 0;
	for( std::list<IInputDevice*>::const_iterator i = controllers_.begin(); != controllers_.end() ++i, ++ret ) {
		if( (*i)->GetTypeOfDevice() == IDT_Null ) {
			return ret;
		}
	}
	return -1;	// not found empty port
}
bool CInputDevicePort::InsertDeviceToPort( int num, IInputDevice* device )
{
	if( num >= 0 && num < NUM_OF_PORTS && device ) {
		if( controllers_[num] ) {
			delete controllers_[num];
		}
		controllers_[num] = device;
		return true;
	}
	return false;
}
*/
//! コントロールパネル - ゲームコントローラで優先デバイスに設定されているジョイスティックのGUIDを得る
void CInputDevicePort::GetPreferredDevice()
{
	DIJOYCONFIG		PreferredJoyCfg = {0};
	is_valid_preferred_device_guid_ = false;

	HRESULT	hr;
	CComPtr<IDirectInputJoyConfig8>			pJoyConfig;
	if( FAILED( hr = direct_input_->QueryInterface( IID_IDirectInputJoyConfig8, (void**)&pJoyConfig ) ) ) {
		Log( L"Failed to Call QueryInterface.", hr );
		return;
	}

	PreferredJoyCfg.dwSize = sizeof(PreferredJoyCfg);
	// このメソッドはJoystickがつながっていないと失敗する
	if( SUCCEEDED( pJoyConfig->GetConfig(0, &PreferredJoyCfg, DIJC_GUIDINSTANCE ) ) ) {
		is_valid_preferred_device_guid_ = true;
		preferred_device_guid_ = PreferredJoyCfg.guidInstance;
	}
}

// XInput デバイスを検索して、VIDとPIDを保持しておく
// XInput デバイスの検索には WMI を使用している
bool CInputDevicePort::FindXInputDevice()
{
	HRESULT	hr;
	try {
	    if( FAILED(hr = CoInitialize(NULL)) )
			throw _com_error(hr);

		CComPtr<IWbemLocator> pIWbemLocator;
		if( FAILED(hr = pIWbemLocator.CoCreateInstance(__uuidof(WbemLocator), NULL, CLSCTX_INPROC_SERVER ) ) )
			throw _com_error(hr);

		CComPtr<IWbemServices> pIWbemServices;
		if( FAILED(hr = pIWbemLocator->ConnectServer( _bstr_t(L"\\\\.\\root\\cimv2"), NULL, NULL, 0L, 0L, NULL, NULL, &pIWbemServices )) )
			throw _com_error(hr);

		// Switch security level to IMPERSONATE
		CoSetProxyBlanket( pIWbemServices, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, 0 );                    

		// Get list of Win32_PNPEntity devices
		CComPtr<IEnumWbemClassObject> pEnumDevices;
		if( FAILED(hr = pIWbemServices->CreateInstanceEnum( _bstr_t(L"Win32_PNPEntity"), 0, NULL, &pEnumDevices ) ) )
			throw _com_error(hr);

	    for( ;; ) {
			DWORD	uReturned = 0;
			CComPtr<IWbemClassObject> pDevice;
			if( FAILED(hr = pEnumDevices->Next( 10000, 1, &pDevice, &uReturned ) ) )
				throw _com_error(hr);

			if( uReturned == 0 ) break;

			VARIANT var;
			hr = pDevice->Get( _bstr_t(L"DeviceID"), 0L, &var, NULL, NULL );
			if( SUCCEEDED( hr ) && var.vt == VT_BSTR && var.bstrVal != NULL ) {
				// Check if the device ID contains "IG_".  If it does, then it an XInput device
				// Unfortunately this information can not be found by just using DirectInput 
				if( wcsstr( var.bstrVal, L"IG_" ) )
				{
					// If it does, then get the VID/PID from var.bstrVal
					DWORD dwPid = 0, dwVid = 0;
					WCHAR* strVid = wcsstr( var.bstrVal, L"VID_" );
					if( strVid && swscanf( strVid, L"VID_%4X", &dwVid ) != 1 )
						dwVid = 0;

					WCHAR* strPid = wcsstr( var.bstrVal, L"PID_" );
					if( strPid && swscanf( strPid, L"PID_%4X", &dwPid ) != 1 )
						dwPid = 0;

					DWORD dwVidPid = MAKELONG( dwVid, dwPid );
#if 0
					std::wstring	name;
					hr = pDevice->Get( _bstr_t(L"Name"), 0L, &var, NULL, NULL );
					if( SUCCEEDED( hr ) && var.vt == VT_BSTR && var.bstrVal != NULL ) {
						name = std::wstring( (const wchar_t *)var.bstrVal );
					}
#endif
					xinput_vid_pids_.push_back( dwVidPid );
				}
			}
		}
	} catch( _com_error &e ) {
		Log( e.Description() );
	} catch( ... ) {
	}
	CoUninitialize();

	if( FAILED( hr ) ) {
		return false;
	} else {
		return true;
	}
}
//! XInput デバイスかどうかチェックする
bool CInputDevicePort::IsXInputDevice( const GUID& guidProduct ) const
{
	for( std::list<DWORD>::const_iterator i = xinput_vid_pids_.begin(); i != xinput_vid_pids_.end(); ++i ) {
		if( (*i) == guidProduct.Data1 )
			return true;
	}
	return false;
}


}; // namespace gamepad



