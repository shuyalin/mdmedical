#include "serial.h" 


CSerial::CSerial()
{
	m_fd = -1;
    m_sendlen = 0;
    impence = 0;
	memset(m_senddata,0,128);
	memset(m_recvdata,0,128);
}
CSerial::~CSerial()
{
	
}

bool CSerial::openSerial(const char *name,int baudrate)
{
	struct termios options;
    if((m_fd = open(name,O_RDWR|O_NOCTTY|O_NDELAY))==-1)
    {
        printf("Open %s device fail\n",name);
        return false;
    }
	tcgetattr(m_fd,&options);
    if(baudrate == 9600)
    {
        cfsetispeed(&options,B9600);
        cfsetospeed(&options,B9600);
    }
    else if(baudrate == 19200)
    {
        cfsetispeed(&options,B19200);
        cfsetospeed(&options,B19200);
    }
    else if(baudrate == 38400)
    {
        cfsetispeed(&options,B38400);
        cfsetospeed(&options,B38400);
    }
    else if(baudrate == 57600)
    {
        cfsetispeed(&options,B57600);
        cfsetospeed(&options,B57600);
    }
    else if(baudrate == 115200)
    {
        cfsetispeed(&options,B115200);
        cfsetospeed(&options,B115200);
    }
    else
    {
        printf("not support baudrate!\n");
    }
	options.c_cflag|=(CLOCAL|CREAD); 
	options.c_cflag &= ~PARENB;        
	options.c_iflag &= ~(IGNCR | INLCR | ICRNL | IXON);
	options.c_cflag &= ~CSTOPB;  
	options.c_cflag&=~CSIZE; 
	options.c_cflag|=CS8;    
	options.c_lflag&=~(ICANON|ECHO|ISIG); 
	options.c_oflag&=~OPOST;  

	tcflush(m_fd,TCIFLUSH);
	tcsetattr(m_fd,TCSANOW,&options);
    return true;
}

/*构造要发送的数据*/
void CSerial::ConstructionSendData(char command,char datalen,char *data)
{
	unsigned char checkcode,i,j,k;
	memset(m_senddata,0,sizeof(m_senddata));
	m_senddata[0] = 0xAA;
	m_senddata[1] = 0x0;
	m_senddata[2] = datalen;
	m_senddata[3] = command;
	memcpy(m_senddata+4,data,datalen-1);

	checkcode = m_senddata[1];
	
	for(i=2;i<4+datalen-1;i++)
	{
		checkcode ^= m_senddata[i];
	}
	m_senddata[4+datalen-1] = checkcode;
	m_senddata[4+datalen] = 0xBB;

    m_sendlen = datalen + 5;

}



void CSerial::SetAddress(char addr,char realDataLen)
{

	char buff[16] = {0};
	buff[0] = addr;
	ConstructionSendData(0x80,realDataLen+1,buff);	
} 

void CSerial::SetBaudrate(char bandrateIndex,char realDataLen)
{
	char buff[16] = {0};
	#if 0
	if(bandrateIndex == 0x00)
	{
		buff[0] = addr;
	}
	else if(bandrateIndex == 0x01)
	{
		buff[0] = addr;
	}
	else if(bandrateIndex == 0x02)
	{
		buff[0] = addr;
	}
	else if(bandrateIndex == 0x03)
	{
		buff[0] = addr;
	}
	else if(bandrateIndex == 0x04)
	{
		buff[0] = addr;
	}
	else
	{
		buff[0] = addr;
	}
	#endif
	buff[0] = bandrateIndex;
	ConstructionSendData(0x81,realDataLen+1,buff);	
}

void CSerial::SetSerNum(char serialNum[8],char realDataLen)
{
	char buff[16] = {0};
	memcpy(buff,serialNum,sizeof(serialNum));
	ConstructionSendData(0x82,realDataLen+1,buff);	
} 

void CSerial::GetSerNum(char period,char cycleNum,char realDataLen)
{
	char buff[16] = {0};
	ConstructionSendData(0x83,realDataLen+1,buff);	
}


