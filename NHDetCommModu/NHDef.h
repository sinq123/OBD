/************************************************* 
*  Copyright (C), 2009-2019,��ɽ�л�������������޹�˾ 
*  All rights reserved
*
*  �ļ�����: NHDef.h
*  ��ǰ�汾: 1.0.28
*  ����: 2011-05-11
*  ժҪ: �������ö���,ͳһ�����ﶨ��,�����ظ�����
*		 Ŀǰ��Ҫ�Ƕ����豸�߳̿���첽������Ϣ
*
*
*  ��ʷ��Ϣ:
*
* �汾��1.0.32
* ���ڣ�2014-11-18
* ���ߣ�Lihy
* ����������޸����ݱ��뿪�غ궨��
*
* �汾��1.0.31
* ���ڣ�2013-05-27
* ���ߣ�Lihy
* ������1)���ϵͳ�Լ���Ϣ
            #define WM_SYSTEM_CHECK_NHV_WORKING			WM_USER + 0x0607  (���ڼ��ϰ滷������У׼����)
			#define WM_SYSTEM_CHECK_RPM_WORKING			WM_USER + 0x0608  (���ڼ��ϰ�ת���Լ����)
*
* �汾��1.0.30
* ���ڣ�2012-10-08
* ���ߣ�wuyq
* ��������Ӽ�����������ȡֵ��Χ����
*
* �汾��1.0.29
* ���ڣ�2011-09-28
* ���ߣ�Raylee
* ���������Ħ�г�����/˫���ټ��״̬����
*
* �汾��1.0.28
* ���ڣ�2011-05-11
* ���ߣ�hemincong
* ������1)��Ӳ��ͻ����ͻ���ʼ�Զ�׼����Ϣ��WM_GASOLINE_VEHICLE_EQUIPMENT_STARTAUTOPREPARE��WM_DIESEL_VEHICLE_EQUIPMENT_STARTAUTOPREPARE
*
* �汾��1.0.27
* ���ڣ�2011-03-08
* ���ߣ�Ming
* ������1)�޸Ķ��峵�����״̬��״̬6Ϊ����ȡ����⣨CANCEL_TEST��
*
* �汾��1.0.26
* ���ڣ�2010-09-19
* ���ߣ�hemincong
* ������1)���'`~'��ӳ����Ϣ#define WM_HOTKEY_VK_OEM_3			WM_USER+0x070F
*
* �汾��1.0.25
* ���ڣ�2010-09-14
* ���ߣ�hemincong
* ������1)���������ɨ���ǵ���Ϣ�궨��
*
* �汾��1.0.24
* ���ڣ�2010-07-11
* ���ߣ�Cui
* �������ڱ��ļ���Ӵ��룺#include "NHStructDef.h"
*
* �汾��1.0.23
* ���ڣ�2010-07-2
* ���ߣ�Ming
* ������1)ASM������ӿ�ʼ5025���Ϳ�ʼ2540���궨��,LUGDOWN������ӿ�ʼVELMAXHP����ʼ90VELMAXHP���̡���ʼ80VELMAXHP���̺궨��
*
* �汾��1.0.22
* ���ڣ�2010-06-21
* ���ߣ�Ming
* ������1)��Ӱ�ȫ��ʾ���̻ص���Ϣ�궨��
*
* �汾��1.0.19
* ʱ�䣺2010-05-27
* ���ߣ�Raylee
* ��������������Ʊ궨��Ŀ��ö�ٶ���
*
* �汾��1.0.18
* ʱ�䣺2010-05-22
* ���ߣ�Raylee
* ��������������Ʊ궨�Ļص���Ϣ�궨��
*
* �汾��1.0.17
* ʱ�䣺2010-05-18
* ���ߣ�Raylee
* ��������Ӹ��������궨�Ļص���Ϣ�궨��
*
* �汾��1.0.16
* ʱ�䣺2010-05-12
* ���ߣ�Hyh
* ������1)��Ӳ����ɼ�����Ϣ����
*
* �汾��1.0.15
* ʱ�䣺2010-05-07
* ���ߣ�Hyh
* ������1)��Ӽ���ӳ����Ϣ����
*
* �汾��1.0.14
* ʱ�䣺2010-05-04
* ���ߣ�Hyh
* ������1)��Ӹ���ⷽ��ʵʱ״̬�ص���Ϣ
*
* �汾��1.0.13
* ʱ�䣺2010-04-12
* ���ߣ�Raylee
* ��������Ӽ�����͡��궨��Ŀ�����״̬����ɫ���͵�ö�ٶ���
*
* �汾��1.0.12
* ʱ�䣺2010-01-13
* ���ߣ�Cui
* ��������������д��Dyna����Ϊ��Dyn��
*
* �汾��1.0.11
* ʱ�䣺2010-01-13
* ���ߣ�hyh
* ������1)ͳһ������Ϣ��������(0x0601~0x700)
*       2)����ϵͳ�Լ���Ϣ
*        #define WM_SYSTEM_CHECK_ANA_WORKING			WM_USER + 0x0603
*        #define WM_SYSTEM_CHECK_FLO_WORKING			WM_USER + 0x0604
*        #define WM_SYSTEM_CHECK_DYN_WORKING			WM_USER + 0x0605
*        #define WM_SYSTEM_CHECK_OPA_WORKING			WM_USER + 0x0606
*
* �汾��1.0.10
* ʱ�䣺2009-12-09
* ���ߣ�hyh
* ���������VMASTEST��Ϣ����
*
* �汾��1.0.9
* ʱ�䣺2009-12-08
* ���ߣ�Cui
* ���������¶����豸�첽������Ϣ�����涨����
*
* �汾��1.0.8
* ʱ�䣺2009-11-10
* ���ߣ�hyh
* ������ �޸� WM_ANALYZER_PREPARE Ϊ WM_GASOLINE_VEHICLE_EQUIPMENT_PREPARE
*        �޸� WM_OPACIMETER_PREPARE Ϊ WM_DIESEL_VEHICLE_EQUIPMENT_PREPARE
*        ɾ�� WM_FLOWMETER_PREPARE
*
* �汾��1.0.7
* ʱ�䣺2009-11-09
* ���ߣ�Cui
* ��������͸��NHT1L�첽У׼��Ϣ:WM_NHT1L_ADJUST->WM_NHT1L_CALIBRATE
*       �⹦���첽ȡ״̬��Ϣ:WM_NHC_GET_STATE->WM_NHC_GET_STATUS
*
* �汾��1.0.6
* ʱ�䣺2009-11-09
* ���ߣ�Cui
* ����������LUGDOWN�ŷż�����̺궨��
*
* �汾��1.0.5
* ʱ�䣺2009-11-07
* ���ߣ�Cui
* ����������ASM�ŷż�����̺궨��
*
* �汾��1.0.4
* ʱ�䣺2009-11-07
* ���ߣ�hyh
* ����������תwchart���ȶ���ת��NHStructDef.h
*
* �汾��1.0.3
* ʱ�䣺2009-11-03
* ���ߣ�hyh
* ��������������豸׼����Ϣ���������תwchart���ȶ���
*
* �汾��1.0.2
* ʱ�䣺2009-10-28
* ���ߣ�Cui
* �������������ɼ����ŷż�����̺궨��
*
* �汾��1.0.1
* ʱ�䣺2009-10-26
* ���ߣ�Cui
* ����������˫�����ŷż�����̺궨��
*
* �汾��1.0.0
* ʱ�䣺2009-09-12
* ���ߣ�Cui
* ���������,����ʹ��
*************************************************/

