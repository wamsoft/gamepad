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

#include "GamePadForceFeedbackDevice.h"
#include "GamePadLog.h"

#define _USE_MATH_DEFINES 
#include <cmath> 

namespace gamepad {

CForceFeedbackDevice::CForceFeedbackDevice( IDirectInputDevice8* input, CInputDevicePort* port )
 : CDirectInputDevice(input,port), left_magnitude_(0.0), right_magnitude_(0.0)
{
}

// フォースフィードバッグデバイスを初期化
bool CForceFeedbackDevice::Initialize( HWND hWnd )
{
	if( !device_ ) {
		return false;
	}

	HRESULT		hr;
	if( FAILED( hr = device_->SetDataFormat( &c_dfDIJoystick ) ) ) {
		Log( L"Failed to Call SetDataFormat.", hr );
		return false;
	}

	// フォースフィードバックを使う場合、DISCL_EXCLUSIVE を指定する必要がある
	if( FAILED( hr = device_->SetCooperativeLevel( hWnd, DISCL_EXCLUSIVE |  DISCL_FOREGROUND ) ) ) {
		Log( L"Failed to Call SetCooperativeLevel.", hr );
		return false;
	}

	// フォースフィードバックを使う場合、auto-centering が問題になるらしいので、無効にする
	DIPROPDWORD dipdw;
	dipdw.diph.dwSize		= sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize	= sizeof(DIPROPHEADER);
	dipdw.diph.dwObj		= 0;
	dipdw.diph.dwHow		= DIPH_DEVICE;
	dipdw.dwData			= FALSE;
	if( FAILED( hr = device_->SetProperty( DIPROP_AUTOCENTER, &dipdw.diph ) ) ) {
		Log( L"Failed to Call SetProperty( DIPROP_AUTOCENTER ).", hr );
		return false;
	}

	if( !CDirectInputDevice::InitializeDeviceDetail() ) {
		return false;
	}

	// フォースフィードバックするものは2個までのサポートにする
	if( enable_objs_.forcefeedback > 2 ) {
		Log( L"This system support less than 3 axes force freedback. rewrite number of axes." );
		enable_objs_.forcefeedback = 2;
	}

	// DirectInput のフォースフィードバックではかなりいろいろな設定が出来るが、それらを使うと複雑になってしまうため、
	// コンスタントフォースを用いて、ソフト側で値を変えてコントロールするようにする。
	// XInput はそのようなスタイルのようなので、それと合わせる。
	rgdw_axes_[0] = DIJOFS_X;
	rgdw_axes_[1] = DIJOFS_Y;
	rgl_direction_[0] = 0;
	rgl_direction_[1] = 0;
	cf_.lMagnitude = 0;

	ZeroMemory( &effect_param_, sizeof(effect_param_) );
	effect_param_.dwSize					= sizeof(DIEFFECT);
	effect_param_.dwFlags					= DIEFF_CARTESIAN | DIEFF_OBJECTOFFSETS;	// 直交座標で値指定 
	effect_param_.dwDuration				= INFINITE;
	effect_param_.dwSamplePeriod			= 0;
	effect_param_.dwGain					= DI_FFNOMINALMAX;
	effect_param_.dwTriggerButton			= DIEB_NOTRIGGER;
	effect_param_.dwTriggerRepeatInterval	= 0;
	effect_param_.cAxes						= enable_objs_.forcefeedback;
	effect_param_.rgdwAxes					= rgdw_axes_;
	effect_param_.rglDirection				= rgl_direction_;
	effect_param_.lpEnvelope				= 0;
	effect_param_.cbTypeSpecificParams		= sizeof(DICONSTANTFORCE);
	effect_param_.lpvTypeSpecificParams		= &cf_;
	effect_param_.dwStartDelay				= 0;
	if( FAILED( hr = device_->CreateEffect( GUID_ConstantForce, &effect_param_, &effect_, NULL ) ) ) {
		Log( L"Failed to Call CreateEffect.", hr );
		return false;
	}

	return true;
}

void CForceFeedbackDevice::SetLeftVibration( double val )
{
	if( val < 0.0 ) val = 0.0;
	if( val > 1.0 ) val = 1.0;
	left_magnitude_ = val;
}
void CForceFeedbackDevice::SetRightVibration( double val )
{
	if( val < 0.0 ) val = 0.0;
	if( val > 1.0 ) val = 1.0;
	right_magnitude_ = val;
}
//! フォースフィードバックを実行。震えさせる
bool CForceFeedbackDevice::UpdateFF()
{
	// エフェクトがないよ…
	if( !effect_ ) {
		return false;
	}

	// フォースフィードバック軸がないよ…
	if( enable_objs_.forcefeedback == 0 )
		return true;

	int	left_force = (int)(DI_FFNOMINALMAX * left_magnitude_);
	int	right_force = (int)(DI_FFNOMINALMAX * right_magnitude_);

	if( enable_objs_.forcefeedback == 1 ) {
		// 1個だけの時は、左だけと考える
		cf_.lMagnitude = left_force;
		rgl_direction_[0] = 0;
		rgl_direction_[1] = 0;
	} else {
		// 2軸の時は、軸値とマグニチュードで制御
		rgl_direction_[0] = left_force;
		rgl_direction_[1] = right_force;
		cf_.lMagnitude = (DWORD)sqrt( (double)left_force * (double)left_force + (double)right_force * (double)right_force );
	}

	// フォースを適用！ 震えろ！
	HRESULT	hr;
	if( FAILED( hr = effect_->SetParameters( &effect_param_, DIEP_DIRECTION | DIEP_TYPESPECIFICPARAMS | DIEP_START ) ) ) {
		Log( L"Failed to Call SetParameters.", hr );
		return false;
	}
	return true;
}



}; // namespace gamepad


