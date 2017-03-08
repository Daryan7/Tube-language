antlr -gt ${1}.g
if (($? == 0)); then
	dlg -ci parser.dlg scan.c
	if (($? == 0)); then
		g++ -I/usr/include/pccts $1.c scan.c err.c;
	fi
fi