#ifndef NHDEF_H
#define NHDEF_H

#include "NHStructDef.h"
#include "NHEngPro.h"
#include "FuncSwitch.h"

// �궨��
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// LUGDOWN������̺궨��begin
#ifndef NH_APP_TEST_ERROR
#define NH_APP_TEST_ERROR	0xFF // �������
#endif

#ifndef NH_APP_TEST_FINISH
#define NH_APP_TEST_FINISH	0x00 // ������
#endif

#ifndef NH_APP_TEST_EXIT
#define NH_APP_TEST_EXIT	0x01 // ����˳�
#endif

#ifndef NH_APP_TEST_PREV
#define NH_APP_TEST_PREV	0x02 // ��һ��
#endif

#ifndef NH_APP_TEST_FINISH_ALL
#define NH_APP_TEST_FINISH_ALL	0x03 // ������(�������״̬��Ϊ���)
#endif

#ifndef NH_APP_STATUS_START_TEST
#define NH_APP_STATUS_START_TEST 0x03 // ���״̬-��ʼ
#endif

#ifndef NH_APP_STATUS_STOP_TEST
#define NH_APP_STATUS_STOP_TEST 0x04 // ���״̬-ֹͣ
#endif

#ifndef NH_APP_STATUS_FINISH_TEST
#define NH_APP_STATUS_FINISH_TEST 0x05 // ���״̬-����
#endif

#ifndef NH_APP_STATUS_REALTIME_MESSAGE
#define NH_APP_STATUS_REALTIME_MESSAGE 0x06 // ���״̬-ʵʱ��Ϣ
#endif

#ifndef NH_OBD_TEST_ERROR
#define NH_OBD_TEST_ERROR	0xFF // �������
#endif

#ifndef NH_OBD_TEST_FINISH
#define NH_OBD_TEST_FINISH	0x00 // ������
#endif

#ifndef NH_OBD_TEST_EXIT
#define NH_OBD_TEST_EXIT	0x01 // ����˳�
#endif

#ifndef NH_OBD_TEST_PREV
#define NH_OBD_TEST_PREV	0x02 // ��һ��
#endif

#ifndef NH_OBD_TEST_FINISH_ALL
#define NH_OBD_TEST_FINISH_ALL	0x03 // ������(�������״̬��Ϊ���)
#endif

#ifndef NH_OBD_STATUS_START_TEST
#define NH_OBD_STATUS_START_TEST 0x03 // ���״̬-��ʼ
#endif

#ifndef NH_OBD_STATUS_STOP_TEST
#define NH_OBD_STATUS_STOP_TEST 0x04 // ���״̬-ֹͣ
#endif

#ifndef NH_OBD_STATUS_FINISH_TEST
#define NH_OBD_STATUS_FINISH_TEST 0x05 // ���״̬-����
#endif

#ifndef NH_OBD_STATUS_REALTIME_MESSAGE
#define NH_OBD_STATUS_REALTIME_MESSAGE 0x06 // ���״̬-ʵʱ��Ϣ
#endif

#ifndef NH_LUGDOWN_TEST_ERROR
#define NH_LUGDOWN_TEST_ERROR	0xFF // �������
#endif

#ifndef NH_LUGDOWN_TEST_FINISH
#define NH_LUGDOWN_TEST_FINISH	0x00 // ������
#endif

#ifndef NH_LUGDOWN_TEST_EXIT
#define NH_LUGDOWN_TEST_EXIT	0x01 // ����˳�
#endif

#ifndef NH_LUGDOWN_TEST_PREV
#define NH_LUGDOWN_TEST_PREV	0x02 // ��һ��
#endif

#ifndef NH_LUGDOWN_RESULT_ERROR
#define NH_LUGDOWN_RESULT_ERROR		0xFF // �������
#endif

#ifndef NH_LUGDOWN_RESULT_CONTINUE
#define NH_LUGDOWN_RESULT_CONTINUE	0x00 // �����ɼ������
#endif

#ifndef NH_LUGDOWN_RESULT_EXIT
#define NH_LUGDOWN_RESULT_EXIT		0x01 // �����ɷ���������
#endif

#ifndef NH_LUGDOWN_STATUS_START_TEST
#define NH_LUGDOWN_STATUS_START_TEST 0x01 // ���״̬-��ʼ
#endif

#ifndef NH_LUGDOWN_STATUS_STOP_TEST
#define NH_LUGDOWN_STATUS_STOP_TEST 0x02 // ���״̬-ֹͣ
#endif

#ifndef NH_LUGDOWN_STATUS_FINISH_TEST
#define NH_LUGDOWN_STATUS_FINISH_TEST 0x03 // ���״̬-����
#endif

#ifndef NH_LUGDOWN_STATUS_REALTIME_DATA
#define NH_LUGDOWN_STATUS_REALTIME_DATA 0x04 // ���״̬-ʵʱ����
#endif

#ifndef NH_LUGDOWN_STATUS_REALTIME_MESSAGE
#define NH_LUGDOWN_STATUS_REALTIME_MESSAGE 0x05 // ���״̬-ʵʱ��Ϣ
#endif

#ifndef NH_LUGDOWN_STATUS_START_VELMAXHP_TEST
#define NH_LUGDOWN_STATUS_START_VELMAXHP_TEST 0x06 // ���״̬-��ʼVELMAXHP����
#endif

