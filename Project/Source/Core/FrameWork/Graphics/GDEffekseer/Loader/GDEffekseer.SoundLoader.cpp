#include <Godot.hpp>
#include <AudioStream.hpp>
#include "GDEffekseer.SoundLoader.h"
#include "GDEffekseer.Utils.h"
#include "GDEffekseer.SoundResources.h"

namespace GDEffekseer
{

SoundLoader::SoundLoader(godot::Ref<godot::Reference> soundContext)
	: soundContext_(soundContext)
{
}

Effekseer::SoundDataRef SoundLoader::Load(const char16_t* path)
{
	// Load by Godot
	godot::Ref<godot::AudioStream> resource = soundContext_->call("load_sound", ToGdString(path));
	if (!resource.is_valid())
	{
		return nullptr;
	}

	auto result = Effekseer::MakeRefPtr<SoundData>(resource);

	return result;
}

void SoundLoader::Unload(Effekseer::SoundDataRef textureData)
{
}

} // namespace GDEffekseer
