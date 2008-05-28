# You should have ROSE predefined


# Change this to your own installation of ROSE
DIAGNOSE_CMD = env LD_LIBRARY_PATH=${ROSE}/lib ./diagnose

# Location of include directory after "make install"
ROSE_INCLUDE_DIR = $(ROSE)/include

# Location of library directory after "make install"
ROSE_LIB_DIR = $(ROSE)/lib

# We can't use automake conditional support here
# Support for Gabriel's QRose GUI Library 
QT_LDFLAGS          = 
LIB_QT              = 

# DQ (8/14/2005): Updated from $(top_srcdir)/config/Makefile.for.ROSE.includes.and.libs
ROSE_LIBS = -lrose -lfl -lm -lrt $(MYSQL_DATABASE_LIBS) $(LIB_QT)


# Default make rule to use
all: pgms tests

pgms: diagnose 
diagnose: diagnose.o \
		EXP.o \
		ARR.o \
		STR.o \
		MEM.o \
		FIO.o \
		ENV.o \
		SIG.o \
		MSC.o \
		POS.o \
		type.o \
		utilities.o
	libtool --mode=link g++ -Wall -g -o diagnose -L$(ROSE)/lib $(ROSE_LIBS) $^

%.o: %.C
	libtool --mode=compile g++ -c -g -DHAVE_CONFIG_H -I$(ROSE_INCLUDE_DIR) -o $@ $<

tests:	cce.test ncce.test

ncce.output:	diagnose
	@echo > $@
	@for file in ncce/*.c ; do ${DIAGNOSE_CMD} $$file >> $@ 2>&1 ; echo Ran $$file; done
	@perl -p -i -e "s|${PWD}|\.|;" $@
	@perl -p -i -e 's|diagnose: .*/EDG_SAGE_Connection|diagnose: .../EDGE_SAGE_Connection|;' $@
	@perl -n -i -e 'print if (!/Aborted/);' $@

cce.output:	diagnose
	@echo > $@
	@for file in cce/*.c ; do ${DIAGNOSE_CMD} $$file >> $@ 2>&1 ; echo Ran $$file; done
	@perl -p -i -e "s|${PWD}|\.|;" $@
	@perl -p -i -e 's|diagnose: .*/EDG_SAGE_Connection|diagnose: .../EDGE_SAGE_Connection|;' $@
	@perl -n -i -e 'print if (!/Aborted/);' $@

%.test:	%.output %.ans
	@echo "Checking $*"
	@-if [ -n "`cmp $*.output $*.ans 2>&1`" ]; then \
		echo "WARNING: $* failure" ;\
		diff -u $*.ans $*.output ;\
	fi

clean:
	-libtool --mode=clean rm diagnose a.out *flymake.ti *flymake.C *.o *.output *~ *# */*.o *.lo .libs nul */*~ */*#


# support for flymake.el
check-syntax:
	gcc -Wall -o nul -S -DHAVE_CONFIG_H -I$(ROSE_INCLUDE_DIR) ${CHK_SOURCES}
#check-syntax:
#	${DIAGNOSE_CMD} --edg:no_wrap_diagnostics --edg:remarks --edg:display_error_number -DHAVE_CONFIG_H -I$(ROSE_INCLUDE_DIR) ${CHK_SOURCES}
