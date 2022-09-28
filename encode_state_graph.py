'''
This script exists so that we can easily define the choice game state
graph with python dicts and then easily serialize it into a csv-like format
that is easy to load in cpp.

Output format:
- One state per line:
	state description|choice 1 text|choice 2 text|choice 3 text|choice 1 linenum|choice 2 linenum|choice 3 linenum
	linenum is 0-indexed, -1 represents no choice
	i.e. 'name|Hello world!|A|B|C|1|2|3'
		 'name|Another example|A|B||4|0|-1'
- Start state is 0th state (on the 0th line)
- Win state is 1st state
'''


filepath = 'dist/stategraph.txt'
MAX_CHOICES = 3


graph = [
	{ # 0 - Start state
		'name': 'ESCAPE FROM LAGRANGE STATION',
		'text': 'A text-adventure game built for 15-466 F22. Press [1/2/3] to select choices and [r] to restart at any time.',
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
		'text': '''You wake up in complete darkness, not knowing which way is up. It is silent except for your breathing and heartbeat. Amidst a rusty stench, you feel something wet flowing over your left eye. You reflexively reach to wipe away the liquid. Your glove clunks against the glass of your visor, and you realize you are in a station engineer's EVA suit. You suspect that you've hit your head against your helmet and are now bleeding.''',
		'choices': [
			{
				'text': '''Remove your helmet.''',
				'linenum': 5
			},
			{
				'text': '''Turn on your EVA suit's light.''',
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
		'text': '''As you take a moment to think, you remember where you are: Earth's Lagrange 2 research and observation station. You are an engineer, underqualified and underpaid. Your company was commissioned to run scheduled maintenance on the station's four external fusion reactors. After you had put on your EVA suit and stepped into the airlock with your tools for robotic arm attachment, you suddenly blacked out. Now that you have woken up, the station is dark. The L2 station is positioned so that Earth is directly between the station and Sol to avoid solar radiation polluting the numerous research sensors on the station. With very little natural light and what light that is available being similar to that of an eclipse, the station was designed with very few viewports. If station completely loses power, there is almost no light within. The wetness stinging your eye continues to irritate you.''',
		'choices': [
			{
				'text': '''Remove your helmet.''',
				'linenum': 5
			},
			{
				'text': '''Turn on your EVA suit's light.''',
				'linenum': 4
			},
			{
				'text': '''Try to find a wall.''',
				'linenum': 8
			}
		]
	},
	{ # 4
		'name': 'No Luck',
		'text': '''The inescapable darkness is irrefutable proof that the always-on EVA suit LEDs are not functioning. Perhaps they were damaged somehow when you were knocked out. After a moment of consideration, you remember that there is a backup option. You feel around on your suit for your flashlight. The attachment point on your EVA suit for its cord is empty. It must have still been in your toolbox, waiting to be attached when you blacked out.''',
		'choices': [
			{
				'text': '''Try to find a wall.''',
				'linenum': 8
			}
		]
	},
	{ # 5
		'name': 'Drowsiness',
		'text': '''You remove your helmet and wipe away what feels like half-dried blood from your face with your gloved hand. You are feeling lightheaded. You still cannot see in the darkness, but you can now faintly smell burnt plastic. You hear a muffled warning alarm beeping from somewhere far within the station but do not remember what it signified. You feel drowsy.''',
		'choices': [
			{
				'text': '''Put your helmet back on.''',
				'linenum': 7
			},
			{
				'text': '''Turn on your EVA suit's light.''',
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
		'text': '''The world feels like a dream. You are desperately tired. It feels like each of your blinks lasts an eternity. You struggle to stay awake. Your eyes shut...and they never open again. With the low oxygen alarm rhythmically sounding in the far distance, you asphyxiate in your sleep.''',
		'choices': [
		]
	},
	{ # 7
		'name': 'Disorientation (cont.)',
		'text': '''You put your helmet back on. As you take a moment to think, you remember where you are: Earth's Lagrange 2 research and observation station. You are an engineer, underqualified and underpaid. Your company was commissioned to run scheduled maintenance on the station's four external fusion reactors. After you had put on your EVA suit and stepped into the airlock with your tools for robotic arm attachment, you suddenly blacked out. Now that you have woken up, the station is dark. The L2 station is positioned so that Earth is directly between the station and Sol to avoid solar radiation polluting the numerous research sensors on the station. With very little natural light and what light that is available being similar to that of an eclipse, the station was designed with very few viewports. If station completely loses power, there is almost no light within.''',
		'choices': [
			{
				'text': '''Turn on your EVA suit's light.''',
				'linenum': 4
			},
			{
				'text': '''Try to find a wall.''',
				'linenum': 8
			}
		]
	},
	{ # 8
		'name': 'Suspended',
		'text': '''As you try to reach the wall, all your attempts end with grasping air. It seems you have floated to the middle of the airlock room while you were passed out. The Lagrange 2 research station has centrifugal habitation rings that rotate to produce simulated gravity, albeit with high Coriolis consequences. However, the airlock you are in is part of the central spine that the ring is centered around. You are stuck in microgravity. The company made you sit through an instructional video that covered what to do in this situation. You desperately try to remember its contents.''',
		'choices': [
			{
				'text': '''Take off your helmet and throw it as "propellant."''',
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
		'name': 'Warning',
		'text': '''You hear a loud bang as your helmet collides with the station wall. A moment later, you softly bump into the opposite wall. The act of throwing the helmet propelled yourself due to Newton's third law of motion. Your plan has succeeded. As you rejoice in no longer being stuck, you hear a muffled warning alarm beeping from somewhere far within the station. You do not remember what it signified. You feel drowsy.''',
		'choices': [
			{
				'text': '''Crawl along the wall and look for something useful.''',
				'linenum': 6
			}
		]
	},
	{ # 10
		'name': 'Futility',
		'text': '''You try all sorts of swimming forms but do not feel the wall within reach. The EVA suit feels extra heavy amidst the laborious motions of swimming. You cannot tell if you are making any progress. You reason that being suspended in the air must be similar to being suspended in water, but is air really like water?''',
		'choices': [
			{
				'text': '''Continue swimming.''',
				'linenum': 10
			},
			{
				'text': '''Flail around.''',
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
		'text': '''As you flap your arms, the EVA suit feels extra heavy. You feel ridiculous imitating a bird in a 50 kg EVA suit. You reason that, if this motion lets dinosaurs fly, it should at least let you move to the wall. You persist for several minutes and finally feel your army collide with a wall...only to almost push yourself away from it again. As you scramble to grab the wall in panic, you grasp onto a cord with one hand and a handle attached to the wall with the other. Breathing a sigh of relief, you examine what the cord leads to and realize it is your flashlight.''',
		'choices': [
			{
				'text': '''Turn it on and look around.''',
				'linenum': 12
			}
		]
	},
	{ # 12
		'name': 'Light',
		'text': '''A blinding volume of light fills the room as you turn on the industrial flashlight used for lighting up entire external portions of the station. When your eyes finally adjust, you take note of the state of the airlock. Nothing in the room is powered. Aside from a few minor dents in the wall, the airlock room is intact. Your toolbox is floating on the opposite side of the room. The tools that were once inside are floating freely. It looks like the hatch connecting to the airlock was remotely shut sometime before the station lost power.''',
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
				'text': '''Open the hatch and head deeper into the station.''',
				'linenum': 14
			},
		]
	},
	{ # 14
		'name': 'Lifeless Station',
		'text': '''You are in the rear portion of the station's spine, near where the external fusion reactors are attached. This portion contains electric systems, spare parts, and miscellaneous items. There is a lab in this area. Shining your light through the entrance, you notice blood plastered against the wall. You quickly turn away after noticing a body-like shape with something protruding from the neck. Deeper into the station, you come across a heavy-duty laptop strapped to the wall. Maybe it still has battery power.''',
		'choices': [
			{
				'text': '''Try the laptop.''',
				'linenum': 15
			},
		]
	},
	{ # 15
		'name': 'A Desperate Situation',
		'text': '''As you open the laptop, you notice a message sent from another crew member: "Station has lost power. Liquid O2 storage in subsection 4 on the left wing of the station exploded. Front of station spine is GONE. Rings are depressurized. DO NOT REMOVE YOUR HELMET. Internal atmosphere almost pure nitrogen. Won't realize suffocating. Running out of O2, gonna leave in escape capsule 0. There are seven more."''',
		'choices': [
			{
				'text': '''Head to the nearest escape capsule.''',
				'linenum': 16
			}
		]
	},
	{ # 16
		'name': 'Hope',
		'text': '''Thankfully, there are two escape capsules docked near the rear spine. Escape capsule operation was the one part of the briefing they made sure you remembered. You make your way to the docking point. Both capsules 4 on the left and 5 on the right appear intact from the inside. You can escape! The capsules attached are barebones, and rely on their thrusters' alternator to generate the power needed to run its computer systems. Normally, the station can jumpstart the capsule. With the station dead, the only way is to undock with the manual release to trigger the automatic ignition mechanism of the thrusters.''',
		'choices': [
			{
				'text': '''Launch in capsule 4.''',
				'linenum': 19
			},
			{
				'text': '''Launch in capsule 5.''',
				'linenum': 17
			},
			{
				'text': '''Re-read the crewmate's message''',
				'linenum': 15
			}
		]
	},
	{ # 17
		'name': 'Point of No Return',
		'text': '''With the rumble the thrusters, the capsule's systems come to life. You have no idea how to pilot it, but thankfully it is automatically following a pre-programmed course back to Earth. As you clear the station, you get a better view of the starscape.''',
		'choices': [
			{
				'text': '''Look out the viewport.''',
				'linenum': 18
			}
		]
	},
	{ # 18
		'name': 'Infero',
		'text': '''A scarred red sphere sits where Earth should be. The intricate pattern of city lights has been replaced with seemingly random blots of crimson. With the ring of red light from the Sun leaking around the planet, the planet stares into you like the cosmic eye of an angry deity. The Earth is burning.''',
		'choices': [
			{
				'text': '''You are on your own.''',
				'linenum': 1
			}
		]
	},
	{ # 19
		'name': 'Game Over',
		'text': '''As capsule 4 undocks from the station with you inside, there is only silence. A full eternity passes. The thrusters are not igniting like they should have. You search through the capsule thoroughly for any issues and wrack your brain trying to think of something to do. You search again. And again. Pressing yourself against the roof, you can barely see the capsule's rear through the viewport. You notice two large punctures probably caused by shrapnel from the station's liquid O2 tanks that exploded. As the capsule slowly runs out of oxygen, your industrial flashlight finally runs out of power. In the cold, dark capsule, you slowly sink into despair as you feel yourself beginning to suffocate.''',
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



