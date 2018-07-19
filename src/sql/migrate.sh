#!/bin/bash
FINAL="$PWD/bootstrap-final.sql"
USERCREATE="$PWD/user-creation"
cat $USERCREATE > $FINAL
sudo su postgres -c "psql -f $FINAL"
for i in $(cat create.list); do
	sudo su postgres -c "psql -f $PWD/create/$i"
done
if [[ $? == 0 ]]; then
	rm $FINAL
fi
