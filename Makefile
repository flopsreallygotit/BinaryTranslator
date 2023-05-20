.PHONY: all
all:
		@make -C ./Frontend/ -f Makefile
		@make -C ./Backend/  -f Makefile

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

clear:		
		@make -C ./Frontend/ -f Makefile clear
		@make -C ./Backend/  -f Makefile clear

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
