INCS = -I/usr/local/include/json-c
CFLAGS = $(INCS) -L/usr/local/lib -ljson-c
RPATH = -Wl,-rpath=/usr/local/lib/

json-c_demo:json-c_demo.o
	gcc -o json-c_demo json-c_demo.o $(CFLAGS)
	#gcc -o json-c_demo json-c_demo.o $(CFLAGS) $(RPATH)
json-c_demo.o:json-c_demo.c
	gcc $(INCS) -c -o $@ $<

clean:
	rm json-c_demo.o json-c_demo 
