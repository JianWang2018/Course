char buf[1024];
int main() {
  char *s;
  int fds[2];
  int size;

  s = "hello world\n";
  pipe(fds);
  if (fork() == 0) {
    write(fds[1], s, 12);
    close(fds[1]);
    size = read(fds[0], buf, 12);
    printf("child process, size = %d, buf = %s\n", size, buf);
    exit(0);
  }
  close(fds[1]);
  size = read(fds[0], buf, 12);
  printf("parent process, buf = %s, size = %d\n", buf, size);
}
