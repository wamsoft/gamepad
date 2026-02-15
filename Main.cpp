//---------------------------------------------------------------------------
#include "tp_stub.h"
#include "GamePadInputDevicePort.h"
#include "GamePadInputDevice.h"
#include "GamepadInputContext.h"
#include <assert.h>
//---------------------------------------------------------------------------
//! ネイティブインスタンス
class NI_GamepadPort : public tTJSNativeInstance
{
public:
	gamepad::CInputDevicePort	port_;

	NI_GamepadPort() {}
	tjs_error TJS_INTF_METHOD Construct(tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *tjs_obj) {
		return S_OK;
	}
	void TJS_INTF_METHOD Invalidate() {}
};
//---------------------------------------------------------------------------
/*! これはオブジェクトを作成して返すだけの関数です。
	後述の TJSCreateNativeClassForPlugin の引数として渡します。 */
static iTJSNativeInstance * TJS_INTF_METHOD Create_NI_GamepadPort()
{
	return new NI_GamepadPort();
}
//---------------------------------------------------------------------------
/*! TJS2 のネイティブクラスは一意な ID で区別されている必要があります。
	これは後述の TJS_BEGIN_NATIVE_MEMBERS マクロで自動的に取得されますが、
	その ID を格納する変数名と、その変数をここで宣言します。
	初期値には無効な ID を表す -1 を指定してください。 */
#define TJS_NATIVE_CLASSID_NAME ClassID_GamepadPort
static tjs_int32 TJS_NATIVE_CLASSID_NAME = -1;
//---------------------------------------------------------------------------
//! TJS2 用の「クラス」を作成して返す関数です。
static iTJSDispatch2 * Create_NC_GamepadPort()
{
	/// クラスオブジェクトの作成
	tTJSNativeClassForPlugin * classobj = TJSCreateNativeClassForPlugin(TJS_W("GamepadPort"), Create_NI_GamepadPort);

	/// メンバ定義
	TJS_BEGIN_NATIVE_MEMBERS(/*TJS class name*/GamepadPort)

		TJS_DECL_EMPTY_FINALIZE_METHOD

//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_CONSTRUCTOR_DECL( /*var.name*/_this, /*var.type*/NI_GamepadPort, /*TJS class name*/GamepadPort)
		{
			// NI_GamepadPort::Construct にも内容を記述できるのでここでは何もしない
			return TJS_S_OK;

		} TJS_END_NATIVE_CONSTRUCTOR_DECL(/*TJS class name*/GamepadPort)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_METHOD_DECL(/*func. name*/initialize)
		{
			TJS_GET_NATIVE_INSTANCE(/*var. name*/_this, /*var. type*/NI_GamepadPort);

			if( numparams < 1 ) return TJS_E_BADPARAMCOUNT;
			_this->port_.InitializeControllers( (HWND)(tjs_intptr_t)*param[0] );
			return TJS_S_OK;

		} TJS_END_NATIVE_METHOD_DECL(/*func. name*/initialize)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_METHOD_DECL(/*func. name*/getController)
		{
			TJS_GET_NATIVE_INSTANCE(/*var. name*/_this, /*var. type*/NI_GamepadPort);

			if( numparams < 1 ) return TJS_E_BADPARAMCOUNT;
			if( result ) {
				gamepad::IInputDevice*	device = _this->port_.GetController( (tjs_int)*param[0] );
				if( device ) {
					tTJSVariant varScripts;
					TVPExecuteExpression(TJS_W("Gamepad"), &varScripts);
					iTJSDispatch2 *dispatch = varScripts.AsObjectNoAddRef();

					tTJSVariant val;
					val = tTJSVariant( (tjs_intptr_t)device );
					tTJSVariant *varsp[1] = {&val};
					iTJSDispatch2 *instance = NULL;
//					if( SUCCEEDED( dispatch->CreateNew( 0, NULL, NULL, &instance, 0, NULL, _this))) {
					if( SUCCEEDED( dispatch->CreateNew( 0, NULL, NULL, &instance, 1, varsp, objthis))) {
						if( instance ) {
							instance->AddRef();
//							tTJSVariant val;
//							val = tTJSVariant( (tjs_int)device );
//							instance->PropSet( TJS_MEMBERENSURE, TJS_W("instance"), NULL, &val, _this );
							*result = instance;
							return TJS_S_OK;
						}
					}
				}
			}
			return TJS_E_INVALIDPARAM;

		} TJS_END_NATIVE_METHOD_DECL(/*func. name*/getController)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(count) // count 読み出し専用プロパティ
		{
			TJS_BEGIN_NATIVE_PROP_GETTER
			{
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_GamepadPort);

				if( result ) {
					*result = tjs_int( _this->port_.GetNumberOfEnableControllers() );
				}
				return TJS_S_OK;

			} TJS_END_NATIVE_PROP_GETTER

			TJS_DENY_NATIVE_PROP_SETTER

		} TJS_END_NATIVE_PROP_DECL(count)
