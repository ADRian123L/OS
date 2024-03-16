vzip: serial.c
	gcc serial.c -lz -o vzip -Wall

test:
	rm -f video.vzip
	./vzip frames
	./check.sh

clean:
	rm -f vzip video.vzip

