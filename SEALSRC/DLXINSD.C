#include<io.h>
#include<stdio.h>
#include<string.h>

#define DLX_ID(a,b,c,d)     ((a<<24) | (b<<16) | (c<<8) | d)

#define ID_DLXDATAFILE DLX_ID('0','T','A','D')

int main(int argc, char **argv)
{

  FILE *input;
  FILE *output;
  long  ilen;
  long  olen;
  long  id = ID_DLXDATAFILE;
  char *data;

  if (argc < 3)
  {
    printf("\nUsage: dlxinsd output.dlx input.dat\n");
    return 1;
  }

  output = fopen(argv[1], "ab");
  input = fopen(argv[2], "rb");

  ilen = filelength(fileno(input));

  data = (char*)malloc(ilen);

  if ( !data ) {

    printf("\nError : Not enough memory\n");
    fclose(input);
    fclose(output);
    return 2;

  };

  fread(data, ilen, 1, input);
  fclose(input);
  fwrite(data, ilen, 1, output);
  fwrite(&ilen, sizeof(long), 1, output);
  fwrite(&id, sizeof(long), 1, output);
  fclose(output);

  free(data);
};

