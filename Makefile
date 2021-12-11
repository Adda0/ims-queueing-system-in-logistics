#===============================================================================================================
# File:        Makefile
# Project:     VUT, FIT, IMS, Queueing system in logistics
# Date:        11. 12. 2021
# Authors:     David Chocholatý (xchoch08@stud.fit.vutbr.cz), David Mihola (xmihol00@stud.fit.vutbr.cz)
#===============================================================================================================

.PHONY: all clean run pack

all:
	@cd src && make -j 8

run:
	@cd src && make -j 8 && cd - && ./model

clean:
	@rm -rf model build 

pack:
	echo "TODO" ##tar -cvf ...
