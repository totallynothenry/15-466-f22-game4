'''
This script exists so that we can easily define the choice game state
graph with python dicts and then easily serialize it into a csv-like format
that is easy to load in cpp.

Output format:
- One state per line:
	state description|choice 1 text|choice 2 text|choice 3 text|choice 4 text|choice 1 linenum|choice 2 linenum
	linenum is 0-indexed, -1 represents no choice
	i.e. 'Hello world!|A|B|C|D|1|2|3|4'
		 'Another example|A|B|||4|0|-1|-1'
- Start state is 0th state (on the 0th line)
- Win state is 1st state
'''


filepath = 'dist/stategraph.txt'
MAX_CHOICES = 4


graph = [
	{ # Start state
		'text': 'Hello world!',
		'choices': [
			{
				'text': 'Go to win',
				'linenum': 1
			},
			{
				'text': 'Stay here!',
				'linenum': 0
			}
		]
	},
	{ # Win state
		'text': 'You win!',
		'choices': []
	}
]

# Serialization code
with open(filepath, 'w+') as file:
	for state in graph:
		print('Serializing state:', state)
		choice_texts = []
		choice_linenums = []
		cnt = MAX_CHOICES
		# decompose choices
		for choice in state['choices']:
			choice_texts.append(choice['text'])
			choice_linenums.append(str(choice['linenum']))
			cnt -= 1
		# pad with dummy choices
		while cnt > 0:
			choice_texts.append('')
			choice_linenums.append(str(-1))
			cnt -= 1
		line_comps = [state['text']] + choice_texts + choice_linenums
		assert len(line_comps) == 1 + 2 * MAX_CHOICES
		line = '|'.join([state['text']] + choice_texts + choice_linenums)
		print('\tresult:', line)
		file.write(line + '\n')



