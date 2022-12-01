//
// Created by storm on 2021/5/9.
//
#include "pch.h"
#include <iostream>
//#include "motor.h"
#include "kvaser.h"

using namespace std;

/*
 * ��ʼ�� can ͨ���� can ����
 */
int Kvaser::canInit(int channel_number)
{
	// ��ʼ�� can �������ظ�����ֻ�����һ�Σ�
	canInitializeLibrary();
	// �� can ͨ��
	handle = canOpenChannel(channel_number, canOPEN_ACCEPT_VIRTUAL);
	if (handle < 0)
	{
		TRACE("can Open Channel error! \n");
		return EXIT_FAILURE;
	}
	// ��������Ϣʱ���� TimeOut ���󣬵��������ʣ�һ���� canBITRATE_500K��
	status = canSetBusParams(handle, canBITRATE_500K, 0, 0, 0, 0, 0);
	if (checkStatus("canSetBusParams"))
	{
		TRACE("can Set Bus Params error! \n");
		return EXIT_FAILURE;
	}
	// �� can ����
	status = canBusOn(handle);
	if (checkStatus("canBusOn"))
	{
		TRACE("can Bus On error! \n");
		return EXIT_FAILURE;
	}
	TRACE("can initialized! \n");
	return EXIT_SUCCESS;
}

/*
 * �ر� can ͨ��
 */
int Kvaser::canRelease()
{
	status = canBusOff(handle);
	if (checkStatus("canBusOff"))
	{
		return EXIT_FAILURE;
	}
	status = canClose(handle);
	if (checkStatus("canClose"))
	{
		return EXIT_FAILURE;
	}
	TRACE("can released! \n");
	return EXIT_SUCCESS;
}

/*
 * ���� can ��Ϣ
 */
