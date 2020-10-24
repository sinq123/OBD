/*
* Copyright (c) ��ɽ���ϻ������ɷ����޹�˾
* All rights reserved.
*
* �ļ����ƣ�FuncSwitch.h
* �ļ���ʶ��
* ���������ܿ���
*
* �汾��1.0
* ���ڣ�2015-09-28
* ���ߣ�Cyx
*
*/


#ifndef FUNC_SWITCH_H
#define FUNC_SWITCH_H

/////////////////////////VMAS begin

// ������������
#ifndef NH_VMAS_ALLOW_SKIP_OIL_TEMP
#define NH_VMAS_ALLOW_SKIP_OIL_TEMP
#endif

// ��������CO+CO2���
#ifndef NH_VMAS_ALLOW_SKIP_COCO2_CHECK
#define NH_VMAS_ALLOW_SKIP_COCO2_CHECK
#endif

// �������ðٹ����ͺ���Сֵ
#ifndef NH_VMAS_ALLOW_100KM_FULECONSUMPTION_MIN
#define NH_VMAS_ALLOW_100KM_FULECONSUMPTION_MIN
#endif

//// �����޸��ۼƳ���
//#ifndef NH_VMAS_ALLOW_MODIFY_TOTAL_OVER_TIME
//#define NH_VMAS_ALLOW_MODIFY_TOTAL_OVER_TIME
//#endif

//// �����޸���������
//#ifndef NH_VMAS_ALLOW_MODIFY_ONCE_OVER_TIME
//#define NH_VMAS_ALLOW_MODIFY_ONCE_OVER_TIME
//#endif

//// �����޸ĳ��Χ
//#ifndef NH_VMAS_ALLOW_MODIFY_TOLERANCE_RANGE
//#define NH_VMAS_ALLOW_MODIFY_TOLERANCE_RANGE
//#endif

//// ��������95L/s���
//#ifndef NH_VMAS_ALLOW_SKIP_95CHECK
//#define NH_VMAS_ALLOW_SKIP_95CHECK
//#endif

//// ���������������������
//#ifndef NH_VMAS_ALLOW_SKIP_ANA_FLUXCHECK
//#define NH_VMAS_ALLOW_SKIP_ANA_FLUXCHECK
//#endif

// �����޸ķ���������С��ֵ��GB2018�Ѿ���Ҫ��
#ifndef NH_VMAS_ALLOW_MODIFY_MINEXHANSTFLUX
#define NH_VMAS_ALLOW_MODIFY_MINEXHANSTFLUX
#endif

//// �����޸ĵ��ٵȴ�ʱ��
//#ifndef NH_VMAS_ALLOW_MODIFY_IDLE_WAIT_TIME
//#define NH_VMAS_ALLOW_MODIFY_IDLE_WAIT_TIME
//#endif

/////////////////////////VMAS end



/////////////////////////ASM begin

// ��������ת��
#ifndef NH_ASM_ALLOW_SKIP_REV_CHECK
#define NH_ASM_ALLOW_SKIP_REV_CHECK
#endif

// ������������
#ifndef NH_ASM_ALLOW_SKIP_OIL_TEMP
#define NH_ASM_ALLOW_SKIP_OIL_TEMP
#endif

// ��������NO���
#ifndef NH_ASM_ALLOW_SKIP_NO_CHECK
#define NH_ASM_ALLOW_SKIP_NO_CHECK
#endif

//// ��������CO+CO2���
//#ifndef NH_ASM_ALLOW_SKIP_COCO2_CHECK
//#define NH_ASM_ALLOW_SKIP_COCO2_CHECK
//#endif

//// ���������������
//#ifndef NH_ASM_ALLOW_SKIP_FLUX_CHECK
//#define NH_ASM_ALLOW_SKIP_FLUX_CHECK
//#endif