//-------------------------------------------------------------------

	TJS_END_NATIVE_MEMBERS

	return classobj;
}
//---------------------------------------------------------------------------
//	TJS_NATIVE_CLASSID_NAME は一応 undef しておいたほうがよいでしょう
#undef TJS_NATIVE_CLASSID_NAME
//---------------------------------------------------------------------------






//---------------------------------------------------------------------------
//! ネイティブインスタンス
class NI_Gamepad : public tTJSNativeInstance
{
public:
	gamepad::IInputDevice	*device_;
	gamepad::CInputContext	context_;

	NI_Gamepad() {}
	tjs_error TJS_INTF_METHOD Construct(tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *tjs_obj) {
		return S_OK;
	}
	void TJS_INTF_METHOD Invalidate() { device_ = NULL; }

	void Update();
};
void NI_Gamepad::Update()
{
	if( device_ ) {
		device_->Update();
		gamepad::GamePadState	state;
		state.leftX = device_->GetLeftThumbStickX();
		state.leftY = device_->GetLeftThumbStickY();
		state.rightX = device_->GetRightThumbStickX();
		state.rightY = device_->GetRightThumbStickY();
		state.leftTrigger = device_->GetLeftTrigger();
		state.rightTrigger = device_->GetRightTrigger();
		state.keyState = device_->GetKeyState();
		context_.Input( state );
	}
}
//---------------------------------------------------------------------------
/*! これはオブジェクトを作成して返すだけの関数です。
	後述の TJSCreateNativeClassForPlugin の引数として渡します。 */
static iTJSNativeInstance * TJS_INTF_METHOD Create_NI_Gamepad()
{
	return new NI_Gamepad();
}
//---------------------------------------------------------------------------
/*! TJS2 のネイティブクラスは一意な ID で区別されている必要があります。
	これは後述の TJS_BEGIN_NATIVE_MEMBERS マクロで自動的に取得されますが、
	その ID を格納する変数名と、その変数をここで宣言します。
	初期値には無効な ID を表す -1 を指定してください。 */
