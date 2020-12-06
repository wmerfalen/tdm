#!/bin/bash
function perform-parse(){
VK_OFFSET=$(grep -nE '^vk_[^,]+,$' mods/values.hpp | head -n 1 | cut -d: -f 1);
for i in $(grep -nE '^vk_[^,]+,$' mods/values.hpp | grep -v =); do
 VK=$(echo $i | cut -d: -f 2 | sed -e 's|^vk_||' -e 's|,||g' );
 NUM=$(echo -ne "$i" | cut -d: -f 1);
 echo -ne "$VK -> "; echo $(($NUM - $VK_OFFSET));
done 
}

if [[ $# -gt 0 ]]; then
	perform-parse | grep $@
else
	perform-parse
fi
