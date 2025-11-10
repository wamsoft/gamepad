/****************************************************************************/
/*! @file
@brief 

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

#ifndef __GAME_PAD_DIRECT_INPUT_DEVICE_H__
#define __GAME_PAD_DIRECT_INPUT_DEVICE_H__

#include <windows.h>
#include <dinput.h>
#include <string>
#include <atlbase.h>
#include "GamePadInputDeviceBase.h"
#include "GamePadDeviceInspector.h"

namespace gamepad {

//! ボタンや軸の対応を表す
//! Slider の状況によっては変更するかも
struct DirectInputObjectMap
{
	int		obj_num;
	int		direction;	// 軸が反転している時は -1、そうでない時は 1。POV や ボタンの時は意味なし
};

enum DirectInputObjectNum {
	DIObj_AxisX = 0,
	DIObj_AxisY = 1,
	DIObj_AxisZ = 2,
	DIObj_AxisRotX = 3,
	DIObj_AxisRotY = 4,
	DIObj_AxisRotZ = 5,
	DIObj_Slider_0 = 6,
	DIObj_Slider_1 = 7,
	DIObj_POV_0 = 8,
	DIObj_POV_1 = 9,
	DIObj_POV_2 = 10,
	DIObj_POV_3 = 11,
	DIObj_Button1 = 12,
	DIObj_Button2 = 13,
	DIObj_Button3 = 14,
	DIObj_Button4 = 15,
	DIObj_Button5 = 16,
	DIObj_Button6 = 17,
	DIObj_Button7 = 18,
	DIObj_Button8 = 19,
	DIObj_Button9 = 20,
	DIObj_Button10 = 21,
	DIObj_Button11 = 22,
	DIObj_Button12 = 23,
	DIObj_Button13 = 24,
	DIObj_Button14 = 25,
	DIObj_Button15 = 26,
	DIObj_Button16 = 27,
	DIObj_Button17 = 28,
	DIObj_Button18 = 29,
	DIObj_Button19 = 30,
	DIObj_Button20 = 31,
	DIObj_Button21 = 32,
	DIObj_Button22 = 33,
	DIObj_Button23 = 34,
	DIObj_Button24 = 35,
	DIObj_Button25 = 36,
	DIObj_Button26 = 37,
	DIObj_Button27 = 38,
	DIObj_Button28 = 39,
	DIObj_Button29 = 40,
	DIObj_Button30 = 41,
	DIObj_Button31 = 42,
	DIObj_Button32 = 43,
	DIObj_Disable = 44,
	DIObj_EOT
};

enum DirectInputButtonMapNum {
	DIBMAP_NUM_DPAD_UP			= 0,	// 十字キー 上
	DIBMAP_NUM_DPAD_DOWN		= 1,	// 十字キー 下
	DIBMAP_NUM_DPAD_LEFT		= 2,	// 十字キー 左
	DIBMAP_NUM_DPAD_RIGHT		= 3,	// 十字キー 右
	DIBMAP_NUM_START			= 4,	// START
	DIBMAP_NUM_BACK				= 5,	// BACK
	DIBMAP_NUM_LEFT_THUMB		= 6,	// 左サムボタン
	DIBMAP_NUM_RIGHT_THUMB		= 7,	// 右サムボタン
	DIBMAP_NUM_LEFT_SHOULDER	= 8,	// 左肩ボタン
	DIBMAP_NUM_RIGHT_SHOULDER	= 9,	// 右肩ボタン
	DIBMAP_NUM_A				= 10,	// Aボタン
	DIBMAP_NUM_B				= 11,	// Bボタン
	DIBMAP_NUM_X				= 12,	// Xボタン
	DIBMAP_NUM_Y				= 13,	// Yボタン
	DIBMAP_NUM_EOT
};

enum DirectInputTriggerMapNum {
	DITMAP_NUM_LEFT = 0,
	DITMAP_NUM_RIGHT = 1,
	DITMAP_NUM_EOT
};
enum DirectInputAxisMapNum {
	DIAMAP_NUM_LX = 0,		// 左スティックX軸
	DIAMAP_NUM_LY = 1,		// 左スティックY軸
	DIAMAP_NUM_RX = 2,		// 右スティックX軸
	DIAMAP_NUM_RY = 3,		// 右スティックY軸
	DIAMAP_NUM_EOT
};

struct DirectInputObjectMappingTable
{
	DirectInputObjectMap	button[DIBMAP_NUM_EOT];	// 十字キーとボタン
	DirectInputObjectMap	trigger[DITMAP_NUM_EOT];
	DirectInputObjectMap	axis[DIAMAP_NUM_EOT];
	bool					leftforcefirst;
};


class CDirectInputDevice : public CInputDeviceBase
{
protected:
	struct DirectInputObjects
	{
		static const int AXIS_X		= 0x01 << 0;
		static const int AXIS_Y		= 0x01 << 1;
		static const int AXIS_Z		= 0x01 << 2;
		static const int AXIS_ROT_X	= 0x01 << 3;
		static const int AXIS_ROT_Y	= 0x01 << 4;
		static const int AXIS_ROT_Z	= 0x01 << 5;

		int		axis;
		int		slider;
		int		pov;
		int		button;
		int		forcefeedback;

		DirectInputObjects()
		: axis(0), slider(0), pov(0), button(0), forcefeedback(0)
		{}
		void clear()
		{
			axis = 0;
			slider = 0;
			pov = 0;
			button = 0;
			forcefeedback = 0;
		}
	};
	struct DirectInputState {
		DIJOYSTATE	state;
		BYTE		dummy[4];	//!< ダミー配列。DIJOYSTATE のボタン配列を超えてアクセスしてもここになるようにする
	};

	DirectInputObjects		enable_objs_;
	CGamePadDeviceInspector	inspector_;

	CComPtr<IDirectInputDevice8>	device_;

	WORD	vendor_id_;		//!< ベンダーID
	WORD	product_id_;	//!< プロダクトID

	GUID	instance_guid_;	//!< インスタンスID 再生成する時に必要

	DirectInputState	state_;

	DirectInputObjectMappingTable	key_map_;	//!< 各ボタンなどの割り当て

	bool		device_creating_;	//!< デバイスの再生成中かどうか
	bool		request_poll_method_;

	std::wstring	physical_port_name_;	//!< 接続されているポートの名前

	static const long		AXIS_RANGE_MAX = +32767;
	static const long		AXIS_RANGE_MIN = -32768;
	static const long		AXIS_RANGE = 65535;

	static const long		DEADZONE = 8689;

	void ClearState() {
		ZeroMemory( &state_, sizeof(state_) );
		state_.state.rgdwPOV[0] = -1;
		state_.state.rgdwPOV[1] = -1;
		state_.state.rgdwPOV[2] = -1;
		state_.state.rgdwPOV[3] = -1;
	}

	static BOOL CALLBACK EnumObjectsCallback( const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext );
	bool EnumObjectsCallback( const DIDEVICEOBJECTINSTANCE* pdidoi );

	void RetrieveVendorAndProductID();
	void RetrieveConnectedPortName();

	bool InitializeDeviceDetail();

	bool Acquire();

	void RecreateDevice();

	//! 全てのキーが無効になるように設定する
	void ClearKeyMap();

	//! HID の page と usage を得る
	void GetHIDDesc( WORD page, WORD id, std::wstring& pageName, std::wstring& usage );

	// POV 値から押されているかどうかを判定する
	static inline bool IsPressUpForPovVal( DWORD pov ) {
		return ( ( pov >= 0 && pov < 9000 ) || ( pov > 27000 && pov <= 36000 ) );
	}
	static inline bool IsPressDownForPovVal( DWORD pov ) { return ( pov > 9000 && pov < 27000 ); }
	static inline bool IsPressLeftForPovVal( DWORD pov ) { return ( pov > 18000 && pov < 36000 ); }
	static inline bool IsPressRightForPovVal( DWORD pov ) { return ( pov > 0 && pov < 18000 ); }
	static double GetAxisValue( long val, int dir );

	bool IsPressPlusKey( long key, int num, int dir ) const;
	double GetThumbStickValue( int num, int dir, bool h ) const;
	double GetTriggerValue( int num ) const;

	//! 出来るだけパッドに合うキーマッピングを行う
	void DoAutoKeyMapping();

	//! Force Feedback Deviceの設定を反映する
	virtual bool UpdateFF() { return true; }
public:
	CDirectInputDevice( IDirectInputDevice8* input, CInputDevicePort* port );
	virtual ~CDirectInputDevice() {}

	virtual bool Initialize( HWND hWnd );

	virtual void Update();

	virtual long GetTypeOfDevice() const { return IDT_DirectInput; };

	virtual unsigned long GetKeyState() const;
	virtual double GetLeftTrigger() const;
	virtual double GetRightTrigger() const;
	virtual double GetLeftThumbStickX() const;
	virtual double GetLeftThumbStickY() const;
	virtual double GetRightThumbStickX() const;
	virtual double GetRightThumbStickY() const;

	//! 非フォースフィードバックデバイスにバイブはない
	virtual void SetLeftVibration( double val ) {}
	virtual void SetRightVibration( double val ) {}

	virtual unsigned long Sensing() const;

	// DirectInput 用
//	virtual int StartSensingRow( bool multikey );
//	virtual unsigned long SensingRow() const;

#if 0
	virtual void SetButtonMapping( int id, int dkey );
	virtual void SetAxisMapping( int id, int dkey, int dir );
	virtual void SetTriggerMapping( int id, int dkey );
	virtual void SetLeftForceFirst( bool b );

	virtual int GetButtonMapping( int id );
	virtual int GetAxisMapping( int id );
	virtual int GetAxisDirectionMapping( int id );
	virtual int GetTriggerMapping( int id );
	virtual bool GetLeftForceFirst();

	//! 指定されたキーのマッピングを試みる
	// 軸にボタンを設定する時など特殊なエラーコードを定義する必要あり
	virtual int SensingMapping( int id, int dir );
#endif
};

}; // namespace gamepad


#endif // __GAME_PAD_DIRECT_INPUT_DEVICE_H__

