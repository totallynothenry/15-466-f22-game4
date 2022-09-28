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
	{ # 0 - Start state
		'name': 'ESCAPE FROM LAGRANGE STATION',
		'text': 'A text adventure game built for 15-466 F22. Press [1/2/3/4] to select choices and [r] to restart at any time.',
		'choices': [
			{
				'text': 'Start',
				'linenum': 2
			}
		]
	},
	{ # 1 - Win state
		'name': 'END',
		'text': 'Thanks for playing!',
		'choices': []
	},
	{ # 2
		'name': 'Disorientation',
		'text': '''You wake up in complete darkness not knowing which way is up. It is silent except for your own breathing and heartbeat. Admist a rusty stench, you feel something wet flowing over your left eye. You reflexively reach to wipe away the liquid and the clunk of your glove against the glass of your visor reminds you that you are in a station engineer's EVA suit. You suspect that you've hit your head against your helmet and are now bleeding.''',
		'choices': [
			{
				'text': '''Remove your helmet''',
				'linenum': 5
			},
			{
				'text': '''Turn on your EVA suit's flashlight''',
				'linenum': 4
			},
			{
				'text': '''...''',
				'linenum': 3
			}
		]
	},
	{ # 3
		'name': 'Disorientation (cont.)',
		'text': '''As you take a moment to think, you remember where you are: Earth's Lagrange 2 observation reserach station. You are an engineer, underqualified and underpaid. Your company was comissioned to run scheduled maintainence on the station's four external fusion reactors. After you had put on your EVA suit and stepped into the airlock with your tools for robotic arm attachment, you suddenly blacked out. Now that you have woken up, the station is dark. L2 station is positioned so that Earth is directly in between the station and Sol to avoid solar radiation polluting the numerous research sensors on the station. With very little natural light available, the station has no viewports. If power goes out, there is almost no light within the station. The wetness stinging your eye continues to irritate you.''',
		'choices': [
			{
				'text': '''Remove your helmet''',
				'linenum': 5
			},
			{
				'text': '''Turn on your EVA suit's flashlight''',
				'linenum': 4
			},
			{
				'text': '''Try to find a wall''',
				'linenum': 8
			}
		]
	},
	{ # 4
		'name': 'No Luck',
		'text': '''The always-on EVA suit LEDs clearly are not functioning. Perhaps they were damaged somehow, but you remember that you also have a backup option. You feel around on your suit for your flashlight and notice that it's not attached to your suit. It must have still been in your toolbox waiting to be attached when you blacked out.''',
		'choices': [
			{
				'text': '''Try to find a wall''',
				'linenum': 8
			}
		]
	},
	{ # 5
		'name': 'Drowsiness',
		'text': '''You remove your helmet and wipe away what feels like half-dried blood from your face with your gloved hand. You are feeling lightheaded. You still cannot see in darkness, but you can now faintly smell burnt plastic. You hear a muffled warning alarm beeping from somewhere far within the station but do not remember what it signified. You feel drowsy.''',
		'choices': [
			{
				'text': '''Put your helmet back on''',
				'linenum': 7
			},
			{
				'text': '''Turn on your EVA suit's flashlight''',
				'linenum': 6
			},
			{
				'text': '''...''',
				'linenum': 6
			}
		]
	},
	{ # 6
		'name': 'Game Over',
		'text': '''The world feels like a dream. You are desparately tired and it feels like each of your blinks lasts an eternity. You struggle to stay awake. Your eyes shut...and they never open. With the low oxygen alarm rythmically sounding in the far distance, you asphysiate in your sleep.''',
		'choices': [
		]
	},
	{ # 7
		'name': 'Disorientation (cont.)',
		'text': '''You put your helmet back on. As you take a moment to think, you remember where you are: Earth's Lagrange 2 observation reserach station. You are an engineer, underqualified and underpaid. Your company was comissioned to run scheduled maintainence on the station's four external fusion reactors. After you had put on your EVA suit and stepped into the airlock with your tools for robotic arm attachment, you suddenly blacked out. Now that you have woken up, the station is dark. L2 station is positioned so that Earth is directly in between the station and Sol to avoid solar radiation polluting the numerous research sensors on the station. With very little natural light available, the station has no viewports. If power goes out, there is almost no light within the station.''',
		'choices': [
			{
				'text': '''Turn on your EVA suit's flashlight''',
				'linenum': 4
			},
			{
				'text': '''Try to find a wall''',
				'linenum': 8
			}
		]
	},
	{ # 8
		'name': 'Suspended',
		'text': '''As you try to reach the wall, all your attempts end with grasping air. It seems you have floated to the middle of the airlock room when you were passed out. Lagrange station has centrifugal habitation rings that rotate to produce simulated gravity, albeit with high coriolis consequences, but the airlock you are in is in the central spine of the station that the ring rotates around. The company made you sit through an instructional video that covered what to do in this situation and you desparately try to remember its contents.''',
		'choices': [
			{
				'text': '''Take off your helmet and chuck it with all your strength.''',
				'linenum': 9
			},
			{
				'text': '''Try a swimming motion.''',
				'linenum': 10
			},
			{
				'text': '''Flap your arms back and forth like a bird.''',
				'linenum': 11
			}
		]
	},
	{ # 9
		'name': 'Shattered',
		'text': '''You hear a loud bang as your helmet collides with the station wall. A moment later, you softly bump into the opposite wall. Your plan succeeded. You hear a muffled warning alarm beeping from somewhere far within the station but do not remember what it signified. You feel drowsy.''',
		'choices': [
			{
				'text': '''Crawl along the way and look for something useful.''',
				'linenum': 6
			}
		]
	},
	{ # 10
		'name': 'Futility',
		'text': '''You try all sorts of swimming forms but do not feel the wall within reach. The EVA suit feels extra heavy admist the laborious motions of swimming designed for water.''',
		'choices': [
			{
				'text': '''Continue swimming''',
				'linenum': 10
			},
			{
				'text': '''Flail around''',
				'linenum': 8
			},
			{
				'text': '''Flap your arms back and forth like a bird.''',
				'linenum': 11
			}
		]
	},
	{ # 11
		'name': 'A Stroke of Luck',
		'text': '''As you flap your arms, the EVA suit feels extra heavy. You feel ridiculous immitating a bird in a 50 kg EVA suit, but reason that if it let the dinosaurs fly then it should at least let you move yourself to the walk. You persist for several minutes and finally feel your army collide with a wall...only to almost push yourself away from it again. As you scramble to grab the wall in panic, you grasp onto a cord with one hand and...a handle attached to the wall with the other. Breathing a sigh of relief, you examine what the cord leads to and realise it is your flashlight.''',
		'choices': [
			{
				'text': '''Turn it on and look around.''',
				'linenum': 12
			}
		]
	},
	{ # 12
		'name': 'Light',
		'text': '''A blinding volume of light fills the room as you turn on the industrial flashlight used for lighting up entire external portions of the station. When you eyes finally adjust, you take note of the state of the airlock. Nothing in the room is powered. Aside from a few minor dents in the wall, the airlock room is mostly in tact. Your toolbox, which is lightly magnetized so that it sticks to the floor, is floating on the opposite side of the room and the tools that were once inside are strewn about. It looks like the hatch connecting to the airlock remotely shut sometime before the station lost power.''',
		'choices': [
			{
				'text': '''Open the hatch and head deeper into the station.''',
				'linenum': 14
			},
			{
				'text': '''Collect your tools.''',
				'linenum': 13
			}
		]
	},
	{ # 13
		'name': 'Why? Why Not?',
		'text': '''You gather up all of your tools inside your toolbox. You have no idea how your tools for EVA access and inspection are going to be of use, but it doesn't hurt to have them.''',
		'choices': [
			{
				'text': '''Open the hatch and head deeper into the station''',
				'linenum': 14
			},
		]
	},
	{ # 14
		'name': 'Lifeless Station',
		'text': '''You are in the rear portion of the main station's spine, near where the external fusion reactors are attached. As you head deeper into the station you see a heavy duty laptop strapped to the wall. Maybe it still has battery power.''',
		'choices': [
			{
				'text': '''Try the laptop.''',
				'linenum': 15
			},
		]
	},
	{ # 15
		'name': 'A Desperate Situation',
		'text': '''As you open the laptop, you notice a message sent from another crew member: Station has lost power. Liquid O2 storage in subsection 4 on the left wing of the station exploded. Front of station spine is gone. Rings are depressurized. DO NOT REMOVE YOUR HELMET. Internal atmosphere is almost pure nitrogen. You'll die without even realizing you're not breathing in any oxygen. I'm leaving in emergency escape capsule 0. There are seven more. If you are reading this, I suggested you do the same.''',
		'choices': [
			{
				'text': '''Head to the escape capsules.''',
				'linenum': 16
			}
		]
	},
	{ # 16
		'name': 'Hope',
		'text': '''Thankfully, there are two escape capsules docked near the rear spine. As you make your way to the docking point, capsule 4 on the left and 5 on the right look like they are intact from the inside. You can escape! The capsules attached are ancient tech, and rely on their thrusters' alternator to generate the power needed to run its computer systems. Normally, the station can jumpstart the capsule. With the station dead, the only way to turn on the capsule is to undock with the manual release, which will also ignite the thrusters.''',
		'choices': [
			{
				'text': '''Launch in capsule 4.''',
				'linenum': 18
			},
			{
				'text': '''Launch in capsule 5.''',
				'linenum': 16
			},
			{
				'text': '''Re-read the crewmate's message''',
				'linenum': 15
			}
		]
	},
	{ # 16
		'name': 'Point of No Return',
		'text': '''With the rumble the thrusters, the capsule's systems come to life. You have no idea how to pilot it, but thankfully it is automatically following a pre-programmed course back to Earth. As you clear the station, you get a better view of the starscape.''',
		'choices': [
			{
				'text': '''...''',
				'linenum': 17
			}
		]
	},
	{ # 17
		'name': 'Infero',
		'text': '''A dark red sphere sits where Earth used to be. With the ring of red light from the Sun leaking around the planet, the planet stares into you like the cosmic eye of an angry deity. The Earth is burning.''',
		'choices': [
			{
				'text': '''...''',
				'linenum': 1
			}
		]
	},
	{ # 18
		'name': 'Game Over',
		'text': '''As capsule 4 undocks from the station with you inside, there is only silence. What feels like a full eternity passes. The thrusters are not igniting. You search through the capsule thorough. You search again. And again. No luck. Pressing yourself against the roof, you can barely see the capsule's rear through the viewport and notice two large punctures probably caused by shrapnel from the liquid O2 tanks that exploded on the station. As the capsule slowly runs out of oxygen, your industrial flashlight finally runs out of power. In the cold, dark capsule, you slowly sink into despair as you feel yourself beginning to suffocate.''',
		'choices': [
		]
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
		line_comps = [state['name'], state['text']] + choice_texts + choice_linenums
		assert len(line_comps) == 2 + 2 * MAX_CHOICES
		line = '|'.join(line_comps)
		print('\tresult:', line)
		file.write(line + '\n')



