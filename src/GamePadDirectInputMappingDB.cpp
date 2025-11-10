/****************************************************************************/
/*! @file
@brief 

-----------------------------------------------------------------------------
	Copyright (C) 2008 Takenori Imoto. All rights reserved.
	( http://www.kaede-software.com/ )
-----------------------------------------------------------------------------
@author		T.Imoto
@date		2008/02/01
@note

	SQLite の DB を使うようにしたほうがいいかも

	vender_tabel( pid integer primary, name text )
	product_table( pid integer primary, vender_id integer, product_id integer, map_id integer, name text )
	map_table( pid integer primary, mapping... )


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

#include "GamePadDirectInputDevice.h"
#include "GamePadDirectInputMappingDB.h"

namespace gamepad {

struct ProductMappingTable
{
	unsigned short	product;
	const DirectInputObjectMappingTable*	table;
	// 存在する軸やボタンの数も保持しておいた方がよさげ
};
struct VenderMappingTable
{
	unsigned short	vender;
	const ProductMappingTable*	table;
};
// Logtech Rumble Pad 2
static const DirectInputObjectMappingTable LogitechRumblePad2 =
{
	{	// buttons
		{ DIObj_POV_0, 1 },	// UP
		{ DIObj_POV_0, 1 },	// DOWN
		{ DIObj_POV_0, 1 },	// LEFT
		{ DIObj_POV_0, 1 },	// RIGHT

		{ DIObj_Button10, 1 },	// START
		{ DIObj_Button9, 1 },	// BACK
		{ DIObj_Button11, 1 },	// 左サムボタン
		{ DIObj_Button12, 1 },	// 右サムボタン
		{ DIObj_Button5, 1 },	// 左肩ボタン
		{ DIObj_Button6, 1 },	// 右肩ボタン
#if 0	// 
		{ DIObj_Button2, 1 },	// Aボタン
		{ DIObj_Button3, 1 },	// Bボタン
		{ DIObj_Button1, 1 },	// Xボタン
		{ DIObj_Button4, 1 },	// Yボタン
#else
		{ DIObj_Button1, 1 },	// Aボタン
		{ DIObj_Button2, 1 },	// Bボタン
		{ DIObj_Button3, 1 },	// Xボタン
		{ DIObj_Button4, 1 },	// Yボタン
#endif
	},
	{	// triggers
		{ DIObj_Button7, 1 },	// 左トリガー
		{ DIObj_Button8, 1 },	// 右トリガー
	},
	{	// axis
		{ DIObj_AxisX, 1 },		// 左スティックX軸
		{ DIObj_AxisY, -1 },	// 左スティックY軸
		{ DIObj_AxisZ, 1 },		// 右スティックX軸
		{ DIObj_AxisRotZ, -1 },	// 右スティックY軸
	},
	true,
};
static const ProductMappingTable LogitechProduct[] = 
{
	{ 0xC218, &LogitechRumblePad2 },
	{ 0, NULL },	// 終端
};
static const VenderMappingTable VenderTable[] = 
{
	{ 0x046D, LogitechProduct },
	{ 0, NULL },	// 終端
};

int CopyKeyMap( DirectInputObjectMappingTable& map, unsigned short vender, unsigned short product )
{
	for( int v = 0; ; v++ ) {
		if( VenderTable[v].table == NULL ) break;

		if( VenderTable[v].vender == vender ) {
			// found vender
			const ProductMappingTable*	table = VenderTable[v].table;
			for( int p = 0; ; p++ ) {
				if( table[p].table == NULL ) break;

				if( table[p].product == product ) {
					// found product
					memcpy( &map, table[p].table, sizeof(DirectInputObjectMappingTable) );
					return KMR_SuccessProductMapping;
				}
			}
			// 見付からなかった。同じベンダーの最初のテーブルをコピーする
			if( table[0].table != NULL ) {
				memcpy( &map, table[0].table, sizeof(DirectInputObjectMappingTable) );
				return KMR_SuccessVenderMapping;
			}
		}
	}
	return KMR_NotFound;
}


}; // namespace gamepad

