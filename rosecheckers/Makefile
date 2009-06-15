## Copyright (C) 2007-2009 by Carnegie Mellon University.
##
## @OPENSOURCE_HEADER_START@
##
## Use of the ROSE Checkers system and related source code is subject to
## the terms of the following license:
##
## GNU Public License (GPL) Rights pursuant to Version 2, June 1991
##
## NO WARRANTY
##
## ANY INFORMATION, MATERIALS, SERVICES, INTELLECTUAL PROPERTY OR OTHER
## PROPERTY OR RIGHTS GRANTED OR PROVIDED BY CARNEGIE MELLON UNIVERSITY
## PURSUANT TO THIS LICENSE (HEREINAFTER THE "DELIVERABLES") ARE ON AN
## "AS-IS" BASIS. CARNEGIE MELLON UNIVERSITY MAKES NO WARRANTIES OF ANY
## KIND, EITHER EXPRESS OR IMPLIED AS TO ANY MATTER INCLUDING, BUT NOT
## LIMITED TO, WARRANTY OF FITNESS FOR A PARTICULAR PURPOSE,
## MERCHANTABILITY, INFORMATIONAL CONTENT, NONINFRINGEMENT, OR ERROR-FREE
## OPERATION. CARNEGIE MELLON UNIVERSITY SHALL NOT BE LIABLE FOR INDIRECT,
## SPECIAL OR CONSEQUENTIAL DAMAGES, SUCH AS LOSS OF PROFITS OR INABILITY
## TO USE SAID INTELLECTUAL PROPERTY, UNDER THIS LICENSE, REGARDLESS OF
## WHETHER SUCH PARTY WAS AWARE OF THE POSSIBILITY OF SUCH DAMAGES.
## LICENSEE AGREES THAT IT WILL NOT MAKE ANY WARRANTY ON BEHALF OF
## CARNEGIE MELLON UNIVERSITY, EXPRESS OR IMPLIED, TO ANY PERSON
## CONCERNING THE APPLICATION OF OR THE RESULTS TO BE OBTAINED WITH THE
## DELIVERABLES UNDER THIS LICENSE.
##
## Licensee hereby agrees to defend, indemnify, and hold harmless Carnegie
## Mellon University, its trustees, officers, employees, and agents from
## all claims or demands made against them (and any related losses,
## expenses, or attorney's fees) arising out of, or relating to Licensee's
## and/or its sub licensees' negligent use or willful misuse of or
## negligent conduct or willful misconduct regarding the Software,
## facilities, or other rights or assistance granted by Carnegie Mellon
## University under this License, including, but not limited to, any
## claims of product liability, personal injury, death, damage to
## property, or violation of any laws or regulations.
##
## @OPENSOURCE_HEADER_END@
##

# You should have ROSE predefined to point to ROSE

# Change this to your own installation of ROSE
ROSECHECKERS_CMD = ./rosecheckers

# Location of include directory after "make install"
ROSE_INCLUDE_DIR = $(ROSE)/include

# Location of library directory after "make install"
ROSE_LIB_DIR = $(ROSE)/lib

# We can't use automake conditional support here
# Support for Gabriel's QRose GUI Library 
QT_LDFLAGS          = 
LIB_QT              = 

# DQ (8/14/2005): Updated from $(top_srcdir)/config/Makefile.for.ROSE.includes.and.libs
ROSE_LIBS = -lrose $(MYSQL_DATABASE_LIBS) $(LIB_QT)

SRCS = PRE.C DCL.C EXP.C ARR.C FLP.C INT.C STR.C MEM.C FIO.C ENV.C SIG.C ERR.C OBJ.C MSC.C POS.C \
	categories.C type.C value.C utilities.C utilities_cpp.C
OBJS = $(SRCS:.C=.o)
PGMS = rosecheckers virtualCFG

