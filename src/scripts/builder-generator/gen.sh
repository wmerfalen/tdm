#!/bin/bash

if [[ $# -lt 2 ]]; then
	echo 'usage: ormgen <table> <input-file>'
	exit 1
fi

CURDIR=${MUD_ROOT}/scripts/builder-generator/
GEN_ORM_ONE=$CURDIR/orm-part-1
GEN_ORM_TWO=$CURDIR/orm-part-2
GEN_ORM_CPP=$CURDIR/orm.cpp
TABLE=$1
INPUT_FILE=$2
CAPS_TABLE=${TABLE^^}
ORM_HPP=${MUD_ROOT}/mods/orm/${TABLE}.hpp
ORM_CPP=${MUD_ROOT}/mods/orm/${TABLE}.cpp
TUPLE_FILE=$CURDIR/.tmp-gen-tuple.hpp
TMP_FILE=$CURDIR/.tmp-general
TMP=$CURDIR/.tmp

for tmp in $TUPLE_FILE $TMP_FILE $TMP; do
	touch $tmp
done

echo '' > $TMP

function str_fields(){
	cat $INPUT_FILE | 
		grep '\[std::string\]' -A 50 |
		grep -E '^[[:blank:]]*$' -B 20 --max-count=1 |
		grep -v '\[std::string\]' |
		grep -E '[a-z0-9_]+' 
}

function mob_vnum_fields(){
	cat $INPUT_FILE | 
		grep '\[mob_vnum\]' -A 50 |
		grep -E '^[[:blank:]]*$' -B 20 --max-count=1 |
		grep -v '\[mob_vnum\]' |
		grep -E '[a-z0-9_]+'
}


function player_id_fields(){
	cat $INPUT_FILE | 
		grep '\[player_id\]' -A 50 |
		grep -E '^[[:blank:]]*$' -B 20 --max-count=1 |
		grep -v '\[player_id\]' |
		grep -E '[a-z0-9_]+'
}

function room_vnum_fields(){
	cat $INPUT_FILE | 
		grep '\[room_vnum\]' -A 50 |
		grep -E '^[[:blank:]]*$' -B 20 --max-count=1 |
		grep -v '\[room_vnum\]' |
		grep -E '[a-z0-9_]+'
}

function zone_vnum_fields(){
	cat $INPUT_FILE | 
		grep '\[zone_vnum\]' -A 50 |
		grep -E '^[[:blank:]]*$' -B 20 --max-count=1 |
		grep -v '\[zone_vnum\]' |
		grep -E '[a-z0-9_]+'
}

function all_fields(){
	cat $INPUT_FILE |
		grep -v -E '^\[.*\]$' |
		grep -E '[a-z0-9_]+'
}

function all_fields_count(){
	INIT=$(initialize_row_fields | wc -l)
	INIT=$(( INIT ))
	ALL=$(all_fields | wc -l)
	ALL=$(( ALL - INIT))
	ALL=$(( ALL + 1 ))
	echo $ALL
}

function initialize_row_fields(){
	cat $INPUT_FILE | 
		grep '\[initialize_row\]' -A 50 |
		grep -E '^[[:blank:]]*$' -B 20 --max-count=1 |
		grep -v '\[initialize_row\]' |
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

LC=$(all_fields_count)
CTR=1
str_fields > $TMP_FILE
for field in $(cat $TMP_FILE | cut -d',' -f 1); do
	echo -n "(std::string,$field,\"\",3,null,1)" >> $TUPLE_FILE
	CTR=$(( CTR + 1 ))
	if [[ $CTR -lt $LC ]]; then
		echo -ne ', \\\n' >> $TUPLE_FILE
	fi
done

mob_vnum_fields > $TMP_FILE 
for field in $(cat $TMP_FILE); do
	echo -n "(mob_vnum,$field,0,1,null,1)" >> $TUPLE_FILE
	CTR=$(( CTR + 1 ))
	if [[ $CTR -lt $LC ]]; then
		echo -ne ', \\\n' >> $TUPLE_FILE
	fi
done

room_vnum_fields > $TMP_FILE
for field in $(cat $TMP_FILE); do
	echo -n "(room_vnum,$field,0,1,null,1)" >> $TUPLE_FILE
	CTR=$(( CTR + 1 ))
	if [[ $CTR -lt $LC ]]; then
		echo -ne ', \\\n' >> $TUPLE_FILE
	fi
done

zone_vnum_fields > $TMP_FILE
for field in $(cat $TMP_FILE); do
	echo -n "(zone_vnum,$field,0,1,null,1)" >> $TUPLE_FILE
	CTR=$(( CTR + 1 ))
	if [[ $CTR -lt $LC ]]; then
		echo -ne ', \\\n' >> $TUPLE_FILE
	fi
done

echo ')' >> $TUPLE_FILE


cat $GEN_ORM_ONE | sed -e "s|___\[\[\[TABLE\]\]\]___|${TABLE}|g" > $ORM_HPP
cat $ORM_HPP | sed -e "s|___\[\[\[CAPS_TABLE\]\]\]___|${CAPS_TABLE}|g" > $TMP; cat $TMP > $ORM_HPP

cat $GEN_ORM_CPP | sed -e "s|___\[\[\[TABLE\]\]\]___|${TABLE}|g" > $ORM_CPP
cat $ORM_CPP | sed -e "s|___\[\[\[CAPS_TABLE\]\]\]___|${CAPS_TABLE}|g" > $TMP; cat $TMP > $ORM_CPP


cat $TUPLE_FILE >> $ORM_HPP
if [[ $(cat $INPUT_FILE | grep '\\[initialize_row\\]' ) -eq 0 ]]; then
	echo -e "\n#define ${CAPS_TABLE}_INITIALIZE_ROW_MEMBERS \\" >> $ORM_HPP
	initialize_row_fields > $TMP_FILE
	LC=$(cat $TMP_FILE | wc -l)
	CTR=0
	for field in $(cat $TMP_FILE); do
		echo -n "($field)" >> $ORM_HPP
		echo -ne ' \\\n' >> $ORM_HPP
	done
	echo -ne ' \n' >> $ORM_HPP
fi


cat $GEN_ORM_TWO | sed -e "s|___\[\[\[TABLE\]\]\]___|${TABLE}|g" >> $ORM_HPP
cat $ORM_HPP | sed -e "s|___\[\[\[CAPS_TABLE\]\]\]___|${CAPS_TABLE}|g" > $TMP; cat $TMP > $ORM_HPP
echo '' > $TMP

for tmp in $TUPLE_FILE $TMP_FILE $TMP; do
	rm $tmp
done

echo File generated at $ORM_HPP
echo File generated at $ORM_CPP



