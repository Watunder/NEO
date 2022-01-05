#ifndef KRSOLOUD_H
#define KRSOLOUD_H

#include "krsoloud.base.h"

// -----------------------------------------------------------------



// -----------------------------------------------------------------

namespace KrSoLoud
{
	enum AUDIOSOURCE_TYPE
	{
		WAV = 0,
		OPENMPT,
		WAVSTREAM,
		SFXR,
		BUS
	};

	enum FILTER_TYPE
	{
		ECHO = 10,
		LOFI,
		FLANGER,
		VERB,
		BQRF
	};

	class KrSoloud
	{
	public:
		/* audiosource number */
		tjs_int audiosource_number;
		// wavstream number
		tjs_int wavstream_number;
		// openmpt number
		tjs_int openmpt_number;
		// sfxr number
		tjs_int sfxr_number;
		// wav number
		tjs_int wav_number;
		// bus number
		tjs_int bus_number;
		/* filter number */
		tjs_int filter_number;
		// echofilter number
		tjs_int echofilter_number;
		// lofifilter number
		tjs_int lofifilter_number;
		// verbfilter number
		tjs_int verbfilter_number;
		// bqrffilter number
		tjs_int bqrffilter_number;
		// flangerfilter number
		tjs_int flangerfilter_number;
		// bgm group
		AudioHandle bgm_group;
		// voice group
		AudioHandle voice_group;
		// effect group
		AudioHandle effect_group;

		/* audiosource */

		// wavstream source
		SoLoud::WavStream wavstream[MAX_AUDIOSOURCE];

		// opoenmpt source
		SoLoud::Openmpt openmpt[MAX_AUDIOSOURCE];

		// sfxr source
		SoLoud::Sfxr sfxr[MAX_AUDIOSOURCE];

		// wav source
		SoLoud::Wav wav[MAX_AUDIOSOURCE];

		// bus source
		SoLoud::Bus bus[MAX_AUDIOSOURCE];

		/* filter */

		// echofilter
		SoLoud::EchoFilter echofilter[MAX_FILTER];

		// lofifilter
		SoLoud::LofiFilter lofifilter[MAX_FILTER];

		// verbfilter
		SoLoud::FreeverbFilter verbfilter[MAX_FILTER];

		// flangerfilter
		SoLoud::FlangerFilter flangerfilter[MAX_FILTER];

		// bqrffilter
		SoLoud::BiquadResonantFilter bqrffilter[MAX_FILTER];

		// Ctor
		KrSoloud();
		// Dtor
		~KrSoloud();
		// (internal) _loadFilter
		FilterHandle _loadFilter(tjs_int type, float param_1, float param_2, float param_3, float param_4);
		// (internal) _loadSoloud
		AudioHandle _loadSound(tjs_int audioaource_type, tTJSVariantOctet* octet, BOOL is3d);
		// (internal) _MixBus 
		AudioHandle _MixBus(tjs_int audiosource_number, tjs_int bus_number, BOOL is3d);
		// (internal) _addSoundToGroup
		void _addSoundToGroup(AudioHandle audiohanle, GroupHandle grouphanle);
		// (TJS Property) setter 3dSoundSpeed
		tjs_error set3dSoundSpeed(const tTJSVariant* speed);
		// (TJS Property) getter 3dSoundSpeed
		tjs_error get3dSoundSpeed(tTJSVariant* result);
		// (TJS Method) set3dListenerParameters
		tjs_error set3dListenerParameters(tTJSVariant* result, tjs_int numparams, tTJSVariant** param);
		// (TJS Method) load filter
		tjs_error loadFilter(tTJSVariant* result, tjs_int numparams, tTJSVariant** param);
		// (TJS Method) load sound
		tjs_error loadSound(tTJSVariant* result, tjs_int numparams, tTJSVariant** param);
		// (TJS Method) mix bus
		tjs_error MixBus(tTJSVariant* result, tjs_int numparams, tTJSVariant** param);
		// (TJS Method) add sound to group
		tjs_error addSoundToGroup(tTJSVariant* result, tjs_int numparams, tTJSVariant** param);
		// (TJS Method) play
		tjs_error play(tTJSVariant* result, tjs_int numparams, tTJSVariant** param);
		// (TJS Method) finalize
		tjs_error finalize();
		// Factory
		static tjs_error CreateNew(KrSoloud*& instance, tjs_int numparams, tTJSVariant** param);
	};
};
// -----------------------------------------------------------------

#endif
