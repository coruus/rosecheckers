#
# Copyright (c) 2009 Carnegie Mellon University.
# All rights reserved.
#
# Permission to use this software and its documentation for any purpose is hereby granted,
# provided that the above copyright notice appear and that both that copyright notice and
# this permission notice appear in supporting documentation, and that the name of CMU not
# be used in advertising or publicity pertaining to distribution of the software without
# specific, written prior permission.
#
# CMU DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES
# OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL CMU BE LIABLE FOR ANY SPECIAL, INDIRECT OR
# CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
# WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, RISING OUT OF OR IN
# CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
#


# You should have ROSE predefined to point to ROSE

# Change this to your own installation of ROSE
DIAGNOSE_CMD = ./diagnose

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
	categories.C value.C utilities.C utilities_cpp.C
OBJS = $(SRCS:.C=.o)
PGMS = diagnose

ANSWERS = $(wildcard test/*.ans)
OUTPUTS = $(ANSWERS:.ans=.output)
TESTS = $(ANSWERS:.ans=.test)

.PRECIOUS: $(OUTPUTS)

# Default make rule to use
all: pgms tests

pgms: $(PGMS)
$(PGMS): %: %.o $(OBJS)
	libtool --mode=link g++ -Wall -g -o $@ -L$(ROSE)/lib $(ROSE_LIBS) $^
%.o: %.C
	libtool --mode=compile g++ -Wall -c -g -DHAVE_CONFIG_H -I$(ROSE_INCLUDE_DIR) -o $@ $<

tests:	$(TESTS)

%.output:	%.c pgms
	@echo -n > $@
	@${DIAGNOSE_CMD} $*.c >> $@ 2>&1 || true
	@perl -p -i -e "s|${PWD}|\.|;" $@
	@perl -p -i -e 's|diagnose: .*/EDG_SAGE_Connection|diagnose: .../EDGE_SAGE_Connection|;' $@
	@perl -n -i -e 'print if (!/Aborted/);' $@

%.output:	%.C pgms
	@echo -n > $@
	@${DIAGNOSE_CMD} $*.C >> $@ 2>&1 || true
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
	-libtool --mode=clean rm $(PGMS) a.out *.ti *.loT *flymake.C *.o test/*.output *~ *# */*.o *.lo .libs nul */*~ */*#

doc:
	@echo "bulding documentation..."
	doxygen Doxyfile

# support for flymake.el
check-syntax:
	gcc -std=c99 -Wall -o nul -S -DHAVE_CONFIG_H -I$(ROSE_INCLUDE_DIR) ${CHK_SOURCES}
#check-syntax:
#	${DIAGNOSE_CMD} --edg:no_wrap_diagnostics --edg:remarks --edg:display_error_number -DHAVE_CONFIG_H -I$(ROSE_INCLUDE_DIR) ${CHK_SOURCES}
