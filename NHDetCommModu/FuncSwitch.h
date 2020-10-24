/*
* Copyright (c) 佛山市南华仪器股份有限公司
* All rights reserved.
*
* 文件名称：FuncSwitch.h
* 文件标识：
* 描述：功能开关
*
* 版本：1.0
* 日期：2015-09-28
* 作者：Cyx
*
*/


#ifndef FUNC_SWITCH_H
#define FUNC_SWITCH_H

/////////////////////////VMAS begin

// 允许跳过油温
#ifndef NH_VMAS_ALLOW_SKIP_OIL_TEMP
#define NH_VMAS_ALLOW_SKIP_OIL_TEMP
#endif

// 允许跳过CO+CO2检查
#ifndef NH_VMAS_ALLOW_SKIP_COCO2_CHECK
#define NH_VMAS_ALLOW_SKIP_COCO2_CHECK
#endif

// 允许配置百公里油耗最小值
#ifndef NH_VMAS_ALLOW_100KM_FULECONSUMPTION_MIN
#define NH_VMAS_ALLOW_100KM_FULECONSUMPTION_MIN
#endif

//// 允许修改累计超差
//#ifndef NH_VMAS_ALLOW_MODIFY_TOTAL_OVER_TIME
//#define NH_VMAS_ALLOW_MODIFY_TOTAL_OVER_TIME
//#endif

//// 允许修改连续超差
//#ifndef NH_VMAS_ALLOW_MODIFY_ONCE_OVER_TIME
//#define NH_VMAS_ALLOW_MODIFY_ONCE_OVER_TIME
//#endif

//// 允许修改超差范围
//#ifndef NH_VMAS_ALLOW_MODIFY_TOLERANCE_RANGE
//#define NH_VMAS_ALLOW_MODIFY_TOLERANCE_RANGE
//#endif

//// 允许跳过95L/s监控
//#ifndef NH_VMAS_ALLOW_SKIP_95CHECK
//#define NH_VMAS_ALLOW_SKIP_95CHECK
//#endif

//// 允许跳过分析仪流量检查
//#ifndef NH_VMAS_ALLOW_SKIP_ANA_FLUXCHECK
//#define NH_VMAS_ALLOW_SKIP_ANA_FLUXCHECK
//#endif

// 允许修改废气流量最小限值，GB2018已经不要求
#ifndef NH_VMAS_ALLOW_MODIFY_MINEXHANSTFLUX
#define NH_VMAS_ALLOW_MODIFY_MINEXHANSTFLUX
#endif

//// 允许修改怠速等待时间
//#ifndef NH_VMAS_ALLOW_MODIFY_IDLE_WAIT_TIME
//#define NH_VMAS_ALLOW_MODIFY_IDLE_WAIT_TIME
//#endif

/////////////////////////VMAS end



/////////////////////////ASM begin

// 允许跳过转速
#ifndef NH_ASM_ALLOW_SKIP_REV_CHECK
#define NH_ASM_ALLOW_SKIP_REV_CHECK
#endif

// 允许跳过油温
#ifndef NH_ASM_ALLOW_SKIP_OIL_TEMP
#define NH_ASM_ALLOW_SKIP_OIL_TEMP
#endif

// 允许跳过NO检查
#ifndef NH_ASM_ALLOW_SKIP_NO_CHECK
#define NH_ASM_ALLOW_SKIP_NO_CHECK
#endif

//// 允许跳过CO+CO2检查
//#ifndef NH_ASM_ALLOW_SKIP_COCO2_CHECK
//#define NH_ASM_ALLOW_SKIP_COCO2_CHECK
//#endif

//// 允许跳过流量检查
//#ifndef NH_ASM_ALLOW_SKIP_FLUX_CHECK
//#define NH_ASM_ALLOW_SKIP_FLUX_CHECK
//#endif

//// 允许跳过有效数据判断
//#ifndef NH_ASM_ALLOW_SKIP_VALID_DATA_JUD
//#define NH_ASM_ALLOW_SKIP_VALID_DATA_JUD
//#endif

