#include <ResourceLoader.hpp>
#include "GDEffekseer.ModelLoader.h"
#include "GDEffekseer.RenderResources.h"
#include "GDEffekseer.Utils.h"
#include "EffekseerResource.h"

namespace GDEffekseer
{

Effekseer::ModelRef ModelLoader::Load(const char16_t* path)
{
	// Load by Godot
	auto loader = godot::ResourceLoader::get_singleton();
	auto resource = loader->load(ToGdString(path), "");
	if (!resource.is_valid())
	{
		return nullptr;
	}

	auto efkres = godot::detail::get_custom_class_instance<godot::EffekseerResource>(resource.ptr());
	auto& data = efkres->get_data_ref();

	return Load(data.read().ptr(), data.size());
}

Effekseer::ModelRef ModelLoader::Load(const void* data, int32_t size)
{
	return Effekseer::MakeRefPtr<Model>(data, size);
}

void ModelLoader::Unload(Effekseer::ModelRef data)
{
}

} // namespace GDEffekseer

