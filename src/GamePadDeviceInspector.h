/****************************************************************************/
/*! @file
@brief ゲームパッドインスペクタ

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


#ifndef __GAME_PAD_DEVICE_INSPECTOR_H__
#define __GAME_PAD_DEVICE_INSPECTOR_H__

#include <string>
#include <windows.h>
#include <dinput.h>

namespace gamepad {


class CGamePadDeviceInspector
{
	DIDEVCAPS				device_capabilities_;
	DIDEVICEINSTANCE		device_instance_info_;

public:
	CGamePadDeviceInspector();
	bool GetInfomation( IDirectInputDevice8* device );

	// ログに詳細情報を書き出す
	void PrintDescription() const;

	static bool GetCaps( IDirectInputDevice8* device, DIDEVCAPS& caps );
	static bool GetInstInfo( IDirectInputDevice8* device, DIDEVICEINSTANCE& info );

	// インスタンスを生成せずに、デバイスのポインタだけ渡して判定する
	static bool IsForceFeedbackDevice( IDirectInputDevice8* device );

	static bool IsForceFeedbackDevice( unsigned long flag ) {
		return (flag & DIDC_FORCEFEEDBACK) ? true : false;
	}

	//! デバイスのインスタンスに対する一意な識別子。
	//! アプリケーションは、このインスタンス グローバル一意識別子 (GUID) を構成ファイルに保存し、後で使うこともできる。
	//! インスタンス GUID は、コンピュータごとに固有である。
	//! 1 つのコンピュータから取得されるインスタンス GUID は、他のコンピュータ上のインスタンス GUID とは無関係である。 
	GUID GetInstanceGuid() const { return device_instance_info_.guidInstance; }
	//! 製品の一意な識別子。この識別子は、デバイス メーカーが設定する。 
	GUID GetProductGuid() const { return device_instance_info_.guidProduct; }
	//! インスタンスの登録名。たとえば、"Joystick 1"。 
	void GetInstanceName( std::wstring& name ) const { name = std::wstring(device_instance_info_.tszInstanceName); }
	//! 製品の登録名。 
	void GetProductName( std::wstring& name ) const { name = std::wstring(device_instance_info_.tszProductName); }
	//! フォース フィードバックに使われるドライバの一意な識別子。ドライバのメーカーがこの識別子を設定する。 
	GUID GetForceFeedbackDriverGuid() const { return device_instance_info_.guidFFDriver; }
	//! デバイスが HID (Human Interface Device) デバイスである場合、このメンバには、HID 使用ページ コードが含まれる。 
	WORD GetUsagePage() const { return device_instance_info_.wUsagePage; }
	//! デバイスが HID (Human Interface Device) デバイスである場合、このメンバには、HID 使用コードが含まれる。
	WORD GetUsage() const { return device_instance_info_.wUsage; }


	//! デバイスに関連付けられたフラグ。
	unsigned long GetDeviceFlag() const { return device_capabilities_.dwFlags; }
	//! デバイス タイプ指定子。このメンバは、DIDEVICEINSTANCE 構造体の dwDevType メンバと同等な値を格納できる。 
	void GetDeviceType( int& main, int& sub ) { main = device_capabilities_.dwDevType&0xFF; sub = (device_capabilities_.dwDevType&0xFF00)>>8; }
	//! デバイス上で使用可能な軸の数
	unsigned long GetNumberOfAxes() const { return device_capabilities_.dwAxes; }
	//! デバイス上で使用可能なボタンの数
	unsigned long GetNumberOfButtons() const { return device_capabilities_.dwButtons; }
	//! デバイス上で使用可能な視点コントローラの数
	unsigned long GetNumberOfPOVs() const { return device_capabilities_.dwPOVs; }
	//! 未加工のフォース コマンドを連続して再生する際の間隔を表す、マイクロ秒単位の最小時間
	unsigned long GetForceFeedbackSamplePeriod() const { return device_capabilities_.dwFFSamplePeriod; }
	//! マイクロ秒単位のデバイスの分解能の最小時間。デバイスは、すべての時間を最も近い増加値に丸める。
	//! たとえば、dwFFMinTimeResolution の値が 1000 の場合、デバイスはすべての時間を最も近いミリ秒に丸める。
	unsigned long GetForceFeedbackMinTimeResolution() const { return device_capabilities_.dwFFMinTimeResolution; }
	//! デバイスのファームウェア リビジョン番号
	unsigned long GetFirmwareRevisionNumber() const { return device_capabilities_.dwFirmwareRevision; }
	//! デバイスのハードウェア リビジョン番号
	unsigned long GetHardwareRevisionNumber() const { return device_capabilities_.dwHardwareRevision; }
	//! デバイス ドライバのバージョン番号
	unsigned long GetDriverVersionNumber() const { return device_capabilities_.dwFFDriverVersion; }

	//! 各種の値をログに書き出す
	void PrintDetail();

	static void GetDeviceFlagDesc( unsigned long flag, std::wstring& desc );
	static void GetDeviceTypeName( int main, int sub, std::wstring& mainname, std::wstring& subname );
};

}; // namespace gamepad

#endif	// __GAME_PAD_DEVICE_INSPECTOR_H__


