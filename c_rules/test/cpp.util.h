#ifndef UTIL_CPP_H
#define UTIL_CPP_H

/*
 * Copyright (c) 2007 Carnegie Mellon University.
 * All rights reserved.

 * Permission to use this software and its documentation for any purpose is hereby granted,
 * provided that the above copyright notice appear and that both that copyright notice and
 * this permission notice appear in supporting documentation, and that the name of CMU not
 * be used in advertising or publicity pertaining to distribution of the software without
 * specific, written prior permission.
 *
 * CMU DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL CMU BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, RISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>
#include <deque>
#include <algorithm>
#include <set>
#include <functional>
#include <utility>
#include <map>
#include <cassert>
#include <sys/resource.h>
#include <cstdarg>
#include <errno.h>
#include <signal.h>
#include <stdexcept>
#include <cassert>
#include <cstring>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <csetjmp>
#include <iostream>

typedef unsigned int rsize_t;
typedef int errno_t;

extern char **environ;

using namespace std;

/* This is for the case where we just want to run it through a static
 * analysis tool and don't care about runtime errors. This is not 
 * specified as a #define to avoid being ignored by the tools.
 */

#define RUNTIME_IGNORE 1

#endif

