
#include "Graph.hpp"

#include <assert.h>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <sstream>


// #define DEBUG

// Debug logging
#ifdef DEBUG
#define LOG(ARGS) std::cout << ARGS << std::endl;
#else
#define LOG(ARGS)
#endif


inline bool Graph::Node::is_terminal() {
	return choices[0] < 0 && choices[1] < 0 && choices[2] < 0 && choices[3] < 0;
}

inline bool Graph::Node::is_win() {
	return is_terminal() && win;
}

void Graph::Node::print() {
	std::cout << "----------------------------" << std::endl;
	std::cout << "Name: " << name << std::endl;
	std::cout << "Description: " << description << std::endl;
	std::cout << "Win: " << win << std::endl;
	for (int i = 0; i < MAX_CHOICES; i++) {
		std::cout << "Choice " << i << ": " << choice_texts[i] << "(" << choices[i] << ")" << std::endl;
	}
	std::cout << "----------------------------\n" << std::endl;
}


Graph::Graph(std::string filepath) {
	/**
	 * Partially adapted from my game1 code that loads .csv files:
	 * https://github.com/totallynothenry/15-466-f22-game1
	 */

	// Load from file
	std::ifstream file(filepath);

	if (!file.is_open()) {
		throw new std::runtime_error("Failed to load file for state graph");
	}

	std::string line, token;
	for (int linenum = 0; file.good(); linenum++) {
		LOG("line: " << linenum);
		std::getline(file, line);
		std::stringstream linestream(line);

		// Parsing a single state
		if (!std::getline(linestream, token, '|')) {
			break;
		}
		LOG("name" << token);
		graph.emplace_back();
		Node &state = graph.back();
		state.win = false;
		state.name = token;

		if (!std::getline(linestream, token, '|')) {
			break;
		}

		LOG("description" << token);
		state.description = token;

		for (int i = 0; i < MAX_CHOICES && std::getline(linestream, token, '|'); i++) {
			LOG("choice text[" << i+1 << "]: " << token);
			state.choice_texts[i] = token;
		}

		for (int i = 0; i < MAX_CHOICES && std::getline(linestream, token, '|'); i++) {
			LOG("choice[" << i+1 << "]: " << token);
			state.choices[i] = stoi(token);
		}

		if (linenum == 1) {
			LOG("win state!");
			state.win = true;
		}

		LOG("\n");
	}

	// print();
	// std::cout << "start node: " << std::endl;
	// graph[start_idx].print();

	// Current node is start node
	start_idx = 0;
	current_idx = start_idx;

	// Some sanity checks
	assert(0 <= current_idx && current_idx < (int)graph.size());
	assert(0 <= start_idx && start_idx < (int)graph.size());
}

void Graph::print() {
	int count = 0;
	for (auto &node : graph) {
		std::cout << "node " << count << std::endl;
		node.print();
		count++;
	}
}

Graph::Node &Graph::get_current_node() {
	assert(0 <= current_idx && current_idx < (int)graph.size());
	return graph[current_idx];
}

void Graph::make_choice(unsigned int choice_idx) {
	choice_idx--;

	assert(0 <= choice_idx && choice_idx < MAX_CHOICES);
	int graph_idx = get_current_node().choices[choice_idx];

	if (graph_idx < 0) {
		return;
	}
	assert(0 <= graph_idx && graph_idx < (int)graph.size());

	current_idx = graph_idx;

	// get_current_node().print();
}