
struct MX25L8005_StatusRegister {
	bool WIP;
 	bool WEL;
 	bool BP0;
 	bool BP1;
 	bool BP2;
 	bool SRWD;
};

#ifndef __MX25L8005_Commands__
const MX25L8005_WREN = 0x06; //Write enable
const MX25L8005_WRDI = 0x04; //Write disable
const MX25L8005_RDID = 0x9F; //Read identification
const MX25L8005_RDSR = 0x05; //Read status register
const MX25L8005_WRSR = 0x01; //Write status register
const MX25L8005_READ = 0x03; //Read data
const MX25L8005_FREAD = 0x0B; //Fast read data
const MX25L8005_SE = 0x20; //Sector erase
const MX25L8005_BE = 0x52; //Block erase
const MX25L8005_CE = 0x60; //Chip erase
const MX25L8005_PP = 0x02; //Page Program
const MX25L8005_DP = 0xB9; //Deep power down
const MX25L8005_RDP = 0xAB; //Release from deep power down
const MX25L8005_RES = 0xAB; //Read Electronic id
const MX25L8005_REMS = 0x90; //Read Electronic manufacturer & device id

const WIRINGPI_MAXBUFFER_LEN = 1024;
#define __MX25L8005_Commands__;
#endif

class MX25L8005 {
	private:
		int _spiChannel = 0;
		int _spiFreq = 50000; //Hertz
		int _spiMode = 3;
		unsigned char _spiBuffer[WIRINGPI_MAXBUFFER_LEN] = {};

		void clearInternalBuffer();
		int dataRW(unsigned char *buffer, int len);

	public:
		MX25L8005(int _spiChannel, int _spiMode);
		void init();
		void writeStatusRegister();
		unsigned char readStatusRegister();

		MX25L8005_StatusRegister decodeStatusRegister(unsigned char statusRegisterBinary);
};