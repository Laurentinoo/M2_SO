# makefile for scheduling program
#
# make rr_p - for round-robin scheduling com prioridade
# make edf - for edf scheduling
# make rr - para round-robin simples

CC=gcc
CFLAGS=-Wall

clean:
	rm -rf *.o
	rm -rf rr_p
	rm -rf edf
	rm -rf rr
	rm -rf aging

aging: driver.o list.o CPU.o schedule_pa.o
	$(CC) $(CFLAGS) -o aging driver.o schedule_pa.o list.o CPU.o

rr_p: driver.o list.o CPU.o schedule_rr_p.o
	$(CC) $(CFLAGS) -o rr_p driver.o schedule_rr_p.o list.o CPU.o

edf: driver.o list.o CPU.o schedule_edf.o
	$(CC) $(CFLAGS) -o edf driver.o schedule_edf.o list.o CPU.o -lpthread

rr: driver.o list.o CPU.o schedule_rr.o
	$(CC) $(CFLAGS) -o rr driver.o schedule_rr.o list.o CPU.o

driver.o: driver.c
	$(CC) $(CFLAGS) -c driver.c

schedule_rr_p.o: schedule_rr_p.c
	$(CC) $(CFLAGS) -c schedule_rr_p.c

schedule_edf.o: schedule_edf.c
	$(CC) $(CFLAGS) -c schedule_edf.c

schedule_rr.o: schedule_rr.c
	$(CC) $(CFLAGS) -c schedule_rr.c

list.o: list.c list.h
	$(CC) $(CFLAGS) -c list.c

CPU.o: CPU.c CPU.h
	$(CC) $(CFLAGS) -c CPU.c
