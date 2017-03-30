#include "TN901.h"
#include "OLED.h"
#include "Delay.h"


unsigned char flag;

unsigned char TN_Data_Buff[5];		//红外模块数据缓存数组
unsigned char DATA_INDEX;
unsigned char DATA_NUM;				//八位数据计数
unsigned char data_tmp;				//红外模块数据缓存
unsigned char table_mbtemp[]="00.00^C";
unsigned char con=0,bre=0,co=0; //测温模式 连续 间续
float iTemp,MBTemp=11.01;				//温度数据



//==========================================================================
//	//	C格式：	void TN_IRACK_EN(void);
//	实现功能：	红外模块启动函数
//	入口参数：	无
//	出口参数：	无
//==========================================================================
void TN_IRACK_EN(void)
{    
	unsigned char j;
	flag = 0;
	A_TN9 = 0;
	DLY_us(1);
  DATA_NUM = 0;
  DATA_INDEX = 0;

	for(j=0;j<=4;j++)
		TN_Data_Buff[j] = 0;

}
//==========================================================================
//	C格式：		void TN_IRACK_UN(void);
//	实现功能：	红外模块关闭函数
//	入口参数：	无
//	出口参数：	无
//==========================================================================	
void TN_IRACK_UN(void)
{
	A_TN9 = 1;
}

//==========================================================================
//	C格式：		int TN_ReadData(void);
//	实现功能：	读测得数据
//==========================================================================	
void TN_ReadData(void)
{
	data_tmp=0;
	DATA_NUM=0;
	DATA_INDEX=0;
  while(DATA_INDEX<5)
  {
    if(!CLK_TN9)
    {
      if(flag==0)
      {	
        flag=1;			
        DATA_NUM++;
        data_tmp<<=1;	
        if(DATA_TN9)   
        {
          data_tmp=data_tmp | 0x01;
        }
        else
        {
          data_tmp=data_tmp & 0xfe;
        }	

        if(DATA_NUM==8)
        {

          TN_Data_Buff[DATA_INDEX]=data_tmp;
          DATA_NUM=0;
          DATA_INDEX++;
          data_tmp=0;
        }
      }
    }
    else
    {
      if(flag==1)
      {
        flag=0;
      }

    }
  }
}

//======================================================================//
//Program:TN红外传感器目标数据测量子程序
//InPut:NULL
//OutPut:unsigned int returnData	测量结果的出错标识
//Note:
//Edit by xinqiang 20050324
//======================================================================//
unsigned char TN_IR_GetData()
{
	unsigned char iItem,MSB,LSB;
	unsigned char Back_Data;                          //定义返回变量,返回0表示读出正确数据
	Back_Data = 0xaa;

  TN_IRACK_UN();
	DLY_ms(10);
	TN_IRACK_EN();	//enable the TN	
	DLY_us(1);
	DLY_us(1);
	DLY_us(1);			                     
	TN_ReadData();
	DLY_us(1);


	iItem = TN_Data_Buff[0];                //取读到第一个字节数据
	DLY_us(1);
	if(iItem==0x4c)                                //判断第一个字节数据是否正确
	{
		MSB = (TN_Data_Buff[1]);              //取读到第二个字节数据
		LSB = (TN_Data_Buff[2]);              //取读到第三个字节数据
		if(TN_Data_Buff[4] == 0x0d)     //判断是否读到结束标志
		{
			iTemp = MSB*256 +LSB;                       //计算温度值，计算方法请参考红外测温模块
			iTemp = iTemp/16 - 273.15;
			Back_Data = 0;		                     //返回变量赋0
		}
	}
	TN_IRACK_UN();				                     //Unable the TN
	return Back_Data;                                //返回Back_Data
}

/*****************目标温度值MBtemp处理子程序*************/
void dis_mbtemp(void)
{
	unsigned int mb;
	mb=MBTemp*100;					  //变成整数，便于单片机处理
	table_mbtemp[4]=mb%10+0x30;		  //分别取温度十位、个位、小数点后一位、小数点后二位
	mb=mb/10;
	table_mbtemp[3]=mb%10+0x30;
	mb=mb/10;
	table_mbtemp[1]=mb%10+0x30;
	mb=mb/10;
	table_mbtemp[0]=mb%10+0x30;
}



void test(void)
{
  if(!TN_IR_GetData()) 
	{    
		MBTemp = iTemp;
	} 
	dis_mbtemp();							   //目标温度值MBtemp处理
}