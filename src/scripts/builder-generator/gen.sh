#!/bin/bash

TABLE=raid
ORM_HPP=$PWD/gen-orm-${TABLE}.hpp
ORM_CPP=$PWD/gen-orm-${TABLE}.cpp
TUPLE_FILE=$PWD/gen-tuple.hpp


function str_fields(){
	cat input | 
		grep '\[std::string\]' -A 50 |
		grep -E '^[[:blank:]]*$' -B 20 --max-count=1 |
		grep -v '\[std::string\]' |
		grep -E '[a-z0-9_]+' 
}

function mob_vnum_fields(){
	cat input | 
		grep '\[mob_vnum\]' -A 50 |
		grep -E '^[[:blank:]]*$' -B 20 --max-count=1 |
		grep -v '\[mob_vnum\]' |
		grep -E '[a-z0-9_]+'
}


function player_id_fields(){
	cat input | 
		grep '\[player_id\]' -A 50 |
		grep -E '^[[:blank:]]*$' -B 20 --max-count=1 |
		grep -v '\[player_id\]' |
		grep -E '[a-z0-9_]+'
}

function room_vnum_fields(){
	cat input | 
		grep '\[room_vnum\]' -A 50 |
		grep -E '^[[:blank:]]*$' -B 20 --max-count=1 |
		grep -v '\[room_vnum\]' |
		grep -E '[a-z0-9_]+'
}

function zone_vnum_fields(){
	cat input | 
		grep '\[zone_vnum\]' -A 50 |
		grep -E '^[[:blank:]]*$' -B 20 --max-count=1 |
		grep -v '\[zone_vnum\]' |
		grep -E '[a-z0-9_]+'
}

function all_fields(){
	cat input |
		grep -v -E '^\[.*\]$' |
		grep -E '[a-z0-9_]+'
}

#		 *
#		 * Member var type: int, std::string, etc
#		 * Member Var Name: id, s_sequence_vnum, etc
#		 * initialized value: the value to set it to when ::init() is called
#		 * pqxx conversion type: 1 = use .as<var_type>(), 2 = use vectorize, 3 = use .c_str()
#		 * native object field: the member variable name of mods::scripted_step
#		 * slot list: 1 = list this var in slot_list(), 0 = don't list in slot_list
#		 */

echo -n '' > $TUPLE_FILE

LC=$(all_fields | wc -l)
CTR=1
str_fields > f
for field in $(cat f | cut -d',' -f 1); do
	echo -n "(std::string,$field,\"\",3,null,1)" >> $TUPLE_FILE
	CTR=$(( CTR + 1 ))
	if [[ $CTR -lt $LC ]]; then
		echo -ne ', \\\n' >> $TUPLE_FILE
	fi
done

mob_vnum_fields > f
for field in $(cat f); do
	echo -n "(mob_vnum,$field,0,1,null,1)" >> $TUPLE_FILE
	CTR=$(( CTR + 1 ))
	if [[ $CTR -lt $LC ]]; then
		echo -ne ', \\\n' >> $TUPLE_FILE
	fi
done

room_vnum_fields > f
for field in $(cat f); do
	echo -n "(room_vnum,$field,0,1,null,1)" >> $TUPLE_FILE
	CTR=$(( CTR + 1 ))
	if [[ $CTR -lt $LC ]]; then
		echo -ne ', \\\n' >> $TUPLE_FILE
	fi
done

zone_vnum_fields > f
for field in $(cat f); do
	echo -n "(zone_vnum,$field,0,1,null,1)" >> $TUPLE_FILE
	CTR=$(( CTR + 1 ))
	if [[ $CTR -lt $LC ]]; then
		echo -ne ', \\\n' >> $TUPLE_FILE
	fi
done


cat ./orm-part-1.hpp | sed -e "s|___\[\[\[TABLE\]\]\]___|${TABLE}|g" > $ORM_HPP

##
## Generate the string limits tuple
#str_fields > f
#if [[ $(cat f | wc -l) -gt 0 ]]; then
#CTR=1
#STRING_FIELD_COUNT=$(str_fields | wc -l)
#for field in $(cat f); do
#	echo -n "($field)" >> $TUPLE_FILE
#	CTR=$(( CTR + 1 ))
#	if [[ $CTR -lt $STRING_FIELD_COUNT ]]; then
#		echo -ne ', \\\n' >> $TUPLE_FILE
#	fi
#done
#echo -ne '\\\n)\n' >> $TUPLE_FILE



cat $TUPLE_FILE >> $ORM_HPP
cat ./orm-part-2.hpp | sed -e "s|___\[\[\[TABLE\]\]\]___|${TABLE}|g" >> $ORM_HPP
