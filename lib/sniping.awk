BEGIN {
	
	printf "--[sniping stat report]-- RS = " RS
	FS=","
}
{
	print $1 "\t" $2 "\t" $15
}
END {
	printf "--[done]--\n"
}
