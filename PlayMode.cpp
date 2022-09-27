#include "PlayMode.hpp"

#include "Load.hpp"
#include "Text.hpp"
#include "gl_errors.hpp"
#include "data_path.hpp"

#define FONT_SIZE 72

static Load< Text * > red_hat_mono_text(LoadTagDefault, []() -> Text ** {
	Text **res = new Text *;
	*res = new Text(data_path("font/RedHatMono-Medium.ttf"), FONT_SIZE);
	return res;
});

PlayMode::PlayMode() {
}

PlayMode::~PlayMode() {
}

bool PlayMode::handle_event(SDL_Event const &evt, glm::uvec2 const &wsize) {

	if (evt.type == SDL_KEYDOWN) {
		if (evt.key.keysym.sym == SDLK_ESCAPE) {
			SDL_SetRelativeMouseMode(SDL_FALSE);
			return true;
		} else if (evt.key.keysym.sym == SDLK_a) {
			left.downs += 1;
			left.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_d) {
			right.downs += 1;
			right.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_w) {
			up.downs += 1;
			up.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_s) {
			down.downs += 1;
			down.pressed = true;
			return true;
		}
	} else if (evt.type == SDL_KEYUP) {
		if (evt.key.keysym.sym == SDLK_a) {
			left.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_d) {
			right.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_w) {
			up.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_s) {
			down.pressed = false;
			return true;
		}
	}

	window_size = wsize;

	return false;
}

void PlayMode::update(float elapsed) {
	{
		glm::vec2 move = glm::vec2(0.0f);
		if (left.pressed && !right.pressed) move.x =-1.0f;
		if (!left.pressed && right.pressed) move.x = 1.0f;
		if (down.pressed && !up.pressed) move.y =-1.0f;
		if (!down.pressed && up.pressed) move.y = 1.0f;
		(void)move;
	}

	//reset button press counters:
	left.downs = 0;
	right.downs = 0;
	up.downs = 0;
	down.downs = 0;
}

void PlayMode::draw(glm::uvec2 const &drawable_size) {
	// Set background (from PPU466)
	glClearColor(
		background_color.r / 255.0f,
		background_color.g / 255.0f,
		background_color.b / 255.0f,
		1.0f
	);
	glClear(GL_COLOR_BUFFER_BIT);

	// Display text
	(*red_hat_mono_text)->display("Hello World!", window_size, 25.0f, 25.0f, 0.5f, glm::vec3(0.5, 0.8f, 0.2f));
	GL_ERRORS();
}
