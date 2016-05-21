CC:=gcc
CFLAGS:= -O2
TARGETCLI:=sobucli
TARGETSRV:=sobusrv

all: $(TARGETSRV) $(TARGETCLI)

$(TARGETCLI): sobucli.o
	$(CC) $(CFLAGS) $^ -o $@ 

$(TARGETSRV): sobusrv.o
	$(CC) $(CFLAGS) $^ -o $@ 

sobucli.o: sobucli.c
	$(CC) $(CFLAGS) -c $^

sobusrv.o: sobusrv.c
	$(CC) $(CFLAGS) -c $^

clean:
	@$(RM) *.o