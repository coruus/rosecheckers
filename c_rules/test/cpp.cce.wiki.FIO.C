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

void FIO00_0() {
  const char *error_msg = "Resource not available to user.";
  int error_type = 3;
  /* ... */
  printf("Error (type %d): %s\n", error_type, error_msg);
}

void FIO00_1() {
  const char *error_msg = "Resource not available to user.";
  int error_type = 3;
  /* ... */
  cout << "Error (type " << error_type << ": " << error_msg << endl;
}

void FIO00() {
  FIO00_0();
  FIO00_1();
}


/* FIO01_cpp */

void FIO01() {
  char *file_name;
  int fd;

  /* initialize file_name */

  fd = open(
      file_name,
      O_WRONLY | O_CREAT | O_EXCL,
      S_IRWXU
      );
    
  if (fd == -1) {
    /* Handle error */
  }

  /* ... */

  if (fchmod(fd, S_IRUSR) == -1) {
    /* Handle error */
  }
}



/* FIO02_cpp */

char FIO02_argv[] = "string";

void FIO02_0() {
  char *realpath_res = NULL;

  /* Verify FIO02_argv is supplied */

  realpath_res = realpath(FIO02_argv, NULL);
  if (realpath_res == NULL) {
    /* Handle error */
  }

  if (!verify_file(realpath_res)) {
    /* Handle error */
  }

  if (fopen(realpath_res, "w") == NULL) {
    /* Handle error */
  }

  /* ... */

  free(realpath_res);
  realpath_res = NULL;
}

void FIO02_1() {
  char *realpath_res = NULL;
  char *canonical_filename = NULL;
  size_t path_size = 0;

  /* Verify FIO02_argv is supplied */

  path_size = (size_t)PATH_MAX;

  if (path_size > 0) {
    canonical_filename = (char *) malloc(path_size);

    if (canonical_filename == NULL) {
      /* Handle error */
    }

    realpath_res = realpath(FIO02_argv, canonical_filename);
  }

  if (realpath_res == NULL) {
    /* Handle error */
  }

  if (!verify_file(realpath_res)) {
    /* Handle error */
  }
  if (fopen(realpath_res, "w") == NULL ) {
    /* Handle error */
  }

  /* ... */

  free(canonical_filename);
  canonical_filename = NULL;
}

void FIO02_2() {
  /* Verify FIO02_argv is supplied */

  char *canonical_filename = canonicalize_file_name(FIO02_argv);
  if (canonical_filename == NULL) {
    /* Handle error */
  }

  /* Verify file name */

  if (fopen(canonical_filename, "w") == NULL) {
    /* Handle error */
  }

  /* ... */

  free(canonical_filename);
  canonical_filename = NULL;
}

/* End {code} */
void FIO02() {
  FIO02_0();
  FIO02_1();
  FIO02_2();
}


