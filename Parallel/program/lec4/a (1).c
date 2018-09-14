/* char buf[1024]; */
int main(void)
{
  char *s, buf[1024];
  int fds[2];
  int i;

  s = "hello world\n";

  for (i=0; i< 1024; i++) buf[i] = '\0';
  pipe(fds);
  close(fds[1]);
  printf("1\n");
  i=read(fds[0], buf, strlen(s));
  printf("2\n");
  printf("3\n");
  printf("fds[0] = %d, fds[1] = %d, %d buf = %s\n", fds[0], fds[1], i, buf);
  return 0;
}
