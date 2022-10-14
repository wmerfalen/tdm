#!/bin/bash

echo "VK Script ran at $(date)" > /tmp/vk.run.log
cd $WEBROOT
pwd >> /tmp/vk.run.log

STORAGE=scripts/vk-data/values.hpp
cat scripts/vk-data/prefix.hpp.fragment > $STORAGE

while IFS= read -r line
do
	# check if this is a type define (i.e.: [uint8_t])
	echo $line | grep -E '^\[' 2>&1 > /dev/null
	if [[ $? -eq 0 ]]; then
		continue
	fi
	# check if empty line
	echo $line | grep -E '^$' 2>&1 > /dev/null
	if [[ $? -eq 0 ]]; then
		# empty line, continue
		continue
	fi
	VK_DEFINE=$(echo -n $line | sed -e 's|,.*$||g')
  echo -ne "vk_${VK_DEFINE},\n" >> $STORAGE
done < mods/values-list.cpp

cat scripts/vk-data/fragment-1 >> $STORAGE

while IFS= read -r line
do
	# check if this is a type define (i.e.: [uint8_t])
	echo $line | grep -E '^\[' 2>&1 > /dev/null
	if [[ $? -eq 0 ]]; then
		VK_TYPE=$(echo $line | sed -e 's|^\[||g' -e 's|\]$||g')
		continue
	fi
	# check if empty line
	echo $line | grep -E '^$' 2>&1 > /dev/null
	if [[ $? -eq 0 ]]; then
		# empty line, continue
		continue
	fi
	VK_DEFINE=$(echo -n $line | sed -e 's|,.*$||g')
	echo "CGET(${VK_TYPE},${VK_DEFINE});" >> $STORAGE
done < mods/values-list.cpp

cat scripts/vk-data/fragment-2 >> $STORAGE

STORAGE=scripts/vk-data/values.cpp
cat scripts/vk-data/values.cpp.fragment-1 > $STORAGE
VK_TYPE=
while IFS= read -r line
do
	# check if this is a type define (i.e.: [uint8_t])
	echo $line | grep -E '^\[' 2>&1 > /dev/null
	if [[ $? -eq 0 ]]; then
		continue
	fi
	# check if empty line
	echo $line | grep -E '^$' 2>&1 > /dev/null
	if [[ $? -eq 0 ]]; then
		# empty line, continue
		continue
	fi
	VK_DEFINE=$(echo $line | sed -e 's|,.*$||g')
	echo -E "case vk_${VK_DEFINE}: return \"${VK_DEFINE}\";" >> $STORAGE
done < mods/values-list.cpp

cat scripts/vk-data/values.cpp.fragment-1a >> $STORAGE

VK_TYPE=
while IFS= read -r line
do
	# check if this is a type define (i.e.: [uint8_t])
	echo $line | grep -E '^\[' 2>&1 > /dev/null
	if [[ $? -eq 0 ]]; then
		continue
	fi
	# check if empty line
	echo $line | grep -E '^$' 2>&1 > /dev/null
	if [[ $? -eq 0 ]]; then
		# empty line, continue
		continue
	fi
	VK_DEFINE=$(echo $line | sed -e 's|,.*$||g')
	echo -E "if(!s.compare(\"${VK_DEFINE}\")){ return vk_${VK_DEFINE};}" >> $STORAGE
done < mods/values-list.cpp

echo " return (value_key_t)0;}" >> $STORAGE

VK_TYPE=
while IFS= read -r line
do
	# check if this is a type define (i.e.: [uint8_t])
	echo $line | grep -E '^\[' 2>&1 > /dev/null
	if [[ $? -eq 0 ]]; then
		if [[ ! -z $VK_TYPE ]]; then
			echo "};" >> $STORAGE
		fi
		VK_TYPE=$(echo $line | sed -e 's|^\[||g' -e 's|\]$||g')
		CLEAN_TYPE=$(echo -E $line | sed -e 's|_t||g' -e 's|std::||g' -e 's|\[||' -e 's|\]||g')
		echo 'static const std::vector<value_key_t> FOOBARTYPE_types = {' | sed -e "s|FOOBARTYPE|${CLEAN_TYPE}|g"  >> $STORAGE
		continue
	fi
	# check if empty line
	echo $line | grep -E '^$' 2>&1 > /dev/null
	if [[ $? -eq 0 ]]; then
		# empty line, continue
		continue
	fi
	VK_DEFINE=$(echo $line | sed -e 's|,.*$||g')
	echo "vk_${VK_DEFINE}," >> $STORAGE
done < mods/values-list.cpp

cat scripts/vk-data/values.cpp.fragment-2 >> $STORAGE

VK_TYPE=
while IFS= read -r line
do
	# check if this is a type define (i.e.: [uint8_t])
	echo $line | grep -E '^\[' 2>&1 > /dev/null
	if [[ $? -eq 0 ]]; then
		VK_TYPE=$(echo -En $line | sed -e 's|^\[||g' -e 's|\]$||g')
		continue
	fi
	# check if empty line
	echo -E $line | grep -E '^$' 2>&1 > /dev/null
	if [[ $? -eq 0 ]]; then
		# empty line, continue
		continue
	fi
	VK_DEFINE=$(echo -En "$line" | sed -e 's|,.*$||g')
	VK_VALUE=$(echo -En "$line" | sed -E 's|^[^,]+,||')
	echo -E "CGET_DEF(${VK_TYPE},${VK_DEFINE},${VK_VALUE});" >> $STORAGE
done < mods/values-list.cpp

cat scripts/vk-data/values.cpp.fragment-3 >> $STORAGE

diff scripts/vk-data/values.hpp mods/values.hpp 2>&1 > /dev/null
if [[ $? -eq 1 ]]; then
	# files are different. update values.hpp
	cp scripts/vk-data/values.hpp mods/
fi

diff scripts/vk-data/values.cpp mods/values.cpp 2>&1 > /dev/null

if [[ $? -eq 1 ]]; then
	# files are different. update values.hpp
	cp scripts/vk-data/values.cpp mods/
fi
