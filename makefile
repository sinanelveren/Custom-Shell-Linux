CC = gcc
DB = gdb
CFLAGS = -o
DFLAGS = -g
MAIN = 111044074_main
LS = 111044074_ls
CAT = 111044074_cat
WC = 111044074_wc
PROGNAME = GTU_Shell
PROGLS = ls
PROGCAT = cat
PROGWC = wc

all:
	$(CC) -std=c11 -c $(MAIN).c $(LS).c $(CAT).c $(WC).c
	$(CC) $(MAIN).o $(CFLAGS) $(PROGNAME)
	$(CC) $(LS).o $(CFLAGS) $(PROGLS)
	$(CC) $(CAT).o $(CFLAGS) $(PROGCAT)
	$(CC) $(WC).o $(CFLAGS) $(PROGWC)

debug:
	$(CC) -std=c11 $(DFLAGS) $(MAIN).c $(CFLAGS) $(PROGNAME)
	$(DB) ./$(PROGNAME)


clean:
	rm -f $(PROGNAME) $(PROGLS) $(PROGCAT) $(PROGWC) *.o *.txt
