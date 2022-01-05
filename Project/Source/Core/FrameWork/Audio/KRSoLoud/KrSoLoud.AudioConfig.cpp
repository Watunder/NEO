#include "krsoloud.audioconfig.h"

// -----------------------------------------------------------------

namespace KrSoLoud
{
	AudioConfig::AudioConfig(){
		
	};

	AudioConfig::~AudioConfig() {
		
	}

	// -------------------------------------------------------------

	void
	AudioConfig::_update() {
		soloud.update3dAudio();
	}

	// -------------------------------------------------------------

	tjs_error 
	AudioConfig::setLooping(tTJSVariant* result, tTJSVariant* audiohandle, tTJSVariant* islooping) {

		tjs_int audiosource_number = (tjs_int)(round(audiohandle->AsReal() / 2049 / 2));
		bool aloop = (bool)(islooping->AsInteger());

		audiosource[audiosource_number]->setLooping(aloop);

		return TJS_S_OK;
	}

	tjs_error
	AudioConfig::setFilter(tTJSVariant* result, tTJSVariant* audiohandle, tTJSVariant* filterhandle) {

		tjs_int audiosource_number = (tjs_int)(round(audiohandle->AsReal() / 2049 / 2));
		tjs_int filter_number = (tjs_int)(filterhandle->AsInteger());

		audiosource[audiosource_number]->setFilter(audiosource_number, filter[filter_number]);

		return TJS_S_OK;
	}

	tjs_error
	AudioConfig::setSingleInstance(tTJSVariant* result, tTJSVariant* audiohandle, tTJSVariant* isSingleInstance) {

		tjs_int audiosource_number = (tjs_int)(round(audiohandle->AsReal() / 2049 / 2));
		bool aSingleInstance = (bool)(isSingleInstance->AsInteger());

		audiosource[audiosource_number]->setSingleInstance(aSingleInstance);

		return TJS_S_OK;
	}

	tjs_error
	AudioConfig::set3dMinMaxDistance(tTJSVariant* result, tjs_int numparams, tTJSVariant** param) {

		if (numparams < 3) {
			return TJS_E_BADPARAMCOUNT;
		}

		AudioHandle audiosource_number = (tjs_int)(round((tjs_real)*param[0] / 2049 / 2));

		float aMinDistance = (float)(tjs_real)*param[1];
		float aMaxDistance = (float)(tjs_real)*param[2];

		audiosource[audiosource_number]->set3dMinMaxDistance(aMinDistance, aMaxDistance);

		_update();

		return TJS_S_OK;
	}

	tjs_error
	AudioConfig::set3dAttenuation(tTJSVariant* result, tjs_int numparams, tTJSVariant** param) {

		if (numparams < 3) {
			return TJS_E_BADPARAMCOUNT;
		}

		AudioHandle audiosource_number = (tjs_int)(round((tjs_int)*param[0] / 2049 / 2));

		unsigned int aAttenuationModel = (tjs_int)*param[1];
		float aAttenuationRolloffFactor = (float)(tjs_real)*param[2];

		audiosource[audiosource_number]->set3dAttenuation(aAttenuationModel, aAttenuationRolloffFactor);

		_update();

		return TJS_S_OK;
	}

	tjs_error
	AudioConfig::set3dDopplerFactor(tTJSVariant* result, tTJSVariant* audiohandle, tTJSVariant* DopplerFactor) {

		tjs_int audiosource_number = (tjs_int)(round(audiohandle->AsReal() / 2049 / 2));
		float aDopplerFactor = (float)(DopplerFactor->AsReal());

		audiosource[audiosource_number]->set3dDopplerFactor(aDopplerFactor);

		return TJS_S_OK;
	}

	tjs_error
	AudioConfig::set3dListenerRelative(tTJSVariant* result, tTJSVariant* audiohandle, tTJSVariant* is3dListenerRelative) {

		tjs_int audiosource_number = (tjs_int)(round(audiohandle->AsReal() / 2049 / 2));
		bool aListenerRelative = (bool)(is3dListenerRelative->AsInteger());

		audiosource[audiosource_number]->set3dListenerRelative(aListenerRelative);

		return TJS_S_OK;
	}

