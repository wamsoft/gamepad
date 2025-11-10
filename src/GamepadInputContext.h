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


#ifndef __GAME_PAD_INPUT_CONTEXT_H__
#define __GAME_PAD_INPUT_CONTEXT_H__

namespace gamepad {

struct GamePadState
{
	double	leftX;
	double	leftY;
	double	rightX;
	double	rightY;
	double	leftTrigger;
	double	rightTrigger;
	unsigned long	keyState;

	void clear() {
		leftX = leftY = rightX = rightY = leftTrigger = rightTrigger = 0.0;
		keyState = 0;
	}
};

struct GamePadContext
{
	int analogLeftUp;
	int analogLeftDown;
	int analogLeftLeft;
	int analogLeftRight;

	int analogRightUp;
	int analogRightDown;
	int analogRightLeft;
	int analogRightRight;

	int digitalUp;
	int digitalDown;
	int digitalLeft;
	int digitalRight;

	int buttonStart;
	int buttonBack;
	int buttonLeftThumb;
	int buttonRightThumb;
	int buttonLeftShoulder;
	int buttonLeftTrigger;
	int buttonRightShoulder;
	int buttonRightTrigger;
	int buttonA;
	int buttonB;
	int buttonX;
	int buttonY;

	void clear() {
		analogLeftUp = analogLeftDown = analogLeftLeft = analogLeftRight = 0;
		analogRightUp = analogRightDown = analogRightLeft = analogRightRight = 0;
		digitalUp = digitalDown = digitalLeft = digitalRight = 0;
		buttonStart = buttonBack = buttonLeftThumb = buttonRightThumb = buttonLeftShoulder
		 = buttonLeftTrigger = buttonRightShoulder = buttonRightTrigger
		 = buttonA = buttonB = buttonX = buttonY = 0;
	}
};

class CInputContext
{
public:
	static const int TOUCH_NO = 0;				//!< 押されていない
	static const int TOUCH_DOWN = 1;			//!< 押された瞬間
	static const int TOUCH_LIFTOFF = 0;			//!< 離された
	// 押され続けているかどうか TOUCH_DOWN より大きいかどうかで判定
	// 0 の時 離された瞬間、1の時押された瞬間、押されている時間が長いほど値は大きくなる
	// また、押されていない時間が長いほど、負の値で値が小さくなる
	// val >= TOUCH_DOWN : 押されている (値が大きいほど長く押されている
	// val <= TOUCH_NO : 押されていない (値が小さいほど長く離されている
	// val == TOUCH_DOWN 押された瞬間
	// val == TOUCH_LIFTOFF : 離された瞬間

protected:
	GamePadState	game_pad_;
	GamePadContext	game_pad_ctx_;

	void UpdateAxisState( int& stateMinus, int& statePlus, double current );
	void UpdateButtonState( int& state, bool current );

public:
	CInputContext();
	~CInputContext() {}

	inline void ClearAll() {
		game_pad_.clear();
		game_pad_ctx_.clear();
	}

	// パッドの状態を更新
	void Input( GamePadState& state );

	inline int GetAnalogLeftUpCount() const { return game_pad_ctx_.analogLeftUp; }
	inline int GetAnalogLeftDownCount() const { return game_pad_ctx_.analogLeftDown; }
	inline int GetAnalogLeftLeftCount() const { return game_pad_ctx_.analogLeftLeft; }
	inline int GetAnalogLeftRightCount() const { return game_pad_ctx_.analogLeftRight; }

	inline int GetAnadloRightUpCount() const { return game_pad_ctx_.analogRightUp; }
	inline int GetAnadloRightDownCount() const { return game_pad_ctx_.analogRightDown; }
	inline int GetAnadloRightLeftCount() const { return game_pad_ctx_.analogRightLeft; }
	inline int GetAnadloRightRightCount() const { return game_pad_ctx_.analogRightRight; }

	inline int GetDegitalUpCount() const { return game_pad_ctx_.digitalUp; }
	inline int GetDegitalDownCount() const { return game_pad_ctx_.digitalDown; }
	inline int GetDegitalLeftCount() const { return game_pad_ctx_.digitalLeft; }
	inline int GetDegitalRightCount() const { return game_pad_ctx_.digitalRight; }

	inline int GetButtonStartCount() const { return game_pad_ctx_.buttonStart; }
	inline int GetButtonBackCount() const { return game_pad_ctx_.buttonBack; }
	inline int GetButtonLeftThumbCount() const { return game_pad_ctx_.buttonLeftThumb; }
	inline int GetButtonRightThumbCount() const { return game_pad_ctx_.buttonRightThumb; }
	inline int GetButtonLeftShoulderCount() const { return game_pad_ctx_.buttonLeftShoulder; }
	inline int GetButtonLeftTriggerCount() const { return game_pad_ctx_.buttonLeftTrigger; }
	inline int GetButtonRightShoulderCount() const { return game_pad_ctx_.buttonRightShoulder; }
	inline int GetButtonRightTriggerCount() const { return game_pad_ctx_.buttonRightTrigger; }
	inline int GetButtonACount() const { return game_pad_ctx_.buttonA; }
	inline int GetButtonBCount() const { return game_pad_ctx_.buttonB; }
	inline int GetButtonXCount() const { return game_pad_ctx_.buttonX; }
	inline int GetButtonYCount() const { return game_pad_ctx_.buttonY; }
};

};	// end of namespace gamepad


#endif // __GAME_PAD_INPUT_CONTEXT_H__

