#include <iostream>
#include <fstream>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

using namespace std;

unsigned int kljuc[4]={0xEF58,0x73A30,0x1FEBE,0x77C80};
char red[1000];
char txt[10000];

#define BLOCK_SIZE 8

void generate_shellcode(char argv[], char arg[])
{
    FILE *f;
    int i, j, c, size_tb;
    char *arr_name;

    f = fopen(argv, "rb");
    if (f == NULL)
    {
        fprintf(stderr, "ERROR : Failed to open the file");
    }

    // calculate the size of the tb
    for (size_tb=0;;size_tb++)
        if ((c = fgetc(f)) == EOF) break;
    if (size_tb % 8 != 0)
        size_tb = (size_tb / 8) + 1;
    else
        size_tb = (size_tb / 8);

    fclose(f);
    f = fopen(argv, "rb");
    if (f == NULL)
    {
        fprintf(stderr, "ERROR : Failed to open the file");
    }

    freopen (arg,"w",stdout);
    arr_name = "at7SnZjX";

    printf("unsigned char %s[%d][8];\n", arr_name, size_tb);
    printf("void assignment(){\n");
    for (j=0;j < size_tb;j++)
    {
        for (i=0;i < 8;i++)
        {
            if ((c = fgetc(f)) == EOF) break;
            printf("%s[%d][%d] = ", arr_name, j, i);
            printf("0x%.2X;\n", (unsigned char)c);
        }
    }
    printf("}\n");
	printf("unsigned int size = %d;\n", size_tb);
    fclose(f);
	fclose(stdout);
}

void encipher(unsigned int num_rounds, uint32_t v[2], uint32_t const kljuc[4]){
    unsigned int i;
    uint32_t v0=v[0], v1=v[1], sum=0, delta=0x9E3779B9;
    for (i=0; i < num_rounds; i++){
        v0 += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + kljuc[sum & 3]);
        sum += delta;
        v1 += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + kljuc[(sum>>11) & 3]);
    }
    v[0]=v0; v[1]=v1;
}

void decipher(unsigned int num_rounds, uint32_t v[2], uint32_t const kljuc[4]){
     unsigned int i;
     uint32_t v0=v[0], v1=v[1], delta=0x9E3779B9, sum=delta*num_rounds;
     for (i=0; i < num_rounds; i++){
        v1 -= (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + kljuc[(sum>>11) & 3]);
        sum -= delta;
        v0 -= (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + kljuc[sum & 3]);
     }
     v[0]=v0; v[1]=v1;
}

void crypto(char nazivDat[] ,bool naredba){
      fstream dat(nazivDat,ios::in | ios::out | ios::binary);
      if(!dat) cout << "no datas" << endl;

      unsigned size;

      dat.seekg(0,ios::end);
      size=dat.tellg();
      dat.seekg(ios::beg);

      dat.clear();

      unsigned pos;

      int n_blocks=size/BLOCK_SIZE;
      if(size%BLOCK_SIZE!=0)
          ++n_blocks;

      for(int i=0;i<n_blocks;i++){
          unsigned char data[BLOCK_SIZE];
          pos=dat.tellg();

          dat.read((char*)data,BLOCK_SIZE);

          if(naredba) encipher(32,(uint32_t*)data,kljuc);
          else decipher(32,(uint32_t*)data,kljuc);

          dat.seekp(pos);
          dat.write((char*)data,BLOCK_SIZE);

          memset(data,0,BLOCK_SIZE);
      }
      dat.close();
}

int main(int argc, char *argv[]){
    if (argc < 3) {
        fprintf(stderr, "Error : Please set an exe to encrypt");
        return 1;
    }
    crypto(argv[1], true);
    generate_shellcode(argv[1], argv[2]);
    system("pause");
    return (0);
}
