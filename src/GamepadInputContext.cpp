/****************************************************************************/
/*! @file
@brief 

-----------------------------------------------------------------------------
	Copyright (C) 2008 Takenori Imoto (井元 武則). All rights reserved.
	( http://www.kaede-software.com/ )
-----------------------------------------------------------------------------
@author		T.Imoto
@date		2008/05/28
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

#include "GamepadInputContext.h"
#include "GamePadDirectInputDevice.h"
#include <limits.h>

namespace gamepad {

CInputContext::CInputContext()
{
	ClearAll();
}

// アナログ方向キーを判定
void CInputContext::UpdateAxisState( int& stateMinus, int& statePlus, double current )
{
	if( current == 0.0 ) {
		if( stateMinus >= TOUCH_DOWN ) {
			// 前回は押されていた
			stateMinus = 0;
		} else {
			// 未入力継続
			if( stateMinus != INT_MIN ) stateMinus--;
		}

		if( statePlus >= TOUCH_DOWN ) {
			statePlus = 0;
		} else {
			if( statePlus != INT_MIN ) statePlus--;
		}
	} else if( current > 0.0 ) {
		if( stateMinus >= TOUCH_DOWN ) {
			stateMinus = 0;
		} else {
			if( stateMinus != INT_MIN ) stateMinus--;
		}

		if( statePlus <= TOUCH_NO ) {
			statePlus = TOUCH_DOWN;
		} else {
			if( statePlus != INT_MAX ) statePlus++;
		}
	} else {	// current < 0.0
		if( stateMinus <= TOUCH_NO ) {
			stateMinus = TOUCH_DOWN;
		} else {
			if( stateMinus != INT_MAX ) stateMinus++;
		}

		if( statePlus >= TOUCH_DOWN ) {
			statePlus = 0;
		} else {
			if( statePlus != INT_MIN ) statePlus--;
		}
	}
}

void CInputContext::UpdateButtonState( int& state, bool current )
{
	if( current == false ) {
		if( state >= TOUCH_DOWN ) {
			state = 0;
		} else {
			if( state != INT_MIN ) state--;
		}
	} else {	// current == true
		if( state <= TOUCH_NO ) {
			state = TOUCH_DOWN;
		} else {
			if( state != INT_MAX ) state++;
		}
	}
}

void CInputContext::Input( GamePadState& state )
{
	UpdateAxisState( game_pad_ctx_.analogLeftLeft, game_pad_ctx_.analogLeftRight, state.leftX );
	UpdateAxisState( game_pad_ctx_.analogLeftDown, game_pad_ctx_.analogLeftUp, state.leftY );
	UpdateAxisState( game_pad_ctx_.analogRightLeft, game_pad_ctx_.analogRightRight, state.rightX );
	UpdateAxisState( game_pad_ctx_.analogRightDown, game_pad_ctx_.analogRightUp, state.rightY );

	UpdateButtonState( game_pad_ctx_.buttonLeftTrigger, state.leftTrigger != 0.0 );
	UpdateButtonState( game_pad_ctx_.buttonRightTrigger, state.rightTrigger != 0.0 );

	UpdateButtonState( game_pad_ctx_.digitalUp, (state.keyState & IDBTN_DPAD_UP) != 0 );
	UpdateButtonState( game_pad_ctx_.digitalDown, (state.keyState & IDBTN_DPAD_DOWN) != 0 );
	UpdateButtonState( game_pad_ctx_.digitalLeft, (state.keyState & IDBTN_DPAD_LEFT) != 0 );
	UpdateButtonState( game_pad_ctx_.digitalRight, (state.keyState & IDBTN_DPAD_RIGHT) != 0 );

	UpdateButtonState( game_pad_ctx_.buttonStart, (state.keyState & IDBTN_START) != 0 );
	UpdateButtonState( game_pad_ctx_.buttonBack, (state.keyState & IDBTN_BACK) != 0 );
	UpdateButtonState( game_pad_ctx_.buttonLeftThumb, (state.keyState & IDBTN_LEFT_THUMB) != 0 );
	UpdateButtonState( game_pad_ctx_.buttonRightThumb, (state.keyState & IDBTN_RIGHT_THUMB) != 0 );
	UpdateButtonState( game_pad_ctx_.buttonLeftShoulder, (state.keyState & IDBTN_LEFT_SHOULDER) != 0 );
	UpdateButtonState( game_pad_ctx_.buttonRightShoulder, (state.keyState & IDBTN_RIGHT_SHOULDER) != 0 );

	UpdateButtonState( game_pad_ctx_.buttonA, (state.keyState & IDBTN_A) != 0 );
	UpdateButtonState( game_pad_ctx_.buttonB, (state.keyState & IDBTN_B) != 0 );
	UpdateButtonState( game_pad_ctx_.buttonX, (state.keyState & IDBTN_X) != 0 );
	UpdateButtonState( game_pad_ctx_.buttonY, (state.keyState & IDBTN_Y) != 0 );

	game_pad_ = state;
}


};	// end of namespace gamepad