#ifndef NH_LUGDOWN_STATUS_START_90VELMAXHP_TEST
#define NH_LUGDOWN_STATUS_START_90VELMAXHP_TEST 0x07 // ���״̬-��ʼ90VELMAXHP����
#endif

#ifndef NH_LUGDOWN_STATUS_START_80VELMAXHP_TEST
#define NH_LUGDOWN_STATUS_START_80VELMAXHP_TEST 0x08 // ���״̬-��ʼ80VELMAXHP����
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// LUGDOWN������̺궨��end

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ASM������̺궨��begin
#ifndef NH_ASM_TEST_ERROR
#define NH_ASM_TEST_ERROR	0xFF // �������
#endif

#ifndef NH_ASM_TEST_FINISH
#define NH_ASM_TEST_FINISH	0x00 // ������
#endif

#ifndef NH_ASM_TEST_EXIT
#define NH_ASM_TEST_EXIT	0x01 // ����˳�
#endif

#ifndef NH_ASM_TEST_PREV
#define NH_ASM_TEST_PREV	0x02 // ��һ��
#endif

#ifndef NH_ASM_RESULT_ERROR
#define NH_ASM_RESULT_ERROR		0xFF // �������
#endif

#ifndef NH_ASM_RESULT_CONTINUE
#define NH_ASM_RESULT_CONTINUE	0x00 // �����ɼ������
#endif

#ifndef NH_ASM_RESULT_EXIT
#define NH_ASM_RESULT_EXIT		0x01 // �����ɷ���������
#endif

#ifndef NH_ASM_STATUS_START_TEST
#define NH_ASM_STATUS_START_TEST 0x01 // ���״̬-��ʼ���
#endif

#ifndef NH_ASM_STATUS_STOP_TEST
#define NH_ASM_STATUS_STOP_TEST 0x02 // ���״̬-ֹͣ���
#endif

#ifndef NH_ASM_STATUS_FINISH_TEST
#define NH_ASM_STATUS_FINISH_TEST 0x03 // ���״̬-������
#endif

#ifndef NH_ASM_STATUS_REALTIME_DATA
#define NH_ASM_STATUS_REALTIME_DATA 0x04 // ���״̬-ʵʱ����
#endif

#ifndef NH_ASM_STATUS_REALTIME_MESSAGE
#define NH_ASM_STATUS_REALTIME_MESSAGE 0x05 // ���״̬-ʵʱ��Ϣ
#endif

#ifndef NH_ASM_STATUS_REALTIME_VELOCITY
#define NH_ASM_STATUS_REALTIME_VELOCITY 0x06 // ���״̬-ʵʱ�ٶ�
#endif

#ifndef NH_ASM_STATUS_START_5025_TEST
#define NH_ASM_STATUS_START_5025_TEST 0x07 // ���״̬-��ʼ5025���
#endif

#ifndef NH_ASM_STATUS_START_2540_TEST
#define NH_ASM_STATUS_START_2540_TEST 0x08 // ���״̬-��ʼ2540���
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ASM������̺궨��end

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// VMAS������̺궨��begin
#ifndef NH_VMAS_TEST_ERROR
#define NH_VMAS_TEST_ERROR	0xFF // �������
#endif

#ifndef NH_VMAS_TEST_FINISH
#define NH_VMAS_TEST_FINISH	0x00 // ������
#endif

#ifndef NH_VMAS_TEST_EXIT
#define NH_VMAS_TEST_EXIT	0x01 // ����˳�
#endif

#ifndef NH_VMAS_TEST_PREV
#define NH_VMAS_TEST_PREV	0x02 // ��һ��
#endif

#ifndef NH_VMAS_RESULT_ERROR
#define NH_VMAS_RESULT_ERROR		0xFF // �������
#endif

#ifndef NH_VMAS_RESULT_CONTINUE
#define NH_VMAS_RESULT_CONTINUE	0x00 // �����ɼ������
#endif

#ifndef NH_VMAS_RESULT_EXIT
#define NH_VMAS_RESULT_EXIT		0x01 // �����ɷ���������
#endif

#ifndef NH_VMAS_STATUS_START_TEST
#define NH_VMAS_STATUS_START_TEST 0x01 // ���״̬-��ʼ
#endif

#ifndef NH_VMAS_STATUS_STOP_TEST
#define NH_VMAS_STATUS_STOP_TEST 0x02 // ���״̬-ֹͣ
#endif

#ifndef NH_VMAS_STATUS_FINISH_TEST
#define NH_VMAS_STATUS_FINISH_TEST 0x03 // ���״̬-����
#endif

#ifndef NH_VMAS_STATUS_REALTIME_DATA
#define NH_VMAS_STATUS_REALTIME_DATA 0x04 // ���״̬-ʵʱ����
#endif

#ifndef NH_VMAS_STATUS_TESTING_15KM
#define NH_VMAS_STATUS_TESTING_15KM 0x05 // ���״̬-����15����
#endif

#ifndef NH_VMAS_STATUS_TESTING_32KM
#define NH_VMAS_STATUS_TESTING_32KM 0x06 // ���״̬-ʵʱ����
#endif

#ifndef NH_VMAS_STATUS_TESTING_50KM
#define NH_VMAS_STATUS_TESTING_50KM 0x07 // ���״̬-ʵʱ����
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// VMAS������̺궨��end


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ˫���ټ�����̺궨��begin
#ifndef NH_DIS_TEST_ERROR
#define NH_DIS_TEST_ERROR	0xFF // �������
#endif

#ifndef NH_DIS_TEST_FINISH
#define NH_DIS_TEST_FINISH	0x00 // ������
#endif

#ifndef NH_DIS_TEST_EXIT
#define NH_DIS_TEST_EXIT	0x01 // ����˳�
#endif

#ifndef NH_DIS_TEST_PREV
#define NH_DIS_TEST_PREV	0x02 // ��һ��
#endif

#ifndef NH_DIS_RESULT_ERROR
#define NH_DIS_RESULT_ERROR		0xFF // �������
#endif

#ifndef NH_DIS_RESULT_CONTINUE
#define NH_DIS_RESULT_CONTINUE	0x00 // �����ɼ������
#endif

#ifndef NH_DIS_RESULT_EXIT
#define NH_DIS_RESULT_EXIT		0x01 // �����ɷ���������
#endif