///* FIO03_cpp */
//
///* Begin {code} */
//
//char *file_name;
//int new_file_mode;
//
///* initialize file_name and new_file_mode */
//
//int fd = open(file_name, O_CREAT | O_EXCL | O_WRONLY, new_file_mode);
//if (fd == -1) {
//  /* Handle error */
//}
//
///* End {code} */
//
///* Begin {code} */
//
//char *file_name;
//
///* initialize file_name */
//
//FILE *fp = fopen(file_name, "wx");
//if (!fp) {
//  /* Handle error */
//}
//
///* End {code} */
//
///* Begin {code} */
//
//char *file_name;
//int new_file_mode;
//FILE *fp;
//int fd;
//
///* initialize file_name and new_file_mode */
//
//fd = open(file_name, O_CREAT | O_EXCL | O_WRONLY, new_file_mode);
//if (fd == -1) {
//  /* Handle error */
//}
//
//fp = fdopen(fd, "w");
//if (fp == NULL) {
//  /* Handle error */
//}
//
///* End {code} */
//void FIO03() {
//}
//
//
///* FIO04_cpp */
//void FIO04() {
//}
//
//
///* FIO05_cpp */
//
///* Begin {code} */
//
//struct stat orig_st;
//struct stat new_st;
//char *file_name;
//
///* initialize file_name */
//
//int fd = open(file_name, O_WRONLY);
//if (fd == -1) {
//  /* Handle error */
//}
//
///*... write to file ...*/
//
//if (fstat(fd, &orig_st) == -1) {
//  /* Handle error */
//}
//close(fd);
//fd = -1;
//
///* ... */
//
//fd = open(file_name, O_RDONLY);
//if (fd == -1) {
//  /* Handle error */
//}
//
//if (fstat(fd, &new_st) == -1) {
//  /* Handle error */
//}
//
//if ((orig_st.st_dev != new_st.st_dev) ||
//    (orig_st.st_ino != new_st.st_ino)) {
//  /* file was tampered with! */
//}
//
///*... read from file ...*/
//
//close(fd);
//fd = -1;
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
///*... write to file ...*/
//
///* go to beginning of file */
//fseek(fd, 0, SEEK_SET);
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
//struct stat st;
//char *file_name;
//
///* initialize file_name */
//
//int fd = open(file_name, O_RDONLY);
//if (fd == -1) {
//  /* Handle error */
//}
//
//if ((fstat(fd, &st) == -1) ||
//    (st.st_uid != getuid()) ||
//    (st.st_gid != getgid())) {
//  /* file does not belong to user */
//}
//
///*... read from file ...*/
//
//close(fd);
//fd = -1;
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
//errno_t res = fopen_s(&fp, file_name, "w");
//if (res != 0) {
//  /* Handle error */
//}
//
///* End {code} */
//
///* Begin {code} */
//
//char *file_name;
//int file_access_permissions;
//
///* initialize file_name and file_access_permissions */
//
//int fd = open(
//    file_name,
//    O_CREAT | O_WRONLY,
//    file_access_permissions
//    );
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
//if (fseek(fp, 0L, SEEK_SET) != 0) {
//  /* Handle repositioning error */
//}
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
//FILE *file;
//char *file_name;
//
///* initialize file_name */
//
//file = fopen(file_name, "w+");
//if (file == NULL) {
//  /* Handle error condition */
//}
//
//if (unlink(file_name) != 0) {
//  /* Handle error condition */
//}
//
///*... continue performing I/O operations on file ...*/
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
//char buf[25];
//char *end_ptr;
//
///* initialize file */
//
//if (fgets(buf, 1, file) == NULL) {
//  /* Handle error */
//}
//
//data.c = buf[0];
//
//if (fgets(buf, sizeof(buf), file) == NULL) {
//  /* Handle Error */
//}
//
//data.l = strtol(buf, &end_ptr, 10);
//
//  if ((ERANGE == errno)
//      || (end_ptr == buf)
//      || ('\n' != *end_ptr && '\0' != *end_ptr)) {
//    /* Handle Error */
//  }
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
//
//if (access(dest_file, F_OK) != 0) {
//  if (rename(src_file, dest_file) != 0) {
//    /* Handle error condition */
//  }
//} 
//else {
//  /* Handle file-exists condition */
//}
//
///* End {code} */
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
//
//if (_access_s(dest_file, 0) == 0) {
//  if (remove(dest_file) != 0) {
//    /* Handle error condition */
//  }
//}
//
//if (rename(src_file, dest_file) != 0) {
//  /* Handle error condition */
//}
//
///* End {code} */
//
///* Begin {code} */
//
//const char *src_file = /* ... */;
//const char *dest_file = /* ... */;
//if (rename(src_file, dest_file) != 0) {
//  /* Handle error condition */
//}
//
///* End {code} */
//
///* Begin {code} */
//
//const char *src_file = /* ... */;
//const char *dest_file = /* ... */;
//
//(void)remove(dest_file);
//
//if (rename(src_file, dest_file) != 0) {
//  /* Handle error condition */
//}
//
///* End {code} */
//
///* Begin {code} */
//
//const char *src_file = /* ... */;
//const char *dest_file = /* ... */;
//
//if (!file_exists(dest_file)) {
//  if (rename(src_file, dest_file) != 0) {
//    /* Handle error condition */
//  }
//} 
//else {
//  /* Handle file-exists condition */
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
//char *buf = NULL;
///* Setup file */
//if (setvbuf(file, buf, buf ? _IOFBF : _IONBF, BUFSIZ) != 0) {
//  /* Handle error */
//}
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
//fpos_t pos;
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
//if (fgetpos(fp, &pos)) {
//  /* Handle error */
//}
//
///* read the data that will be "pushed back" */
//
//if (fsetpos(fp, &pos)) {
//  /* Handle error */
//}
//
///* Continue on */
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
//#include <stdlib.h>
//#include <unistd.h>
//#include <limits.h>
//#include <libgen.h>
//#include <sys/stat.h>
//#include <string.h>
//
///* Returns nonzero if directory is secure, zero otherwise */
//int secure_dir(const char *fullpath) {
//  char *path_copy = NULL;
//  char *dirname_res = NULL;
//  char ** dirs = NULL;
//  int num_of_dirs = 0;
//  int secure = 1;
//  int i;
//  struct stat buf;
//  uid_t my_uid = geteuid();
//
//  if (!(path_copy = strdup(fullpath))) {
//    /* Handle error */
//  }
//
//  dirname_res = path_copy;
//  /* Figure out how far it is to the root */
//  while (1) {
//    dirname_res = dirname(dirname_res);
//
//    num_of_dirs++;
//
//    if ((strcmp(dirname_res, "/") == 0) ||
//        (strcmp(dirname_res, "//") == 0)) {
//      break;
//    }
//  }
//  free(path_copy);
//  path_copy = NULL;
//
//  /* Now allocate and fill the dirs array */
//  if (!(dirs = (char **)malloc(num_of_dirs*sizeof(*dirs)))) {
//    /* Handle error */
//  }
//  if (!(dirs[num_of_dirs - 1] = strdup(fullpath))) {
//    /* Handle error */
//  }
//
//  if (!(path_copy = strdup(fullpath))) {
//    /* Handle error */
//  }
//
//  dirname_res = path_copy;
//  for (i = 1; i < num_of_dirs; i++) {
//    dirname_res = dirname(dirname_res);
//
//    dirs[num_of_dirs - i - 1] = strdup(dirname_res);
//
//  }
//  free(path_copy);
//  path_copy = NULL;
//
//  /* Traverse from the root to the leaf, checking
//   * permissions along the way */
//  for (i = 0; i < num_of_dirs; i++) {
//    if (stat(dirs[i], &buf) != 0) {
//      /* Handle error */
//    }
//    if ((buf.st_uid != my_uid) && (buf.st_uid != 0)) {
//      /* Directory is owned by someone besides user or root */
//      secure = 0;
//    } else if ((buf.st_mode & (S_IWGRP | S_IWOTH))
//        && ((i == num_of_dirs - 1) || !(buf.st_mode & S_ISVTX))) {
//      /* Others have permissions to the leaf directory
//       * or are able to delete or rename files along the way */
//      secure = 0;
//    }
//
//    free(dirs[i]);
//    dirs[i] = NULL;
//  }
//
//  free(dirs);
//  dirs = NULL;
//
//  return secure;
//}
//
///* End {code} */
//
///* Begin {code} */
//
//char *dir_name;
//char *canonical_dir_name;
//const char *file_name = "passwd"; /* file name within the secure directory */
//FILE *fp;
//
///* initialize dir_name */
//
//canonical_dir_name = realpath(dir_name, NULL);
//if (canonical_dir_name == NULL) {
//  /* Handle error */
//}
//
//if (!secure_dir(canonical_dir_name)) {
//  /* Handle error */
//}
//
//if (chdir(canonical_dir_name) == -1) {
//  /* Handle error */
//}
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
///*
// * Make sure that the chroot/jail directory exists within 
// * the current working directory. Also assign appropriate 
// * permissions to the directory to restrict access. Close 
// * all file system descriptors to outside resources lest 
// * they escape the jail.
// */
//
//if (setuid(0) == -1) {
//  /* Handle error */
//}
//
//if (chroot("chroot/jail") == -1) {
//  /* Handle error */
//}
//
//if (chdir("/") == -1) {
//  /* Handle error */
//}
//
///* Drop privileges permanently */
//if (setgid(getgid()) == -1) {
//  /* Handle error */
//}
//
//if (setuid(getuid()) == -1) {
//  /* Handle error */
//}
//
///* Perform unprivileged operations */
//enum {array_mex = 100};
//
//FILE *fp = fopen(argv[1], "w");
//char x[array_max];
//strncpy(x, argv[2], array_max);
//x[array_max - 1] = '\0';
//
///* Write operation safe is safe within jail */
//if (fwrite(x, sizeof(x[0]), sizeof(x)/sizeof(x[0]), fp) <
//    sizeof(x)/sizeof(x[0])) {
//  /* Handle error */
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
//#include <iostream>
//#include <fstream>
//#include <string>
//using namespace std;
//
//int main(int argc, char *argv[]) {
//  string filename;
//  ifstream ifs;
//
//  cin >> filename;
//  ifs.open(filename.c_str());
//
//  if (ifs.fail()) {
//    cerr << "Error opening file << filename << endl;
//    exit(-1);
//  }
//  ifs.close();
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
//int main () {
//  char * buffer;
//  size_t size, length;&#xA0;
//
//  ofstream outfile ("new.txt", ofstream::binary);
//  // suppose buffer is populated with a c-string
//  size = sizeof(buffer);
//  outfile.write (buffer, length); // suppose length is already initialized.&#xA0;
//  if (size != length) {// File is not properly null-terminated. Handle this }
//
//  delete[] buffer;
//  outfile.close();
//  return 0;
//}
//
///* End {code} */
//void FIO18() {
//}
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
//  if (fputs(msg, stderr) == EOF) {
//    /* Handle error */
//  }
//  delete[] msg;
//  msg = NULL;
//}
//
///* End {code} */
//
///* Begin {code} */
//
//void incorrect_password(const char *user) {
//  fprintf(stderr, "%s cannot be authenticated.\n", user);
//}
//
///* End {code} */
//
///* Begin {code} */
//
//void incorrect_password(const char *user) {
//  cerr << user << " cannot be authenticated." << endl;
//}
//
///* End {code} */
//
///* Begin {code} */
//
//void incorrect_password(const char *user) {
//  syslog(LOG_INFO, "%s cannot be authenticated.", user);
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
//void do_stuff(ostream& logfile) {
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
//  do_stuff(logfile);
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
//#ifdef O_NOFOLLOW
//#define OPEN_FLAGS O_NOFOLLOW | O_NONBLOCK
//#else
//#define OPEN_FLAGS O_NONBLOCK
//#endif
//
///* ... */
//
//struct stat orig_st;
//struct stat open_st;
//int fd;
//int flags;
//char *file_name;
//
///* initialize file_name */
//
//if (!fgets(file_name, sizeof(file_name), stdin)) {
//  /* Handle error */
//}
//
//  if ((lstat(file_name, &orig_st) != 0)
//      || (!S_ISREG(orig_st.st_mode)))
//{
//  /* Handle error */
//}
//
///* A TOCTOU race condition exists here, see below */
//
//fd = open(file_name, OPEN_FLAGS | O_WRONLY);
//if (fd == -1) {
//  /* Handle error */
//}
//
//if (fstat(fd, &open_st) != 0) {
//  /* Handle error */
//}
//
//if ((orig_st.st_mode != open_st.st_mode) ||
//    (orig_st.st_ino  != open_st.st_ino) ||
//    (orig_st.st_dev  != open_st.st_dev)) {
//  /* file was tampered with */
//}
//
///* Optional: drop the O_NONBLOCK now that we are sure
// * this is a good file */
//if ((flags = fcntl(fd, F_GETFL)) == -1) {
//  /* Handle error */
//}
//
//if (fcntl(fd, F_SETFL, flags & ~O_NONBLOCK) != 0) {
//  /* Handle error */
//}
//
///* Operate on file */
//
//close(fd);
//
///* End {code} */
//
///* Begin {code} */
//
//HANDLE hFile = CreateFile(
//    pFullPathName, 0, 0, NULL, OPEN_EXISTING, 0, NULL
//    );
//if (hFile == INVALID_HANDLE_VALUE) {
//  /* Handle error */
//}
//else {
//  if (GetFileType(hFile) != FILE_TYPE_DISK) {
//    /* Handle error */
//  }
//  /* operate on file */
//}
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
//char *p;
//
//if (fgets(buf, sizeof(buf), stdin)) {
//  /* fgets succeeds, scan for newline character */
//  p = strchr(buf, '\n');
//  if (p) {
//    *p = '\0';
//  }
//  else {
//    /* newline not found, flush stdin to end of line */
//    while ((getchar() != '\n')
//        && !feof(stdin)
//        && !ferror(stdin)
//        );
//  }
//}
//else {
//  /* fgets failed, handle error */
//}
//
///* End {code} */
//
///* Begin {code} */
//
//FILE * fptr = fopen(file_name, "w");
//if (fptr == NULL) {
//  /* Handle error */
//}
//else {
//  /* process file */
//
//  /* End {code} */
//
//  /* Begin {code} */
//
//  enum { BUFFERSIZE = 200 };
//  char buffer[BUFFERSIZE];
//  char s[] = "computer";
//  char c = 'l';
//  int i = 35;
//  int j = 0;
//  int rc = 0;
//  float fp = 1.7320534f;
//
//  /* Format and print various data: */
//  rc = snprintf(
//      buffer,
//      sizeof(buffer),
//      " String: %s\n",
//      s
//      );
//  if (rc == -1 || rc >= sizeof(buffer)) /* Handle error */ ;
//  else j += rc;
//
//  rc = snprintf(
//      buffer + j,
//      sizeof(buffer) - j,
//      " Character: %c\n",
//      c
//      );
//  if (rc == -1 || rc >= sizeof(buffer) - j) /* Handle error */ ;
//  else j += rc;
//
//  rc = snprintf(
//      buffer + j,
//      sizeof(buffer) - j,
//      " Integer: %d\n",
//      i
//      );
//  if (rc == -1 || rc >= sizeof(buffer) - j) /* Handle error */ ;
//  else j += rc;
//
//  rc = snprintf(
//      buffer + j,
//      sizeof(buffer) - j,
//      " Real: %f\n",
//      fp
//      );
//  if (rc == -1 || rc >= sizeof(buffer) - j) /* Handle error */ ;
//
//  /* End {code} */
//  void FIO33() {
//  }
//
//
//  /* FIO34_cpp */
//
//  /* Begin {code} */
//
//  char buf[BUFSIZ];
//  int c;
//  int i = 0;
//
//  while (((c = cin.get()) != '\n')
//      && !cin.eof()
//      && !cin)
//  {
//    if (i < BUFSIZ-1) {
//      buf[i++] = c;
//    }
//  }
//  buf[i] = '\0'; /* terminate NTBS */
//
//  /* End {code} */
//  void FIO34() {
//  }
//
//
//  /* FIO35_cpp */
//
//  /* Begin {code} */
//
//  int c;
//
//  do {
//    /* ... */
//    c = getchar();
//    /* ... */
//  } while (!feof(stdin) && !ferror(stdin));
//
//  /* End {code} */
//
//  /* Begin {code} */
//
//  int c;
//
//  static_assert(sizeof(char) < sizeof(int));
//  do {
//    /* ... */
//    c = getchar();
//    /* ... */
//  } while (c != EOF);
//
//  /* End {code} */
//  void FIO35() {
//  }
//
//
//  /* FIO36_cpp */
//
//  /* Begin {code} */
//
//  char buf[BUFSIZ + 1];
//  char *p;
//
//  if (fgets(buf, sizeof(buf), stdin)) {
//    p = strchr(buf, '\n');
//    if (p) {
//      *p = '\0';
//    }
//  }
//  else {
//    /* Handle error condition */
//  }
//
//  /* End {code} */
//  void FIO36() {
//  }
//
//
//  /* FIO37_cpp */
//
//  /* Begin {code} */
//
//  char buf[BUFSIZ + 1];
//  char *p;
//
//  if (fgets(buf, sizeof(buf), stdin)) {
//    p = strchr(buf, '\n');
//    if (p) {
//      *p = '\0';
//    }
//  }
//  else {
//    /* Handle error condition */
//  }
//
//  /* End {code} */
//  void FIO37() {
//  }
//
//
//  /* FIO38_cpp */
//
//  /* Begin {code} */
//
//  int main(void) {
//    FILE *my_stdout = stdout;
//    if (fputs("Hello, World!\n", my_stdout) == EOF) {
//      /* Handle error */
//    }
//    return 0;
//  }
//
//  /* End {code} */
//  void FIO38() {
//  }
//
//
//  /* FIO39_cpp */
//
//  /* Begin {code} */
//  char data[BUFFERSIZE];
//  char append_data[BUFFERSIZE];
//  char *file_name;
//  FILE *file;
//
//  /* initialize file_name */
//
//  file = fopen(file_name, "a+");
//  if (file == NULL) {
//    /* Handle error */
//  }
//
//  /* Initialize append_data */
//
//  if (fwrite(append_data, BUFFERSIZE, 1, file) != BUFFERSIZE) {
//    /* Handle error */
//  }
//
//  if (fseek(file, 0L, SEEK_SET) != 0) {
//    /* Handle error */
//  }
//
//  if (fread(data, BUFFERSIZE, 1, file) != 0) {
//    /* Handle there not being data */
//  }
//
//  fclose(file);
//
//  /* End {code} */
//  void FIO39() {
//  }
//
//
//  /* FIO40_cpp */
//
//  /* Begin {code} */
//
//  char buf[BUFSIZ];
//  FILE *file;
//  /* Initialize file */
//
//  if (fgets(buf, sizeof(buf), file) == NULL) {
//    /* set error flag and continue */
//    *buf = '\0';
//  }
//
//  /* End {code} */
//  void FIO40() {
//  }
//
//
//  /* FIO41_cpp */
//
//  /* Begin {code} */
//
//  int c;
//  char *file_name;
//  FILE *fptr;
//
//  /* Initialize file_name */
//
//  fptr = fopen(file_name, "r");
//  if (fptr == NULL) {
//    /* Handle error */
//  }
//
//  c = getc(fptr);
//  if (c == EOF) {
//    /* Handle error */
//  }
//
//  /* End {code} */
//
//  /* Begin {code} */
//
//  char *file_name;
//
//  /* Initialize file_name */
//
//  FILE *fptr = fopen(file_name, "w");
//  if (fptr == NULL) {
//    /* Handle error */
//  }
//
//  int c = 'a';
//
//  while (c <= 'z') {
//    if (putc(c++, fptr) == EOF) {
//      /* Handle error */
//    }
//  }
//
//  /* End {code} */
//  void FIO41() {
//  }
//
//
//  /* FIO42_cpp */
//
//  /* Begin {code} */
//
//  FILE* f;
//  const char *editor;
//  char *file_name;
//
//  /* Initialize file_name */
//
//  f = fopen(file_name, "r");
//  if (f == NULL) {
//    /* Handle fopen() error */
//  }
//  /* ... */
//  fclose(f);
//  f = NULL;
//  editor = getenv("EDITOR");
//  if (editor == NULL) {
//    /* Handle getenv() error */
//  }
//  /* Sanitize environment before calling system()! */
//  if (system(editor) == -1) {
//    /* Handle Error */
//  }
//
//  /* End {code} */
//
//  /* Begin {code} */
//
//  int flags;
//  char *editor;
//  char *file_name;
//
//  /* Initialize file_name */
//
//  int fd = open(file_name, O_RDONLY);
//  if (fd == -1) {
//    /* Handle error */
//  }
//
//  flags = fcntl(fd, F_GETFD);
//  if (flags == -1) {
//    /* Handle error */
//  }
//
//  if (fcntl(fd, F_SETFD, flags | FD_CLOEXEC) == -1) {
//    /* Handle error */
//  }
//
//  /* ... */
//
//  editor = getenv("EDITOR");
//  if (editor == NULL) {
//    /* Handle getenv() error */
//  }
//  if (system(editor) == -1) {
//    /* Handle error */
//  }
//
//  /* End {code} */
//
//  /* Begin {code} */
//
//  char *editor;
//  char *file_name;
//
//  /* Initialize file_name */
//
//  int fd = open(file_name, O_RDONLY | O_CLOEXEC);
//  if (fd == -1) {
//    /* Handle error */
//  }
//
//  /* ... */
//
//  editor = getenv("EDITOR");
//  if (editor == NULL) {
//    /* Handle getenv() error */
//  }
//  if (system(editor) == -1) {
//    /* Handle error */
//  }
//
//  /* End {code} */
//  void FIO42() {
//  }
//
//
//  /* FIO43_cpp */
//
//  /* Begin {code} */
//
//  const char *sdn = "/home/usr1/";
//  char sfn[] = "/home/usr1/temp-XXXXXX";
//  FILE *sfp;
//
//  if (!secure_dir(sdn)) {
//    /* Handle error */
//  }
//
//  int fd = mkstemp(sfn);
//  if (fd == -1) {
//    /* Handle error */
//  }
//
//  /*
//   * Unlink immediately to hide the file name.
//   * The race condition here is inconsequential if the file
//   * is created with exclusive permissions (glibc >= 2.0.7)
//   */
//
//  if (unlink(sfn) == -1) {
//    /* Handle error */
//  }
//
//  sfp = fdopen(fd, "w+");
//  if (sfp == NULL) {
//    close(fd);
//    /* Handle error */
//  }
//
//  /* use temporary file */
//
//  fclose(sfp); /* also closes fd */
//
//  /* End {code} */
//  void FIO43() {
//  }
//
//
//  /* FIO44_cpp */
//
//  /* Begin {code} */
//
//  enum { NO_FILE_POS_VALUES = 3 };
//
//  int opener(FILE* file, /* ... */) {
//    int rc;
//    fpos_t offset;
//
//    /* ... */
//
//    if (file == NULL) { return EINVAL; }
//
//    rc = fgetpos(file, &offset);
//    if (rc != 0 ) { return rc; }
//
//    /* Read in data from file */
//
//    rc = fsetpos(file, &offset);
//    if (rc != 0 ) { return rc; }
//
//    /* ... */
//
//    return 0;
//  }
//
//  /* End {code} */
//  void FIO44() {
//  }
//
//
//  /* FIO45_cpp */
//
//  /* Begin {code} */
//
//  char * buffer;
//  size_t size;
//
//  ifstream infile ("test.txt", ifstream::binary);
//
//  // get length of the file
//  infile.seekg(0, ifstream::end);
//  length = infile.tellg();
//  infile.seekg(0);
//
//  // allocate memory for the content of the file
//  buffer = new char [size];
//  infile.read (buffer, size);&#xA0;
//  if (size != length) {
//    // buffer is not properly null-terminated. Handle this case
//  }
//
//  infile.close();
//
//  /* End {code} */
//  void FIO45() {
//  }
