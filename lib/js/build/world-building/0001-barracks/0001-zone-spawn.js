let zone_data = {
	'id': '0001-barracks',
	'manifest': [],/** TODO: manifest will have the IDs of all the rooms to import into the game */

	'npc': [
		{'id': 0,
			'name': 'Dr. '
		},
	],
	'room_list': ['decon', 'steril','morgue','morgue-hallway'],
		{'id': 0,
			'ufid': 'decon',
			'name': 'Decontainment Room',
			'desc': {'file': 'decon'},
			's': 1,
			'e': 3,
			'w': 2
		},
		{'id': 1,
			'ufid': 'steril',
			'name': 'Sterilization Room',
			'obj': [
				{'id': 'deep-sink',
					'desc': 'A deep sink for instrument cleaning.',
					'act': ['clean','douse','stop-fire'],
					'match': [/deep/i,/sink\.1/i,/deep sink/i]
				},
				{'id': 'h-sink',
					'desc': 'A handwashing sink equipped with anti-splash devices.',
					'act': ['clean','douse','stop-fire'],
					'match': [/handwashing/i,/sink\.2/i,/handwashing sink/i,/hsink/i]
				}
			]
			'desc': {'file': 'steril'},
			's': 1,
			'e': 3,
			'w': 2
		},
	
};


