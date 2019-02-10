#!/bin/bash
for table in $(cat mud.sql | sed -e 's|
	cat mud.sql | sed -e 's|
	 sed -E "s|([a-zA-Z0-9]{1})'{2}([a-zA-Z0-9]{1})|\1\\\\'\2|g"  | sed -E "s|([ ]{1})'{2}([a-zA-Z0-9]{1})|\1\\\\'\2|g" | 
	 sed -E "s|([a-zA-Z0-9]{1})'{2}([ ]{1})|\1\\\\'\2|g" |
		grep 'INSERT' | grep $table | sed -e 's|INSERT INTO public.||g' | 
		sed -e 's| VALUES (|:|g' |  sed -e 's|);$||gi'; 
done > all.lmdb