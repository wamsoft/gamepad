/****************************************************************************/
/*! @file
@brief DLL Loader

-----------------------------------------------------------------------------
	Copyright (C) 2005-2008 Takenori Imoto (井元 武則). All rights reserved.
	( http://www.kaede-software.com/ )
-----------------------------------------------------------------------------
@author		T.Imoto
@date		2005/12/22
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

#ifndef __GAMEPAD_DLL_LOADER_H__
#define __GAMEPAD_DLL_LOADER_H__

#include <windows.h>

namespace gamepad {
//----------------------------------------------------------------------------
//! @brief DLLを読み込むためのクラス
/*! @note 例:
	if( D3DDll_.IsLoaded() == false )
		D3DDll_.Load("d3d9.dll");

	if( D3DDll_.IsLoaded() == false )
		return D3DDll_.GetLastError();

	typedef IDirect3D9 *(WINAPI *FuncDirect3DCreate9)( UINT SDKVersion );
	FuncDirect3DCreate9 pDirect3DCreate9 = (FuncDirect3DCreate9)D3DDll_.GetProcAddress("Direct3DCreate9");
*/
//----------------------------------------------------------------------------
class CDLLLoader
{
	HMODULE		module_handle_;

public:
	CDLLLoader() : module_handle_(NULL) {}
	CDLLLoader(LPCTSTR lpFileName)
	{
		module_handle_ = ::LoadLibrary( lpFileName );
	}
	~CDLLLoader() { Free(); }
	inline bool IsLoaded() const { return( module_handle_ != NULL ); }
	bool Load( LPCTSTR lpFileName )
	{
		Free();
		module_handle_ = ::LoadLibrary( lpFileName );
		return( module_handle_ != NULL );
	}
	void Free()
	{
		if( module_handle_ )
			::FreeLibrary( module_handle_ );
		module_handle_ = NULL;
	}
	inline HRESULT GetLastError() const { return HRESULT_FROM_WIN32( ::GetLastError() ); }

	//! @param lpProcName : 関数名
	inline FARPROC GetProcAddress( LPCSTR lpProcName ) const
	{
		if( IsLoaded() ) {
			return ::GetProcAddress( module_handle_, lpProcName );
		} else {
			return NULL;
		}
	}
};

}; // namespace gamepad

#endif // __GAMEPAD_DLL_LOADER_H__

