void set_flag(int number, int *sign_flag) {
  if (number > 0) {
    *sign_flag = 1;
  }
  else if (number < 0) {
    *sign_flag = -1;
  }
}

void func(int number) {
  int sign;

  set_flag(number,&sign);
  /* ... */
}

