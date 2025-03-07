#include "../inch.h"

// setbuf == setvbuf(stream, buf ? fullbuf : nobuff, size)
int m_setbuf(FILE* restrict stream, char* restrict buf, int mode, size_t size){
  return setvbuf(stream, buf, buf ? _IOFBF : _IONBF, size);
}

int main(int argc, char *argv[])
{
  return EXIT_SUCCESS;
}
