#!/bin/bash
for table in $(cat mud.sql | sed -e 's|*||g' | grep CREATE | grep 'TABLE' | cut -d '.' -f 2 | cut -d ' ' -f 1); do  
	cat mud.sql | sed -e 's|*||g' |  sed -E "s|([a-zA-Z0-9]{1})'{2}([a-zA-Z0-9]{1})|\1\\\\'\2|g" | 
	 sed -E "s|([a-zA-Z0-9]{1})'{2}([a-zA-Z0-9]{1})|\1\\\\'\2|g"  | sed -E "s|([ ]{1})'{2}([a-zA-Z0-9]{1})|\1\\\\'\2|g" | 
	 sed -E "s|([a-zA-Z0-9]{1})'{2}([ ]{1})|\1\\\\'\2|g" |
		grep 'INSERT' | grep $table | sed -e 's|INSERT INTO public.||g' | 
		sed -e 's| VALUES (|:|g' |  sed -e 's|);$||gi'; 
done > all.lmdb