//// 允许跳过扭矩超差
//#ifndef NH_ASM_ALLOW_SKIP_TORQUE_ERROR
//#define NH_ASM_ALLOW_SKIP_TORQUE_ERROR
//#endif

//// 允许修改扭矩超差
//#ifndef NH_ASM_ALLOW_MODIFY_TORQUE_ERROR
//#define NH_ASM_ALLOW_MODIFY_TORQUE_ERROR
//#endif

/////////////////////////ASM end



/////////////////////////LUGDOWN begin

// 允许跳过油温
#ifndef NH_LUG_ALLOW_SKIP_OIL_TEMP
#define NH_LUG_ALLOW_SKIP_OIL_TEMP
#endif

// 允许跳过转速监控
#ifndef NH_LUG_ALLOW_SKIP_REV_CHECK
#define NH_LUG_ALLOW_SKIP_REV_CHECK
#endif

//// 允许修改超差范围
//#ifndef NH_LUG_ALLOW_MODIFY_REV_RANGE
//#define NH_LUG_ALLOW_MODIFY_REV_RANGE
//#endif

//// 允许使用模拟转速
//#ifndef NH_LUG_ALLOW_USE_REV_SIMULATE
//#define NH_LUG_ALLOW_USE_REV_SIMULATE
//#endif

/////////////////////////LUGDOWN end



/////////////////////////DIS begin

// 允许单次跳过转速
#ifndef NH_DIS_ALLOW_SKIP_REV_CHECK_ONCE
#define NH_DIS_ALLOW_SKIP_REV_CHECK_ONCE
#endif

//// 允许跳过转速
//#ifndef NH_DIS_ALLOW_SKIP_REV_CHECK
//#define NH_DIS_ALLOW_SKIP_REV_CHECK
//#endif

//// 允许跳过预热转速
//#ifndef NH_DIS_ALLOW_SKIP_WARMUP_REV_CHECK
//#define NH_DIS_ALLOW_SKIP_WARMUP_REV_CHECK
//#endif

//// 允许跳过高怠速转速
//#ifndef NH_DIS_ALLOW_SKIP_HIS_REV_CHECK
//#define NH_DIS_ALLOW_SKIP_HIS_REV_CHECK
//#endif

//// 允许跳过怠速转速
//#ifndef NH_DIS_ALLOW_SKIP_IS_REV_CHECK
//#define NH_DIS_ALLOW_SKIP_IS_REV_CHECK
//#endif

// 允许跳过油温
#ifndef NH_DIS_ALLOW_SKIP_OIL_TEMP_ONCE
#define NH_DIS_ALLOW_SKIP_OIL_TEMP_ONCE
#endif

// 允许跳过CO+CO2检查
#ifndef NH_DIS_ALLOW_SKIP_COCO2_CHECK
#define NH_DIS_ALLOW_SKIP_COCO2_CHECK
#endif

//// 允许跳过流量检查
//#ifndef NH_DIS_ALLOW_SKIP_FLUX_CHECK
//#define NH_DIS_ALLOW_SKIP_FLUX_CHECK
//#endif

//// 允许修正过量空气系数
//#ifndef NH_DIS_CORRECT_LAMBDA
//#define NH_DIS_CORRECT_LAMBDA
//#endif

//// 允许使用模拟转速
//#ifndef NH_DIS_ALLOW_USE_REV_SIMULATE
//#define NH_DIS_ALLOW_USE_REV_SIMULATE
//#endif

//// 允许修正转速范围
//#ifndef NH_DIS_ALLOW_MODIFY_REV_RANGE
//#define NH_DIS_ALLOW_MODIFY_REV_RANGE
//#endif

/////////////////////////DIS end



/////////////////////////FSUNHT begin

// 允许跳过转速
#ifndef NH_FSUNHT_ALLOW_SKIP_REV_CHECK
#define NH_FSUNHT_ALLOW_SKIP_REV_CHECK
#endif

