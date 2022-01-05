#include "RmlUI.h"

void RmlUI::_register_methods()
{
	register_method("_init", &RmlUI::_init);
	register_method("_ready", &RmlUI::_ready);
	register_method("_process", &RmlUI::_process);
	register_method("_enter_tree", &RmlUI::_enter_tree);
	register_method("_exit_tree", &RmlUI::_exit_tree);
	register_method("_update_draw", &RmlUI::_update_draw);
}