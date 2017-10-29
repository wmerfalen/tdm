function quest_31(person){
	send_to_char(person,"quest 31");
	db_set(person + ":quest_trigger","quest_31('{character}');");
}
quest_31("{character}");
