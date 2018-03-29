#ifndef __SERIAL_H_
#define __SERIAL_H_

//#include <iostream>

//extern "C"
//{
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <termios.h>
	#include <unistd.h>

	#include <sys/select.h>

	#include <sys/time.h>
	#include <sys/types.h>



	#include <sys/stat.h>
	#include <fcntl.h>
	#include <sys/ioctl.h>
//}


//using namespace std;



#define DEV "/dev/ttyS5"



typedef struct CReturnDataArch
{
	char datalen;
	char *p_datafiled;
	char returnstatus;

}CRETURNDATAARCH;

class CSerial
{
	public:
		CSerial();
		~CSerial();
        bool openSerial(const char *name,int bandrate);
		
		void ConstructionSendData(char command,char datalen,char *data);
		
		void SetAddress(char addr,char realDataLen);
		
		void SetBaudrate(char bandrateIndex,char realDataLen);
		
		void SetSerNum(char serialNum[8],char realDataLen);
		
		void GetSerNum(char period,char cycleNum,char realDataLen);
		
		void CureComand(float power,char realDataLen);
		
		void WriteUserInfo(char areaNum,char writeDataLen,char realDataLen);
		
		void ReadUserInfo(char areaNum,char readDataLen,char realDataLen);
		
		void GetVersionNum(char realDataLen);
		
		void ControlLed(char period,char cycleNum,char realDataLen);
		
		void ControlBuzzer(char period,char cycleNum,char realDataLen);
		
		void IdleSend(char realDataLen);
		
		void PrepareSend(char realDataLen);
		
		void PwmdSend(char realDataLen);
		
		void PwmiSend(char realDataLen);
		
		bool SendData();
		
		bool RecvData();
		
		bool ParseRecvData(unsigned char command);
		
		void GetRealDataField();
		
		bool ParseSetAddressReturnData();
		
		bool ParseSetBaudrateReturnData();
		
		bool ParseSetSerNumReturnData();
		
		bool ParseGetSerNumReturnData();
		
		bool ParseCureComandReturnData();	
		
		bool ParseWriteUserInfoReturnData();
		
		bool ParseReadUserInfoReturnData();
		
		bool ParseGetVersionNumReturnData();
		
		bool ParseControlLedReturnData();
		
		bool ParseControlBuzzerReturnData();
		
		bool ParsePrepareSendReturnData();
		
		bool ParsePwmdSendReturnData();

        bool ParseIdleSendReturnData();

        int m_fd;
        unsigned short impence;
        char m_recvdata[128];
        char powerrate[3];


	private:

		char m_senddata[128];
        char m_sendlen;
		CRETURNDATAARCH realDataFiled;
};



#endif
