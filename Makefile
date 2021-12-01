
.PHONY: all clean run pack

all:
	@cd src && make -j 8

run:
	@cd src && make -j 8 && cd - && ./sim

clean:
	@rm -rf sim build 

pack:
	echo "TODO" ##tar -cvf ...