void CSerial::CureComand(float power,char realDataLen)
{
	char buff[16] = {0};
	gcvt(power,realDataLen,buff);
	ConstructionSendData(0x93,realDataLen+1,buff);	
}


void CSerial::WriteUserInfo(char areaNum,char writeDataLen,char realDataLen)
{
	if(writeDataLen > 120)
	{
		printf("the number of reading is request less than 120!\n");
		return ;
	}
	char buff[16] = {0};
	buff[0] = areaNum;
	buff[1] = writeDataLen;
	ConstructionSendData(0x84,realDataLen+1,buff);	
} 

void CSerial::ReadUserInfo(char areaNum,char readDataLen,char realDataLen)
{
	if(readDataLen > 120)
	{
		printf("the number of reading is request less than 120!\n");
		return ;
	}
	char buff[16] = {0};
	buff[0] = areaNum;
	buff[1] = readDataLen;
	ConstructionSendData(0x85,realDataLen+1,buff);	
} 

void CSerial::GetVersionNum(char realDataLen)
{
	char buff[16] = {0};
	ConstructionSendData(0x92,realDataLen+1,buff);
}

void CSerial::ControlLed(char period,char cycleNum,char realDataLen)
{
	char buff[16] = {0};
	buff[0] = period;
	buff[1] = cycleNum;
	ConstructionSendData(0x88,realDataLen+1,buff);	
} 

void CSerial::ControlBuzzer(char period,char cycleNum,char realDataLen)
{
	char buff[16] = {0};
	buff[0] = period;
	buff[1] = cycleNum;
	ConstructionSendData(0x89,realDataLen+1,buff);	
}

void CSerial::IdleSend(char realDataLen)
{
	char buff[16] = {0};
    ConstructionSendData(0x91,realDataLen+1,buff);
}


void CSerial::PrepareSend(char realDataLen)
{
	char buff[16] = {0};
	ConstructionSendData(0x92,realDataLen+1,buff);
}

void CSerial::PwmdSend(char realDataLen)
{
	char buff[16] = {0};
	ConstructionSendData(0x94,realDataLen+1,buff);
}

void CSerial::PwmiSend(char realDataLen)
{
	char buff[16] = {0};
	ConstructionSendData(0x96,realDataLen+1,buff);
}

bool CSerial::SendData()
{
    int j=0;
    char k;
	tcflush(m_fd, TCIOFLUSH);
//    printf("senddata is : ");
//    for(j=0;j<m_sendlen;j++)
//    {
//        k = m_senddata[j];
//        printf(" %x  ",k);
//    }
//    printf("\n");

    if(write(m_fd,m_senddata,m_sendlen) < 0)
	{
		printf("Write Serial fail\n");
		return false;
	} 
	return true;
}

bool CSerial::RecvData()
{
	
	struct timeval tv;	
    int i=0,j=0;
	fd_set rdfds; 
	int maxfd = -1;
	int ret;
	char buff[512];
	int len;
	tv.tv_sec = 5;
	tv.tv_usec = 0; 	
	fflush(stdout);

	while(1)
	{

		FD_ZERO(&rdfds);   
		FD_SET(0,&rdfds);

        maxfd = m_fd>0?m_fd:0;
        FD_SET(maxfd, &rdfds);


        maxfd = maxfd > 0 ? maxfd:0;
		ret = select(maxfd + 1, &rdfds, NULL, NULL, &tv);
		if(ret < 0)
		{
			perror("Select fail\n");
			return false;
		}
		else if(ret == 0)
		{
			continue;
		}
		else
		{  
			if(FD_ISSET(m_fd, &rdfds)) 
			{		
				memset(buff, 0, 512);
				i = 0;
				while((read(m_fd,&buff[i++],1) > 0)&&(i<511));
				buff[i] = '\0';

                printf("receve data id : ");
                for(j=0;j<i;j++)
                    printf("  %x ",buff[j]);
                printf("\n");
				memcpy(m_recvdata,buff,i);
				fflush(stdout);
				break;
			}
		}
	}
	return true;
}



