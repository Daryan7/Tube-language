echo $1
/opt/pccts/bin/antlr -gt ${1}.g
/opt/pccts/bin/dlg -ci parser.dlg scan.c
g++ -std=c++11 -o $1 ${1}.c -w -I/home/soft/PCCTS_v1.33/include/ scan.c err.c
