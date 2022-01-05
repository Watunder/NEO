#include "GDEffekseer.SoundResources.h"

namespace GDEffekseer
{

SoundData::SoundData(godot::Ref<godot::AudioStream> stream)
	: stream_(stream)
{
}

SoundData::~SoundData()
{
}

} // namespace GDEffekseer