bool CSerial::ParseRecvData(unsigned char command)
{
	switch(command)
	{
		case 0x80:
			 ParseSetAddressReturnData();	
			 break;
		case 0x81:
			ParseSetBaudrateReturnData();
			 break;
		case 0x82:
			ParseSetSerNumReturnData();
			 break;
		case 0x83:
			ParseGetSerNumReturnData();
			 break;
		case 0x93:;break;
		case 0x84:
			ParseWriteUserInfoReturnData();
			 break;
		case 0x85:
			ParseReadUserInfoReturnData();
			 break;
		case 0x86:
			ParseGetVersionNumReturnData();
			 break;
		case 0x88:
			ParseControlLedReturnData();
			 break;
		case 0x89:
			ParseControlBuzzerReturnData();
			 break;
		case 0x91:;break;
		case 0x92:
			ParsePrepareSendReturnData();
			 break;
		case 0x94:
			ParsePwmdSendReturnData();
			 break;
		default :;break;			

	}
	return true;
}

//??????????????????
void CSerial::GetRealDataField()
{
	realDataFiled.p_datafiled = NULL;
    realDataFiled.datalen = m_recvdata[2] - 1;
    if(realDataFiled.datalen != 0)
    {
        realDataFiled.p_datafiled = (char *)malloc(sizeof(char)*realDataFiled.datalen);
        memcpy(realDataFiled.p_datafiled,&m_recvdata[4],realDataFiled.datalen);
    }
    realDataFiled.returnstatus = m_recvdata[3];
}


bool CSerial::ParseSetAddressReturnData()
{
	char *p_tmp;
	char newaddr;
	bool status;
	GetRealDataField();
	p_tmp = realDataFiled.p_datafiled;
	if(realDataFiled.returnstatus == 0x00)
	{
		newaddr = *p_tmp;
		status = true;
	}
	else if(realDataFiled.returnstatus == 0x01)
	{
		if(*p_tmp == 0x85)
		{
			printf("input para or command format is wrong!\n");
		}
		else if(*p_tmp == 0x8f)
		{
			printf("input command code is not exist!\n");
		}
		status = false;
	}
	else
	{
		printf("unknown return data!\n");
		status = false;
	}
    if(realDataFiled.p_datafiled)
        free(realDataFiled.p_datafiled);
	return status;
}

bool CSerial::ParseSetBaudrateReturnData()
{
	char *p_tmp;
	char baudrate;
	bool status;
	GetRealDataField();
	p_tmp = realDataFiled.p_datafiled;
	if(realDataFiled.returnstatus == 0x00)
	{
		baudrate = *p_tmp;
		status = true;
	}
	else if(realDataFiled.returnstatus == 0x01)
	{
		if(*p_tmp == 0x85)
		{
			printf("input para or command format is wrong!\n");
		}
		else if(*p_tmp == 0x8f)
		{
			printf("input command code is not exist!\n");
		}
		status = false;
	}
	else
	{
		printf("unknown return data!\n");
		status = false;
	}
    if(realDataFiled.p_datafiled)
        free(realDataFiled.p_datafiled);
	return status;
}

bool CSerial::ParseSetSerNumReturnData()
{
	char *p_tmp;
	bool status;
	GetRealDataField();
	p_tmp = realDataFiled.p_datafiled;
	if(realDataFiled.returnstatus == 0x00)
	{
		if(*p_tmp == 0x80)
		{
			printf("operation is succesful!\n");	
		}
		status = true;
	}
	else if(realDataFiled.returnstatus == 0x01)
	{
		if(*p_tmp == 0x85)
		{
			printf("input para or command format is wrong!\n");
		}
		else if(*p_tmp == 0x8f)
		{
			printf("input command code is not exist!\n");
		}
		else if(*p_tmp == 0x81)
		{
			printf("operation is failed!\n");
		}
		status = false;
	}
	else
	{
		printf("unknown return data!\n");
		status = false;
	}
    if(realDataFiled.p_datafiled)
        free(realDataFiled.p_datafiled);
	return status;
}