#ifndef NH_DIS_STATUS_START_TEST
#define NH_DIS_STATUS_START_TEST 0x01 // ���״̬-��ʼ
#endif

#ifndef NH_DIS_STATUS_STOP_TEST
#define NH_DIS_STATUS_STOP_TEST 0x02 // ���״̬-ֹͣ
#endif

#ifndef NH_DIS_STATUS_FINISH_TEST
#define NH_DIS_STATUS_FINISH_TEST 0x03 // ���״̬-����
#endif

#ifndef NH_DIS_STATUS_REALTIME_DATA_HIS
#define NH_DIS_STATUS_REALTIME_DATA_HIS 0x04 // ���״̬-ʵʱ����(�ߵ���)
#endif

#ifndef NH_DIS_STATUS_REALTIME_DATA_IS
#define NH_DIS_STATUS_REALTIME_DATA_IS 0x05 // ���״̬-ʵʱ����(����)
#endif

#ifndef NH_DIS_STATUS_REALTIME_MESSAGE
#define NH_DIS_STATUS_REALTIME_MESSAGE 0x06 // ���״̬-ʵʱ��Ϣ
#endif

#ifndef NH_DIS_STATUS_REALTIME_REV
#define NH_DIS_STATUS_REALTIME_REV 0x07 // ���״̬-ʵʱת��
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ˫���ټ�����̺궨��end

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ���ɼ��ټ�����̺궨��begin
#ifndef NH_FSU_TEST_ERROR
#define NH_FSU_TEST_ERROR	0xFF // �������
#endif

#ifndef NH_FSU_TEST_FINISH
#define NH_FSU_TEST_FINISH	0x00 // ������
#endif

#ifndef NH_FSU_TEST_EXIT
#define NH_FSU_TEST_EXIT	0x01 // ����˳�
#endif

#ifndef NH_FSU_TEST_PREV
#define NH_FSU_TEST_PREV	0x02 // ��һ��
#endif

#ifndef NH_FSU_RESULT_ERROR
#define NH_FSU_RESULT_ERROR		0xFF // �������
#endif

#ifndef NH_FSU_RESULT_CONTINUE
#define NH_FSU_RESULT_CONTINUE	0x00 // �����ɼ������
#endif

#ifndef NH_FSU_RESULT_EXIT
#define NH_FSU_RESULT_EXIT		0x01 // �����ɷ���������
#endif

#ifndef NH_FSU_STATUS_START_TEST
#define NH_FSU_STATUS_START_TEST 0x01 // ���״̬-��ʼ
#endif

#ifndef NH_FSU_STATUS_STOP_TEST
#define NH_FSU_STATUS_STOP_TEST 0x02 // ���״̬-ֹͣ
#endif

#ifndef NH_FSU_STATUS_FINISH_TEST
#define NH_FSU_STATUS_FINISH_TEST 0x03 // ���״̬-����
#endif

#ifndef NH_FSU_STATUS_REALTIME_DATA
#define NH_FSU_STATUS_REALTIME_DATA 0x04 // ���״̬-ʵʱ����
#endif

#ifndef NH_FSU_STATUS_REALTIME_MESSAGE
#define NH_FSU_STATUS_REALTIME_MESSAGE 0x05 // ���״̬-ʵʱ��Ϣ
#endif

#ifndef NH_FSU_STATUS_REALTIME_REV
#define NH_FSU_STATUS_REALTIME_REV 0x06 // ���״̬-ʵʱת��
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ���ɼ��ټ�����̺궨��end

///////////////////////////////////////////////// Ħ�г����ټ�����̺궨��begin
#ifndef NH_MI_TEST_ERROR
#define NH_MI_TEST_ERROR	0xFF // �������
#endif

#ifndef NH_MI_TEST_FINISH
#define NH_MI_TEST_FINISH	0x00 // ������
#endif

#ifndef NH_MI_TEST_EXIT
#define NH_MI_TEST_EXIT	0x01 // ����˳�
#endif

#ifndef NH_MI_TEST_PREV
#define NH_MI_TEST_PREV	0x02 // ��һ��
#endif

#ifndef NH_MI_RESULT_ERROR
#define NH_MI_RESULT_ERROR		0xFF // �������
#endif

#ifndef NH_MI_RESULT_CONTINUE
#define NH_MI_RESULT_CONTINUE	0x00 // �����ɼ������
#endif

#ifndef NH_MI_RESULT_EXIT
#define NH_MI_RESULT_EXIT		0x01 // �����ɷ���������
#endif

#ifndef NH_MI_STATUS_START_TEST
#define NH_MI_STATUS_START_TEST 0x01 // ���״̬-��ʼ
#endif

#ifndef NH_MI_STATUS_STOP_TEST
#define NH_MI_STATUS_STOP_TEST 0x02 // ���״̬-ֹͣ
#endif

#ifndef NH_MI_STATUS_FINISH_TEST
#define NH_MI_STATUS_FINISH_TEST 0x03 // ���״̬-����
#endif

#ifndef NH_MI_STATUS_REALTIME_DATA_IS1
#define NH_MI_STATUS_REALTIME_DATA_IS1 0x04 // ���״̬-ʵʱ����(����1)
#endif

#ifndef NH_MI_STATUS_REALTIME_DATA_IS2
#define NH_MI_STATUS_REALTIME_DATA_IS2 0x05 // ���״̬-ʵʱ����(����2)
#endif

#ifndef NH_MI_STATUS_REALTIME_MESSAGE
#define NH_MI_STATUS_REALTIME_MESSAGE 0x06 // ���״̬-ʵʱ��Ϣ
#endif

#ifndef NH_MI_STATUS_REALTIME_REV
#define NH_MI_STATUS_REALTIME_REV 0x07 // ���״̬-ʵʱת��
#endif
//////////////////////////////////////////////// Ħ�г�������̺궨��end

//////////////////////////////////////////////// Ħ�г�˫���ټ�����̺궨��begin
#ifndef NH_MDIS_TEST_ERROR
#define NH_MDIS_TEST_ERROR	0xFF // �������
#endif

#ifndef NH_MDIS_TEST_FINISH
#define NH_MDIS_TEST_FINISH	0x00 // ������
#endif

#ifndef NH_MDIS_TEST_EXIT
#define NH_MDIS_TEST_EXIT	0x01 // ����˳�
#endif

#ifndef NH_MDIS_TEST_PREV
#define NH_MDIS_TEST_PREV	0x02 // ��һ��
#endif

