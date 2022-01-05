#ifndef KRSOLOUD_BASE_H
#define KRSOLOUD_BASE_H

// -----------------------------------------------------------------

#include <Windows.h>
#include "tp_stub.h"

#include <stdlib.h>
#include <stdio.h>

#include "soloud.h"
/* audiosource */
#include "soloud_bus.h"
#include "soloud_wav.h"
#include "soloud_wavstream.h"
#include "soloud_openmpt.h"
#include "soloud_sfxr.h"
/* filter */
#include "soloud_echofilter.h"
#include "soloud_lofifilter.h"
#include "soloud_flangerfilter.h"
#include "soloud_freeverbfilter.h"
#include "soloud_biquadresonantfilter.h"

// -----------------------------------------------------------------

#define DEFAULT_VOLUME -1.0f
#define DEFAULT_PAN 0.0f
#define DEFAULT_POSX 0.0
#define DEFAULT_POSY 0.0
#define DEFAULT_POSZ 0.0
#define DEFAULT_VELX 0.0f
#define DEFAULT_VELY 0.0f
#define DEFAULT_VELZ 0.0f
#define MAX_AUDIOSOURCE 30
#define MAX_FILTER 30

extern SoLoud::Soloud soloud;
extern SoLoud::Filter* filter[MAX_FILTER * 5];
extern SoLoud::AudioSource* audiosource[MAX_AUDIOSOURCE * 5];

// -----------------------------------------------------------------

namespace KrSoLoud
{
	typedef tjs_int GroupHandle;
	typedef tjs_int AudioHandle;
	typedef tjs_int FilterHandle;
};

// -----------------------------------------------------------------

#endif