bool CSerial::ParseGetSerNumReturnData()
{
	char *p_tmp;
	char deviceaddr;
	char serialNum[8]={0};
	bool status;
	GetRealDataField();
	p_tmp = realDataFiled.p_datafiled;
	if(realDataFiled.returnstatus == 0x00)
	{
		deviceaddr = *p_tmp;
		memcpy(serialNum,p_tmp+1,8);
		status = true;
	}
	else if(realDataFiled.returnstatus == 0x01)
	{
		if(*p_tmp == 0x85)
		{
			printf("input para or command format is wrong!\n");
		}
		else if(*p_tmp == 0x8f)
		{
			printf("input command code is not exist!\n");
		}
		status = false;
	}
	else
	{
		printf("unknown return data!\n");
		status = false;
	}
    if(realDataFiled.p_datafiled)
        free(realDataFiled.p_datafiled);
	return status;
}

//bool CSerial::ParseCureComandReturnData()
//{
	
//}

bool CSerial::ParseWriteUserInfoReturnData()
{	
	char *p_tmp;
	bool status;
	GetRealDataField();
	p_tmp = realDataFiled.p_datafiled;
	if(realDataFiled.returnstatus == 0x00)
	{
		if(*p_tmp == 0x80)
		{
			printf("operation is succesful!\n");	
		}
		status = true;
	}
	else if(realDataFiled.returnstatus == 0x01)
	{
		if(*p_tmp == 0x85)
		{
			printf("input para or command format is wrong!\n");
		}
		else if(*p_tmp == 0x8f)
		{
			printf("input command code is not exist!\n");
		}
		else if(*p_tmp == 0x81)
		{
			printf("operation is failed!\n");
		}
		status = false;
	}
	else
	{
		printf("unknown return data!\n");
		status = false;
	}
    if(realDataFiled.p_datafiled)
        free(realDataFiled.p_datafiled);
	return status;
	
}

bool CSerial::ParseReadUserInfoReturnData()
{
	char *p_tmp;
	bool status;
	char userinfo[120];
	memset(userinfo,0,120);
	GetRealDataField();
	p_tmp = realDataFiled.p_datafiled;
	if(realDataFiled.returnstatus == 0x00)
	{
		memcpy(userinfo,p_tmp,realDataFiled.datalen-1);
		status = true;
	}
	else if(realDataFiled.returnstatus == 0x01)
	{
		if(*p_tmp == 0x85)
		{
			printf("input para or command format is wrong!\n");
		}
		else if(*p_tmp == 0x8f)
		{
			printf("input command code is not exist!\n");
		}
		status = false;
	}
	else
	{
		printf("unknown return data!\n");
		status = false;
	}
    if(realDataFiled.p_datafiled)
        free(realDataFiled.p_datafiled);
	return status;
}

bool CSerial::ParseGetVersionNumReturnData()
{
	char *p_tmp;
	bool status;
	char versionNum[64];
	memset(versionNum,0,64);
	GetRealDataField();
	p_tmp = realDataFiled.p_datafiled;
	if(realDataFiled.returnstatus == 0x00)
	{
		memcpy(versionNum,p_tmp,realDataFiled.datalen-1);
		status = true;
	}
	else if(realDataFiled.returnstatus == 0x01)
	{
		if(*p_tmp == 0x85)
		{
			printf("input para or command format is wrong!\n");
		}
		else if(*p_tmp == 0x8f)
		{
			printf("input command code is not exist!\n");
		}
		status = false;
	}
	else
	{
		printf("unknown return data!\n");
		status = false;
	}
	free(realDataFiled.p_datafiled);
	return status;
}

