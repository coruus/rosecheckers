/*
 * Copyright (c) 2009 Carnegie Mellon University.
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

#include "cpp.util.h"
#include "cpp.lib.FIO.h"


void FIO00();
void FIO01();
void FIO02();
void FIO03();
void FIO04();
void FIO05();
void FIO06();
void FIO07();
void FIO08();
void FIO09();
void FIO10();
void FIO11();
void FIO12();
void FIO13();
void FIO14();
void FIO15();
void FIO16();
void FIO17();
void FIO18();
void FIO30();
void FIO31();
void FIO32();
void FIO33();
void FIO34();
void FIO35();
void FIO36();
void FIO37();
void FIO38();
void FIO39();
void FIO40();
void FIO41();
void FIO42();
void FIO43();
void FIO44();
void FIO45();

void FIO() {
  FIO00();
  FIO01();
  FIO02();
  FIO03();
  FIO04();
  FIO05();
  FIO06();
  FIO07();
  FIO08();
  FIO09();
  FIO10();
  FIO11();
  FIO12();
  FIO13();
  FIO14();
  FIO15();
  FIO16();
  FIO17();
  FIO18();
  FIO30();
  FIO31();
  FIO32();
  FIO33();
  FIO34();
  FIO35();
  FIO36();
  FIO37();
  FIO38();
  FIO39();
  FIO40();
  FIO41();
  FIO42();
  FIO43();
  FIO44();
  FIO45();
}



/* FIO00_cpp */
void FIO00() {
  const char *error_msg = "Resource not available to user.";
  int error_type = 3;
  /* ... */
  printf("Error (type %s): %d\n", error_type, error_msg);
}


/* FIO01_cpp */
void FIO01_0() {
  char *file_name;
  FILE *f_ptr;

  /* initialize file_name */

  f_ptr = fopen(file_name, "w");
  if (f_ptr == NULL) {
    /* Handle error */
  }

  /*... Process file ...*/

  if (fclose(f_ptr) != 0) {
    /* Handle error */
  }

  if (remove(file_name) != 0) {
    /* Handle error */
  }
}

void FIO01_1() {
  char *file_name;
  FILE *f_ptr;

  /* initialize file_name */

  f_ptr = fopen(file_name, "w");
  if (f_ptr == NULL)  {
    /* Handle error */
  }

  /* ... */

  if (chmod(file_name, S_IRUSR) == -1) {
    /* Handle error */
  }
}

/* End {code} */
void FIO01() {
  FIO01_0();
  FIO01_1();
}


/* FIO02_cpp */

/* Begin {code} */

/* Verify argv[1] is supplied */

void FIO02_0() {
  char string[] = "string";
  if (!verify_file(string)) {
    /* Handle error */
  }

  if (fopen(string, "w") == NULL) {
    /* Handle error */
  }
  /* ... */
}


