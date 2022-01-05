#include <Godot.hpp>
#include <Node2D.hpp>

namespace RmlGodot {

class RmlUI : public Node2D
{
	GODOT_CLASS(RmlUI, Node2D)

public:
	static void _register_methods();

	RmlUI();

	~RmlUI();

	void _init();

	void _ready();

	void _enter_tree();

	void _exit_tree();

	void _process(float delta);

	void _update_draw();

private:

};

}