bool CSerial::ParseControlLedReturnData()
{
	char *p_tmp;
	bool status;
	GetRealDataField();
	p_tmp = realDataFiled.p_datafiled;
	if(realDataFiled.returnstatus == 0x00)
	{
		if(*p_tmp == 0x80)
		{
			printf("operation is succesful!\n");	
		}
		status = true;
	}
	else if(realDataFiled.returnstatus == 0x01)
	{
		if(*p_tmp == 0x85)
		{
			printf("input para or command format is wrong!\n");
		}
		else if(*p_tmp == 0x8f)
		{
			printf("input command code is not exist!\n");
		}
		else if(*p_tmp == 0x81)
		{
			printf("operation is failed!\n");
		}
		status = false;
	}
	else
	{
		printf("unknown return data!\n");
		status = false;
	}
    if(realDataFiled.p_datafiled)
        free(realDataFiled.p_datafiled);
	return status;
}

bool CSerial::ParseControlBuzzerReturnData()
{
	char *p_tmp;
	bool status;
	GetRealDataField();
	p_tmp = realDataFiled.p_datafiled;
	if(realDataFiled.returnstatus == 0x00)
	{
		if(*p_tmp == 0x80)
		{
			printf("operation is succesful!\n");	
		}
		status = true;
	}
	else if(realDataFiled.returnstatus == 0x01)
	{
		if(*p_tmp == 0x85)
		{
			printf("input para or command format is wrong!\n");
		}
		else if(*p_tmp == 0x8f)
		{
			printf("input command code is not exist!\n");
		}
		else if(*p_tmp == 0x81)
		{
			printf("operation is failed!\n");
		}
		status = false;
	}
	else
	{
		printf("unknown return data!\n");
		status = false;
	}
    if(realDataFiled.p_datafiled)
        free(realDataFiled.p_datafiled);
	return status;
}



bool CSerial::ParsePrepareSendReturnData()
{
	char *p_tmp;
	bool status;
    char i = 0;
    //char impedance[2] = {0};
	GetRealDataField();
	p_tmp = realDataFiled.p_datafiled;

//    printf("realDataFiled.datalen %d\n",realDataFiled.datalen);
//    for(i=0;i<realDataFiled.datalen;i++)
//        printf(" 0x%x ",*(p_tmp++));
//    printf(" \n");

	if(realDataFiled.returnstatus == 0x00)
	{
        if(realDataFiled.datalen == 5)
        {
            impence = (unsigned short)(*p_tmp);
            p_tmp+=2;
            powerrate[0] = *p_tmp;
            p_tmp++;
            powerrate[1] = *p_tmp;
            p_tmp++;
            powerrate[2] = *p_tmp;
        }
        else if(realDataFiled.datalen == 6)
        {
            impence =(*p_tmp);
            //printf(" impence is  %d  %d  %hu ",*p_tmp,*(--p_tmp),impence);
            printf("111    %d\n",*p_tmp);
            p_tmp++;
            impence = (impence<<8)|(*p_tmp);
            printf("222    %d\n",*p_tmp);
            printf(" impence is   %hu ",impence);

            p_tmp+=2;
            powerrate[0] = *p_tmp;
            p_tmp++;
            powerrate[1] = *p_tmp;
            p_tmp++;
            powerrate[2] = *p_tmp;
        }
		status = true;
	}
    if(realDataFiled.p_datafiled)
        free(realDataFiled.p_datafiled);
    realDataFiled.p_datafiled = NULL;
    p_tmp = NULL;
	return status;
}

bool CSerial::ParsePwmdSendReturnData()
{
	char *p_tmp;
    bool status = true;
	char power[16] = {0};
	GetRealDataField();
	p_tmp = realDataFiled.p_datafiled;

	memcpy(power,p_tmp,realDataFiled.datalen-1);	
    if(realDataFiled.p_datafiled)
        free(realDataFiled.p_datafiled);
	return status;
}

bool CSerial::ParseIdleSendReturnData()
{
    GetRealDataField();
    if(realDataFiled.returnstatus == 0x00)
        return true;
    else
        return false;
}