/////////////////////////FSUNHT end



/////////////////////////FSUYD begin
// 允许跳过转速
#ifndef NH_FSUYD_ALLOW_SKIP_REV_CHECK
#define NH_FSUYD_ALLOW_SKIP_REV_CHECK
#endif

/////////////////////////FSUYD end




/////////////////////////AVFSU begin

// 允许跳过转速
#ifndef NH_AVFSU_ALLOW_SKIP_REV_CHECK
#define NH_AVFSU_ALLOW_SKIP_REV_CHECK
#endif

/////////////////////////AVFSU end




/////////////////////////MDIS begin

// 允许单次跳过转速
#ifndef NH_MDIS_ALLOW_SKIP_REV_CHECK_ONCE
#define NH_MDIS_ALLOW_SKIP_REV_CHECK_ONCE
#endif

//// 允许跳过转速
//#ifndef NH_MDIS_ALLOW_SKIP_REV_CHECK
//#define NH_MDIS_ALLOW_SKIP_REV_CHECK
//#endif
//
//// 允许跳过预热转速
//#ifndef NH_MDIS_ALLOW_SKIP_WARMUP_REV_CHECK
//#define NH_MDIS_ALLOW_SKIP_WARMUP_REV_CHECK
//#endif
//
//// 允许跳过高怠速转速
//#ifndef NH_MDIS_ALLOW_SKIP_HIS_REV_CHECK
//#define NH_MDIS_ALLOW_SKIP_HIS_REV_CHECK
//#endif
//
//// 允许跳过怠速转速
//#ifndef NH_MDIS_ALLOW_SKIP_IS_REV_CHECK
//#define NH_MDIS_ALLOW_SKIP_IS_REV_CHECK
//#endif

// 允许跳过油温
#ifndef NH_MDIS_ALLOW_SKIP_OIL_TEMP
#define NH_MDIS_ALLOW_SKIP_OIL_TEMP
#endif

//// 允许跳过CO+CO2检查
//#ifndef NH_MDIS_ALLOW_SKIP_COCO2_CHECK
//#define NH_MDIS_ALLOW_SKIP_COCO2_CHECK
//#endif

//// 允许跳过流量检查
//#ifndef NH_MDIS_ALLOW_SKIP_FLUX_CHECK
//#define NH_MDIS_ALLOW_SKIP_FLUX_CHECK
//#endif

//// 允许修正转速范围
//#ifndef NH_MDIS_ALLOW_MODIFY_REV_RANGE
//#define NH_MDIS_ALLOW_MODIFY_REV_RANGE
//#endif

/////////////////////////MDIS end




/////////////////////////MI begin

// 允许跳过油温
#ifndef NH_MI_ALLOW_SKIP_OIL_TEMP
#define NH_MI_ALLOW_SKIP_OIL_TEMP
#endif

//// 允许跳过CO+CO2检查
//#ifndef NH_MI_ALLOW_SKIP_COCO2_CHECK
//#define NH_MI_ALLOW_SKIP_COCO2_CHECK
//#endif

//// 允许跳过流量检查
//#ifndef NH_MI_ALLOW_SKIP_FLUX_CHECK
//#define NH_MI_ALLOW_SKIP_FLUX_CHECK
//#endif

/////////////////////////MI end

/////////////////////////// GASOLINEVEHICLEEQUIPMENTPREPARE begin

//// 允许修改物理调零超时时间
//#ifndef ALLOW_MODIFY_ZERO_OVERTIME
//#define ALLOW_MODIFY_ZERO_OVERTIME
//#endif

//// 允许修改物理调零完成等待时间
//#ifndef ALLOW_MODIFY_ZERO_WAITTIME
//#define ALLOW_MODIFY_ZERO_WAITTIME
//#endif

//// 允许修改流量计环境氧检查开风机时间
//#ifndef ALLOW_MODIFY_CHECKO2TIME
//#define ALLOW_MODIFY_CHECKO2TIME
//#endif

