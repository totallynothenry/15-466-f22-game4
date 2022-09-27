#pragma once

#include <glm/glm.hpp>
#include <hb.h>
#include <hb-ft.h>
#include <string>
#include <unordered_map>

struct Text {
	// Character (from OpenGL-FreeType tutorial)
	struct Glyph {
		unsigned int textureId;  // ID handle of the glyph texture
		glm::ivec2   size;       // Size of glyph
		glm::ivec2   bearing;    // Offset from baseline to left/top of glyph
		unsigned int advance;    // Offset to advance to next glyph
	};

	Text(std::string font_file_path, unsigned int size);
	~Text();

	Glyph &get_glyph(hb_codepoint_t glyph_idx);
	void display(std::string const &text, glm::uvec2 const &drawable_size, float x, float y,
		float const &scale, glm::vec3 const &color);

	// FaceType resources
	FT_Library ft_library;
	FT_Face ft_face;

	// HalfBuzz resources
	hb_font_t *hb_font;
	hb_buffer_t *hb_buffer;

	// Glyph cache
	std::unordered_map< hb_codepoint_t, Glyph > glyphs;
};