#ifndef NH_MDIS_RESULT_ERROR
#define NH_MDIS_RESULT_ERROR		0xFF // �������
#endif

#ifndef NH_MDIS_RESULT_CONTINUE
#define NH_MDIS_RESULT_CONTINUE	0x00 // �����ɼ������
#endif

#ifndef NH_MDIS_RESULT_EXIT
#define NH_MDIS_RESULT_EXIT		0x01 // �����ɷ���������
#endif

#ifndef NH_MDIS_STATUS_START_TEST
#define NH_MDIS_STATUS_START_TEST 0x01 // ���״̬-��ʼ
#endif

#ifndef NH_MDIS_STATUS_STOP_TEST
#define NH_MDIS_STATUS_STOP_TEST 0x02 // ���״̬-ֹͣ
#endif

#ifndef NH_MDIS_STATUS_FINISH_TEST
#define NH_MDIS_STATUS_FINISH_TEST 0x03 // ���״̬-����
#endif

#ifndef NH_MDIS_STATUS_REALTIME_DATA_HIS
#define NH_MDIS_STATUS_REALTIME_DATA_HIS 0x04 // ���״̬-ʵʱ����(�ߵ���)
#endif

#ifndef NH_MDIS_STATUS_REALTIME_DATA_IS
#define NH_MDIS_STATUS_REALTIME_DATA_IS 0x05 // ���״̬-ʵʱ����(����)
#endif

#ifndef NH_MDIS_STATUS_REALTIME_MESSAGE
#define NH_MDIS_STATUS_REALTIME_MESSAGE 0x06 // ���״̬-ʵʱ��Ϣ
#endif

#ifndef NH_MDIS_STATUS_REALTIME_REV
#define NH_MDIS_STATUS_REALTIME_REV 0x07 // ���״̬-ʵʱת��
#endif
//////////////////////////////////////////////// Ħ�г�˫���ټ�����̺궨��end

/////////////////////////////////////////////// ���׼�����̺궨��begin
#ifndef NH_TESTPREPARE_STATUS_REALTIME_MESSAGE
#define NH_TESTPREPARE_STATUS_REALTIME_MESSAGE 0x04  //���״̬-��ǰ��Ϣ
#endif
#ifndef NH_TESTPREPARE_STATUS_REALTIME_MESSAGE_DRIVER
#define NH_TESTPREPARE_STATUS_REALTIME_MESSAGE_DRIVER 0x05  //���״̬-��ǰ����Ա
#endif
#ifndef NH_TESTPREPARE_STATUS_REALTIME_MESSAGE_RN
#define NH_TESTPREPARE_STATUS_REALTIME_MESSAGE_RN 0x06  //���״̬-��ǰ��ˮ��
#endif
////////////////////////////////////////////// ���׼�����̺궨��end

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ��ȫ�����ʾ���̺궨��begin
#ifndef NH_SAFE_WARNING_PREV
#define NH_SAFE_WARNING_PREV        0x01     // ��һ��
#endif

#ifndef NH_SAFE_WARNING_NEXT
#define NH_SAFE_WARNING_NEXT        0x02     // ��һ��
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ��ȫ�����ʾ���̺궨��end

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// �⹦�����ػ������̺궨��begin
#define NH_DYN_CL_START_LOW			0x01	// ���ػ���(����)-��ʼ
#define NH_DYN_CL_START_HIGH		0x02	// ���ػ���(����)-��ʼ
#define NH_DYN_CL_STOP				0x03	// ���ػ���-ֹͣ
#define NH_DYN_CL_FINISH			0x04	// ���ػ���-���
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// �⹦�����ػ������̺궨��end

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// �⹦���������ʻ������̺궨��begin
#define NH_DYN_PLHP_START			0x01	// �������ʻ���-��ʼ
#define NH_DYN_PLHP_STOP			0x02	// �������ʻ���-ֹͣ
#define NH_DYN_PLHP_FINISH			0x03	// �������ʻ���-���
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// �⹦���������ʻ������̺궨��end

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// �⹦����У׼���̺궨��begin
#define NH_DYN_FORCE_CAL_START		0x01	// ��У׼-��ʼ
#define NH_DYN_FORCE_CAL_STOP		0x02	// ��У׼-ֹͣ
#define NH_DYN_FORCE_CAL_FINISH		0x03	// ��У׼-���
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// �⹦����У׼���̺궨��end

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// �⹦�������������̺궨��begin
#define NH_DYN_DIW_START		0x01	// ��������-��ʼ
#define NH_DYN_DIW_STOP		0x02	// ��������-ֹͣ
#define NH_DYN_DIW_FINISH		0x03	// ��������-���
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// �⹦�������������̺궨��end

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// �⹦����������̺궨��begin
#define NH_DYN_FORCE_CHK_START		0x01	// �����-��ʼ
#define NH_DYN_FORCE_CHK_STOP		0x02	// �����-ֹͣ
#define NH_DYN_FORCE_CHK_FINISH		0x03	// �����-���
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// �⹦����������̺궨��end

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ����������У׼���̺궨��begin
#define NH_ANA_GAS_CAL_START		0x01	// ����У׼-��ʼ
#define NH_ANA_GAS_CAL_STOP			0x02	// ����У׼-ֹͣ
#define NH_ANA_GAS_CAL_FINISH		0x03	// ����У׼-���
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ����������У׼���̺궨��end

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ���������������̺궨��begin
#define NH_ANA_GAS_CHK_START		0x01	// ������-��ʼ
#define NH_ANA_GAS_CHK_STOP			0x02	// ������-ֹͣ
#define NH_ANA_GAS_CHK_FINISH		0x03	// ������-���
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ���������������̺궨��end

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ��͸��У׼������̺궨��begin
#define NH_OPA_CAL_CHK_START		0x01	// ��͸��У׼���-��ʼ
#define NH_OPA_CAL_CHK_STOP			0x02	// ��͸��У׼���-ֹͣ
#define NH_OPA_CAL_CHK_FINISH		0x03	// ��͸��У׼���-���
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ��͸��У׼������̺궨��end

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ��͸�����Լ�����̺궨��begin
#define NH_OPA_LINE_CHK_START		0x01	// ��͸�����Լ��-��ʼ
#define NH_OPA_LINE_CHK_STOP		0x02	// ��͸�����Լ��-ֹͣ
#define NH_OPA_LINE_CHK_FINISH		0x03	// ��͸�����Լ��-���
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ��͸�����Լ�����̺궨��end

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// �̶ȼƼ�����̺궨��begin
#define NH_SMO_CHK_START			0x01	// �̶ȼƼ��-��ʼ
#define NH_SMO_CHK_STOP				0x02	// �̶ȼƼ��-ֹͣ
#define NH_SMO_CHK_FINISH			0x03	// �̶ȼƼ��-���
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// �̶ȼƼ�����̺궨��end

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ����������У׼���̺궨��begin
#define NH_FLO_FLUXCAL_START			0x01	// ����������У׼-��ʼ
#define NH_FLO_FLUXCAL_STOP				0x02	// ����������У׼-ֹͣ
#define NH_FLO_FLUXCAL_FINISH			0x03	// ����������У׼-���
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ����������У׼���̺궨��end

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ����������������̺궨��begin
#define NH_FLO_FLUXCHK_START			0x01	// �������������-��ʼ
#define NH_FLO_FLUXCHK_STOP				0x02	// �������������-ֹͣ
#define NH_FLO_FLUXCHK_FINISH			0x03	// �������������-���
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ����������������̺궨��end

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ������ϡ����У׼���̺궨��begin
#define NH_FLO_DILO2CAL_START			0x01	// ������ϡ����У׼-��ʼ
#define NH_FLO_DILO2CAL_STOP			0x02	// ������ϡ����У׼-ֹͣ
#define NH_FLO_DILO2CAL_FINISH			0x03	// ������ϡ����У׼-���
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ������ϡ����У׼���̺궨��end

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ������ѹ��У׼���̺궨��begin
#define NH_FLO_PRESCAL_START			0x01	// ������ѹ��У׼-��ʼ
#define NH_FLO_PRESCAL_STOP				0x02	// ������ѹ��У׼-ֹͣ
#define NH_FLO_PRESCAL_FINISH			0x03	// ������ѹ��У׼-���
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ������ѹ��У׼���̺궨��end

