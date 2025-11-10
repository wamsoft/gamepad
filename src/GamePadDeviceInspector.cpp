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


#include "GamePadDeviceInspector.h"
#include "GamePadLog.h"
#include "GamePadUtil.h"
#include <sstream>

namespace gamepad {


CGamePadDeviceInspector::CGamePadDeviceInspector()
{
	ZeroMemory( &device_capabilities_, sizeof(device_capabilities_) );
	ZeroMemory( &device_instance_info_, sizeof(device_instance_info_) );
}

bool CGamePadDeviceInspector::GetInfomation( IDirectInputDevice8* device )
{
	if( !GetCaps( device, device_capabilities_ ) ) {
		return false;
	}

	if( !GetInstInfo( device, device_instance_info_ ) ) {
		return false;
	}

	return true;
}
bool CGamePadDeviceInspector::GetCaps( IDirectInputDevice8* device, DIDEVCAPS& caps )
{
	HRESULT hr;
	caps.dwSize = sizeof(DIDEVCAPS);
	if( FAILED( hr = device->GetCapabilities( &caps ) ) ) {
		Log( L"Failed to Call GetCapabilities.", hr );
		return false;
	}
	return true;
}
bool CGamePadDeviceInspector::GetInstInfo( IDirectInputDevice8* device, DIDEVICEINSTANCE& info )
{
	HRESULT hr;
	info.dwSize = sizeof(DIDEVICEINSTANCE);
	if( FAILED( hr = device->GetDeviceInfo( &info ) ) ) {
		Log( L"Failed to Call GetDeviceInfo.", hr );
		return false;
	}
	return true;
}
bool CGamePadDeviceInspector::IsForceFeedbackDevice( IDirectInputDevice8* device )
{
	DIDEVCAPS	caps;
	if( GetCaps( device, caps ) ) {
		return IsForceFeedbackDevice(caps.dwFlags);
	}
	return false;
}

//! 各種の値をログに書き出す
void CGamePadDeviceInspector::PrintDetail()
{
	std::wstringstream mesStream;
	mesStream << L"Devie Detail :" << std::endl;

	std::string	productName;
	std::wstring	wproductName;
	GetProductName( productName );
	if( EncodeToUTF16( wproductName, productName ) ) {
		mesStream << L"Product Name : " << wproductName << std::endl;
	}
	int	main, sub;
	GetDeviceType( main, sub );
	std::wstring mainname, subname;
	GetDeviceTypeName( main, sub, mainname, subname );
	mesStream << L"Device Type Main : " << mainname << std::endl;
	mesStream << L"Device Type Sub : " << subname << std::endl;

	std::wstring desc;
	GetDeviceFlagDesc( GetDeviceFlag(), desc );
	mesStream << L"Devie Description : " << std::endl << desc << std::endl;

	mesStream << L"Number of Axes : " << GetNumberOfAxes() << std::endl;
	mesStream << L"Number of Buttons : " << GetNumberOfButtons() << std::endl;
	mesStream << L"Number of POV : " << GetNumberOfPOVs() << std::endl;
	mesStream << L"Force Feedback Sample Period : " << GetForceFeedbackSamplePeriod() << L" [msec]" << std::endl;
	mesStream << L"Force Feedback Min Time Resolution : " << GetForceFeedbackMinTimeResolution() << L" [msec]" << std::endl;
	mesStream << L"Firmware Revision : " << GetFirmwareRevisionNumber() << std::endl;
	mesStream << L"Hardware Revision : " << GetHardwareRevisionNumber() << std::endl;
	mesStream << L"Driver Version : " << GetDriverVersionNumber() << std::endl;

	Log( mesStream.str().c_str() );
}

void CGamePadDeviceInspector::GetDeviceFlagDesc( unsigned long flag, std::wstring& desc )
{
	desc.clear();

	if( flag & DIDC_ALIAS ) {
		desc += L"デバイスは、別の DirectInput デバイスの複製である。\n";
	}

	if( flag & DIDC_ATTACHED ) {
		desc += L"デバイスは、物理的にアタッチされている。\n";
	}

	if( flag & DIDC_DEADBAND ) {
		desc += L"デバイスは、少なくとも 1 つのフォース フィードバック条件でデッドバンドをサポートしている。\n";
	}

	if( flag & DIDC_EMULATED ) {
		desc += L"このフラグが設定されると、データは HID (Human Interface Device) などのユーザー モード デバイス インターフェイスから、または他のリング 3 の手段により入力される。";
		desc += L"フラグが設定されない場合、データはカーネル モード ドライバから直接入力される。\n";
	}

	if( flag & DIDC_FORCEFEEDBACK ) {
		desc += L"デバイスは、フォース フィードバックをサポートする。\n";
	}

	if( flag & DIDC_FFFADE ) {
		desc += L"フォース フィードバック システムは、少なくとも 1 つのエフェクトに対するフェード パラメータをサポートしている。\n";
	}

	if( flag & DIDC_FFATTACK ) {
		desc += L"フォース フィードバック システムは、少なくとも 1 つのエフェクトに対するアタック パラメータをサポートしている。\n";
	}

	if( flag & DIDC_HIDDEN ) {
		desc += L"デバイス ドライバがキーボード イベントやマウス イベントを生成できるように作成された仮想デバイス。\n";
	}

	if( flag & DIDC_PHANTOM ) {
		desc += L"プレースホルダ。仮想デバイスは、デフォルトでは IDirectInput8::EnumDevices によって列挙されない。\n";
	}

	if( flag & DIDC_POLLEDDATAFORMAT ) {
		desc += L"現状のデータ形式で少なくとも 1 つのオブジェクトが、割り込み駆動ではなく、ポーリングされる。\n";
	}

	if( flag & DIDC_POLLEDDEVICE ) {
		desc += L"デバイス上の少なくとも 1 つのオブジェクトが、割り込み駆動ではなく、ポーリングされる。\n";
	}

	if( flag & DIDC_POSNEGCOEFFICIENTS ) {
		desc += L"フォース フィードバック システムは、少なくとも 1 つの条件に対して条件の 2 つの係数値をサポートする (一方は、軸の正移動、他方は、軸の負移動)。";
		desc += L"デバイスが一方の係数しかサポートしていない場合、DICONDITION 構造体の負の係数は無視される。\n";
	}

	if( flag & DIDC_POSNEGSATURATION ) {
		desc += L"フォース フィードバック システムは、少なくとも 1 つの条件に対して、正負の両方のフォース出力に対する最大飽和をサポートする。";
		desc += L"デバイスが一方の飽和値しかサポートしていない場合、DICONDITION 構造体の負の飽和値は無視される。\n";
	}

	if( flag & DIDC_SATURATION ) {
		desc += L"フォース フィードバック システムは、少なくとも 1 つの条件に対して条件エフェクトの飽和をサポートする。";
		desc += L"デバイスが飽和をサポートしていない場合、ある条件により生成されるフォースは、デバイスが生成できる最大フォースによってのみ制限される。\n";
	}

	if( flag & DIDC_STARTDELAY ) {
		desc += L"フォース フィードバック システムは、少なくとも 1 つのエフェクトに対する遅延パラメータをサポートする。";
		desc += L"デバイスが開始遅延をサポートしていない場合、DIEFFECT 構造体の dwStartDelay メンバは無視される。\n";
	}
}

void CGamePadDeviceInspector::GetDeviceTypeName( int main, int sub, std::wstring& mainname, std::wstring& subname )
{
	switch( main ) {
	case DI8DEVTYPE_1STPERSON:
		mainname = L"一人称型のアクション ゲーム デバイス";
		switch( sub ) {
		case DI8DEVTYPE1STPERSON_LIMITED:
			subname = L"アクション マップ対象のデバイス オブジェクトの最小数を指定しないデバイス";
			break;
		case DI8DEVTYPE1STPERSON_SHOOTER:
			subname = L"一人称型のシューティング ゲーム用に設計されたデバイス";
			break;
		case DI8DEVTYPE1STPERSON_SIXDOF:
			subname = L"6 段階 (3 つの横方向軸と 3 つの回転軸) の自由を備えたデバイス";
			break;
		case DI8DEVTYPE1STPERSON_UNKNOWN:
		default:
			subname = L"不明なサブタイプ";
			break;
		}
		break;
	case DI8DEVTYPE_DEVICE:
		mainname = L"別のカテゴリに分類されないデバイス";
		subname = L"別のカテゴリに分類されないデバイス";
		break;
	case DI8DEVTYPE_DEVICECTRL:
		mainname = L"アプリケーションのコンテキストから別タイプのデバイスの制御に使われる入力デバイス";
		switch( sub ) {
		case DI8DEVTYPEDEVICECTRL_COMMSSELECTION:
			subname = L"通信の選択に使われるコントロール";
			break;
		case DI8DEVTYPEDEVICECTRL_COMMSSELECTION_HARDWIRED:
			subname = L"デフォルトの構成を使う必要があり、マッピングを変更できないデバイス";
			break;
		case DI8DEVTYPEDEVICECTRL_UNKNOWN:
		default:
			subname = L"不明なサブタイプ";
			break;
		}
		break;
	case DI8DEVTYPE_DRIVING:
		mainname = L"ステアリング用デバイス";
		switch( sub ) {
		case DI8DEVTYPEDRIVING_COMBINEDPEDALS:
			subname = L"単一の軸からアクセルとブレーキ ペダルの値を報告するステアリング デバイス";
			break;
		case DI8DEVTYPEDRIVING_DUALPEDALS:
			subname = L"別々の軸からアクセルとブレーキ ペダルの値を報告するステアリング デバイス";
			break;
		case DI8DEVTYPEDRIVING_HANDHELD:
			subname = L"ハンドヘルドのステアリング デバイス";
			break;
		case DI8DEVTYPEDRIVING_LIMITED:
			subname = L"アクション マップ対象のデバイス オブジェクトの最小数を指定しないステアリング デバイス";
			break;
		case DI8DEVTYPEDRIVING_THREEPEDALS:
			subname = L"別々の軸からアクセル、ブレーキ、およびクラッチ ペダルの値を報告するステアリング デバイス";
			break;
		default:
			subname = L"不明なサブタイプ";
			break;
		}
		break;
	case DI8DEVTYPE_FLIGHT:
		mainname = L"フライト シミュレーション用コントローラ";
		switch( sub ) {
		case DI8DEVTYPEFLIGHT_LIMITED:
			subname = L"アクション マップ対象のデバイス オブジェクトの最小数を指定しないフライト コントローラ";
			break;
		case DI8DEVTYPEFLIGHT_RC:
			subname = L"模型飛行機のリモート コントロールに基づくフライト デバイス";
			break;
		case DI8DEVTYPEFLIGHT_STICK:
			subname = L"ジョイスティック";
			break;
		case DI8DEVTYPEFLIGHT_YOKE:
			subname = L"操縦輪";
			break;
		default:
			subname = L"不明なサブタイプ";
			break;
		}
		break;
	case DI8DEVTYPE_GAMEPAD:
		mainname = L"ゲームパッド";
		switch( sub ) {
		case DI8DEVTYPEGAMEPAD_LIMITED:
			subname = L"アクション マップ対象のデバイス オブジェクトの最小数を指定しないゲームパッド";
			break;
		case DI8DEVTYPEGAMEPAD_STANDARD:
			subname = L"アクション マップ対象のデバイス オブジェクトの最小数を指定しない標準ゲームパッド";
			break;
		case DI8DEVTYPEGAMEPAD_TILT:
			subname = L"コントローラの姿勢から x 軸と y 軸のデータを報告するゲームパッド";
			break;
		default:
			subname = L"不明なサブタイプ";
			break;
		}
		break;
	case DI8DEVTYPE_JOYSTICK:
		mainname = L"ジョイスティック";
		switch( sub ) {
		case DI8DEVTYPEJOYSTICK_LIMITED:
			subname = L"アクション マップ対象のデバイス オブジェクトの最小数を指定しないジョイスティック";
			break;
		case DI8DEVTYPEJOYSTICK_STANDARD:
			subname = L"アクション マップ対象のデバイス オブジェクトの最小数を指定する標準ジョイスティック";
			break;
		default:
			subname = L"不明なサブタイプ";
			break;
		}
		break;
	case DI8DEVTYPE_KEYBOARD:
		mainname = L"キーボードまたはキーボード類似デバイス";
		switch( sub ) {
		case DI8DEVTYPEKEYBOARD_UNKNOWN:
			subname = L"サブタイプは確定できない";
			break;
		case DI8DEVTYPEKEYBOARD_PCXT:
			subname = L"IBM PC/XT 83 キー キーボード";
			break;
		case DI8DEVTYPEKEYBOARD_OLIVETTI:
			subname = L"Olivetti 102 キー キーボード";
			break;
		case DI8DEVTYPEKEYBOARD_PCAT:
			subname = L"IBM PC/AT 84 キー キーボード";
			break;
		case DI8DEVTYPEKEYBOARD_PCENH:
			subname = L"IBM PC 拡張 101/102 キー式キーボードまたは Microsoft NaturalR keyboard キーボード";
			break;
		case DI8DEVTYPEKEYBOARD_NOKIA1050:
			subname = L"Nokia 1050 キーボード";
			break;
		case DI8DEVTYPEKEYBOARD_NOKIA9140:
			subname = L"Nokia 9140 キーボード";
			break;
		case DI8DEVTYPEKEYBOARD_NEC98:
			subname = L"日本語 NEC PC98 キーボード";
			break;
		case DI8DEVTYPEKEYBOARD_NEC98LAPTOP:
			subname = L"日本語 NEC PC98 ラップトップ キーボード";
			break;
		case DI8DEVTYPEKEYBOARD_NEC98106:
			subname = L"日本語 NEC PC98 106 キー キーボード";
			break;
		case DI8DEVTYPEKEYBOARD_JAPAN106:
			subname = L"日本語 106 キー キーボード";
			break;
		case DI8DEVTYPEKEYBOARD_JAPANAX:
			subname = L"日本語 AX キーボード";
			break;
		case DI8DEVTYPEKEYBOARD_J3100:
			subname = L"日本語 J3100 キーボード";
			break;
		default:
			subname = L"不明なサブタイプ";
			break;
		}
		break;
	case DI8DEVTYPE_MOUSE:
		mainname = L"マウスまたはマウス類似デバイス (トラックボールなど)";
		switch( sub ) {
		case DI8DEVTYPEMOUSE_ABSOLUTE:
			subname = L"絶対軸データを返すマウス";
			break;
		case DI8DEVTYPEMOUSE_FINGERSTICK:
			subname = L"フィンガースティック";
			break;
		case DI8DEVTYPEMOUSE_TOUCHPAD:
			subname = L"タッチパッド";
			break;
		case DI8DEVTYPEMOUSE_TRACKBALL:
			subname = L"トラックボール";
			break;
		case DI8DEVTYPEMOUSE_TRADITIONAL:
			subname = L"従来型マウス";
			break;
		case DI8DEVTYPEMOUSE_UNKNOWN:
			subname = L"サブタイプは確定できない";
			break;
		default:
			subname = L"不明なサブタイプ";
			break;
		}
		break;
	case DI8DEVTYPE_REMOTE:
		mainname = L"リモート コントロール デバイス";
		switch( sub ) {
		case DI8DEVTYPEREMOTE_UNKNOWN:
			subname = L"サブタイプは確定できない";
			break;
		default:
			subname = L"不明なサブタイプ";
			break;
		}
		break;
	case DI8DEVTYPE_SCREENPOINTER:
		mainname = L"スクリーン ポインタ";
		switch( sub ) {
		case DI8DEVTYPESCREENPTR_UNKNOWN:
			subname = L"不明なサブタイプ";
			break;
		case DI8DEVTYPESCREENPTR_LIGHTGUN:
			subname = L"ライト ガン";
			break;
		case DI8DEVTYPESCREENPTR_LIGHTPEN:
			subname = L"ライト ペン";
			break;
		case DI8DEVTYPESCREENPTR_TOUCH:
			subname = L"タッチ スクリーン";
			break;
		default:
			subname = L"不明なサブタイプ";
			break;
		}
		break;
	case DI8DEVTYPE_SUPPLEMENTAL:
		mainname = L"ホイールと共に使うペダルなど、アプリケーションのメイン コントロールに適さない機能を備えた特殊デバイス";
		switch( sub ) {
		case DI8DEVTYPESUPPLEMENTAL_2NDHANDCONTROLLER:
			subname = L"2 次的ハンドヘルド コントローラ";
			break;
		case DI8DEVTYPESUPPLEMENTAL_COMBINEDPEDALS:
			subname = L"主機能として、単一の軸からアクセルとブレーキ ペダルの値を報告するデバイス";
			break;
		case DI8DEVTYPESUPPLEMENTAL_DUALPEDALS:
			subname = L"主機能として、別々の軸からアクセルとブレーキ ペダルの値を報告するデバイス";
			break;
		case DI8DEVTYPESUPPLEMENTAL_HANDTRACKER:
			subname = L"手の移動を追跡するデバイス";
			break;
		case DI8DEVTYPESUPPLEMENTAL_HEADTRACKER:
			subname = L"頭の移動を追跡するデバイス";
			break;
		case DI8DEVTYPESUPPLEMENTAL_RUDDERPEDALS:
			subname = L"方向舵ペダルを備えたデバイス";
			break;
		case DI8DEVTYPESUPPLEMENTAL_SHIFTER:
			subname = L"軸からギア選択を報告するデバイス";
			break;
		case DI8DEVTYPESUPPLEMENTAL_SHIFTSTICKGATE:
			subname = L"ボタン状態からギア選択を報告するデバイス";
			break;
		case DI8DEVTYPESUPPLEMENTAL_SPLITTHROTTLE:
			subname = L"主機能として、2 つ以上のスロットル値を報告するデバイス。その他のコントロールを持つ場合がある";
			break;
		case DI8DEVTYPESUPPLEMENTAL_THREEPEDALS:
			subname = L"主機能として、別々の軸からアクセル、ブレーキ、およびクラッチ ペダルの値を報告するデバイス";
			break;
		case DI8DEVTYPESUPPLEMENTAL_THROTTLE:
			subname = L"主機能として、単一のスロットル値を報告するデバイス。その他のコントロールを持つ場合がある";
			break;
		case DI8DEVTYPESUPPLEMENTAL_UNKNOWN:
		default:
			subname = L"不明なサブタイプ";
			break;
		}
		break;
	default:
		mainname = L"不明なデバイス";
		subname = L"不明なサブタイプ";
	}
}


}; // namespace gamepad

