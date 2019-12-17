void *detectchar(const void *src_void, int c) {
  const unsigned char d = c, *src = (const unsigned char *)src_void;
  for (; *src != d; src++)
    ;
  return (void *)src;
}