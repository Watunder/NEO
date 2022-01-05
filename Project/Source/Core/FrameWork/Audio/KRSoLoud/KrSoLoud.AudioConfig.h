#ifndef KRSOLOUD_AUDIOCONFIG_H
#define KRSOLOUD_AUDIOCONFIG_H

#include "krsoloud.base.h"

// -----------------------------------------------------------------



// -----------------------------------------------------------------

namespace KrSoLoud
{
	class AudioConfig
	{
	public:
		// Ctor
		AudioConfig();
		// Dtor
		~AudioConfig();
		// (internal) _update
		void _update();
		// (TJS Method) setLooping
		tjs_error setLooping(tTJSVariant* result, tTJSVariant* audiohandle, tTJSVariant* islooping);
		// (TJS Method) setFilter
		tjs_error setFilter(tTJSVariant* result, tTJSVariant* audiohandle, tTJSVariant* filterhandle);
		// (TJS Method) setSingleInstance
		tjs_error setSingleInstance(tTJSVariant* result, tTJSVariant* audiohandle, tTJSVariant* isSingleInstance);
		// (TJS Method) set3dMinMaxDistance
		tjs_error set3dMinMaxDistance(tTJSVariant* result, tjs_int numparams, tTJSVariant** param);
		// (TJS Method) set3dSourceAttenuation
		tjs_error set3dAttenuation(tTJSVariant* result, tjs_int numparams, tTJSVariant** param);
		// (TJS Method) set3dDopplerFactor
		tjs_error set3dDopplerFactor(tTJSVariant* result, tTJSVariant* audiohandle, tTJSVariant* DopplerFactor);
		// (DELETE METHOD) set3dProcessing
		// (TJS Method) set3dListenerRelative
		tjs_error set3dListenerRelative(tTJSVariant* result, tTJSVariant* audiohandle, tTJSVariant* is3dListenerRelative);
		// (TJS Method) set3dDistanceDelay
		tjs_error set3dDistanceDelay(tTJSVariant* result, tTJSVariant* audiohandle, tTJSVariant* is3dDistanceDelay);
		// (TJS Method) setInaudibleBehavior
		tjs_error setInaudibleBehavior(tTJSVariant* result, tjs_int numparams, tTJSVariant** param);
		// (NOT INIT) set3dCollider
		// (NOT INIT) set3dAttenuator
		// (TJS Method) setVolume
		tjs_error setVolume(tTJSVariant* result, tTJSVariant* audiohandle, tTJSVariant* volume);
		// (TJS Method) setPause
		tjs_error setPause(tTJSVariant* result, tTJSVariant* audiohandle, tTJSVariant* isPaused);
		// (TJS Method) set3dSourceParameters
		tjs_error set3dSourceParameters(tTJSVariant* result, tjs_int numparams, tTJSVariant** param);
		// (TJS Method) set3dSourcePosition
		tjs_error set3dSourcePosition(tTJSVariant* result, tjs_int numparams, tTJSVariant** param);
		// (TJS Method) set3dSourceVelocity
		tjs_error set3dSourceVelocity(tTJSVariant* result, tjs_int numparams, tTJSVariant** param);
		// Factory
		static tjs_error CreateNew(AudioConfig*& instance, tjs_int numparams, tTJSVariant** param);
	};
}

// -----------------------------------------------------------------

#endif