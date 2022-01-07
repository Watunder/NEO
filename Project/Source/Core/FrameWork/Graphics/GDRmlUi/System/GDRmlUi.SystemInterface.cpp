#include "GDRmlUi.SystemInterface.h"

#include <OS.hpp>

namespace GDRmlUi
{
	double SystemInterface::GetElapsedTime()
	{
		auto os = godot::OS::get_singleton();

		return double(os->get_unix_time());
	}

	bool SystemInterface::LogMessage(Rml::Log::Type type, const Rml::String& message)
	{
		Rml::String Type;

		switch (type)
		{
		case Rml::Log::LT_ALWAYS:
			Type = "[Always]";
			break;
		case Rml::Log::LT_ERROR:
			Type = "[Error]";
			break;
		case Rml::Log::LT_ASSERT:
			Type = "[Assert]";
			break;
		case Rml::Log::LT_WARNING:
			Type = "[Warning]";
			break;
		case Rml::Log::LT_INFO:
			Type = "[Info]";
			break;
		case Rml::Log::LT_DEBUG:
			Type = "[Debug]";
			break;
		case Rml::Log::LT_MAX:
			break;
		};

		godot::Godot::print("%s - %s\n", Type.c_str(), message.c_str());

		return true;
	}
}