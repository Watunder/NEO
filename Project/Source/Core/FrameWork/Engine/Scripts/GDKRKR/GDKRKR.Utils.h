#ifndef TJS2PARSER_BASE_H
#define TJS2PARSER_BASE_H

#include <Godot.hpp>
#include <Reference.hpp>

#include "tjsCommHead.h"
#include "tjs.h"
#include "tjsArray.h"
#include "UtilStreams.h"
#include "CharacterSet.h"

// -----------------------------------------------------------------

extern void TVPLoadMessage();

static void
Error_TJStoGD(tjs_error hr) {

	switch (hr)
	{
	case TJS_E_MEMBERNOTFOUND:
		throw godot::Error::ERR_METHOD_NOT_FOUND;
		break;

	case TJS_E_NOTIMPL:
		TJS_eTJSError(TJSNotImplemented);
		break;

	case TJS_E_INVALIDPARAM:
		TJS_eTJSError(TJSInvalidParam);
		break;

	case TJS_E_BADPARAMCOUNT:
		TJS_eTJSError(TJSBadParamCount);
		break;

	case TJS_E_INVALIDTYPE:
		throw godot::Error::ERR_INVALID_PARAMETER;
		break;

	case TJS_E_INVALIDOBJECT:
		TJS_eTJSError(TJSInvalidObject);
		break;

	case TJS_E_ACCESSDENYED:
		throw godot::Error::ERR_BUSY;
		break;

	case TJS_E_NATIVECLASSCRASH:
		TJS_eTJSError(TJSNativeClassCrash);
		break;

	default:
		if (TJS_FAILED(hr))
		{
			throw godot::Error::ERR_BUG;
		}
		break;
	}
}

static tTJSVariant
Variant_GDtoTJS(godot::Variant v) {

	switch (v.get_type()) {

	case godot::Variant::BOOL: return static_cast<tjs_int>(v);
	case godot::Variant::INT: return static_cast<tjs_int>(v);
	case godot::Variant::REAL: return static_cast<tjs_real>(v);
	case godot::Variant::STRING: return (tjs_char*)(godot::String(v).unicode_str());
	//case godot::Variant::VECTOR2: 
	//case godot::Variant::RECT2:
	//case godot::Variant::VECTOR3:
	//case godot::Variant::TRANSFORM2D:
	//case godot::Variant::PLANE:
	//case godot::Variant::QUAT:
	//case godot::Variant::RECT3:
	//case godot::Variant::BASIS:
	//case godot::Variant::TRANSFORM:
	//case godot::Variant::COLOR:
	//case godot::Variant::NODE_PATH:
	//case godot::Variant::_RID: return (tjs_int)(godot::RID(godot::Object::cast_to<godot::Object>(v)).get_id());
	//case godot::Variant::OBJECT: return  new iTJSDispatch2Wrapper(godot::Object::cast_to<godot::Object>(v));
	//case godot::Variant::DICTIONARY:
	case godot::Variant::ARRAY:
	case godot::Variant::POOL_BYTE_ARRAY:
	case godot::Variant::POOL_INT_ARRAY:
	case godot::Variant::POOL_REAL_ARRAY:
	case godot::Variant::POOL_STRING_ARRAY:
	case godot::Variant::POOL_VECTOR2_ARRAY:
	case godot::Variant::POOL_VECTOR3_ARRAY:
	case godot::Variant::POOL_COLOR_ARRAY: {

		godot::Array gd_array(v);
		iTJSDispatch2* tjs_array = TJSCreateArrayObject();

		for (tjs_int num = 0; num < gd_array.size(); num++) {

			tTJSVariant param = Variant_GDtoTJS(gd_array[num]);
			tjs_array->PropSetByNum(TJS_MEMBERENSURE, num, &param, tjs_array);
		}

		return tTJSVariant(tjs_array, tjs_array);
	}
	default: return tTJSVariant();
	}
}

static godot::Variant
Variant_TJStoGD(tTJSVariant v) {
	
	switch (v.Type()) {

	case tvtVoid: return godot::Variant();
	case tvtReal: return godot::Variant(v.AsReal());
	case tvtString: return godot::Variant(v.GetString());
	case tvtInteger: return godot::Variant(v.AsInteger());
	case tvtObject: {

		godot::Array gd_array;
		iTJSDispatch2* tjs_array = v.AsObjectNoAddRef();

		for (tjs_int num = 0, hr = 0; hr == 0; num++) {

			tTJSVariant param;
			hr = tjs_array->PropGetByNum(TJS_MEMBERMUSTEXIST, num, &param, tjs_array);
			if (hr == TJS_S_OK) gd_array.append(Variant_TJStoGD(param));
		}

		//if (tjs_array) tjs_array->Release(); // do not release iTJSDispatch2 ptr here
		return gd_array;
	}
	default: return godot::Variant();
	}
}

// -----------------------------------------------------------------

#endif