#ifndef ERROR_MAX
#define ERROR_MAX 1024 // �����������ַ���
#endif

#ifndef SQLMAX
#define SQLMAX 1024 // sql �����������ַ���
#endif


// ��Ϣ����

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// �⹦������begin
// �涨��WM_USER + 0x0001 �� WM_USER + 0x0100
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////ִ�в⹦���첽������Ϣbegin
#define WM_DYN_GET_REALTIME_DATA				WM_USER + 0x0001  // ȡ�⹦��ʵʱ����
#define WM_DYN_GET_STATUS						WM_USER + 0x0002  // ȡ�⹦��״̬
#define WM_DYN_READ_VAR						    WM_USER + 0x0003  // ���⹦������
#define WM_DYN_WRITE_VAR						WM_USER + 0x0004  // д�⹦������
#define WM_DYN_SEND_CONTROL_WORD				WM_USER + 0x0005  // ���Ͳ⹦�������ַ�
#define WM_DYN_START_CONST_LOAD_TEST			WM_USER + 0x0006  // ��ʼ�⹦�����غɲ���
#define WM_DYN_CONST_LOAD_TEST_FINISHED		    WM_USER + 0x0007  // �⹦�����غɲ������
#define WM_DYN_START_DIW_TEST					WM_USER + 0x0008  // ��ʼ�⹦��������������
#define WM_DYN_START_PLHP_TEST					WM_USER + 0x0009  // ��ʼ�⹦���������ʲ���
#define WM_DYN_PLHP_TEST_FINISHED				WM_USER + 0x000A  // �⹦���������ʲ������
#define WM_DYN_PLHP_SECTION_FINISHED			WM_USER + 0x000B  // ��������ĳһ���ٶȶ����
#define WM_DYN_START_VARLOAD_TEST				WM_USER + 0x000C  // ��ʼ�⹦��������������
#define WM_DYN_VARLOAD_TEST_FINISHED			WM_USER + 0x000E  // �⹦���������ʲ������
#define WM_DYN_START_RESPONSE_TIME_TEST			WM_USER + 0x000F  // ��ʼ�⹦����Ӧʱ�����
#define WM_DYN_START_RESPONSE_TIME_FINISHED		WM_USER + 0x0010  // ��ʼ�⹦����Ӧʱ�����
#define WM_DYN_SAVE_RESPONSE_TIME_DATA		    WM_USER + 0x0011  // ����⹦����Ӧʱ��ʵʱ����

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////ִ�в⹦���첽������Ϣend
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// �⹦������end


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// �����ǲ���begin
// �涨��WM_USER + 0x0101 �� WM_USER + 0x0200
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////ִ�з������첽������Ϣbegin
#define WM_ANA_GET_MAJOR_DATA                  WM_USER + 0x0101  // ȡ������
#define WM_ANA_GET_MINOR_DATA			       WM_USER + 0x0102  // ȡ��������
#define WM_ANA_GET_PEF		                   WM_USER + 0x0103  // ȡPefֵ
#define WM_ANA_GET_STATUS	                   WM_USER + 0x0104  // ȡ����״̬
#define WM_ANA_GASCALIBRATION                  WM_USER + 0x0105  // ������У׼
#define WM_ANA_GASCHECK                        WM_USER + 0x0106  // ������У׼
#define WM_ANA_GET_MAJOR_DATA_N                WM_USER + 0x0107  // ȡ�����ݣ�֧��ȡˮλ��Ϣ��
#define WM_ANA_GET_NOX_DATA					   WM_USER + 0x0108  // ȡNOx����
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////ִ�з������첽������Ϣend
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// �����ǲ���end


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// �����Ʋ���begin
// �涨��WM_USER + 0x0201 �� WM_USER + 0x0300
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////ִ���������첽������Ϣbegin
#define WM_ANA_VMAS_REALTIME_DATA				WM_USER + 0x0201  // Vmasʵʱ����
#define WM_ANA_VMAS_PDS_WT_DATA					WM_USER + 0x0202  // Vmas195����������
#define WM_ANA_VMAS_PDS_CT_DATA					WM_USER + 0x0203  // Vmas195��Ũ������
#define WM_ANA_VMAS_TOTAL_WT_DATA				WM_USER + 0x0204  // Vmas�ܿ���
#define WM_ANA_VMAS_START_COLLECT_DATA			WM_USER + 0x0205  // ��ʼ�ɼ�����
#define WM_ANA_VMAS_STOP_COLLECT_DATA			WM_USER + 0x0206  // ֹͣ�ɼ�����
#define WM_ANA_VMAS_START_GETENVIRONMENT_O2		WM_USER + 0x0207  // ��ʼ�ɼ�������
#define WM_ANA_VMAS_GET_ENVIRONMENT_O2			WM_USER + 0x0208  // ��ȡ������
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////ִ���������첽������Ϣend
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// �����Ʋ���end


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ��͸��ȼƲ���begin
// �涨��WM_USER + 0x0301 �� WM_USER + 0x0400
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////ִ�в�͸��ȼ��첽������Ϣbegin
#define WM_OPA_CALIBRATE			        WM_USER + 0x0301  // У׼
#define WM_OPA_GET_REALTIME_DATA			WM_USER + 0x0302  // ȡNHT1Lʵʱ����
#define WM_OPA_GET_MAX_VALUE				WM_USER + 0x0303  // ȡNHT1L���ֵ����
#define WM_OPA_GET_ENV_REALTIME_DATA		WM_USER + 0x0304  // ȡNHT1Lʵʱ��������
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////ִ�в�͸��ȼ��첽������Ϣend
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ��͸��ȼƲ���end


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// �̶ȼƲ���begin
// �涨��WM_USER + 0x0401 �� WM_USER + 0x0500
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////ִ���̶ȼ��첽������Ϣbegin
#define WM_SMO_RESET			            WM_USER + 0x0401  // ��λ
#define WM_SMO_GET_DATA1234					WM_USER + 0x0402  // ��ȡʵʱ����1234
#define WM_SMO_REPEAT_BEGIN                 WM_USER + 0x0403
#define WM_SMO_REAL_BEGIN                   WM_USER + 0x0404
#define WM_SMO_REAL_END                     WM_USER + 0x0405
#define WM_SMO_REPEAT_END                   WM_USER + 0x0406
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////ִ���̶ȼ��첽������Ϣend
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// �̶ȼƲ���end


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ת�ټƲ���begin
// �涨��WM_USER + 0x0501 �� WM_USER + 0x0600
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////ִ��ת�ټ��첽������Ϣbegin
#define WM_TAC_GET_REV			WM_USER + 0x0501
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////ִ��ת�ټ��첽������Ϣend
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ת�ټƲ���end


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ������Ϣ����begin
// �涨��WM_USER + 0x0601 �� WM_USER + 0x0700(������Ϣ����)

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////ִ���豸׼����Ϣbegin
#define WM_GASOLINE_VEHICLE_EQUIPMENT_PREPARE		WM_USER + 0x0601
#define WM_GASOLINE_VEHICLE_EQUIPMENT_STARTAUTOPREPARE		WM_USER + 0x0602
#define WM_DIESEL_VEHICLE_EQUIPMENT_PREPARE			WM_USER + 0x0603
#define WM_DIESEL_VEHICLE_EQUIPMENT_STARTAUTOPREPARE		WM_USER + 0x0604
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////ִ���豸׼����Ϣend

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////ִ��ϵͳ�Լ���Ϣbegin
#define WM_SYSTEM_CHECK_ANA_WORKING			WM_USER + 0x0603
#define WM_SYSTEM_CHECK_FLO_WORKING			WM_USER + 0x0604
#define WM_SYSTEM_CHECK_DYN_WORKING			WM_USER + 0x0605
#define WM_SYSTEM_CHECK_OPA_WORKING			WM_USER + 0x0606
#define WM_SYSTEM_CHECK_NHV_WORKING			WM_USER + 0x0607
#define WM_SYSTEM_CHECK_RPM_WORKING			WM_USER + 0x0608
#define WM_SYSTEM_CHECK_ANA_TESTCOMMUNICATION			WM_USER + 0x0609
#define WM_SYSTEM_CHECK_NOX_WORKING			WM_USER + 0x060A

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////ִ��ϵͳ�Լ���Ϣend

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ������Ϣ����end

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ����ӳ����Ϣ����begin
#define WM_HOTKEY_VK_F2				WM_USER+0x0701
#define WM_HOTKEY_VK_F3				WM_USER+0x0702
#define WM_HOTKEY_VK_F4				WM_USER+0x0703
#define WM_HOTKEY_VK_F5				WM_USER+0x0704
#define WM_HOTKEY_VK_F6				WM_USER+0x0705
#define WM_HOTKEY_VK_F7				WM_USER+0x0706
#define WM_HOTKEY_VK_F8				WM_USER+0x0707
#define WM_HOTKEY_VK_F9				WM_USER+0x0708
#define WM_HOTKEY_VK_F10			    WM_USER+0x0709
#define WM_HOTKEY_VK_F11			    WM_USER+0x070A
#define WM_HOTKEY_VK_F12			    WM_USER+0x070B
#define WM_HOTKEY_VK_OEM_PLUS		    WM_USER+0x070C
#define WM_HOTKEY_VK_OEM_MINUS		WM_USER+0x070D
#define WM_HOTKEY_VK_OEM_5			WM_USER+0x070E
#define WM_HOTKEY_VK_OEM_3			WM_USER+0x070F     //For the US standard keyboard, the '`~' key
#define WM_HOTKEY_VK_ALT_S			WM_USER+0x0710
#define WM_HOTKEY_VK_ALT_C			WM_USER+0x0711
#define WM_HOTKEY_PRINT_SCREEN		WM_USER+0x0712		// ������ݼ���Ϣ
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ����ӳ����Ϣ����end

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// �����ɼ��ǲ���begin
// �涨��WM_USER + 0x0801 �� WM_USER + 0x0900
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////ִ�в����ɼ��ǲ����첽������Ϣbegin
#define WM_PARA_GET_ENTIRE_DATA			            WM_USER + 0x0801  // ��ȫ������
#define WM_PARA_GET_ENVIRONMENT_PARAMETERS			WM_USER + 0x0802  // ����������
#define WM_PARA_GET_OILTEMP_RPM						WM_USER + 0x0803  // ��ת������
#define WM_PARA_DT_COMMAND							WM_USER + 0x0804  // DT����
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////ִ�в����ɼ��ǲ����첽������Ϣend
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// �����ɼ��ǲ���end

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ������ɨ���ǲ���begin
// �涨��WM_USER + 0x0901 �� WM_USER + 0x1000
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////ִ��������ɨ�����첽������Ϣbegin
#define WM_BCS_GET_VALUE			WM_USER + 0x0901
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////ִ��������ɨ�����첽������Ϣend
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ������ɨ���ǲ���end


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// �������Χ����begin
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////����������begin
#define ANA_CO_MAX			1400		// CO���ֵ,�Ŵ�100��,��λ:0.01%
#define ANA_HC_MAX			10000		// HC���ֵ,��λ:10-6
#define ANA_NO_MAX			5000		// NO���ֵ,��λ:10-6
#define ANA_CO2_MAX			1800		// CO2���ֵ,�Ŵ�100��,��λ:0.01%
#define ANA_O2_MAX			2500		// O2���ֵ,�Ŵ�100��,��λ:0.01%
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////����������end
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////��͸��begin
#define OPA_K_MAX			1606		// K���ֵ,�Ŵ�100��,��λ:0.01m-1
#define OPA_N_MAX			9990		// N���ֵ,�Ŵ�100��,��λ:0.01%
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////��͸��end
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////�̶ȼ�begin
#define SMO_RB_MAX			1000		// �̶�ֵ���ֵ,�Ŵ�100��,��λ:0.01Rb
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////�̶ȼ�end
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// �������Χ����end