#define TJS_NATIVE_CLASSID_NAME ClassID_Gamepad
static tjs_int32 TJS_NATIVE_CLASSID_NAME = -1;
//---------------------------------------------------------------------------
//! TJS2 用の「クラス」を作成して返す関数です。
static iTJSDispatch2 * Create_NC_Gamepad()
{
	/// クラスオブジェクトの作成
	tTJSNativeClassForPlugin * classobj = TJSCreateNativeClassForPlugin(TJS_W("Gamepad"), Create_NI_Gamepad);

	/// メンバ定義
	TJS_BEGIN_NATIVE_MEMBERS(/*TJS class name*/Gamepad)

		TJS_DECL_EMPTY_FINALIZE_METHOD

//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_CONSTRUCTOR_DECL( /*var.name*/_this, /*var.type*/NI_Gamepad, /*TJS class name*/Gamepad) {
			TJS_GET_NATIVE_INSTANCE(/*var. name*/_this, /*var. type*/NI_Gamepad);

			if( numparams < 1 ) return TJS_E_BADPARAMCOUNT;

			_this->device_ = (gamepad::IInputDevice*)(tjs_intptr_t)*param[0];
			return TJS_S_OK;

		} TJS_END_NATIVE_CONSTRUCTOR_DECL(/*TJS class name*/Gamepad)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_METHOD_DECL(/*func. name*/update) {
			TJS_GET_NATIVE_INSTANCE(/*var. name*/_this, /*var. type*/NI_Gamepad);
			_this->Update();
			return TJS_S_OK;

		} TJS_END_NATIVE_METHOD_DECL(/*func. name*/update)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(name) {
			TJS_BEGIN_NATIVE_PROP_GETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( result && _this->device_ ) {
					std::wstring name;
					_this->device_->GetDeviceName( name );
					*result = tTJSVariant( (const tjs_char *)name.c_str() );
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_GETTER
			TJS_DENY_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(name)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(type) {
			TJS_BEGIN_NATIVE_PROP_GETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( result && _this->device_ ) {
					*result = (tjs_int)_this->device_->GetTypeOfDevice();
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_GETTER
			TJS_DENY_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(type)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(leftTrigger) {
			TJS_BEGIN_NATIVE_PROP_GETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( result && _this->device_ ) {
					*result = (tjs_real)_this->device_->GetLeftTrigger();
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_GETTER
			TJS_DENY_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(leftTrigger)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(rightTrigger) {
			TJS_BEGIN_NATIVE_PROP_GETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( result && _this->device_ ) {
					*result = (tjs_real)_this->device_->GetRightTrigger();
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_GETTER
			TJS_DENY_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(rightTrigger)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(leftThumbStickX) {
			TJS_BEGIN_NATIVE_PROP_GETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( result && _this->device_ ) {
					*result = (tjs_real)_this->device_->GetLeftThumbStickX();
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_GETTER
			TJS_DENY_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(leftThumbStickX)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(leftThumbStickY) {
			TJS_BEGIN_NATIVE_PROP_GETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( result && _this->device_ ) {
					*result = (tjs_real)_this->device_->GetLeftThumbStickY();
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_GETTER
			TJS_DENY_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(leftThumbStickY)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(rightThumbStickX) {
			TJS_BEGIN_NATIVE_PROP_GETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( result && _this->device_ ) {
					*result = (tjs_real)_this->device_->GetRightThumbStickX();
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_GETTER
			TJS_DENY_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(rightThumbStickX)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(rightThumbStickY) {
			TJS_BEGIN_NATIVE_PROP_GETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( result && _this->device_ ) {
					*result = (tjs_real)_this->device_->GetRightThumbStickY();
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_GETTER
			TJS_DENY_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(rightThumbStickY)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(leftVibration) {
			TJS_DENY_NATIVE_PROP_GETTER
			TJS_BEGIN_NATIVE_PROP_SETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( _this->device_ ) {
					_this->device_->SetLeftVibration( (tjs_real)*param );
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(leftVibration)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(rightVibration) {
			TJS_DENY_NATIVE_PROP_GETTER
			TJS_BEGIN_NATIVE_PROP_SETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( _this->device_ ) {
					_this->device_->SetRightVibration( (tjs_real)*param );
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(rightVibration)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(keyState) {
			TJS_BEGIN_NATIVE_PROP_GETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( result && _this->device_ ) {
					*result = (tjs_int)_this->device_->GetKeyState();
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_GETTER
			TJS_DENY_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(keyState)
//-------------------------------------------------------------------
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(analogLeftUpCount) {
			TJS_BEGIN_NATIVE_PROP_GETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( result ) {
					*result = (tjs_int)_this->context_.GetAnalogLeftUpCount();
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_GETTER
			TJS_DENY_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(analogLeftUpCount)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(analogLeftDownCount) {
			TJS_BEGIN_NATIVE_PROP_GETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( result ) {
					*result = (tjs_int)_this->context_.GetAnalogLeftDownCount();
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_GETTER
			TJS_DENY_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(analogLeftDownCount)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(analogLeftLeftCount) {
			TJS_BEGIN_NATIVE_PROP_GETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( result ) {
					*result = (tjs_int)_this->context_.GetAnalogLeftLeftCount();
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_GETTER
			TJS_DENY_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(analogLeftLeftCount)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(analogLeftRightCount) {
			TJS_BEGIN_NATIVE_PROP_GETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( result ) {
					*result = (tjs_int)_this->context_.GetAnalogLeftRightCount();
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_GETTER
			TJS_DENY_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(analogLeftRightCount)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(analogRightUpCount) {
			TJS_BEGIN_NATIVE_PROP_GETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( result ) {
					*result = (tjs_int)_this->context_.GetAnadloRightUpCount();
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_GETTER
			TJS_DENY_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(analogRightUpCount)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(analogRightDownCount) {
			TJS_BEGIN_NATIVE_PROP_GETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( result ) {
					*result = (tjs_int)_this->context_.GetAnadloRightDownCount();
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_GETTER
			TJS_DENY_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(analogRightDownCount)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(analogRightLeftCount) {
			TJS_BEGIN_NATIVE_PROP_GETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( result ) {
					*result = (tjs_int)_this->context_.GetAnadloRightLeftCount();
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_GETTER
			TJS_DENY_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(analogRightLeftCount)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(analogRightRightCount) {
			TJS_BEGIN_NATIVE_PROP_GETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( result ) {
					*result = (tjs_int)_this->context_.GetAnadloRightRightCount();
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_GETTER
			TJS_DENY_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(analogRightRightCount)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(degitalUpCount) {
			TJS_BEGIN_NATIVE_PROP_GETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( result ) {
					*result = (tjs_int)_this->context_.GetDegitalUpCount();
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_GETTER
			TJS_DENY_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(degitalUpCount)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(degitalDownCount) {
			TJS_BEGIN_NATIVE_PROP_GETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( result ) {
					*result = (tjs_int)_this->context_.GetDegitalDownCount();
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_GETTER
			TJS_DENY_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(degitalDownCount)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(degitalLeftCount) {
			TJS_BEGIN_NATIVE_PROP_GETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( result ) {
					*result = (tjs_int)_this->context_.GetDegitalLeftCount();
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_GETTER
			TJS_DENY_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(degitalLeftCount)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(degitalRightCount) {
			TJS_BEGIN_NATIVE_PROP_GETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( result ) {
					*result = (tjs_int)_this->context_.GetDegitalRightCount();
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_GETTER
			TJS_DENY_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(degitalRightCount)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(buttonStartCount) {
			TJS_BEGIN_NATIVE_PROP_GETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( result ) {
					*result = (tjs_int)_this->context_.GetButtonStartCount();
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_GETTER
			TJS_DENY_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(buttonStartCount)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(buttonBackCount) {
			TJS_BEGIN_NATIVE_PROP_GETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( result ) {
					*result = (tjs_int)_this->context_.GetButtonBackCount();
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_GETTER
			TJS_DENY_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(buttonBackCount)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(buttonLeftThumbCount) {
			TJS_BEGIN_NATIVE_PROP_GETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( result ) {
					*result = (tjs_int)_this->context_.GetButtonLeftThumbCount();
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_GETTER
			TJS_DENY_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(buttonLeftThumbCount)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(buttonRightThumbCount) {
			TJS_BEGIN_NATIVE_PROP_GETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( result ) {
					*result = (tjs_int)_this->context_.GetButtonRightThumbCount();
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_GETTER
			TJS_DENY_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(buttonRightThumbCount)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(buttonLeftShoulderCount) {
			TJS_BEGIN_NATIVE_PROP_GETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( result ) {
					*result = (tjs_int)_this->context_.GetButtonLeftShoulderCount();
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_GETTER
			TJS_DENY_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(buttonLeftShoulderCount)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(buttonLeftTriggerCount) {
			TJS_BEGIN_NATIVE_PROP_GETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( result ) {
					*result = (tjs_int)_this->context_.GetButtonLeftTriggerCount();
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_GETTER
			TJS_DENY_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(buttonLeftTriggerCount)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(buttonRightShoulderCount) {
			TJS_BEGIN_NATIVE_PROP_GETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( result ) {
					*result = (tjs_int)_this->context_.GetButtonRightShoulderCount();
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_GETTER
			TJS_DENY_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(buttonRightShoulderCount)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(buttonRightTriggerCount) {
			TJS_BEGIN_NATIVE_PROP_GETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( result ) {
					*result = (tjs_int)_this->context_.GetButtonRightTriggerCount();
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_GETTER
			TJS_DENY_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(buttonRightTriggerCount)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(buttonACount) {
			TJS_BEGIN_NATIVE_PROP_GETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( result ) {
					*result = (tjs_int)_this->context_.GetButtonACount();
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_GETTER
			TJS_DENY_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(buttonACount)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(buttonBCount) {
			TJS_BEGIN_NATIVE_PROP_GETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( result ) {
					*result = (tjs_int)_this->context_.GetButtonBCount();
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_GETTER
			TJS_DENY_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(buttonBCount)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(buttonXCount) {
			TJS_BEGIN_NATIVE_PROP_GETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( result ) {
					*result = (tjs_int)_this->context_.GetButtonXCount();
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_GETTER
			TJS_DENY_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(buttonXCount)
//-------------------------------------------------------------------
		TJS_BEGIN_NATIVE_PROP_DECL(buttonYCount) {
			TJS_BEGIN_NATIVE_PROP_GETTER {
				TJS_GET_NATIVE_INSTANCE(/*var. name*/_this,	/*var. type*/NI_Gamepad);
				if( result ) {
					*result = (tjs_int)_this->context_.GetButtonYCount();
				}
				return TJS_S_OK;
			} TJS_END_NATIVE_PROP_GETTER
			TJS_DENY_NATIVE_PROP_SETTER
		} TJS_END_NATIVE_PROP_DECL(buttonYCount)
//-------------------------------------------------------------------
	TJS_END_NATIVE_MEMBERS

	return classobj;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//	TJS_NATIVE_CLASSID_NAME は一応 undef しておいたほうがよいでしょう
#undef TJS_NATIVE_CLASSID_NAME
//---------------------------------------------------------------------------

#define EXPORT(hr) extern "C" __declspec(dllexport) hr __stdcall

#ifdef _MSC_VER
# if defined(_M_AMD64) || defined(_M_X64)
#  pragma comment(linker, "/EXPORT:V2Link")
#  pragma comment(linker, "/EXPORT:V2Unlink")
# else
#pragma comment(linker, "/EXPORT:V2Link=_V2Link@4")
#pragma comment(linker, "/EXPORT:V2Unlink=_V2Unlink@0")
#endif
#endif

//---------------------------------------------------------------------------

extern "C"
int WINAPI
DllEntryPoint(HINSTANCE /*hinst*/, unsigned long /*reason*/, void* /*lpReserved*/)
{
	return 1;
}
//---------------------------------------------------------------------------
static tjs_int GlobalRefCountAtInit = 0;

EXPORT(HRESULT) V2Link(iTVPFunctionExporter *exporter)
{
	// スタブの初期化(必ず記述する)
	TVPInitImportStub(exporter);

	tTJSVariant val;

	// TJS のグローバルオブジェクトを取得する
	iTJSDispatch2 * global = TVPGetScriptDispatch();

	TVPExecuteExpression(TJS_W("const gpDInput = 3, gpFFDInput = 2, gpXInput = 1,\
		gpButtonDpadUp = 0x00000001, gpButtonDpadDown = 0x00000002, gpButtonDpadLeft = 0x00000004,\
		gpButtonDpadRight = 0x00000008, gpButtonStart = 0x00000010, gpButtonBack = 0x00000020,\
		gpButtonLeftThumb = 0x00000040, gpButtonRightThumb = 0x00000080, gpButtonLeftShoulder = 0x00000100,\
		gpButtonRightShoulder = 0x00000200, gpButtonA = 0x00001000, gpButtonB = 0x00002000,\
		gpButtonX = 0x00004000, gpButtonY = 0x00008000, gpLeftAxisX = 0x00010000, gpLeftAxisY = 0x00020000,\
		gpRightAxisX = 0x00040000, gpRightAxisY = 0x00080000, gpLeftTrigger = 0x00100000, gpRightTrigger = 0x00200000,\
		gpDIAxisX = 0, gpDIAxisY = 1, gpDIAxisZ = 2, gpDIAxisRotX = 3, gpDIAxisRotY = 4, gpDIAxisRotZ = 5,\
		gpDISlider_0 = 6, gpDISlider_1 = 7, gpDIPOV_0 = 8, gpDIPOV_1 = 9, gpDIPOV_2 = 10, gpDIPOV_3 = 11,\
		gpDIButton1 = 12, gpDIButton2 = 13, gpDIButton3 = 14, gpDIButton4 = 15, gpDIButton5 = 16,\
		gpDIButton6 = 17, gpDIButton7 = 18, gpDIButton8 = 19, gpDIButton9 = 20, gpDIButton10 = 21,\
		gpDIButton11 = 22, gpDIButton12 = 23, gpDIButton13 = 24, gpDIButton14 = 25, gpDIButton15 = 26,\
		gpDIButton16 = 27, gpDIButton17 = 28, gpDIButton18 = 29, gpDIButton19 = 30, gpDIButton20 = 31,\
		gpDIButton21 = 32, gpDIButton22 = 33, gpDIButton23 = 34, gpDIButton24 = 35, gpDIButton25 = 36,\
		gpDIButton26 = 37, gpDIButton27 = 38, gpDIButton28 = 39, gpDIButton29 = 40, gpDIButton30 = 41,\
		gpDIButton31 = 42, gpDIButton32 = 43, gpDIDisable = 44,\
		gpTouchNo = 0, gpTouchDown = 1, gpTouchLiftoff = 0;"));
	{
		//-----------------------------------------------------------------------
		// 1 まずクラスオブジェクトを作成
		iTJSDispatch2 * tjsclass = Create_NC_GamepadPort();

		// 2 tjsclass を tTJSVariant 型に変換
		val = tTJSVariant(tjsclass);

		// 3 すでに val が tjsclass を保持しているので、tjsclass は
		//   Release する
		tjsclass->Release();

		// 4 global の PropSet メソッドを用い、オブジェクトを登録する
		global->PropSet(
			TJS_MEMBERENSURE, // メンバがなかった場合には作成するようにするフラグ
			TJS_W("GamepadPort"), // メンバ名 ( かならず TJS_W( ) で囲む )
			NULL, // ヒント ( 本来はメンバ名のハッシュ値だが、NULL でもよい )
			&val, // 登録する値
			global // コンテキスト ( global でよい )
			);
		//-----------------------------------------------------------------------

		//-----------------------------------------------------------------------
		// 1 まずクラスオブジェクトを作成
		tjsclass = Create_NC_Gamepad();

		// 2 tjsclass を tTJSVariant 型に変換
		val = tTJSVariant(tjsclass);

		// 3 すでに val が tjsclass を保持しているので、tjsclass は
		//   Release する
		tjsclass->Release();

		// 4 global の PropSet メソッドを用い、オブジェクトを登録する
		global->PropSet(
			TJS_MEMBERENSURE, // メンバがなかった場合には作成するようにするフラグ
			TJS_W("Gamepad"), // メンバ名 ( かならず TJS_W( ) で囲む )
			NULL, // ヒント ( 本来はメンバ名のハッシュ値だが、NULL でもよい )
			&val, // 登録する値
			global // コンテキスト ( global でよい )
			);
		//-----------------------------------------------------------------------
	}

	// - global を Release する
	global->Release();

	// もし、登録する関数が複数ある場合は 1 ～ 4 を繰り返す

	// val をクリアする。
	// これは必ず行う。そうしないと val が保持しているオブジェクト
	// が Release されず、次に使う TVPPluginGlobalRefCount が正確にならない。
	val.Clear();


	// この時点での TVPPluginGlobalRefCount の値を
	GlobalRefCountAtInit = TVPPluginGlobalRefCount;
	// として控えておく。TVPPluginGlobalRefCount はこのプラグイン内で
	// 管理されている tTJSDispatch 派生オブジェクトの参照カウンタの総計で、
	// 解放時にはこれと同じか、これよりも少なくなってないとならない。
	// そうなってなければ、どこか別のところで関数などが参照されていて、
	// プラグインは解放できないと言うことになる。

	return S_OK;
}
//---------------------------------------------------------------------------
EXPORT(HRESULT) V2Unlink()
{
	// 吉里吉里側から、プラグインを解放しようとするときに呼ばれる関数。

	// もし何らかの条件でプラグインを解放できない場合は
	// この時点で E_FAIL を返すようにする。
	// ここでは、TVPPluginGlobalRefCount が GlobalRefCountAtInit よりも
	// 大きくなっていれば失敗ということにする。
	if(TVPPluginGlobalRefCount > GlobalRefCountAtInit) return E_FAIL;
		// E_FAIL が帰ると、Plugins.unlink メソッドは偽を返す

	/*
		ただし、クラスの場合、厳密に「オブジェクトが使用中である」ということを
		知るすべがありません。基本的には、Plugins.unlink によるプラグインの解放は
		危険であると考えてください (いったん Plugins.link でリンクしたら、最後ま
		でプラグインを解放せず、プログラム終了と同時に自動的に解放させるのが吉)。
	*/


	// - まず、TJS のグローバルオブジェクトを取得する
	iTJSDispatch2 * global = TVPGetScriptDispatch();

	// - global の DeleteMember メソッドを用い、オブジェクトを削除する
	if(global)
	{
		// TJS 自体が既に解放されていたときなどは
		// global は NULL になり得るので global が NULL でない
		// ことをチェックする

		global->DeleteMember(
			0, // フラグ ( 0 でよい )
			TJS_W("Gamepad"), // メンバ名
			NULL, // ヒント
			global // コンテキスト
			);

		global->DeleteMember(
			0, // フラグ ( 0 でよい )
			TJS_W("GamepadPort"), // メンバ名
			NULL, // ヒント
			global // コンテキスト
			);
	}

	// - global を Release する
	if(global) global->Release();

	// スタブの使用終了(必ず記述する)
	TVPUninitImportStub();

	return S_OK;
}
//---------------------------------------------------------------------------

