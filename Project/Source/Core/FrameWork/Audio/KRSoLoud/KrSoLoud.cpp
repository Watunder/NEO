#include "krsoloud.h"

// -----------------------------------------------------------------

SoLoud::Soloud soloud;
SoLoud::Filter* filter[MAX_FILTER * 5] = { NULL };
SoLoud::AudioSource* audiosource[MAX_AUDIOSOURCE * 5] = { NULL };

// -----------------------------------------------------------------
namespace KrSoLoud
{
	// Ctor
	KrSoloud::KrSoloud() {

		// init value
		audiosource_number = 0;
		wavstream_number = 0;
		openmpt_number = 0;
		sfxr_number = 0;
		wav_number = 0;
		bus_number = 0;
		filter_number = 0;
		echofilter_number = 0;
		lofifilter_number = 0;
		verbfilter_number = 0;
		bqrffilter_number = 0;
		flangerfilter_number = 0;

		//init method
		soloud.init();
		bgm_group = soloud.createVoiceGroup();
		voice_group = soloud.createVoiceGroup();
		effect_group = soloud.createVoiceGroup();
	};

	// Dtor
	KrSoloud::~KrSoloud() {

	};

	// -------------------------------------------------------------

	tjs_error
	KrSoloud::set3dSoundSpeed(const tTJSVariant* speed) {

		soloud.set3dSoundSpeed((float)(speed->AsReal()));

		return TJS_S_OK;
	}

	tjs_error
	KrSoloud::get3dSoundSpeed(tTJSVariant* result) {

		*result = (tjs_real)soloud.get3dSoundSpeed();

		return TJS_S_OK;
	}

	// -------------------------------------------------------------

	FilterHandle
	KrSoloud::_loadFilter(tjs_int type, float param_1, float param_2, float param_3, float param_4) {

		FilterHandle newfilter;
		switch (type) {

		case ECHO:
			echofilter[echofilter_number].setParams(param_1, param_2, param_3);
			filter[filter_number] = &echofilter[echofilter_number];
			echofilter_number++;
			break;

		case LOFI:
			lofifilter[lofifilter_number].setParams(param_1, param_2);
			filter[filter_number] = &lofifilter[lofifilter_number];
			lofifilter_number++;
			break;

		case VERB:
			verbfilter[verbfilter_number].setParams(param_1, param_2, param_3, param_4);
			filter[filter_number] = &verbfilter[verbfilter_number];
			verbfilter_number++;
			break;

		case FLANGER:
			flangerfilter[flangerfilter_number].setParams(param_1, param_2);
			filter[filter_number] = &flangerfilter[flangerfilter_number];
			flangerfilter_number++;
			break;

		case BQRF:
			bqrffilter[bqrffilter_number].setParams((int)param_1, param_2, param_3);
			filter[filter_number] = &bqrffilter[bqrffilter_number];
			bqrffilter_number++;
			break;
		}

		filter_number++;
		return newfilter = filter_number;
	}


	AudioHandle
	KrSoloud::_loadSound(tjs_int audioaource_type, tTJSVariantOctet* octet, BOOL is3d) {

		// get data from octet
		const unsigned char* aData = NULL;
		unsigned int aDataLen = 0;
		if (octet != NULL) {
			aData = octet->GetData();
			aDataLen = octet->GetLength();
		}

		// load sound from memory
		AudioHandle newsound;
		switch (audioaource_type) {

		case WAVSTREAM:
			wavstream[wavstream_number].loadMem(aData, aDataLen);
			audiosource[audiosource_number] = &wavstream[wavstream_number];
			wavstream_number++;
			break;

		case OPENMPT:
			openmpt[openmpt_number].loadMem(aData, aDataLen);
			audiosource[audiosource_number] = &openmpt[openmpt_number];
			openmpt_number++;
			break;

		case SFXR:
			sfxr[wav_number].loadParamsMem(const_cast<unsigned char*>(aData), aDataLen);
			audiosource[audiosource_number] = &sfxr[sfxr_number];
			break;

		case WAV:
			wav[wav_number].loadMem(aData, aDataLen);
			audiosource[audiosource_number] = &wav[wav_number];
			wav_number++;
			break;

		case BUS:
			audiosource[audiosource_number] = &bus[bus_number];
			bus_number++;
			break;

		default:
			return TJS_E_INVALIDPARAM;
		}

		// sound ready to play
		if (is3d) {
			newsound = soloud.play3d(*audiosource[audiosource_number],
									 DEFAULT_POSX,
									 DEFAULT_POSY,
									 DEFAULT_POSZ,
									 DEFAULT_VELX,
									 DEFAULT_VELY,
									 DEFAULT_VELZ,
									 DEFAULT_VOLUME,
									 DEFAULT_PAN,
									 false | audioaource_type ^ BUS);
		}
		else {
			newsound = soloud.play(*audiosource[audiosource_number], DEFAULT_VOLUME, DEFAULT_PAN, false | audioaource_type ^ BUS);
		}

		audiosource_number++;
		return newsound;
	}

