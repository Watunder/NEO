#pragma once

#include <RmlUi/Core/SystemInterface.h>

namespace GDRmlUi
{
	class SystemInterface : public Rml::SystemInterface
	{
	public:
		double time;
		//double GetElapsedTime() override;
		//bool LogMessage(Rml::Log::Type type, const Rml::String& message) override;
	};
}
