BEGIN {
	
	printf "--[medical stat report]-- RS = " RS
	FS=","
}
{
	print $1 "\t" $2 "\t" $19
}
END {
	printf "--[done]--\n"
}