//// ����������Ч�����ж�
//#ifndef NH_ASM_ALLOW_SKIP_VALID_DATA_JUD
//#define NH_ASM_ALLOW_SKIP_VALID_DATA_JUD
//#endif

//// ��������Ť�س���
//#ifndef NH_ASM_ALLOW_SKIP_TORQUE_ERROR
//#define NH_ASM_ALLOW_SKIP_TORQUE_ERROR
//#endif

//// �����޸�Ť�س���
//#ifndef NH_ASM_ALLOW_MODIFY_TORQUE_ERROR
//#define NH_ASM_ALLOW_MODIFY_TORQUE_ERROR
//#endif

/////////////////////////ASM end



/////////////////////////LUGDOWN begin

// ������������
#ifndef NH_LUG_ALLOW_SKIP_OIL_TEMP
#define NH_LUG_ALLOW_SKIP_OIL_TEMP
#endif

// ��������ת�ټ��
#ifndef NH_LUG_ALLOW_SKIP_REV_CHECK
#define NH_LUG_ALLOW_SKIP_REV_CHECK
#endif

//// �����޸ĳ��Χ
//#ifndef NH_LUG_ALLOW_MODIFY_REV_RANGE
//#define NH_LUG_ALLOW_MODIFY_REV_RANGE
//#endif

//// ����ʹ��ģ��ת��
//#ifndef NH_LUG_ALLOW_USE_REV_SIMULATE
//#define NH_LUG_ALLOW_USE_REV_SIMULATE
//#endif

/////////////////////////LUGDOWN end



/////////////////////////DIS begin

// ����������ת��
#ifndef NH_DIS_ALLOW_SKIP_REV_CHECK_ONCE
#define NH_DIS_ALLOW_SKIP_REV_CHECK_ONCE
#endif

//// ��������ת��
//#ifndef NH_DIS_ALLOW_SKIP_REV_CHECK
//#define NH_DIS_ALLOW_SKIP_REV_CHECK
//#endif

//// ��������Ԥ��ת��
//#ifndef NH_DIS_ALLOW_SKIP_WARMUP_REV_CHECK
//#define NH_DIS_ALLOW_SKIP_WARMUP_REV_CHECK
//#endif

//// ���������ߵ���ת��
//#ifndef NH_DIS_ALLOW_SKIP_HIS_REV_CHECK
//#define NH_DIS_ALLOW_SKIP_HIS_REV_CHECK
//#endif

//// ������������ת��
//#ifndef NH_DIS_ALLOW_SKIP_IS_REV_CHECK
//#define NH_DIS_ALLOW_SKIP_IS_REV_CHECK
//#endif

// ������������
#ifndef NH_DIS_ALLOW_SKIP_OIL_TEMP_ONCE
#define NH_DIS_ALLOW_SKIP_OIL_TEMP_ONCE
#endif

// ��������CO+CO2���
#ifndef NH_DIS_ALLOW_SKIP_COCO2_CHECK
#define NH_DIS_ALLOW_SKIP_COCO2_CHECK
#endif

//// ���������������
//#ifndef NH_DIS_ALLOW_SKIP_FLUX_CHECK
//#define NH_DIS_ALLOW_SKIP_FLUX_CHECK
//#endif

//// ����������������ϵ��
//#ifndef NH_DIS_CORRECT_LAMBDA
//#define NH_DIS_CORRECT_LAMBDA
//#endif

//// ����ʹ��ģ��ת��
//#ifndef NH_DIS_ALLOW_USE_REV_SIMULATE
//#define NH_DIS_ALLOW_USE_REV_SIMULATE
//#endif

//// ��������ת�ٷ�Χ
//#ifndef NH_DIS_ALLOW_MODIFY_REV_RANGE
//#define NH_DIS_ALLOW_MODIFY_REV_RANGE
//#endif

/////////////////////////DIS end



/////////////////////////FSUNHT begin

// ��������ת��
#ifndef NH_FSUNHT_ALLOW_SKIP_REV_CHECK
#define NH_FSUNHT_ALLOW_SKIP_REV_CHECK
#endif

