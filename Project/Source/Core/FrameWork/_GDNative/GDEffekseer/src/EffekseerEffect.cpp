#include <File.hpp>
#include <ResourceLoader.hpp>
#include "EffekseerSystem.h"
#include "EffekseerEffect.h"
#include "GDEffekseer.Utils.h"

namespace godot {

void EffekseerEffect::_register_methods()
{
	register_method("_init", &EffekseerEffect::_init);
	register_method("load", &EffekseerEffect::load);
	register_method("release", &EffekseerEffect::release);
	register_method("resolve_dependencies", &EffekseerEffect::resolve_dependencies);
	register_method("setup", &EffekseerEffect::setup);
	register_property<EffekseerEffect, String>("data_path", 
		&EffekseerEffect::set_data_path, &EffekseerEffect::get_data_path, "");
	register_property<EffekseerEffect, PoolByteArray>("data_bytes", 
		&EffekseerEffect::set_data_bytes, &EffekseerEffect::get_data_bytes, {});
	register_property<EffekseerEffect, Dictionary>("subresources", 
		&EffekseerEffect::set_subresources, &EffekseerEffect::get_subresources, {});
	register_property<EffekseerEffect, float>("scale", 
		&EffekseerEffect::set_scale, &EffekseerEffect::get_scale, 1.0f);
}

EffekseerEffect::EffekseerEffect()
{
}

EffekseerEffect::~EffekseerEffect()
{
	release();
}

void EffekseerEffect::_init()
{
}

void EffekseerEffect::load(String path)
{
	m_data_path = path;

	godot::Ref<godot::File> file = godot::File::_new();
	if (file->open(path, godot::File::READ) != godot::Error::OK) {
		Godot::print_error(String("Failed open file: ") + path, __FUNCTION__, "", __LINE__);
		return;
	}

	int64_t size = file->get_len();
	m_data_bytes = file->get_buffer(size);
	file->close();
}

void EffekseerEffect::resolve_dependencies()
{
	auto setting = Effekseer::Setting::Create();
	auto native = Effekseer::Effect::Create(setting, m_data_bytes.read().ptr(), (int32_t)m_data_bytes.size());
	if (native == nullptr)
	{
		Godot::print_error(String("Failed load effect: ") + m_data_path, __FUNCTION__, "", __LINE__);
		return;
	}

	auto nativeptr = native.Get();
	
	char16_t materialPath[1024];
	get_material_path(materialPath, sizeof(materialPath) / sizeof(materialPath[0]));
	godot::String materialDir = GDEffekseer::ToGdString(materialPath) + "/";

	auto loader = godot::ResourceLoader::get_singleton();
	
	auto enumerateResouces = [&](const char16_t* (Effekseer::Effect::*getter)(int) const, int count)
	{
		for (int i = 0; i < count; i++)
		{
			godot::String path = GDEffekseer::ToGdString((nativeptr->*getter)(i));
			m_subresources[path] = loader->load(materialDir + path);
		}
	};

	enumerateResouces(&Effekseer::Effect::GetColorImagePath, native->GetColorImageCount());
	enumerateResouces(&Effekseer::Effect::GetNormalImagePath, native->GetNormalImageCount());
	enumerateResouces(&Effekseer::Effect::GetDistortionImagePath, native->GetDistortionImageCount());
	enumerateResouces(&Effekseer::Effect::GetModelPath, native->GetModelCount());
	enumerateResouces(&Effekseer::Effect::GetCurvePath, native->GetCurveCount());
	enumerateResouces(&Effekseer::Effect::GetMaterialPath, native->GetMaterialCount());
	enumerateResouces(&Effekseer::Effect::GetWavePath, native->GetWaveCount());
}

void EffekseerEffect::setup()
{
	if (m_native != nullptr) return;

	auto system = EffekseerSystem::get_instance();
	if (system == nullptr) return;
	auto manager = system->get_manager();
	if (manager == nullptr) return;

	char16_t materialPath[1024];
	get_material_path(materialPath, sizeof(materialPath) / sizeof(materialPath[0]));

	m_native = Effekseer::Effect::Create(manager, 
		m_data_bytes.read().ptr(), (int32_t)m_data_bytes.size(), m_scale, materialPath);
	if (m_native == nullptr)
	{
		Godot::print_error(String("Failed load effect: ") + m_data_path, __FUNCTION__, "", __LINE__);
		return;
	}
}

void EffekseerEffect::get_material_path(char16_t* path, size_t path_size)
{
	int len = (int)GDEffekseer::ToEfkString(path, m_data_path, path_size);
	
	for (int i = len - 1; i >= 0; i--) {
		if (path[i] == u'/') {
			path[i] = u'\0';
			break;
		}
	}
}

void EffekseerEffect::release()
{
	m_native.Reset();
}

}
