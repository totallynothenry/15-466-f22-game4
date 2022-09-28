#include "PlayMode.hpp"

#include "Graph.hpp"
#include "Load.hpp"
#include "Text.hpp"
#include "gl_errors.hpp"
#include "data_path.hpp"

#define FONT_SIZE 72

// Colors for display
static glm::vec3 const NAME_COLOR = glm::vec3(1.0f, 0.25f, 0.0f);
static glm::vec3 const DESC_COLOR = glm::vec3(1.0f, 1.0f, 1.0f);
static glm::vec3 const CHOICE_COLOR = glm::vec3(0.0f, 1.0f, 0.4f);

static Text *monofet_regular_text = nullptr;
static Text *share_tech_mono_regular_text = nullptr;

static Graph *state_graph = nullptr;

static Load< void > load_fonts(LoadTagDefault, [](){
	monofet_regular_text = new Text(data_path("font/Monofett-Regular.ttf"), FONT_SIZE);
	share_tech_mono_regular_text = new Text(data_path("font/ShareTechMono-Regular.ttf"), FONT_SIZE);
});

static Load< void > load_state_graph(LoadTagDefault, [](){
	state_graph = new Graph(data_path("stategraph.txt"));
});

PlayMode::PlayMode() {
}

PlayMode::~PlayMode() {
	if (monofet_regular_text != nullptr) {
		delete monofet_regular_text;
	}
	if (share_tech_mono_regular_text != nullptr) {
		delete share_tech_mono_regular_text;
	}
	if (state_graph != nullptr) {
		delete state_graph;
	}
}

bool PlayMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) {

	if (evt.type == SDL_KEYDOWN) {
		if (evt.key.keysym.sym == SDLK_r) {
			restart.downs += 1;
			restart.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_1) {
			one.downs += 1;
			one.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_2) {
			two.downs += 1;
			two.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_3) {
			three.downs += 1;
			three.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_4) {
			four.downs += 1;
			four.pressed = true;
			return true;
		}
	} else if (evt.type == SDL_KEYUP) {
		if (evt.key.keysym.sym == SDLK_r) {
			restart.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_1) {
			one.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_2) {
			two.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_3) {
			three.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_4) {
			four.pressed = false;
			return true;
		}
	}

	return false;
}

void PlayMode::update(float elapsed) {
	if (cooldown > 0) {
		cooldown = std::max(0.0f, cooldown - elapsed);
	} else {
		if (restart.pressed) {
			cooldown = ACTION_COOLDOWN;
			state_graph->current_idx = state_graph->start_idx;
		} else if (one.pressed) {
			state_graph->make_choice(1);
			cooldown = ACTION_COOLDOWN;
		} else if (two.pressed) {
			cooldown = ACTION_COOLDOWN;
			state_graph->make_choice(2);
		} else if (three.pressed) {
			cooldown = ACTION_COOLDOWN;
			state_graph->make_choice(3);
		} else if (four.pressed) {
			cooldown = ACTION_COOLDOWN;
			state_graph->make_choice(4);
		}
	}

	//reset button press counters:
	restart.downs = 0;
	one.downs = 0;
	two.downs = 0;
	three.downs = 0;
	four.downs = 0;
}

void PlayMode::draw(glm::uvec2 const &window_size) {
	// Set background (from PPU466)
	glClearColor(
		background_color.r / 255.0f,
		background_color.g / 255.0f,
		background_color.b / 255.0f,
		1.0f
	);
	glClear(GL_COLOR_BUFFER_BIT);

	Graph::Node &node = state_graph->get_current_node();

	// Display state
	float ypos = static_cast< float >(window_size.y) - 100.0f;
	monofet_regular_text->display(node.name, window_size, 50.0f, ypos, 1.0f, NAME_COLOR);
	ypos -= 75.0f;
	share_tech_mono_regular_text->display_wrapped(node.description, window_size, 75.0f, ypos, 0.3f, DESC_COLOR);

	for (int i = 0; i < MAX_CHOICES; i++) {
		if (node.choice_texts[i].empty()) {
			continue;
		}
		share_tech_mono_regular_text->display(
			std::to_string(i+1) + ": " + node.choice_texts[i],
			window_size, 100.0f, 50.0f * (MAX_CHOICES - i), 0.375f, CHOICE_COLOR);
	}
	GL_ERRORS();
}