/////////////////////////FSUNHT end



/////////////////////////FSUYD begin
// ��������ת��
#ifndef NH_FSUYD_ALLOW_SKIP_REV_CHECK
#define NH_FSUYD_ALLOW_SKIP_REV_CHECK
#endif

/////////////////////////FSUYD end




/////////////////////////AVFSU begin

// ��������ת��
#ifndef NH_AVFSU_ALLOW_SKIP_REV_CHECK
#define NH_AVFSU_ALLOW_SKIP_REV_CHECK
#endif

/////////////////////////AVFSU end




/////////////////////////MDIS begin

// ����������ת��
#ifndef NH_MDIS_ALLOW_SKIP_REV_CHECK_ONCE
#define NH_MDIS_ALLOW_SKIP_REV_CHECK_ONCE
#endif

//// ��������ת��
//#ifndef NH_MDIS_ALLOW_SKIP_REV_CHECK
//#define NH_MDIS_ALLOW_SKIP_REV_CHECK
//#endif
//
//// ��������Ԥ��ת��
//#ifndef NH_MDIS_ALLOW_SKIP_WARMUP_REV_CHECK
//#define NH_MDIS_ALLOW_SKIP_WARMUP_REV_CHECK
//#endif
//
//// ���������ߵ���ת��
//#ifndef NH_MDIS_ALLOW_SKIP_HIS_REV_CHECK
//#define NH_MDIS_ALLOW_SKIP_HIS_REV_CHECK
//#endif
//
//// ������������ת��
//#ifndef NH_MDIS_ALLOW_SKIP_IS_REV_CHECK
//#define NH_MDIS_ALLOW_SKIP_IS_REV_CHECK
//#endif

// ������������
#ifndef NH_MDIS_ALLOW_SKIP_OIL_TEMP
#define NH_MDIS_ALLOW_SKIP_OIL_TEMP
#endif

//// ��������CO+CO2���
//#ifndef NH_MDIS_ALLOW_SKIP_COCO2_CHECK
//#define NH_MDIS_ALLOW_SKIP_COCO2_CHECK
//#endif

//// ���������������
//#ifndef NH_MDIS_ALLOW_SKIP_FLUX_CHECK
//#define NH_MDIS_ALLOW_SKIP_FLUX_CHECK
//#endif

//// ��������ת�ٷ�Χ
//#ifndef NH_MDIS_ALLOW_MODIFY_REV_RANGE
//#define NH_MDIS_ALLOW_MODIFY_REV_RANGE
//#endif

/////////////////////////MDIS end




/////////////////////////MI begin

// ������������
#ifndef NH_MI_ALLOW_SKIP_OIL_TEMP
#define NH_MI_ALLOW_SKIP_OIL_TEMP
#endif

//// ��������CO+CO2���
//#ifndef NH_MI_ALLOW_SKIP_COCO2_CHECK
//#define NH_MI_ALLOW_SKIP_COCO2_CHECK
//#endif

//// ���������������
//#ifndef NH_MI_ALLOW_SKIP_FLUX_CHECK
//#define NH_MI_ALLOW_SKIP_FLUX_CHECK
//#endif

/////////////////////////MI end

/////////////////////////// GASOLINEVEHICLEEQUIPMENTPREPARE begin

//// �����޸�������㳬ʱʱ��
//#ifndef ALLOW_MODIFY_ZERO_OVERTIME
//#define ALLOW_MODIFY_ZERO_OVERTIME
//#endif

//// �����޸����������ɵȴ�ʱ��
//#ifndef ALLOW_MODIFY_ZERO_WAITTIME
//#define ALLOW_MODIFY_ZERO_WAITTIME
//#endif

//// �����޸������ƻ�������鿪���ʱ��
//#ifndef ALLOW_MODIFY_CHECKO2TIME
//#define ALLOW_MODIFY_CHECKO2TIME
//#endif

