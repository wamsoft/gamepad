/****************************************************************************/
/*! @file
@brief ジョイスティックポート

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

#ifndef __GAME_PAD_INPUT_DEVICE_PORT_H__
#define __GAME_PAD_INPUT_DEVICE_PORT_H__

#include <windows.h>
#include <dinput.h>
#include <vector>
#include <list>
#include <string>
#include <atlbase.h>
#include "GamePadDLLLoader.h"
#include "GamePadInputDevice.h"

namespace gamepad {

//! 仮想的なクラス
//! ポートにコントローラがつながっているように扱う
class CInputDevicePort
{
//	std::vector<IInputDevice*>	controllers_;		//!< 実使用コントローラのリスト
	std::list<DWORD>			xinput_vid_pids_;	//!< XInput デバイスのベンダーIDとプロダクトIDのリスト

	//! コントロールパネル - ゲームコントローラで優先デバイスに設定されているもののGUID
	GUID				preferred_device_guid_;
	bool				is_valid_preferred_device_guid_;	//!< 優先デバイスGUIDが有効かどうか

	std::list<IInputDevice*>	controllers_;

//	static const size_t NUM_OF_PORTS = 4;

	static CDLLLoader			dinput_dll_;	//!< Direct Input DLL
	CComPtr<IDirectInput8>		direct_input_;	//!< Direct Input

	//! 仮のリストに格納する
	void PutTempControllers( IInputDevice* device ) {
		controllers_.push_back( device );
	}
	//! 仮のリストの先頭に格納する
	void PutTempControllersFront( IInputDevice* device ) {
		controllers_.push_front( device );
	}

	//! XInputの初期化
	void InitializeXInput();
	//! DirectInput の初期化
	void InitializeDirectInput();

	//! DirectInput デバイスを検索する
	void FindDirectInputDevice();
	//! DirectInput デバイス列挙時のコールバック関数
	static BOOL CALLBACK EnumDevicesCallback( const DIDEVICEINSTANCE* pInst, VOID* pContext );
	//! DirectInput デバイス列挙時のコールバック関数
	bool EnumDevicesCallback( const DIDEVICEINSTANCE* pInst );

	//! XInput デバイスを検索する
	bool FindXInputDevice();
	//! XInput デバイスかどうかチェックする
	bool IsXInputDevice( const GUID& GuidProduct ) const;

	//! 優先デバイスの GUID を取得する
	void GetPreferredDevice();

	//! 全デバイスを初期化する
	void InitializeAllDevice( HWND hWnd );

	//! 空のポートを検索する
	// @return < 0 : 空きなし, other : 空きポート
//	int GetEmptyPort();

	//! 指定ポートにデバイスをセットする
//	bool InsertDeviceToPort( int num, IInputDevice* device );

	void ClearAllDevice();

public:
	CInputDevicePort();
	~CInputDevicePort();

	// コントローラを初期化する
	void InitializeControllers( HWND hWnd );

	// 有効なコントローラ数を取得する
	size_t GetNumberOfEnableControllers() const { return controllers_.size(); }

	IInputDevice* GetController( size_t idx );

	//! Direct Input Device をロストし、再取得する必要がある時に使用する
	bool CreateDirectInputDevice( GUID& instGuid, IDirectInputDevice8** device );
};

}; // namespace gamepad

#endif // __GAME_PAD_INPUT_DEVICE_PORT_H__


