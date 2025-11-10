/****************************************************************************/
/*! @file
@brief 

-----------------------------------------------------------------------------
	Copyright (C) 2008 Takenori Imoto (井元 武則). All rights reserved.
	( http://www.kaede-software.com/ )
-----------------------------------------------------------------------------
@author		T.Imoto
@date		2008/01/11
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

#ifndef __GAME_PAD_INPUT_DEVICE_H__
#define __GAME_PAD_INPUT_DEVICE_H__

#include <string>
#include <windows.h>

namespace gamepad {

enum InputDeviceType {
	IDT_Null = 0,
	IDT_XInput = 1,
	IDT_DirectInputFF = 2,
	IDT_DirectInput = 3,
};
// ボタン系のフラグは、XInputと同じにしてある
enum InputDeviceButtonFlag {
	IDBTN_DPAD_UP			= 0x00000001,	// 十字キー 上
	IDBTN_DPAD_DOWN			= 0x00000002,	// 十字キー 下
	IDBTN_DPAD_LEFT			= 0x00000004,	// 十字キー 左
	IDBTN_DPAD_RIGHT		= 0x00000008,	// 十字キー 右
	IDBTN_START				= 0x00000010,	// START
	IDBTN_BACK				= 0x00000020,	// BACK
	IDBTN_LEFT_THUMB		= 0x00000040,	// 左サムボタン
	IDBTN_RIGHT_THUMB		= 0x00000080,	// 右サムボタン
	IDBTN_LEFT_SHOULDER		= 0x00000100,	// 左肩ボタン
	IDBTN_RIGHT_SHOULDER	= 0x00000200,	// 右肩ボタン
	IDBTN_A					= 0x00001000,	// Aボタン
	IDBTN_B					= 0x00002000,	// Bボタン
	IDBTN_X					= 0x00004000,	// Xボタン
	IDBTN_Y					= 0x00008000,	// Yボタン
	IDBTN_LEFT_THUMB_AXIS_X	= 0x00010000,	// 左アナログX軸
	IDBTN_LEFT_THUMB_AXIS_Y	= 0x00020000,	// 左アナログY軸
	IDBTN_RIGHT_THUMB_AXIS_X= 0x00040000,	// 右アナログX軸
	IDBTN_RIGHT_THUMB_AXIS_Y= 0x00080000,	// 右アナログY軸
	IDBTN_LEFT_TRIGGER		= 0x00100000,	// 左トリガー
	IDBTN_RIGHT_TRIGGER		= 0x00200000,	// 右トリガー
	IDBTN_EOT
};

enum InputDeviceButtonNum {
	IDBTN_NUM_DPAD_UP			= 0,	// 十字キー 上
	IDBTN_NUM_DPAD_DOWN			= 1,	// 十字キー 下
	IDBTN_NUM_DPAD_LEFT			= 2,	// 十字キー 左
	IDBTN_NUM_DPAD_RIGHT		= 3,	// 十字キー 右
	IDBTN_NUM_START				= 4,	// START
	IDBTN_NUM_BACK				= 5,	// BACK
	IDBTN_NUM_LEFT_THUMB		= 6,	// 左サムボタン
	IDBTN_NUM_RIGHT_THUMB		= 7,	// 右サムボタン
	IDBTN_NUM_LEFT_SHOULDER		= 8,	// 左肩ボタン
	IDBTN_NUM_RIGHT_SHOULDER	= 9,	// 右肩ボタン
	IDBTN_NUM_A					= 12,	// Aボタン
	IDBTN_NUM_B					= 13,	// Bボタン
	IDBTN_NUM_X					= 14,	// Xボタン
	IDBTN_NUM_Y					= 15,	// Yボタン
	IDBTN_NUM_LEFT_THUMB_AXIS_X	= 16,	// 左アナログX軸
	IDBTN_NUM_LEFT_THUMB_AXIS_Y	= 17,	// 左アナログY軸
	IDBTN_NUM_RIGHT_THUMB_AXIS_X= 18,	// 右アナログX軸
	IDBTN_NUM_RIGHT_THUMB_AXIS_Y= 19,	// 右アナログY軸
	IDBTN_NUM_LEFT_TRIGGER		= 20,	// 左トリガー
	IDBTN_NUM_RIGHT_TRIGGER		= 21,	// 右トリガー
	IDBTN_NUM_EOT
};

class CInputDevicePort;

// デバイスのメソッドを定義するインターフェイスクラス
class IInputDevice
{
public:
	virtual ~IInputDevice() {}

	virtual bool Initialize( HWND hWnd ) = 0;

	virtual void Update() = 0;

	virtual void GetDeviceName( std::wstring& name ) const = 0;

	virtual long GetTypeOfDevice() const = 0;

	virtual unsigned long GetKeyState() const = 0;
	virtual double GetLeftTrigger() const = 0;
	virtual double GetRightTrigger() const = 0;
	virtual double GetLeftThumbStickX() const = 0;
	virtual double GetLeftThumbStickY() const = 0;
	virtual double GetRightThumbStickX() const = 0;
	virtual double GetRightThumbStickY() const = 0;

	virtual void SetLeftVibration( double val ) = 0;
	virtual void SetRightVibration( double val ) = 0;

	virtual unsigned long Sensing() const = 0;
};

}; // namespace gamepad

#endif // __GAME_PAD_INPUT_DEVICE_H__