// ����������
enum NHTestType
{
	ASM = 1,		// ��̬						1
	VMAS,			// ����˲̬					2
	LUGDOWN,		// ���ؼ���					3
	DIS,			// ˫����					4
	FSUNHT,			// ��͸�����ɼ���			5
	FSUYD,			// ��ֽʽ���ɼ���			6
	AVFSU,			// ũ�ó����ɼ���			7
	MI,				// Ħ�г�����				8
	MDIS,			// Ħ�г�˫����				9
};

// ���峵�����״̬
enum NHTestState
{
	WAIT_FOR_TESTING = 0,	// ���������								0
	TESTING,				// ��ȼ�ϳ������ڼ��						1
	TESTING_FUEL1,			// ˫ȼ�ϳ������ڼ���һ��ȼ��				2
	TESTING_FUEL2,			// ˫ȼ�ϳ������ڼ��ڶ���ȼ��				3
	WAIT_FOR_TESTING_FUEL2,	// ˫ȼ�ϳ����ȴ����ڶ���ȼ��				4
	TESTED,					// ȫ����ɼ��								5
	CANCEL_TEST,			// ����ȡ�����								6
	SELECTED_FUEL1,			// ��ȼ�ϳ���ѡ�л�˫ȼ�ϵ�һ��ȼ����ѡ��	7
	WAIT_FOR_CHARGING,		// ����δ�շ�,���շ�						8
	SELECTED_FUEL2,			// ˫ȼ�ϳ����ڶ���ȼ����ѡ��				9
	READ					// ���ŷ������Ѿ���ȡ�ó�����Ϣ				10
};

