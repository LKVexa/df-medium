/* Compile three times against the actual administrative, production and verifier readers. */
#define main original_cli_main
#if defined(TEST_PRODUCTION)
#include "../vm/BOTTLE_ROCKET_5.0.0_VM_110K_RC/host/brctl.c"
#define reader lf
#elif defined(TEST_VERIFIER)
#include "../vm/BOTTLE_ROCKET_5.0.0_VM_110K_RC/host/brverify.c"
#define reader load
#else
#include "../vm/BOTTLE_ROCKET_5.0.0_VM_110K_RC/host/bradmin.c"
#define reader load_file
#endif
#undef main
#include <assert.h>
#include <dirent.h>

static int descriptors(void){DIR*d=opendir("/proc/self/fd");int n=0;assert(d);while(readdir(d))n++;assert(!closedir(d));return n;}
static void small_file(const char*p){FILE*f=fopen(p,"wb");assert(f);assert(fwrite("abc",1,3,f)==3);assert(!fclose(f));}
int main(void){
 char directory[]="/tmp/df-medium-host-XXXXXX",input[256],linkpath[256],fifo[256],priv[256],pub[256];
 uint8_t *data=NULL;size_t n=0;int fd,before;struct stat st;
 assert(mkdtemp(directory));
 assert(snprintf(input,sizeof input,"%s/input",directory)>0);
 assert(snprintf(linkpath,sizeof linkpath,"%s/link",directory)>0);
 assert(snprintf(fifo,sizeof fifo,"%s/fifo",directory)>0);
 assert(snprintf(priv,sizeof priv,"%s/private",directory)>0);
 assert(snprintf(pub,sizeof pub,"%s/public",directory)>0);
 small_file(input);assert(!reader(input,&data,&n)&&n==3);free(data);
 assert(!symlink(input,linkpath));assert(!mkfifo(fifo,0600));
 before=descriptors();
 for(int i=0;i<100;i++){
  assert(reader(linkpath,&data,&n)==-1&&data==NULL&&n==0);
  assert(reader(fifo,&data,&n)==-1&&data==NULL&&n==0);
  assert(reader(directory,&data,&n)==-1&&data==NULL&&n==0);
 }
 assert(descriptors()==before);
 fd=open(input,O_WRONLY|O_TRUNC);assert(fd>=0);assert(!ftruncate(fd,60000));assert(!close(fd));
 assert(reader(input,&data,&n)==-1&&data==NULL&&n==0);
#if !defined(TEST_PRODUCTION) && !defined(TEST_VERIFIER)
 small_file(priv);assert(keygen(priv,pub)!=0);
 assert(!reader(priv,&data,&n)&&n==3&&!memcmp(data,"abc",3));free(data);
 assert(access(pub,F_OK)!=0);assert(!unlink(priv));
 assert(!symlink(input,priv));assert(keygen(priv,pub)!=0);assert(!lstat(priv,&st)&&S_ISLNK(st.st_mode));assert(!unlink(priv));
 assert(!keygen(priv,pub));assert(!stat(priv,&st)&&(st.st_mode&0777)==0600);
 assert(!stat(pub,&st)&&st.st_size==32);assert(!unlink(priv));assert(!unlink(pub));
 small_file(pub);assert(keygen(priv,pub)!=0);
 assert(!reader(pub,&data,&n)&&n==3&&!memcmp(data,"abc",3));free(data);
 assert(!stat(priv,&st)&&(st.st_mode&0777)==0600);assert(!unlink(priv));assert(!unlink(pub));
 {br_insn insn={0};insn.op=BR_HALT;assert(br_host_image_write("/dev/full",&insn,1,NULL,0,1,BR_CAP_CONTROL)==K4);}
#else
 (void)st;
#endif
 assert(!unlink(input)&&!unlink(linkpath)&&!unlink(fifo)&&!rmdir(directory));
 puts("HOST_IO_REGRESSIONS_PASS");return 0;
}
