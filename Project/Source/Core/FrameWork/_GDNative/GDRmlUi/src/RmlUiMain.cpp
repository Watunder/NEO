#include "RmlUiMain.h"

#include <RmlUi/Core.h>
#include <RmlUi/Debugger.h>
#include <Input.h>
#include <Shell.h>

#include "FontEngineInterfaceBitmap.h"

#include "GDRmlUi.RenderInterface.h"

namespace godot
{
	RmlUiMain* RmlUiMain::s_instance = nullptr;

	RmlUiMain::RmlUiMain()
	{
		s_instance = this;

	}

	RmlUiMain::~RmlUiMain()
	{

	}

	void RmlUiMain::_init()
	{

	}


	void RmlUiMain::_register_methods()
	{

	}
}