//// 允许修改逻辑调零反吹时间
//#ifndef ALLOW_MODIFY_ZERO_INVERSEBLOWTIME
//#define ALLOW_MODIFY_ZERO_INVERSEBLOWTIME
//#endif

// 允许修改环境空气检查抽气时间
#ifndef ALLOW_MODIFY_ENVIROMENT_CHECK_TIME
#define ALLOW_MODIFY_ENVIROMENT_CHECK_TIME
#endif

// 允许修改气体残留检查抽气时间
#ifndef ALLOW_MODIFY_RESIDUES_CHECKTIME
#define ALLOW_MODIFY_RESIDUES_CHECKTIME
#endif

//// 允许修改环境空气检查HC限值
//#ifndef ALLOW_MODIFY_EC_HCLIMIT
//#define ALLOW_MODIFY_EC_HCLIMIT
//#endif

//// 允许修改环境空气检查CO限值
//#ifndef ALLOW_MODIFY_EC_COLIMIT
//#define ALLOW_MODIFY_EC_COLIMIT
//#endif

// 允许修改环境空气检查CO2限值
#ifndef ALLOW_MODIFY_EC_CO2LIMIT
#define ALLOW_MODIFY_EC_CO2LIMIT
#endif

//// 允许修改环境空气检查NO限值
//#ifndef ALLOW_MODIFY_EC_NOLIMIT
//#define ALLOW_MODIFY_EC_NOLIMIT
//#endif

//// 允许修改气体残留检查HC限值
//#ifndef ALLOW_MODIFY_RC_HCLIMIT
//#define ALLOW_MODIFY_RC_HCLIMIT
//#endif

//// 允许修改气体残留检查CO限值
//#ifndef ALLOW_MODIFY_RC_COLIMIT
//#define ALLOW_MODIFY_RC_COLIMIT
//#endif

//// 允许修改气体残留检查CO2限值
//#ifndef ALLOW_MODIFY_RC_CO2LIMIT
//#define ALLOW_MODIFY_RC_CO2LIMIT
//#endif

//// 允许修改气体残留检查NO限值
//#ifndef ALLOW_MODIFY_RC_NOLIMIT
//#define ALLOW_MODIFY_RC_NOLIMIT
//#endif

//// 允许修改流量计环境氧检查氧低限值
//#ifndef ALLOW_MODIFY_FLOCHKO2LOWLIMIT
//#define ALLOW_MODIFY_FLOCHKO2LOWLIMIT
//#endif

//// 允许修改流量计环境氧检查氧高限值
//#ifndef ALLOW_MODIFY_FLOCHKO2HIGHLIMIT
//#define ALLOW_MODIFY_FLOCHKO2HIGHLIMIT
//#endif

//// 允许跳过分析仪调零
//#ifndef ALLOW_SKIP_ANAZERO
//#define ALLOW_SKIP_ANAZERO
//#endif

//// 允许修改是否真正调零
//#ifndef ALLOW_MODIFY_TRUEZERO
//#define ALLOW_MODIFY_TRUEZERO
//#endif

//// 允许跳过环境空气检查
//#ifndef ALLOW_SKIP_ANACHKEC
//#define ALLOW_SKIP_ANACHKEC
//#endif

//// 允许跳过气体残留检查
//#ifndef ALLOW_SKIP_ANACHKRC
//#define ALLOW_SKIP_ANACHKRC
//#endif

//// 允许跳过流量计环境氧检查
//#ifndef ALLOW_SKIP_FLOCHKO2
//#define ALLOW_SKIP_FLOCHKO2
//#endif

////允许修改反吹和调零是否同时进行
//#ifndef ALLOW_MODIFY_INVERSEBLOWANDZEROTOGETHER
//#define ALLOW_MODIFY_INVERSEBLOWANDZEROTOGETHER
//#endif

/////////////////////////// GASOLINEVEHICLEEQUIPMENTPREPARE end
#endif
