#!/bin/bash
FINAL="bootstrap-final.sql"

FLAG_DROP=false
FLAG_CONTENT=false
FLAG_USER_CREATE=false
FLAG_DB_CREATE=false
FLAG_DONT_RUN=false
FLAG_PRESERVE_FINAL=false

function usage(){ 
			echo "usage: migrate.sh <options>"
			echo -- "[ list of options ]"
			echo -- "--drop-first       drop all tables before creating them"
			echo -- "--only-content     only import content, do nothing else"
			echo -- "--include-content  import content on top of any other flags"
			echo -- "--create-db        creates the database (mud)"
			echo -- "--dont-run         simply generate the commands and place them in $FINAL"
			echo -- "--preserve-final   preserve the final script file (saved to: $FINAL)"
			echo -- "--echo-final       instead of running the file or saving it to $FINAL, print the contents of $FINAL"
			echo -- "--create-user      create user. Default user is whatever \$USER is set to"
			echo -- "--all              do everything (drops everything then recreates everything)"
			echo -- "--help             this help screen"
			echo -- "[ end of options ]"
			echo -- "[migrate.sh version: 0.01]"
}

ARGS_PROCESSED=0
for i in $@ ; do
	case $i in
		--create-user)
			FLAG_USER_CREATE=true
			ARGS_PROCESSED=$(( $ARGS_PROCESSED + 1 ))
			;;
		--echo-final)
			FLAG_ECHO_FINAL=true
			ARGS_PROCESSED=$(( $ARGS_PROCESSED + 1 ))
			;;
		--drop-first)
			FLAG_DROP=true
			ARGS_PROCESSED=$(( $ARGS_PROCESSED + 1 ))
			;;
		--only-content)
			FLAG_DROP=false
			FLAG_CONTENT=true
			ARGS_PROCESSED=$(( $ARGS_PROCESSED + 1 ))
			;;
		--include-content)
			FLAG_CONTENT=true
			ARGS_PROCESSED=$(( $ARGS_PROCESSED + 1 ))
			;;
		--create-db)
			FLAG_DB_CREATE=true
			ARGS_PROCESSED=$(( $ARGS_PROCESSED + 1 ))
			;;
		--preserve-final)
			FLAG_PRESERVE_FINAL=true
			ARGS_PROCESSED=$(( $ARGS_PROCESSED + 1 ))
			;;
		--help)
			usage
			exit 1
			;;
		--dont-run)
			FLAG_DONT_RUN=true
			ARGS_PROCESSED=$(( $ARGS_PROCESSED + 1 ))
			;;
		--all)
			FLAG_CONTENT=true
			FLAG_DROP=true
			FLAG_USER_CREATE=true
			FLAG_DB_CREATE=true
			ARGS_PROCESSED=$(( $ARGS_PROCESSED + 1 ))
			;;
		esac
done

if [[ $# -eq 0  || $ARGS_PROCESSED -eq 0 ]]; then
	usage
	exit 1
fi

echo '' > $FINAL
if [[ $FLAG_USER_CREATE == true ]]; then
	echo "CREATE ROLE $USER LOGIN;" >> $FINAL
fi

if [[ $FLAG_DB_CREATE == true ]]; then
	echo "CREATE DATABASE mud WITH OWNER = $USER;" >> $FINAL
fi

echo "\\c mud" >> $FINAL

for i in $(cat create.list); do
	cat "$PWD/create/$i" >> $FINAL
done

if [[ $FLAG_CONTENT == true ]]; then
	cat "$PWD/content/mud.sql" >> $FINAL
fi

if [[ $FLAG_ECHO_FINAL == true ]]; then
	cat $FINAL
	exit 0
fi

if [[ $FLAG_DONT_RUN == true ]]; then
	exit 0
fi

psql -f $FINAL
X_PRESERVE_RUN_STATUS=$?

if [[ $FLAG_PRESERVE_FINAL == true ]]; then 
	exit 0
fi

if [[ $X_PRESERVE_RUN_STATUS == 0 ]]; then
	rm $FINAL
else
	echo "There was an error running 'psql -f $FINAL'. A non-zero return status was returned: $X_PRESERVE_RUN_STATUS"
fi