// ����궨��Ŀ
enum NHDemarcationItem
{
	DYN_CL = 1,		// �⹦�����ػ���		1
	DYN_PLHP,		// �⹦���������ʻ���	2
	DYN_FORCE_CAL,	// �⹦����У׼		3
	DYN_FORCE_CHK,	// �⹦�������		4
	ANA_GAS_CAL,	// ����������У׼		5
	ANA_GAS_CHK,	// ������������		6
	OPA_CAL_CHK,	// ��͸��У׼���		7
	SMO_CHK,		// ��ֽʽ�̶ȼƼ��	8
	FLO_FLUXCAL,	// ����������У׼		9
	FLO_FLUXCHK,	// �������������		10
	FLO_DILO2CAL,	// ������ϡ����У׼	11
	FLO_PRESCAL,	// ������ѹ��У׼		12
	DYN_DIW,		// �⹦����������		13
	NOX_GAS_CAL,	// ��������������У׼	14
	NOX_GAS_CHK,	// ����������������	15
};

// �����ɫ����
enum NHRoleType
{
	EPB = 0,		// ������	0
	STATIONMASTER,	// վ  ��	1
	ADMIN,			// ����Ա	2
	OPERATOR,		// ����Ա	3
	BILLER,			// ��¼Ա	4
	SHROFF,			// �շ�Ա	5
	DRIVER,			// ����Ա	6
	ASSESSOR,		// ���Ա	7
	INSPECTOR,		// ���Ա	8
};

// ��������״̬
enum NHLineState
{
	EQU_SHUTDOWN = 0,		// �豸�ر�	0
	EQU_FREE,				// ����		1
	EQU_TESTING,			// ���ڼ��	2
	EQU_SYSTEMCHECK,		// �����Լ�	3
	EQU_DEMARCATION,		// ���ڱ궨	4
};

// ����������
enum NHTestCharacter
{
	TC_OffLine = 1,		// �³����߼���	1
	TC_Import,			// ���ڳ�����		2
	TC_Register,		// ע��ǼǼ���	3
	TC_InUse,			// ���ó�����		4
};

//���ȣ�3λС��
//HC��86/22.4g/L=3.839
//CO:28/22.4g/L=1.250
//NOx:46/22.4g/L=2.050
//CO2:44/22.4g/L=1.964
//ת��Ϊmg/L
const int nHC_Density = 3839;
const int nCO_Density = 1250;
const int nNOx_Density = 2050;
const int nCO2_Density = 1964;

// ת�ٲ�����ʽ
enum RevMeasureMode
{
	// ��������ת��
	RMM_FromNone = 0,
	// �ӽӵ����ڵ�ת������������ת��
	RMM_FromTachometer,
	// �ӷ����ǲ���ת��
	RMM_FromAnalyzer,
	// �Ӳ�͸��ȼƲ���ת��
	RMM_FromOpacimeter,
	// �Ӳ����ɼ���(˾��������)
	RMM_FromParameter,
	// ��OBD��ȡ
	RMM_FromOBD,
};

// ���²�����ʽ
enum OilTempMeasureMode
{
	// ��������
	OTMM_FromNone = 0,
	// �ӷ����ǲ���ת��
	OTMM_FromAnalyzer,
	// �Ӳ�͸�����ת��
	OTMM_FromOpacimeter,
	// �Ӳ����ɼ���(˾��������)
	OTMM_FromParameter,
	// ��OBD��ȡ
	OTMM_FromOBD,
};

// ��̷�ʽ
enum StrokeType
{
	// 2���
	ST_TwoStroke = 2,
	// 4���
	ST_FourStroke = 4,
};
// ���ʽ
enum FireType
{
	// ���ε��
	FT_SingleFire = 1,
	// ���ε��
	FT_DoubleFire = 2,
};

// �������ĸ�ֵ����,��ֵת��
#define NZ(x)	((x)<0 ? 0 : (x))

#endif
