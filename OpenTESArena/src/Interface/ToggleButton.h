#ifndef TOGGLE_BUTTON_H
#define TOGGLE_BUTTON_H

#include <functional>

// A button for toggling on and off. When the button is toggled, the 
// function for the new toggle state is called.

class GameState;
class Int2;

class ToggleButton
{
private:
	std::function<void(GameState*)> onFunction;
	std::function<void(GameState*)> offFunction;
	int x, y, width, height;
	bool on;
public:
	ToggleButton(int x, int y, int width, int height, bool on,
		const std::function<void(GameState*)> &onFunction,
		const std::function<void(GameState*)> &offFunction);
	ToggleButton(const Int2 &center, int width, int height, bool on,
		const std::function<void(GameState*)> &onFunction,
		const std::function<void(GameState*)> &offFunction);
	virtual ~ToggleButton();

	// Returns whether the button is toggled on.
	bool isOn() const;

	// Returns whether the button's area contains the given point.
	bool contains(const Int2 &point);

	// Switches the toggle state of the button.
	void toggle(GameState *gameState);
};

#endif
