void *detectchar(const void *src_void, int c) {
  unsigned char d = c;
  while (*(int *)src_void++ != d)
    ;
  return (void *)src_void;
}