void FIO02_1() {

  char *realpath_res = NULL;
  char *canonical_filename = NULL;
  size_t path_size = 0;
  long pc_result;
  char string[] = "string";

  /* Verify string is supplied */

  errno = 0;

  /* Query for PATH_MAX */
  pc_result = pathconf(string, _PC_PATH_MAX);

  if ( (pc_result == -1) && (errno != 0) ) {
    /* Handle error */
  } else if (pc_result == -1) {
    /* Handle error */
  } else if (pc_result <= 0) {
    /* Handle error */
  }
  path_size = (size_t)pc_result;

  if (path_size > 0) {
    canonical_filename = (char *) malloc(path_size);

    if (canonical_filename == NULL) {
      /* Handle error */
    }

    realpath_res = realpath(string, canonical_filename);
  }

  if (realpath_res == NULL) {
    /* Handle error */
  }

  if (!verify_file(realpath_res)) {
    /* handle error */
  }

  if (fopen(realpath_res, "w") == NULL) {
    /* Handle error */
  }

  /* ... */

  free(canonical_filename);
  canonical_filename = NULL;

}
void FIO02_2() {
  /* ... */

  enum { INITBUFSIZE = 256 };
  //using unsigned long instead of original DOWRD
  unsigned long ret = 0;
  unsigned long new_ret = 0;
  char *canonical_filename;
  char *new_file;
  char *file_name;
  char string[] = "string";

  /* ... */

  file_name = (char *)malloc(strlen(string)+1);
  canonical_filename = (char *)malloc(INITBUFSIZE);

  if ( (file_name != NULL) && (canonical_filename != NULL) ) {
    strcpy(file_name, string);
    strcpy(canonical_filename, "");
  } else {
    /* Handle error */
  }

    //XXX a windows thing
//  ret = GetFullPathName(
//      file_name,
//      INITBUFSIZE,
//      canonical_filename,
//      NULL
//      );
//
//  if (ret == 0) {
//    /* Handle error */
//  } 
//  else if (ret > INITBUFSIZE) {
//    new_file = (char *)realloc(canonical_filename, ret);
//    if (new_file == NULL) {
//      /* Handle error */
//    }
//
//    canonical_filename = new_file;
//
//    new_ret = GetFullPathName(
//        file_name,
//        ret,
//        canonical_filename,
//        NULL
//        );
//    if (new_ret > ret) {
//      /* 
//       * The length of the path changed between calls
//       * to GetFullPathName(), handle error 
//       */
//    } 
//    else if (new_ret == 0) {
//      /* Handle error */
//    }
//  }

  if (!verify_file(canonical_filename)) {
    /* Handle error */
  }
  /* Verify file name before using */
}
void FIO02() {
  FIO02_0();
  FIO02_1();
  FIO02_2();
}


