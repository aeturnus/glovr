#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
void parse(const uint8_t * buffer)
{
  int32_t pos[3];
  int32_t orr[3];
  int8_t fing[6];
  int index = 0;
  for(int i = 0; i < 3; i++)
  {
    pos[i] = *(int32_t *)(&buffer[index]);
    index += sizeof(int32_t);
  }
  for(int i = 0; i < 3; i++)
  {
    orr[i] = *(int32_t *)(&buffer[index]);
    index += sizeof(int32_t);
  }
  for(int i = 0; i < 6; i++)
  {
    fing[i] = *(int8_t *)(&buffer[index]);
    index += sizeof(int8_t);
  }
  printf("x: %d\ny: %d\nz: %d\nyaw:   %d\npitch: %d\nroll:  %d\nthumb: %d\nindex: %d\nmiddle: %d\nring: %d\npinky: %d\nthumb_add: %d\n\n",
         pos[0],pos[1],pos[2],orr[0],orr[1],orr[2],fing[0],fing[1],fing[2],fing[3],fing[4],fing[5]);
}

/*
const char * usbPath = "/dev/ttyUSB0";
int main()
{
  system("stty -F /dev/ttyUSB0 115200 cs8 -cstopb -parity -icanon min 100 time 1");

  FILE * fp;
  fp = fopen(usbPath,"rw");

  uint8_t buffer[256];
  while(1)
  {
    int8_t c = (int8_t)fgetc(stdin);
    //fwrite(buffer,1,16,fp);
    fread(buffer, 1, sizeof(buffer), fp);
  }

  fclose(fp);
}
*/

/*
const char * usbPath = "/dev/ttyUSB0";
int main()
{
  FILE * fp;
  fp = fopen(usbPath,"rw");
  if(!fp)
  {
    fprintf(stderr,"Could not open %s\n",usbPath);
    return -1;
  }
  setvbuf(fp, 0x0,_IONBF, 0);

  char buffer[30];
  while(1)
  {
    int c = fgetc(stdin);
    //fputc(c,fp);
    fputs("POLL",fp);
    fflush(fp);
    fgets(buffer, sizeof(buffer), fp);
    parse(buffer);
  }

  fclose(fp);
}
*/
/*
const char * usbPath = "/dev/ttyUSB0";
int main()
{
  int fd;
  fd = open(usbPath,O_RDWR);
  if(fd<0)
  {
    fprintf(stderr,"Could not open %s\n",usbPath);
    return -1;
  }
  //setvbuf(fp, 0x0,_IONBF, 0);

  char buffer[30];
  while(1)
  {
    int c = fgetc(stdin);
    write(fd,"POLL",4);
    size_t size = read(fd, buffer, sizeof(buffer));
    parse(buffer);
  }

  close(fd);
}
*/

/*
const char * usbPath = "/dev/ttyUSB0";
int main()
{
  system("stty -F /dev/ttyUSB0 115200 sane");
  //system("stty -F /dev/ttyUSB0 115200 cs8 -cstopb -parity -icanon min 100 time 1");
  //system("stty -F /dev/ttyUSB0 115200 cs8 -icanon");
  //system("stty -F /dev/ttyUSB0 115200 cs8 -icanon");
  int fd;
  fd = open(usbPath,O_RDWR);
  if(fd<0)
  {
    fprintf(stderr,"Could not open %s\n",usbPath);
    return -1;
  }
  //setvbuf(fp, 0x0,_IONBF, 0);

  char buffer[30];
  while(1)
  {
    //int c = fgetc(stdin);
    size_t size = read(fd, buffer, sizeof(buffer));
    printf("Size read:%d\n",size);
    parse(buffer);
  }

  close(fd);
}
*/
const char * usbPath = "/dev/ttyUSB0";
int main()
{
  system("stty -F /dev/ttyUSB0 115200 sane");
  //system("stty -F /dev/ttyUSB0 115200 cs8 -parity -icanon min 100 time 1");
  //system("stty -F /dev/ttyUSB0 115200 cs8 -parity -icanon min 100 time 1");
  system("stty -F /dev/ttyUSB0 115200 raw");
  FILE * fp;
  fp = fopen(usbPath,"rwb");
  if(!fp)
  {
    fprintf(stderr,"Could not open %s\n",usbPath);
    return -1;
  }
  setvbuf(fp, 0x0,_IONBF, 0);

  char buffer[30];
  while(1)
  {
    fflush(fp);
    char c = fgetc(stdin);
    c = 10;
    fwrite(&c,1,1,fp);
    size_t read = fread(buffer, 1, sizeof(buffer), fp);
    parse(buffer);
  }

  fclose(fp);
}