//// �����޸��߼����㷴��ʱ��
//#ifndef ALLOW_MODIFY_ZERO_INVERSEBLOWTIME
//#define ALLOW_MODIFY_ZERO_INVERSEBLOWTIME
//#endif

// �����޸Ļ�������������ʱ��
#ifndef ALLOW_MODIFY_ENVIROMENT_CHECK_TIME
#define ALLOW_MODIFY_ENVIROMENT_CHECK_TIME
#endif

// �����޸��������������ʱ��
#ifndef ALLOW_MODIFY_RESIDUES_CHECKTIME
#define ALLOW_MODIFY_RESIDUES_CHECKTIME
#endif

//// �����޸Ļ����������HC��ֵ
//#ifndef ALLOW_MODIFY_EC_HCLIMIT
//#define ALLOW_MODIFY_EC_HCLIMIT
//#endif

//// �����޸Ļ����������CO��ֵ
//#ifndef ALLOW_MODIFY_EC_COLIMIT
//#define ALLOW_MODIFY_EC_COLIMIT
//#endif

// �����޸Ļ����������CO2��ֵ
#ifndef ALLOW_MODIFY_EC_CO2LIMIT
#define ALLOW_MODIFY_EC_CO2LIMIT
#endif

//// �����޸Ļ����������NO��ֵ
//#ifndef ALLOW_MODIFY_EC_NOLIMIT
//#define ALLOW_MODIFY_EC_NOLIMIT
//#endif

//// �����޸�����������HC��ֵ
//#ifndef ALLOW_MODIFY_RC_HCLIMIT
//#define ALLOW_MODIFY_RC_HCLIMIT
//#endif

//// �����޸�����������CO��ֵ
//#ifndef ALLOW_MODIFY_RC_COLIMIT
//#define ALLOW_MODIFY_RC_COLIMIT
//#endif

//// �����޸�����������CO2��ֵ
//#ifndef ALLOW_MODIFY_RC_CO2LIMIT
//#define ALLOW_MODIFY_RC_CO2LIMIT
//#endif

//// �����޸�����������NO��ֵ
//#ifndef ALLOW_MODIFY_RC_NOLIMIT
//#define ALLOW_MODIFY_RC_NOLIMIT
//#endif

//// �����޸������ƻ��������������ֵ
//#ifndef ALLOW_MODIFY_FLOCHKO2LOWLIMIT
//#define ALLOW_MODIFY_FLOCHKO2LOWLIMIT
//#endif

//// �����޸������ƻ��������������ֵ
//#ifndef ALLOW_MODIFY_FLOCHKO2HIGHLIMIT
//#define ALLOW_MODIFY_FLOCHKO2HIGHLIMIT
//#endif

//// �������������ǵ���
//#ifndef ALLOW_SKIP_ANAZERO
//#define ALLOW_SKIP_ANAZERO
//#endif

//// �����޸��Ƿ���������
//#ifndef ALLOW_MODIFY_TRUEZERO
//#define ALLOW_MODIFY_TRUEZERO
//#endif

//// �������������������
//#ifndef ALLOW_SKIP_ANACHKEC
//#define ALLOW_SKIP_ANACHKEC
//#endif

//// ������������������
//#ifndef ALLOW_SKIP_ANACHKRC
//#define ALLOW_SKIP_ANACHKRC
//#endif

//// �������������ƻ��������
//#ifndef ALLOW_SKIP_FLOCHKO2
//#define ALLOW_SKIP_FLOCHKO2
//#endif

////�����޸ķ����͵����Ƿ�ͬʱ����
//#ifndef ALLOW_MODIFY_INVERSEBLOWANDZEROTOGETHER
//#define ALLOW_MODIFY_INVERSEBLOWANDZEROTOGETHER
//#endif

/////////////////////////// GASOLINEVEHICLEEQUIPMENTPREPARE end
#endif