	tjs_error
	AudioConfig::set3dDistanceDelay(tTJSVariant* result, tTJSVariant* audiohandle, tTJSVariant* is3dDistanceDelay) {

		tjs_int audiosource_number = (tjs_int)(round(audiohandle->AsReal() / 2049 / 2));
		bool a3dDistanceDelay = (bool)(is3dDistanceDelay->AsInteger());

		audiosource[audiosource_number]->set3dDistanceDelay(a3dDistanceDelay);

		return TJS_S_OK;
	}

	tjs_error
	AudioConfig::setInaudibleBehavior(tTJSVariant* result, tjs_int numparams, tTJSVariant** param) {

		if (numparams < 3) {
			return TJS_E_BADPARAMCOUNT;
		}

		AudioHandle audiosource_number = (tjs_int)(round((tjs_real)*param[0] / 2049 / 2));

		float aMustTick = (float)(tjs_real)*param[1];
		float aKill = (float)(tjs_real)*param[2];

		audiosource[audiosource_number]->setInaudibleBehavior(aMustTick, aKill);

		_update();

		return TJS_S_OK;
	}

	tjs_error
	AudioConfig::setVolume(tTJSVariant* result, tTJSVariant* audiohandle, tTJSVariant* volume) {

		tjs_int audiosource_number = (tjs_int)(round(audiohandle->AsReal() / 2049 / 2));
		float aVolume = (float)(volume->AsReal());

		audiosource[audiosource_number]->setVolume(aVolume);

		return TJS_S_OK;
	}

	// -------------------------------------------------------------

	tjs_error
	AudioConfig::setPause(tTJSVariant* result, tTJSVariant* audiohandle, tTJSVariant* isPaused) {
		
		AudioHandle handle = (tjs_int)(audiohandle->AsInteger());
		bool aPause = (bool)(isPaused->AsInteger());

		soloud.setPause(handle, aPause);

		return TJS_S_OK;
	}

	tjs_error
	AudioConfig::set3dSourceParameters(tTJSVariant* result, tjs_int numparams, tTJSVariant** param) {

		if (numparams < 7) {
			return TJS_E_BADPARAMCOUNT;
		}

		AudioHandle handle = (tjs_int)* param[0];

		float aPosX = (float)(tjs_real)*param[1];
		float aPosY = (float)(tjs_real)*param[2];
		float aPosZ = (float)(tjs_real)*param[3];

		float aVelocityX = (float)(tjs_real)*param[4];
		float aVelocityY = (float)(tjs_real)*param[5];
		float aVelocityZ = (float)(tjs_real)*param[6];

		soloud.set3dSourceParameters(handle,
									 aPosX, aPosY, aPosZ,
									 aVelocityX, aVelocityY, aVelocityZ);
		_update();

		return TJS_S_OK;
	}

	tjs_error
	AudioConfig::set3dSourcePosition(tTJSVariant* result, tjs_int numparams, tTJSVariant** param) {

		if (numparams < 4) {
			return TJS_E_BADPARAMCOUNT;
		}

		AudioHandle handle = (tjs_int)*param[0];

		float aPosX = (float)(tjs_real)*param[1];
		float aPosY = (float)(tjs_real)*param[2];
		float aPosZ = (float)(tjs_real)*param[3];

		soloud.set3dSourcePosition(handle, aPosX, aPosY, aPosZ);

		_update();

		return TJS_S_OK;
	}

	tjs_error
	AudioConfig::set3dSourceVelocity(tTJSVariant* result, tjs_int numparams, tTJSVariant** param) {

		if (numparams < 4) {
			return TJS_E_BADPARAMCOUNT;
		}

		AudioHandle handle = (tjs_int)*param[0];

		float aVelocityX = (float)(tjs_real)*param[1];
		float aVelocityY = (float)(tjs_real)*param[2];
		float aVelocityZ = (float)(tjs_real)*param[3];

		soloud.set3dSourceVelocity(handle, aVelocityX, aVelocityY, aVelocityZ);

		_update();

		return TJS_S_OK;
	}

	// -------------------------------------------------------------

	tjs_error
	AudioConfig::CreateNew(AudioConfig*& instance, tjs_int numparams, tTJSVariant** param) {
		instance = new AudioConfig();
		return TJS_S_OK;
	}
}

// -----------------------------------------------------------------