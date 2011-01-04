all: time-machine-size

OPTS = -Wall -Os

time-machine-size: %: %.o
	gcc $(OPTS) -o $@ $<

%.o: %.c
	gcc $(OPTS) -c -o $@ $<

clean:
	rm -f *.o time-machine-size

.PHONY: clean