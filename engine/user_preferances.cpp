#include <iostream>
#include "user_preferances.h"

using namespace sf;
using namespace std;

UserPreferences::UserPreferences() ://7 windowed; 8 fullscreen

	fullscreen(7), video_resolution(Vector2f(1080, 720)), changed_fullscreen(false), changed_resolution(false) {

} 
KeyboardPreferences::KeyboardPreferences() :
	UP(Keyboard::Key::Up), LEFT(Keyboard::Key::Left), RIGHT(Keyboard::Key::Right) {

}