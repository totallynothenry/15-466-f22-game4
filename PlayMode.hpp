#include "Mode.hpp"

#include "Scene.hpp"
#include "Sound.hpp"

#include <glm/glm.hpp>

#define ACTION_COOLDOWN 0.5f // seconds

struct PlayMode : Mode {
	PlayMode();
	virtual ~PlayMode();

	//functions called by main loop:
	virtual bool handle_event(SDL_Event const &, glm::uvec2 const &wsize) override;
	virtual void update(float elapsed) override;
	virtual void draw(glm::uvec2 const &drawable_size) override;

	//----- game state -----

	//input tracking:
	struct Button {
		uint8_t downs = 0;
		uint8_t pressed = 0;
	} one, two, three, four, restart;

	glm::u8vec3 background_color = glm::u8vec3(2.0f, 0.0f, 0.0f);

	// Hack to prevent single button press advancing several scenes
	float cooldown = ACTION_COOLDOWN;
};