//    /* FIO03_cpp */
//
//    /* Begin {code} */
//
//    char *file_name;
//    FILE *fp;
//
//    /* initialize file_name */
//
//    fp = fopen(file_name, "w");
//    if (!fp) {
//      /* Handle error */
//    }
//
///* End {code} */
//
///* Begin {code} */
//
//char *file_name;
//FILE *fp;
//
///* initialize file_name */
//errno_t res = fopen_s(&fp, file_name, "w");
//if (res != 0) {
//  /* Handle error */
//}
//
///* End {code} */
//
///* Begin {code} */
//
//#include <iostream>
//#include <fstream>
//
//using namespace std;
//
//int main(void) {
//
//  ifstream fi("foo.txt");// attempt to open as input file
//  if (!fi) {
//    // file doesn't exist; so it's safe to create it and write to it
//    ofstream fo("foo.txt");
//    // can now write to fo, e.g., fo << "Some output" << endl;
//    // /* ... */
//  }
//  else {  // file exists; close and handle error
//    fi.close();
//    // handle error
//  }
//
//}
//
///* End {code} */
//void FIO03() {
//}
//
//
///* FIO04_cpp */
//
///* Begin {code} */
//
//FILE *file;
//long offset;
//
///* initialize file and offset */
//
//fseek(file, offset, SEEK_SET);
///* process file */
//
///* End {code} */
//void FIO04() {
//}
//
//
///* FIO05_cpp */
//
///* Begin {code} */
//
//char *file_name;
//
///* initialize file_name */
//
//FILE *fd = fopen(file_name, "w");
//if (fd == NULL) {
//  /* Handle error */
//}
//
///*... write to file ...*/
//
//fclose(fd);
//fd = NULL;
//
///*
// * A race condition here allows for an attacker to switch 
// * out the file for another 
// */
//
///* ... */
//
//fd = fopen(file_name, "r");
//if (fd == NULL) {
//  /* Handle error */
//}
//
///*... read from file ...*/
//
//fclose(fd);
//fd = NULL;
//
///* End {code} */
//
///* Begin {code} */
//
//char *file_name;
//FILE *fd;
//
///* initialize file_name */
//
//fd = fopen(file_name, "w+");
//if (fd == NULL) {
//  /* Handle error */
//}
//
///* read user's file */
//
//fclose(fd);
//fd = NULL; 
//
///* End {code} */
//void FIO05() {
//}
//
//
///* FIO06_cpp */
//
///* Begin {code} */
//
//char *file_name;
//FILE *fp;
//
///* initialize file_name */
//
//fp = fopen(file_name, "w");
//if (!fp){
//  /* Handle error */
//}
//
///* End {code} */
//
///* Begin {code} */
//
//char *file_name;
//int fd;
//
///* initialize file_name */
//
//fd = open(file_name, O_CREAT | O_WRONLY);
///* access permissions were missing */
//
//if (fd == -1){
//  /* Handle error */
//}
//
///* End {code} */
//void FIO06() {
//}
//
//
///* FIO07_cpp */
//
///* Begin {code} */
//
//char *file_name;
//FILE *fp;
//
///* initialize file_name */
//
//fp = fopen(file_name, "r");
//if (fp == NULL) {
//  /* Handle open error */
//}
//
///* read data */
//
//rewind(fp);
//
///* continue */
//
///* End {code} */
//void FIO07() {
//}
//
//
///* FIO08_cpp */
//
///* Begin {code} */
//
//char *file_name;
//FILE *file;
//
///* initialize file_name */
//
//file = fopen(file_name, "w+");
//if (file == NULL) {
//  /* Handle error condition */
//}
//
///* ... */
//
//if (remove(file_name) != 0) {
//  /* Handle error condition */
//}
//
///* continue performing I/O operations on file */
//
//fclose(file);
//
///* End {code} */
//void FIO08() {
//}
//
//
///* FIO09_cpp */
//
///* Begin {code} */
//
//struct myData {
//  char c;
//  long l;
//};
//
///* ... */
//
//FILE *file;
//struct myData data;
//
///* initialize file */
//
//if (fread(&data, sizeof(struct myData), 1, file) < sizeof(struct myData)) {
//  /* Handle error */
//}
//
///* End {code} */
//void FIO09() {
//}
//
//
///* FIO10_cpp */
//
///* Begin {code} */
//
//const char *src_file = /* ... */;
//const char *dest_file = /* ... */;
//if (rename(src_file, dest_file) != 0) {
//  /* Handle Error */
//}
//
///* End {code} */
//
///* Begin {code} */
//
//const char *src_file = /* ... */;
//const char *dest_file = /* ... */;
//if (rename(src_file, dest_file) != 0) {
//  /* Handle error */
//}
//
///* End {code} */
//void FIO10() {
//}
//
//
///* FIO11_cpp */
//void FIO11() {
//}
//
//
///* FIO12_cpp */
//
///* Begin {code} */
//
//FILE *file;
///* Setup file */
//setbuf(file, NULL);
///* ... */
//
///* End {code} */
//void FIO12() {
//}
//
//
///* FIO13_cpp */
//
///* Begin {code} */
//
//FILE *fp;
//char *file_name;
//
///* initialize file_name */
//
//fp = fopen(file_name, "rb");
//if (fp == NULL) {
//  /* Handle error */
//}
//
///* read data */
//
//if (ungetc('\n', fp) == EOF) {
//  /* Handle error */
//}
//if (ungetc('\r', fp) == EOF) {
//  /* Handle error */
//}
//
///* continue on */
//
///* End {code} */
//void FIO13() {
//}
//
//
///* FIO14_cpp */
//void FIO14() {
//}
//
//
///* FIO15_cpp */
//
///* Begin {code} */
//
//char *file_name;
//FILE *fp;
//
///* initialize file_name */
//
//fp = fopen(file_name, "w");
//if (fp == NULL) {
//  /* Handle error */
//}
//
///*... Process file ...*/
//
//if (fclose(fp) != 0) {
//  /* Handle error */
//}
//
//if (remove(file_name) != 0) {
//  /* Handle error */
//}
//
///* End {code} */
//void FIO15() {
//}
//
//
///* FIO16_cpp */
//
///* Begin {code} */
//
//enum { array_max = 100 };
//
///*
// * Program running with elevated privileges where argv[1] 
// * and argv[2] are supplied by the user 
// */
//
//char x[array_max];
//FILE *fp = fopen(argv[1], "w");
//
//strncpy(x, argv[2], array_max);
//x[array_max - 1] = '\0';
//
///* 
// * Write operation to an unintended file like /etc/passwd 
// * gets executed  
// */
//if (fwrite(x, sizeof(x[0]), sizeof(x)/sizeof(x[0]), fp) <
//    sizeof(x)/sizeof(x[0])) {
//  /* Handle Error */
//}
//
///* End {code} */
//void FIO16() {
//}
//
//
///* FIO17_cpp */
//
///* Begin {code} */
//
//#include <cstdio>
//
//int main(int argc, char *argv[]) {
//  char filename[256];
//  FILE *f;
//  char format[256];
//
//  fscanf(stdin, "%s", filename);
//  f = fopen(filename, "r");  // read only
//
//  if (f == 0) {
//    sprintf(format, "Error opening file %s\n", filename);
//    fprintf(stderr, format);
//    exit(-1);
//  }
//  fclose(f);
//}
//
///* End {code} */
//void FIO17() {
//}
//
//
///* FIO18_cpp */
//
///* Begin {code} */
//#include <fstream>
//using namespace std;&#xA0;
//int main () {&#xA0;&#xA0;
//  char * buffer;
//  long size1, size2;&#xA0;
//  ofstream outfile ("new.txt", ofstream::binary);
//  buffer = new char [size1];&#xA0;
//
//  // write to outfile
//  outfile.write (buffer, size2);&#xA0;
//
//  delete[] buffer;
//  outfile.close();
//  return 0;}
//
//  /* End {code} */
//  void FIO18() {
//  }
//
//
///* FIO30_cpp */
//
///* Begin {code} */
//
//void incorrect_password(const char *user) {
//  /* user names are restricted to 256 characters or less */
//  static const char *msg_format
//    = "%s cannot be authenticated.\n";
//  size_t len = strlen(user) + strlen(msg_format) + 1;
//  char *msg = new char[len];
//  int ret = sprintf(msg, msg_format, user);
//  if (ret < 0 || ((size_t) ret) >= len) {
//    /* Handle error */
//  }
//  fprintf(stderr, msg);
//  delete[] msg;
//  msg = NULL;
//}
//
///* End {code} */
//
///* Begin {code} */
//
//void incorrect_password(const char *user) {
//  /* user names are restricted to 256 characters or less */
//  static const char *msg_format
//    = "%s cannot be authenticated.\n";
//  size_t len = strlen(user) + strlen(msg_format) + 1;
//  char *msg = new char[len];
//  int ret = sprintf(msg, msg_format, user);
//  if (ret < 0 || ((size_t) ret) >= len) {
//    /* Handle error */
//  }
//  syslog(LOG_INFO, msg);
//  delete[] msg;
//  msg = NULL;
//}
//
///* End {code} */
//void FIO30() {
//}
//
//
///* FIO31_cpp */
//
///* Begin {code} */
//
//void do_stuff(void) {
//  ofstream logfile("log", ios::app);
//
//  /* Write logs pertaining to do_stuff() */
//  logfile << "do_stuff" << endl;
//
//  /* ... */
//}
//
//int main(void) {
//  ofstream logfile("log", ios::app);
//
//  /* Write logs pertaining to main() */
//  logfile << "main" << endl;
//
//  do_stuff();
//
//  /* ... */
//}
//
///* End {code} */
//void FIO31() {
//}
//
//
///* FIO32_cpp */
//
///* Begin {code} */
//
//char *file_name;
//FILE *file;
//
///* initialize file_name */
//
//if (!fgets(file_name, sizeof(file_name), stdin)) {
//  /* Handle error */
//}
//
//if ((file = fopen(file_name, "wb")) == NULL) {
//  /* Handle error */
//}
//
///* operate on file */
//
//fclose(file);
//
///* End {code} */
//void FIO32() {
//}
//
//
///* FIO33_cpp */
//
///* Begin {code} */
//
//char buf[BUFSIZ];
//
//fgets(buf, sizeof(buf), stdin);
//buf[strlen(buf) - 1] = '\0'; /* Overwrite newline */
//
///* End {code} */
//
///* Begin {code} */
//
//FILE * fptr = fopen(file_name, "w");
///* process file */
//
///* End {code} */
//
///* Begin {code} */
//
//enum { BUFFERSIZE = 200 };
//char buffer[BUFFERSIZE];
//char s[] = "computer";
//char c = 'l';
//int i = 35;
//int j = 0;
//float fp = 1.7320534f;
//
///* Format and print various data: */
//j = snprintf(
//    buffer,
//    sizeof(buffer),
//    " String:    %s\n",
//    s
//    );
//j += snprintf(
//    buffer + j,
//    sizeof(buffer) - j,
//    " Character: %c\n",
//    c
//    );
//j += snprintf(
//    buffer + j,
//    sizeof(buffer) - j,
//    " Integer:   %d\n",
//    i
//    );
//j += snprintf(
//    buffer + j,
//    sizeof(buffer) - j,
//    " Real:      %f\n",
//    fp
//    );
//
///* End {code} */
//void FIO33() {
//}
//
//
///* FIO34_cpp */
//
///* Begin {code} */
//
//char buf[BUFSIZ];
//char c;
//int i = 0;
//
//while ( (c = cin.get()) != '\n' && c != EOF ) {
//  if (i < BUFSIZ-1) {
//    buf[i++] = c;
//  }
//}
//buf[i] = '\0'; /* terminate NTBS */
//
///* End {code} */
//void FIO34() {
//}
//
//
///* FIO35_cpp */
//
///* Begin {code} */
//
//int c;
//
//do {
//  /* ... */
//  c = getchar();
//  /* ... */
//} while (c != EOF);
//
///* End {code} */
//void FIO35() {
//}
//
//
///* FIO36_cpp */
//
///* Begin {code} */
//
//char buf[BUFSIZ + 1];
//
//if (fgets(buf, sizeof(buf), stdin)) {
//  if (*buf) { /* see FIO37-CPP */
//    buf[strlen(buf) - 1] = '\0';
//  }
//}
//else {
//  /* Handle error condition */
//}
//
///* End {code} */
//void FIO36() {
//}
//
//
///* FIO37_cpp */
//
///* Begin {code} */
//
//char buf[BUFSIZ + 1];
//
//if (fgets(buf, sizeof(buf), stdin) == NULL) {
//  /* Handle error */
//}
//buf[strlen(buf) - 1] = '\0';
//
///* End {code} */
//void FIO37() {
//}
//
//
///* FIO38_cpp */
//
///* Begin {code} */
//
//int main(void) {
//  FILE my_stdout = *(stdout);
//  if (fputs("Hello, World!\n", &my_stdout) == EOF) {
//    /* Handle error */
//  }
//  return 0;
//}
//
///* End {code} */
//void FIO38() {
//}
//
//
///* FIO39_cpp */
//
///* Begin {code} */
//char data[BUFFERSIZE];
//char append_data[BUFFERSIZE];
//char *file_name;
//FILE *file;
//
///* Initialize file_name */
//
//file = fopen(file_name, "a+");
//if (file == NULL) {
//  /* Handle error */
//}
//
///* initialize append_data */
//
//if (fwrite(append_data, BUFFERSIZE, 1, file) != BUFFERSIZE) {
//  /* Handle error */
//}
//if (fread(data, BUFFERSIZE, 1, file) != 0) {
//  /* Handle there not being data */
//}
//
//fclose(file);
//
///* End {code} */
//void FIO39() {
//}
//
//
///* FIO40_cpp */
//
///* Begin {code} */
//
//char buf[BUFSIZ];
//FILE *file;
///* Initialize file */
//
//if (fgets(buf, sizeof(buf), file) == NULL) {
//  /* set error flag and continue */
//}
//
///* End {code} */
//void FIO40() {
//}
//
//
///* FIO41_cpp */
//
///* Begin {code} */
//
//char *file_name;
//FILE *fptr;
//
///* Initialize file_name */
//
//int c = getc(fptr = fopen(file_name, "r"));
//if (c == EOF) {
//  /* Handle error */
//}
//
///* End {code} */
//
///* Begin {code} */
//
//char *file_name;
//FILE *fptr = NULL;
//
///* Initialize file_name */
//
//int c = 'a';
//while (c <= 'z') {
//  if (putc(c++, fptr ? fptr :
//        (fptr = fopen(file_name, "w")) == EOF) {
//      /* Handle error */
//      }
//      }
//
//      /* End {code} */
//      void FIO41() {
//      }
//
//
//      /* FIO42_cpp */
//
//      /* Begin {code} */
//
//      FILE* f;
//      const char *editor;
//      char *file_name;
//
//      /* Initialize file_name */
//
//  f = fopen(file_name, "r");
//  if (f == NULL) {
//    /* Handle fopen() error */
//  }
///* ... */
//editor = getenv("EDITOR");
//if (editor == NULL) {
//  /* Handle getenv() error */
//}
//if (system(editor) == -1) {
//  /* Handle error */
//}
//
///* End {code} */
//void FIO42() {
//}
//
//
///* FIO43_cpp */
//
///* Begin {code} */
//
//char file_name[] = /* hard coded string */;
//
//FILE *fp = fopen(file_name, "wb+");
//if (fp == NULL) {
//  /* Handle error */
//}
//
///* End {code} */
//
///* Begin {code} */
//
//char file_name[L_tmpnam];
//FILE* fp;
//
//if (!tmpnam(file_name)) {
//  /* Handle error */
//}
//
///* A TOCTOU race condition exists here */
//
//fp = fopen(file_name, "wb+");
//if (fp == NULL) {
//  /* Handle error */
//}
//
///* End {code} */
//
///* Begin {code} */
//
//char file_name[L_tmpnam];
//int fd;
//
//if (!(tmpnam(file_name))) {
//  /* Handle error */
//}
//
///* A TOCTOU race condition exists here */
//
//fd = open(file_name, O_WRONLY | O_CREAT | O_EXCL | O_TRUNC, 0600);
//if (fd < 0) {
//  /* Handle error */
//}
//
///* End {code} */
//
///* Begin {code} */
//
//char file_name[L_tmpnam_s];
//int fd;
//
//if (tmpnam_s(file_name, L_tmpnam_s) != 0) {
//  /* Handle error */
//}
//
///* A TOCTOU race condition exists here */
//fd = open(file_name, O_WRONLY | O_CREAT | O_EXCL | O_TRUNC, 0600);
//if (fd < 0) {
//  /* Handle error */
//}
//
///* End {code} */
//
///* Begin {code} */
//
//char file_name[] = "tmp-XXXXXX";
//int fd;
//
//if (!mktemp(file_name)) {
//  /* Handle error */
//}
//
///* A TOCTOU race condition exists here */
//
//fd = open(file_name, O_WRONLY | O_CREAT | O_EXCL | O_TRUNC, 0600);
//if (fd < 0) {
//  /* Handle error */
//}
//
///* End {code} */
//
///* Begin {code} */
//
//FILE* fp = tmpfile();
//if (fp == NULL) {
//  /* Handle error */
//}
//
///* End {code} */
//
///* Begin {code} */
//
//if (tmpfile_s(&fp)) {
//  /* Handle error */
//}
//
///* End {code} */
//void FIO43() {
//}
//
//
///* FIO44_cpp */
//
///* Begin {code} */
//
//enum { NO_FILE_POS_VALUES = 3 };
//
//int opener(FILE* file, /* ... */ ) {
//  int rc;
//  fpos_t offset;
//
//  /* ... */
//
//  memset(&offset, 0, sizeof(offset));
//
//  if (file == NULL) { return EINVAL; }
//
//  /* Read in data from file */
//
//  rc = fsetpos(file, &offset);
//  if (rc != 0 ) { return rc; }
//
//  /* ... */
//
//  return 0;
//}
//
///* End {code} */
//void FIO44() {
//}
//
//
///* FIO45_cpp */
//
///* Begin {code} */
//
//char * buffer;
//size_t size;
//
//ifstream infile ("test.txt", ifstream::binary);
//
//// allocate memory for file content
//buffer = new char [size];
//
//// read content of infile
//infile.read (buffer, size);
//
//infile.close();
//
///* End {code} */
//void FIO45() {
//}
