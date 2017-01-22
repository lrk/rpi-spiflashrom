#include <iostream>
#include <iomanip>
#include <errno.h>
#include <wiringPiSPI.h>
#include <unistd.h>

using namespace std;

static const int CHANNEL = 0;
static const unsigned char RDID = 0x9F;
static const unsigned char RDSR = 0x05;
static const unsigned char READ = 0x03;
static const unsigned char WREN = 0x06;
static const unsigned char WRDI = 0x04;
static const unsigned char WRSR = 0x01;
static const unsigned char SE = 0x20;
static const unsigned char BE = 0x52;
static const unsigned char CE = 0x60;
static const unsigned char PP = 0x02;

struct StatusRegister{
  bool WIP;
  bool WEL;
  bool BP0;
  bool BP1;
  bool BP2;
  bool SRWD;
};

void printHex(unsigned char *buffer, size_t len);
void writeStatusRegister(int _channel);
StatusRegister readStatusRegister(int _channel);
int readIdentification(int _channel);
void dumpData(int _channel, unsigned char *address);
void setWriteLatch(int _channel, bool enabled);

int main()
{
  cout << "Initializing" << endl;

  int fd = wiringPiSPISetup(CHANNEL,50000);

  cout << "Init result: " << fd << endl;

  unsigned char buffer[1024];

  readIdentification(CHANNEL);
  readStatusRegister(CHANNEL);
  /*
  unsigned char address[3] = {};
  address[0] = 0x05;
  address[2] = 0x00;
  for(unsigned int i=0;i<256;i++)
    {
      address[1] = i;
      dumpData(CHANNEL,address);
    }
  */

  setWriteLatch(CHANNEL,false);
  readStatusRegister(CHANNEL);

  setWriteLatch(CHANNEL,true);
  readStatusRegister(CHANNEL);

  setWriteLatch(CHANNEL,false);
  readStatusRegister(CHANNEL);


  cout << "Exiting ..." <<endl;


}

void print_buffer(unsigned char *buffer, size_t len)
{
  cout << setfill('0');
  for(size_t i=0; i< len ; i++)
  {
    cout << hex << setw(2) << (int) buffer[i];
  }
  cout << endl;
}

int readIdentification(int _channel)
{
  cout << "Sending READ IDENTIFICATION CMD" << endl;
  unsigned char buffer[4];
  buffer[0] = RDID;
  wiringPiSPIDataRW(_channel,buffer,4);

  cout << "Result:" << endl;
  print_buffer(buffer,4);
  return 0;
}

void decodeStatusRegister(StatusRegister *sr,unsigned char data)
{
  unsigned int d = (unsigned int) data;
  sr->WIP = (d & 0x01) == 1;
  sr->WEL = ((d >> 1) & 0x01) == 1;
  sr->BP0 = ((d >> 2) & 0x01) == 1 ;
  sr->BP1 = ((d >> 3) & 0x01) == 1;
  sr->SRWD = ((d >> 7) & 0x01) == 1;
}

void writeStatusRegister(int _channel)
{
  cout << "Sending WRITE STATUS REGISTER CMD" << endl;
  unsigned char buffer[1]={};
  buffer[0] = WRSR;
  wiringPiSPIDataRW(_channel,buffer,1);
}
StatusRegister readStatusRegister(int _channel)
{
  cout << "Sending READ STATUS REGISTER CMD" <<endl;
  unsigned char buffer[2];

  buffer[0] = RDSR;
  
  wiringPiSPIDataRW(_channel,buffer,2);

  cout << "Result:"<<endl;
  print_buffer(buffer,2);

  StatusRegister sr;

  decodeStatusRegister(&sr,buffer[1]);
  
  
  cout << "Write in progress: " << sr.WIP << endl;
  cout << "Write enable latch: " << sr.WEL << endl;
  cout << "BP0 level of protected block: " << sr.BP0 << endl;
  cout << "BP1 level of protected block: " << sr.BP1 << endl;
  cout << "BP2 level of protected block: " << sr.BP2 << endl;
  cout << "Status Register Write Protect: " << sr.SRWD << endl;

  return sr;
}



void dumpData(int _channel, unsigned char *address)
{
  cout << "Reading data @";
  print_buffer(address,3);
  cout << endl;
  unsigned char buffer[1024] = {};
  buffer[0] = READ;
  buffer[1] = address[0];
  buffer[2] = address[1];
  buffer[3] = address[2];

  wiringPiSPIDataRW(_channel,buffer,260);

  cout << "Result:"<<endl;

  print_buffer(buffer,260);
}

void setWriteLatch(int _channel, bool enabled)
{
  cout << "Set write latch: " << enabled << endl;

  unsigned char buffer[1] = {};
  buffer[0]= enabled ? WREN: WRDI;

  wiringPiSPIDataRW(_channel,buffer,1);
}