ANSWERS = $(wildcard test/*.ans)
ANSWERS_C = $(wildcard test/c.*.ans)
ANSWERS_CPP = $(wildcard test/cpp.*.ans)
OUTPUTS = $(ANSWERS:.ans=.output)
TESTS = $(ANSWERS:.ans=.test)
TESTS_C = $(ANSWERS_C:.ans=.test)
TESTS_CPP = $(ANSWERS_CPP:.ans=.test)

.PRECIOUS: $(OUTPUTS)

# Default make rule to use
all: pgms tests

pgms: $(PGMS)
$(PGMS): %: %.o $(OBJS)
	libtool --mode=link g++ -Wall -g -o $@ -L$(ROSE)/lib $(ROSE_LIBS) $^
%.o: %.C
	libtool --mode=compile g++ -Wall -c -g -DHAVE_CONFIG_H -I$(ROSE_INCLUDE_DIR) -o $@ $<

tests:	$(TESTS)

%.output:	%.c $(PGMS)
	@${ROSECHECKERS_CMD} $*.c > $@ 2>&1 || true
	@perl -p -i -e "s|${PWD}|\.|;" $@
	@perl -p -i -e 's|rosecheckers: .*/EDG_SAGE_Connection|rosecheckers: .../EDGE_SAGE_Connection|;' $@
	@perl -n -i -e 'print if (!/Aborted/);' $@
	@perl -e 'undef $$/; while (<>) {s/ *\n {10,11}([^ ^])/ \1/g; print;};' $@ > /tmp/testdata
	@mv /tmp/testdata $@

%.output:	%.C $(PGMS)
	@${ROSECHECKERS_CMD} $*.C > $@ 2>&1 || true
	@perl -p -i -e "s|${PWD}|\.|;" $@
	@perl -p -i -e 's|rosecheckers: .*/EDG_SAGE_Connection|rosecheckers: .../EDGE_SAGE_Connection|;' $@
	@perl -n -i -e 'print if (!/Aborted/);' $@
	@perl -e 'undef $$/; while (<>) {s/ *\n {10,11}([^ ^])/ \1/g; print;};' $@ > /tmp/testdata
	@mv /tmp/testdata $@

%.test:	%.output %.ans
	@echo "Checking $*"
	@-if [ -n "`cmp $*.output $*.ans 2>&1`" ]; then \
		echo "WARNING: $* failure" ;\
		diff -u $*.ans $*.output ;\
	fi

tests_c:	$(TESTS_C)

%.output:	%.c $(PGMS)
	@${ROSECHECKERS_CMD} $*.c > $@ 2>&1 || true
	@perl -p -i -e "s|${PWD}|\.|;" $@
	@perl -p -i -e 's|rosecheckers: .*/EDG_SAGE_Connection|rosecheckers: .../EDGE_SAGE_Connection|;' $@
	@perl -n -i -e 'print if (!/Aborted/);' $@
	@perl -e 'undef $$/; while (<>) {s/ *\n {10,11}([^ ^])/ \1/g; print;};' $@ > /tmp/testdata
	@mv /tmp/testdata $@

%.output:	%.C $(PGMS)
	@${ROSECHECKERS_CMD} $*.C > $@ 2>&1 || true
	@perl -p -i -e "s|${PWD}|\.|;" $@
	@perl -p -i -e 's|rosecheckers: .*/EDG_SAGE_Connection|rosecheckers: .../EDGE_SAGE_Connection|;' $@
	@perl -n -i -e 'print if (!/Aborted/);' $@
	@perl -e 'undef $$/; while (<>) {s/ *\n {10,11}([^ ^])/ \1/g; print;};' $@ > /tmp/testdata
	@mv /tmp/testdata $@

%.test:	%.output %.ans
	@echo "Checking $*"
	@-if [ -n "`cmp $*.output $*.ans 2>&1`" ]; then \
		echo "WARNING: $* failure" ;\
		diff -u $*.ans $*.output ;\
	fi

tests_cpp:	$(TESTS_CPP)

%.output:	%.c $(PGMS)
	@${ROSECHECKERS_CMD} $*.c > $@ 2>&1 || true
	@perl -p -i -e "s|${PWD}|\.|;" $@
	@perl -p -i -e 's|rosecheckers: .*/EDG_SAGE_Connection|rosecheckers: .../EDGE_SAGE_Connection|;' $@
	@perl -n -i -e 'print if (!/Aborted/);' $@
	@perl -e 'undef $$/; while (<>) {s/ *\n {10,11}([^ ^])/ \1/g; print;};' $@ > /tmp/testdata
	@mv /tmp/testdata $@

%.output:	%.C $(PGMS)
	@${ROSECHECKERS_CMD} $*.C > $@ 2>&1 || true
	@perl -p -i -e "s|${PWD}|\.|;" $@
	@perl -p -i -e 's|rosecheckers: .*/EDG_SAGE_Connection|rosecheckers: .../EDGE_SAGE_Connection|;' $@
	@perl -n -i -e 'print if (!/Aborted/);' $@
	@perl -e 'undef $$/; while (<>) {s/ *\n {10,11}([^ ^])/ \1/g; print;};' $@ > /tmp/testdata
	@mv /tmp/testdata $@

%.test:	%.output %.ans
	@echo "Checking $*"
	@-if [ -n "`cmp $*.output $*.ans 2>&1`" ]; then \
		echo "WARNING: $* failure" ;\
		diff -u $*.ans $*.output ;\
	fi


clean:
	-libtool --mode=clean rm $(PGMS) a.out *.ti *.loT *flymake.C *.o test/*.output *~ *# */*.o *.lo .libs nul */*~ */*#

doc:
	@echo "bulding documentation..."
	doxygen Doxyfile

# support for flymake.el
check-syntax:
	gcc -std=c99 -Wall -o nul -S -DHAVE_CONFIG_H -I$(ROSE_INCLUDE_DIR) ${CHK_SOURCES}
#check-syntax:
#	${ROSECHECKERS_CMD} --edg:no_wrap_diagnostics --edg:remarks --edg:display_error_number -DHAVE_CONFIG_H -I$(ROSE_INCLUDE_DIR) ${CHK_SOURCES}
