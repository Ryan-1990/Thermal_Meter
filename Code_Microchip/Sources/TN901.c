#include "TN901.h"
#include "OLED.h"
#include "Delay.h"


unsigned char flag;

unsigned char TN_Data_Buff[5];		//����ģ�����ݻ�������
unsigned char DATA_INDEX;
unsigned char DATA_NUM;				//��λ���ݼ���
unsigned char data_tmp;				//����ģ�����ݻ���
unsigned char table_mbtemp[]="00.00^C";
unsigned char con=0,bre=0,co=0; //����ģʽ ���� ����
float iTemp,MBTemp=11.01;				//�¶�����



//==========================================================================
//	//	C��ʽ��	void TN_IRACK_EN(void);
//	ʵ�ֹ��ܣ�	����ģ����������
//	��ڲ�����	��
//	���ڲ�����	��
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
//	C��ʽ��		void TN_IRACK_UN(void);
//	ʵ�ֹ��ܣ�	����ģ��رպ���
//	��ڲ�����	��
//	���ڲ�����	��
//==========================================================================	
void TN_IRACK_UN(void)
{
	A_TN9 = 1;
}

//==========================================================================
//	C��ʽ��		int TN_ReadData(void);
//	ʵ�ֹ��ܣ�	���������
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
//Program:TN���⴫����Ŀ�����ݲ����ӳ���
//InPut:NULL
//OutPut:unsigned int returnData	��������ĳ����ʶ
//Note:
//Edit by xinqiang 20050324
//======================================================================//
unsigned char TN_IR_GetData()
{
	unsigned char iItem,MSB,LSB;
	unsigned char Back_Data;                          //���巵�ر���,����0��ʾ������ȷ����
	Back_Data = 0xaa;

  TN_IRACK_UN();
	DLY_ms(10);
	TN_IRACK_EN();	//enable the TN	
	DLY_us(1);
	DLY_us(1);
	DLY_us(1);			                     
	TN_ReadData();
	DLY_us(1);


	iItem = TN_Data_Buff[0];                //ȡ������һ���ֽ�����
	DLY_us(1);
	if(iItem==0x4c)                                //�жϵ�һ���ֽ������Ƿ���ȷ
	{
		MSB = (TN_Data_Buff[1]);              //ȡ�����ڶ����ֽ�����
		LSB = (TN_Data_Buff[2]);              //ȡ�����������ֽ�����
		if(TN_Data_Buff[4] == 0x0d)     //�ж��Ƿ����������־
		{
			iTemp = MSB*256 +LSB;                       //�����¶�ֵ�����㷽����ο��������ģ��
			iTemp = iTemp/16 - 273.15;
			Back_Data = 0;		                     //���ر�����0
		}
	}
	TN_IRACK_UN();				                     //Unable the TN
	return Back_Data;                                //����Back_Data
}

/*****************Ŀ���¶�ֵMBtemp�����ӳ���*************/
void dis_mbtemp(void)
{
	unsigned int mb;
	mb=MBTemp*100;					  //������������ڵ�Ƭ������
	table_mbtemp[4]=mb%10+0x30;		  //�ֱ�ȡ�¶�ʮλ����λ��С�����һλ��С������λ
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
	dis_mbtemp();							   //Ŀ���¶�ֵMBtemp����
}