int Kvaser::canSend(uint8_t *msg, long can_id, unsigned int dlc)
{
	status = canWrite(handle, can_id, msg, dlc, 0);
	if (checkStatus("canWrite"))
	{
		return EXIT_FAILURE;
	}
	status = canWriteSync(handle, 100);
	if (checkStatus("canWriteSync"))
	{
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

/*
 * ���ӵ��
 */
int Kvaser::connectMotor(long can_id)
{
	uint8_t data1[8] = { 0x82, (uint8_t)can_id, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	uint8_t data2[8] = { 0x01, (uint8_t)can_id, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }; // ID=1
	int flag = canSend(data1, can_id, 8);
	if (flag == EXIT_FAILURE)
	{
		TRACE("Motor %d connect failed! \n", can_id);
		return EXIT_FAILURE;
	}
	Sleep(100); //0.1ms
	flag = canSend(data2, can_id, 8);
	if (flag == EXIT_FAILURE)
	{
		TRACE("Motor %d connect failed! \n", can_id);
		return EXIT_FAILURE;
	}
	// Sleep(100);
	TRACE("Motor %d has been connected! \n", can_id);
	return EXIT_SUCCESS;
}

/*
 * ���ʹ��
 */
int Kvaser::motorEnable(long can_id)
{
	uint8_t data[8] = { 0x4d, 0x4f, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00 }; // mo=1
	long temp_id = can_id | (6 << 7);
	int flag = canSend(data, temp_id, 8);
	if (flag == EXIT_FAILURE)
	{
		TRACE("Motor %d enable failed! \n", can_id);
		return EXIT_FAILURE;
	}
	//Sleep(100);
	flag = canSend(data, temp_id, 8);
	if (flag == EXIT_FAILURE)
	{
		TRACE("Motor %d enable failed! \n", can_id);
		return EXIT_FAILURE;
	}
	TRACE("Motor %d has been enabled! \n", can_id);
	return EXIT_SUCCESS;
}

/*
 * �رյ��
 */
int Kvaser::motorDisable(long can_id)
{
	uint8_t data[8] = { 0x4d, 0x4f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }; // mo=0
	long temp_id = can_id | (6 << 7);
	int flag = canSend(data, temp_id, 8);
	if (flag == EXIT_FAILURE)
	{
		TRACE("Motor %d disabled failed! \n", can_id);
		return EXIT_FAILURE;
	}
	TRACE("Motor %d has been disabled! \n", can_id);
	return EXIT_SUCCESS;
}

/*
 * ѡ�����˶�ģʽ
 */
int Kvaser::modeChoose(long can_id, Mode mode)
{
	uint8_t data1[8] = { 0x55, 0x4D, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00 }; // um=1
	uint8_t data2[8] = { 0x55, 0x4D, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00 }; // um=2
	uint8_t data3[8] = { 0x55, 0x4D, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00 }; // um=3
	uint8_t data4[8] = { 0x55, 0x4D, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00 }; // um=4
	uint8_t data5[8] = { 0x55, 0x4D, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00 }; // um=5
	long temp_id = can_id | (6 << 7);
	motorDisable(can_id);
	//Sleep(100);
	int flag = 0;
	switch (mode)
	{
	case TORQUE_MODE:flag = canSend(data1, temp_id, 8);
		//Sleep(100);
		break;
	case SPEED_MODE:flag = canSend(data2, temp_id, 8);
		//Sleep(100);
		break;
	case MICRO_STEPPING_MODE:flag = canSend(data3, temp_id, 8);
		//Sleep(100);
		break;
	case DUAL_FEEDBACK_POSITION_MODE:flag = canSend(data4, temp_id, 8);
		//Sleep(100);
		break;
	case SINGLE_FEEDBACK_POSITION_MODE:flag = canSend(data5, temp_id, 8);
		//Sleep(50 * 100);
		break;
	default: break;
	}
	if (flag == EXIT_FAILURE)
	{
		TRACE("Motor %d choose mode failed! \n", can_id);
		return EXIT_FAILURE;
	}
	motorEnable(can_id);
	//Sleep(100); // need more time
	TRACE("Motor %d mode has been chosen! \n", can_id);
	return EXIT_SUCCESS;
}

/*
 * �ٶ�ģʽ
 */
int Kvaser::speedMode(long can_id, int32_t speed)
{
	uint8_t data[8] = { 0x4A, 0x56, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }; // JV;
	dataIntegrationInt(data, speed);
	long tempID = can_id | (6 << 7);
	int flag = canSend(data, tempID, 8);//jv
	if (flag == EXIT_FAILURE)
	{
		TRACE("Motor %d start speed mode failed! \n", can_id);
		return EXIT_FAILURE;
	}
	//Sleep(100);
	beginMovement(can_id);
	TRACE("Motor %d start speed mode! \n", can_id);
	return EXIT_SUCCESS;
}

/*
 * λ��ģʽ
 */
int Kvaser::positionMode(long can_id, int32_t position, int32_t speed)
{
	uint8_t data1[8] = { 0x53, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }; // sp
	uint8_t data2[8] = { 0x50, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }; // pa
	dataIntegrationInt(data1, speed);// sp=SpeedValue
	dataIntegrationInt(data2, position);// pa=PosValue
	long tempID = can_id | (6 << 7);
	canSend(data1, tempID, 8);
	//Sleep(100);
	canSend(data2, tempID, 8);
	//Sleep(1000);
	//Sleep(100);
	beginMovement(can_id);
	TRACE("Motor %d start speed mode! \n", can_id);
	return EXIT_SUCCESS;
}

/*
 * ���أ�������ģʽ
 */
int Kvaser::torqueMode(long can_id, float torque)
{
	uint8_t data[8] = { 0x54, 0x43, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }; // TC;
	//uint temp_id=can_id;uint8_t TempPos[8];uint32_t tempRevDlc; int DataTemp=0;
	dataIntegrationFloat(data, torque);
	long temp_id = can_id | (6 << 7);
	int flag = canSend(data, temp_id, 8);
	if (flag == EXIT_FAILURE)
	{
		TRACE("Motor %d start torque failed! \n", can_id);
		return EXIT_FAILURE;
	}
	//GetInfAndRead(data,TempPos,channelNum,temp_id,8,&tempRevDlc);
	return EXIT_SUCCESS;
}

/*
 * ��������
 */
float Kvaser::protect(float torque)
{
	float temp = torque;
	if (torque > 3.00)
	{
		temp = 3.00;
	}
	if (torque < -3.00)
	{
		temp = -3.00;
	}
	return temp;
}

/*
 * ��ʼת��
 */
int Kvaser::beginMovement(long can_id)
{
	uint8_t data[8] = { 0x42, 0x47, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }; // BG
	long temp_id = can_id | (6 << 7);
	int flag = canSend(data, temp_id, 4);
	if (flag == EXIT_FAILURE)
	{
		TRACE("Motor %d begin move! failed! \n", can_id);
		return EXIT_FAILURE;
	}
	TRACE("Motor %d begin move!", can_id);
	return EXIT_SUCCESS;
}

/*
 * ��ȡ��ʵλ��
 */
float Kvaser::getPosition(long can_id, uint16_t encoder_count, uint16_t reduction_ratio)
{
	uint8_t data[8] = { 0x50, 0x58, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }; // px
	long temp_id = can_id;
	uint8_t temp_Pos[8];
	uint32_t temp_rev_dlc;
	int data_temp = 0;
	canFlushReceiveQueue(handle);
	do
	{
		getInfAndRead(data, temp_Pos, temp_id, 4, &temp_rev_dlc);
	} while (!((temp_Pos[0] == 0x50) && (temp_Pos[1] == 0x58) && (temp_Pos[3] != 0x40)));
	data_temp =
		(temp_Pos[7] & 0xff) << 24 | (temp_Pos[6] & 0xff) << 16 | (temp_Pos[5] & 0xff) << 8 | temp_Pos[4] & 0xff;
	// ����ֵ = 2*PI*����ֵ / �ֱ��� / ���ٱ� / elmo�Ŵ���(4)
	return (float)(2.0 * PI * data_temp / 4.0 / encoder_count / reduction_ratio);
}

/*
 * ��ȡ��ʵ�ٶ�
 */
float Kvaser::getVelocity(long can_id, uint16_t encoder_count, uint16_t reduction_ratio)
{
	uint8_t data[8] = { 0x56, 0x58, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }; // vx
	long temp_id = can_id;
	uint8_t temp_vel[8];
	uint32_t temp_rev_dlc;
	int data_temp = 0;
	canFlushReceiveQueue(handle);
	getInfAndRead(data, temp_vel, temp_id, 4, &temp_rev_dlc);
	if ((temp_vel[0] == 0x56) && (temp_vel[1] == 0x58) && (temp_vel[3] != 0x40))
	{
		data_temp = (temp_vel[7] & 0xff) << 24 |
			(temp_vel[6] & 0xff) << 16 | (temp_vel[5] & 0xff) << 8 | temp_vel[4] & 0xff;
	}
	// ���ٶ� = 2*PI*�����ٶ� / �ֱ��� / ���ٱ� / elmo�Ŵ���(4)
	return (float)(2.0 * PI * data_temp / 4.0 / encoder_count / reduction_ratio);
}

/*
 * ��ȡ can ͨ����������
 */
int Kvaser::getInfAndRead(uint8_t *send_msg, uint8_t *read_msg, long can_id, unsigned int send_dlc,
	unsigned int *read_dlc)
{
	unsigned long tempTime;
	unsigned int flag;
	canWrite(handle, can_id | (6 << 7), send_msg, send_dlc, 0);
	while (canReadSpecific(handle, can_id | (5 << 7), read_msg, read_dlc, &flag, &tempTime) != canOK);
	return EXIT_SUCCESS;
}

/*
 * ��� can ͨ��״̬
 */
int Kvaser::checkStatus(const string &id)
{
	if (status != canOK)
	{
		char buf[50];
		buf[0] = '\0';
		canGetErrorText(status, buf, sizeof(buf));
		TRACE("%s : failed, stat = %d info : %s \n", id, (int)status, buf);
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

/*
 * ���ݸ�ʽת��
 */
void Kvaser::dataIntegrationInt(uint8_t *temp, int32_t temp_value)
{
	temp[7] = (temp_value >> 24);
	temp[6] = ((temp_value << 8) >> 24);
	temp[5] = ((temp_value << 16) >> 24);
	temp[4] = ((temp_value << 24) >> 24);
}

/*
 * ���ݸ�ʽת��
 */
void Kvaser::dataIntegrationFloat(uint8_t *temp, float temp_value)
{
	int data = *(int *)&temp_value;
	temp[7] = (data >> 24);
	temp[6] = ((data << 8) >> 24);
	temp[5] = ((data << 16) >> 24);
	temp[4] = ((data << 24) >> 24);
	temp[3] |= 1 << 7;
}