	AudioHandle
	KrSoloud::_MixBus(tjs_int audiosource_number, tjs_int bus_number, BOOL is3d)  {

		// bussound ready to play
		AudioHandle newbussound;
		if (is3d) {
			newbussound = bus[bus_number].play3d(*audiosource[audiosource_number],
												 DEFAULT_POSX,
												 DEFAULT_POSY,
												 DEFAULT_POSZ,
												 DEFAULT_VELX,
												 DEFAULT_VELY,
												 DEFAULT_VELZ,
												 DEFAULT_VOLUME,
												 true);
		}
		else {
			newbussound = bus[bus_number].play(*audiosource[audiosource_number], DEFAULT_VOLUME, DEFAULT_PAN, true);
		}

		return newbussound;
	}

	void
	KrSoloud::_addSoundToGroup(AudioHandle audiohanle, GroupHandle grouphanle) {

		if (soloud.isValidVoiceHandle(audiohanle) & soloud.isVoiceGroup(grouphanle)) {
			soloud.addVoiceToGroup(grouphanle, audiohanle);
		}
		else {
			TVPThrowExceptionMessage(TJS_W("invalid sound or group"));
		}
	}

	// -------------------------------------------------------------

	tjs_error
	KrSoloud::set3dListenerParameters(tTJSVariant* result, tjs_int numparams, tTJSVariant** param) {

		if (numparams < 12) {
			return TJS_E_BADPARAMCOUNT;
		}

		float aPosX = (float)(tjs_real)*param[0];
		float aPosY = (float)(tjs_real)*param[1];
		float aPosZ = (float)(tjs_real)*param[2];

		float aAtX = (float)(tjs_real)*param[3];
		float aAtY = (float)(tjs_real)*param[4];
		float aAtZ = (float)(tjs_real)*param[5];

		float aUpX = (float)(tjs_real)*param[6];
		float aUpY = (float)(tjs_real)*param[7];
		float aUpZ = (float)(tjs_real)*param[8];

		float aVelocityX = (float)(tjs_real)*param[9];
		float aVelocityY = (float)(tjs_real)*param[10];
		float aVelocityZ = (float)(tjs_real)*param[11];

		soloud.set3dListenerParameters(aPosX, aPosY, aPosZ,
			aAtX, aAtY, aAtZ,
			aVelocityX, aVelocityY, aVelocityZ);
		soloud.update3dAudio();

		return TJS_S_OK;
	}

	tjs_error
	KrSoloud::loadFilter(tTJSVariant* result, tjs_int numparams, tTJSVariant** param) {

		if (numparams < 2) {
			return TJS_E_BADPARAMCOUNT;
		}

		tjs_int type = (tjs_int)*param[0];
		float param_1 = (float)(tjs_real)*param[1];
		float param_2 = (float)(tjs_real)*param[2];
		float param_3 = (float)(tjs_real)*param[3];
		float param_4 = (float)(tjs_real)*param[4];

		*result = _loadFilter(type,
							  param_1,
							  param_2,
							  param_3,
							  param_4);

		return TJS_S_OK;
	}

	tjs_error
	KrSoloud::loadSound(tTJSVariant* result, tjs_int numparams, tTJSVariant** param) {

		if (numparams < 3) {
			return TJS_E_BADPARAMCOUNT;
		}

		tjs_int audioaource_type = (tjs_int)*param[0];
		tTJSVariantOctet* octet = param[1]->AsOctet();
		BOOL is3d = (tjs_int)*param[2];

		*result = _loadSound(audioaource_type, octet, is3d);

		return TJS_S_OK;
	}

	tjs_error
	KrSoloud::MixBus(tTJSVariant* result, tjs_int numparams, tTJSVariant** param) {

		if (numparams < 2) {
			return TJS_E_BADPARAMCOUNT;
		}

		/* AudioHandle to audiosoure_number */
		tjs_int audiosource_number = (tjs_int)(round((tjs_real)*param[0] / 2049 / 2));
		/* AudioHandle(BUS) to bus_number */
		tjs_int bus_number = (tjs_int)(round((tjs_int)*param[1] / 2049 / 2));
		BOOL is3d = (tjs_int)*param[2];

		*result = _MixBus(audiosource_number, bus_number, is3d);

		return TJS_S_OK;
	}

	tjs_error
	KrSoloud::addSoundToGroup(tTJSVariant* result, tjs_int numparams, tTJSVariant** param) {

		if (numparams < 2) {
			return TJS_E_BADPARAMCOUNT;
		}

		AudioHandle audiohanle = (tjs_int)*param[0];
		GroupHandle grouphanle = (tjs_int)*param[1];

		_addSoundToGroup(audiohanle, grouphanle);

		return TJS_S_OK;
	}

	tjs_error
	KrSoloud::play(tTJSVariant* result, tjs_int numparams, tTJSVariant** param) {
		return TJS_S_OK;
	}

	// -------------------------------------------------------------

	tjs_error
	KrSoloud::finalize() {
		return TJS_S_OK;
	}

	tjs_error
	KrSoloud::CreateNew(KrSoloud*& instance, tjs_int numparams, tTJSVariant** param) {
		instance = new KrSoloud();
		return TJS_S_OK;
	}
}

// -----------------------------------------------------------------