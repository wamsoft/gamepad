/****************************************************************************/
/*! @file
@brief ゲームパッドログ

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

#include "GamePadLog.h"
#include <string>
#include "GamePadDLLLoader.h"
//#include <dshow.h>
#include <windows.h>

#ifdef __KRKR__
#include "tp_stub.h"
#endif // __KRKR__

#ifndef MAX_ERROR_TEXT_LEN
#define MAX_ERROR_TEXT_LEN 160
#endif // MAX_ERROR_TEXT_LEN

namespace gamepad {

struct quartz
{
	static CDLLLoader			dll_;
	static bool LoadFunctions();
	static bool CheckLoading();

	typedef BOOL (WINAPI* FuncGetErrorText)(HRESULT, WCHAR *, DWORD);

public:
	static bool Initialize();

	static FuncGetErrorText		GetErrorText;
};
CDLLLoader quartz::dll_;
quartz::FuncGetErrorText quartz::GetErrorText = NULL;


bool quartz::Initialize()
{
	if( !GetErrorText ) {
		return LoadFunctions();
	}
	return true;	// 読み込み済み
}

bool quartz::CheckLoading()
{
	if( dll_.IsLoaded() == false ) {
		dll_.Load( L"quartz.dll" );
	}
	return dll_.IsLoaded();
}
bool quartz::LoadFunctions()
{
	if( !CheckLoading() ) {
		return false;
	}

	GetErrorText = (FuncGetErrorText)dll_.GetProcAddress( "AMGetErrorTextW" );
	if( !GetErrorText ) { return false; }

	return true;
}

void Log( const wchar_t* mes )
{
	std::wstring head(L"gamepad : ");
	head += std::wstring( mes );
#ifdef __KRKR__
	TVPAddLog( head.c_str() );
#else	// __KRKR__
	OutputDebugStringW( head.c_str() );
#endif	// __KRKR__
}

void Log( const wchar_t* mes, HRESULT hr )
{
	std::wstring head(L"gamepad : ");

	if( quartz::Initialize() ) {
		wchar_t	errorMes[MAX_ERROR_TEXT_LEN] = {0};
		quartz::GetErrorText( hr, errorMes, MAX_ERROR_TEXT_LEN );
		head += std::wstring( (wchar_t*)errorMes );
		head += std::wstring( L" : " );
	}
	head += std::wstring( mes );
#ifdef __KRKR__
	TVPAddLog( head.c_str() );
#else	// __KRKR__
	OutputDebugStringW( head.c_str() );
#endif	// __KRKR__
}

}; // namespace gamepad


