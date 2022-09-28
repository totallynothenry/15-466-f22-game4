#pragma once

#include <array>
#include <string>
#include <vector>


#define MAX_CHOICES 4


// Sepcialized state graph for choice-based game
struct Graph { // Direct graph
	struct Node { // Represents a single state
		std::string description; // Description of the state, dislayed on screen
		std::array< std::string, MAX_CHOICES > choice_texts; // Description of each choice (a.k.a outgoing edge)
		bool win; // Indicates if this state is a win state (checked when choices are all null)

		// Outgoing edges
		std::array< int, MAX_CHOICES > choices;

		// Some convenience wrappers
		bool is_terminal();
		bool is_win();

		// Debug printing function
		void print();
	};

	// Load from csv file
	Graph(std::string csv_filepath);

	// Gets a reference to the current node
	Node &get_current_node();

	// Go to next state after making choice at current state (also updates current state)
	void make_choice(unsigned int choice_idx);

	// Debug printing function
	void print();

	// All nodes stored in a vector for convenience
	std::vector< Node > graph;

	// Some trackers
	int start_idx = 0; // Index to start node, tracked for quick restart
	int current_idx = -1; // Index to current node, tracks where the player current is
};