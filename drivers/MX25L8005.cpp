#include <wiringPiSPI.h>

MX25L8005::MX25L8005(int _spiChannel, int _spiMode)
{
	this._spiChannel = _spiChannel;
	this._spiMode = _spiMode;

	this.clearInternalBuffer();
}
void  MX25L8005::init()
{
	int result = wiringPiSPISetup(this._spiChannel,this._spiFreq,this._spiMode);
	
	#ifdef __DEBUG__
		cout <<  "SPI Init result:" << result << endl;
	#endif
}

void MX25L8005::clearInternalBuffer()
{
	//Clear spi buffer
	memset(this._spiBuffer,0,sizeof(unsigned char) * WIRINGPI_MAXBUFFER_LEN);
}

int dataRW(unsigned char *buffer, int len)
{
	  wiringPiSPIDataRW(this._spiChannel,buffer,len);
}

MX25L8005_StatusRegister MX25L8005::decodeStatusRegister(unsigned char statusRegisterBinary)
{
	std::bitset<8> data(statusRegisterBinary);

	MX25L8005_StatusRegister sr;
	sr->WIP = data.test(0);
	sr->WEL = data.test(1);
	sr->BP0 = data.test(2) ;
	sr->BP1 = data.test(3);
	sr->BP2 = data.test(4);
 	sr->SRWD = data.test(7);

 	return sr;
}

unsigned char MX25L8005::readStatusRegister(){
	#ifdef __DEBUG__
		cout <<  "Sending READ STATUS REGISTER command" << endl;
	#endif

	this.clearInternalBuffer();
	this._spiBuffer[0] = MX25L8005_RDSR; //Set command


	this.dataRW(this._spiBuffer,2);

	#ifdef __DEBUG__
		cout <<  "Exiting..." << endl;
	#endif
	return this._spiBuffer[1];
}

void  MX25L8005::writeStatusRegister(){
	#ifdef __DEBUG__
		cout <<  "Sending WRITE STATUS REGISTER command" << endl;
	#endif



	#ifdef __DEBUG__
		cout <<  "Exiting..." << endl;
	#endif
}