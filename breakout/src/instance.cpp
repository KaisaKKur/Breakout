#include "breakout/instance.h"

namespace breakout {

Breakout* getInstance() {
	static Breakout* breakout = new Breakout(SCREEN_WIDTH, SCREEN_HEIGHT);
	return breakout;
}

}
