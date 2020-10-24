/************************************************* 
*  Copyright (C), 2009-2019,佛山市华贝软件技术有限公司 
*  All rights reserved
*
*  文件名称: NHDef.h
*  当前版本: 1.0.28
*  日期: 2011-05-11
*  摘要: 定义自用定义,统一由这里定义,避免重复定义
*		 目前主要是定义设备线程库的异步函数消息
*
*
*  历史信息:
*
* 版本：1.0.32
* 日期：2014-11-18
* 作者：Lihy
* 描述：添加修改数据编译开关宏定义
*
* 版本：1.0.31
* 日期：2013-05-27
* 作者：Lihy
* 描述：1)添加系统自检消息
            #define WM_SYSTEM_CHECK_NHV_WORKING			WM_USER + 0x0607  (用于济南版环境参数校准界面)
			#define WM_SYSTEM_CHECK_RPM_WORKING			WM_USER + 0x0608  (用于济南版转速自检界面)
*
* 版本：1.0.30
* 日期：2012-10-08
* 作者：wuyq
* 描述：添加检测仪器的最大取值范围定义
*
* 版本：1.0.29
* 日期：2011-09-28
* 作者：Raylee
* 描述：添加摩托车怠速/双怠速检测状态定义
*
* 版本：1.0.28
* 日期：2011-05-11
* 作者：hemincong
* 描述：1)添加柴油机汽油机开始自动准备消息：WM_GASOLINE_VEHICLE_EQUIPMENT_STARTAUTOPREPARE、WM_DIESEL_VEHICLE_EQUIPMENT_STARTAUTOPREPARE
*
* 版本：1.0.27
* 日期：2011-03-08
* 作者：Ming
* 描述：1)修改定义车辆检测状态，状态6为车辆取消检测（CANCEL_TEST）
*
* 版本：1.0.26
* 日期：2010-09-19
* 作者：hemincong
* 描述：1)添加'`~'键映射消息#define WM_HOTKEY_VK_OEM_3			WM_USER+0x070F
*
* 版本：1.0.25
* 日期：2010-09-14
* 作者：hemincong
* 描述：1)添加条形码扫描仪的消息宏定义
*
* 版本：1.0.24
* 日期：2010-07-11
* 作者：Cui
* 描述：在本文件添加代码：#include "NHStructDef.h"
*
* 版本：1.0.23
* 日期：2010-07-2
* 作者：Ming
* 描述：1)ASM流程添加开始5025检测和开始2540检测宏定义,LUGDOWN流程添加开始VELMAXHP、开始90VELMAXHP过程、开始80VELMAXHP过程宏定义
*
* 版本：1.0.22
* 日期：2010-06-21
* 作者：Ming
* 描述：1)添加安全提示流程回调消息宏定义
*
* 版本：1.0.19
* 时间：2010-05-27
* 作者：Raylee
* 描述：添加流量计标定项目的枚举定义
*
* 版本：1.0.18
* 时间：2010-05-22
* 作者：Raylee
* 描述：添加流量计标定的回调消息宏定义
*
* 版本：1.0.17
* 时间：2010-05-18
* 作者：Raylee
* 描述：添加各种仪器标定的回调消息宏定义
*
* 版本：1.0.16
* 时间：2010-05-12
* 作者：Hyh
* 描述：1)添加参数采集仪消息定义
*
* 版本：1.0.15
* 时间：2010-05-07
* 作者：Hyh
* 描述：1)添加键盘映射消息定义
*
* 版本：1.0.14
* 时间：2010-05-04
* 作者：Hyh
* 描述：1)添加各检测方法实时状态回调消息
*
* 版本：1.0.13
* 时间：2010-04-12
* 作者：Raylee
* 描述：添加检测类型、标定项目、检测状态、角色类型的枚举定义
*
* 版本：1.0.12
* 时间：2010-01-13
* 作者：Cui
* 描述：将所有缩写“Dyna”改为“Dyn”
*
* 版本：1.0.11
* 时间：2010-01-13
* 作者：hyh
* 描述：1)统一其他消息定义区间(0x0601~0x700)
*       2)定义系统自检消息
*        #define WM_SYSTEM_CHECK_ANA_WORKING			WM_USER + 0x0603
*        #define WM_SYSTEM_CHECK_FLO_WORKING			WM_USER + 0x0604
*        #define WM_SYSTEM_CHECK_DYN_WORKING			WM_USER + 0x0605
*        #define WM_SYSTEM_CHECK_OPA_WORKING			WM_USER + 0x0606
*
* 版本：1.0.10
* 时间：2009-12-09
* 作者：hyh
* 描述：添加VMASTEST消息返回
*
* 版本：1.0.9
* 时间：2009-12-08
* 作者：Cui
* 描述：重新定义设备异步函数消息，并规定区间
*
* 版本：1.0.8
* 时间：2009-11-10
* 作者：hyh
* 描述： 修改 WM_ANALYZER_PREPARE 为 WM_GASOLINE_VEHICLE_EQUIPMENT_PREPARE
*        修改 WM_OPACIMETER_PREPARE 为 WM_DIESEL_VEHICLE_EQUIPMENT_PREPARE
*        删除 WM_FLOWMETER_PREPARE
*
* 版本：1.0.7
* 时间：2009-11-09
* 作者：Cui
* 描述：不透光NHT1L异步校准消息:WM_NHT1L_ADJUST->WM_NHT1L_CALIBRATE
*       测功机异步取状态消息:WM_NHC_GET_STATE->WM_NHC_GET_STATUS
*
* 版本：1.0.6
* 时间：2009-11-09
* 作者：Cui
* 描述：增加LUGDOWN排放检测流程宏定义
*
* 版本：1.0.5
* 时间：2009-11-07
* 作者：Cui
* 描述：增加ASM排放检测流程宏定义
*
* 版本：1.0.4
* 时间：2009-11-07
* 作者：hyh
* 描述：数据转wchart长度定义转到NHStructDef.h
*
* 版本：1.0.3
* 时间：2009-11-03
* 作者：hyh
* 描述：增加添加设备准备消息定义和数据转wchart长度定义
*
* 版本：1.0.2
* 时间：2009-10-28
* 作者：Cui
* 描述：增加自由加速排放检测流程宏定义
*
* 版本：1.0.1
* 时间：2009-10-26
* 作者：Cui
* 描述：增加双怠速排放检测流程宏定义
*
* 版本：1.0.0
* 时间：2009-09-12
* 作者：Cui
* 描述：完成,进行使用
*************************************************/

#ifndef NHDEF_H
#define NHDEF_H

#include "NHStructDef.h"
#include "NHEngPro.h"
#include "FuncSwitch.h"

// 宏定义
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// LUGDOWN检测流程宏定义begin
#ifndef NH_APP_TEST_ERROR
#define NH_APP_TEST_ERROR	0xFF // 程序错误
#endif

#ifndef NH_APP_TEST_FINISH
#define NH_APP_TEST_FINISH	0x00 // 检测完成
#endif

#ifndef NH_APP_TEST_EXIT
#define NH_APP_TEST_EXIT	0x01 // 检测退出
#endif

#ifndef NH_APP_TEST_PREV
#define NH_APP_TEST_PREV	0x02 // 上一步
#endif

#ifndef NH_APP_TEST_FINISH_ALL
#define NH_APP_TEST_FINISH_ALL	0x03 // 检测完成(车辆检测状态置为完成)
#endif

#ifndef NH_APP_STATUS_START_TEST
#define NH_APP_STATUS_START_TEST 0x03 // 检测状态-开始
#endif

#ifndef NH_APP_STATUS_STOP_TEST
#define NH_APP_STATUS_STOP_TEST 0x04 // 检测状态-停止
#endif

#ifndef NH_APP_STATUS_FINISH_TEST
#define NH_APP_STATUS_FINISH_TEST 0x05 // 检测状态-结束
#endif

#ifndef NH_APP_STATUS_REALTIME_MESSAGE
#define NH_APP_STATUS_REALTIME_MESSAGE 0x06 // 检测状态-实时信息
#endif

#ifndef NH_OBD_TEST_ERROR
#define NH_OBD_TEST_ERROR	0xFF // 程序错误
#endif

#ifndef NH_OBD_TEST_FINISH
#define NH_OBD_TEST_FINISH	0x00 // 检测完成
#endif

#ifndef NH_OBD_TEST_EXIT
#define NH_OBD_TEST_EXIT	0x01 // 检测退出
#endif

#ifndef NH_OBD_TEST_PREV
#define NH_OBD_TEST_PREV	0x02 // 上一步
#endif

#ifndef NH_OBD_TEST_FINISH_ALL
#define NH_OBD_TEST_FINISH_ALL	0x03 // 检测完成(车辆检测状态置为完成)
#endif

#ifndef NH_OBD_STATUS_START_TEST
#define NH_OBD_STATUS_START_TEST 0x03 // 检测状态-开始
#endif

#ifndef NH_OBD_STATUS_STOP_TEST
#define NH_OBD_STATUS_STOP_TEST 0x04 // 检测状态-停止
#endif

#ifndef NH_OBD_STATUS_FINISH_TEST
#define NH_OBD_STATUS_FINISH_TEST 0x05 // 检测状态-结束
#endif

#ifndef NH_OBD_STATUS_REALTIME_MESSAGE
#define NH_OBD_STATUS_REALTIME_MESSAGE 0x06 // 检测状态-实时信息
#endif

#ifndef NH_LUGDOWN_TEST_ERROR
#define NH_LUGDOWN_TEST_ERROR	0xFF // 程序错误
#endif

#ifndef NH_LUGDOWN_TEST_FINISH
#define NH_LUGDOWN_TEST_FINISH	0x00 // 检测完成
#endif

#ifndef NH_LUGDOWN_TEST_EXIT
#define NH_LUGDOWN_TEST_EXIT	0x01 // 检测退出
#endif

#ifndef NH_LUGDOWN_TEST_PREV
#define NH_LUGDOWN_TEST_PREV	0x02 // 上一步
#endif

#ifndef NH_LUGDOWN_RESULT_ERROR
#define NH_LUGDOWN_RESULT_ERROR		0xFF // 程序错误
#endif

#ifndef NH_LUGDOWN_RESULT_CONTINUE
#define NH_LUGDOWN_RESULT_CONTINUE	0x00 // 检测完成继续检测
#endif

#ifndef NH_LUGDOWN_RESULT_EXIT
#define NH_LUGDOWN_RESULT_EXIT		0x01 // 检测完成返回主界面
#endif

#ifndef NH_LUGDOWN_STATUS_START_TEST
#define NH_LUGDOWN_STATUS_START_TEST 0x01 // 检测状态-开始
#endif

#ifndef NH_LUGDOWN_STATUS_STOP_TEST
#define NH_LUGDOWN_STATUS_STOP_TEST 0x02 // 检测状态-停止
#endif

#ifndef NH_LUGDOWN_STATUS_FINISH_TEST
#define NH_LUGDOWN_STATUS_FINISH_TEST 0x03 // 检测状态-结束
#endif

#ifndef NH_LUGDOWN_STATUS_REALTIME_DATA
#define NH_LUGDOWN_STATUS_REALTIME_DATA 0x04 // 检测状态-实时数据
#endif

#ifndef NH_LUGDOWN_STATUS_REALTIME_MESSAGE
#define NH_LUGDOWN_STATUS_REALTIME_MESSAGE 0x05 // 检测状态-实时信息
#endif

#ifndef NH_LUGDOWN_STATUS_START_VELMAXHP_TEST
#define NH_LUGDOWN_STATUS_START_VELMAXHP_TEST 0x06 // 检测状态-开始VELMAXHP过程
#endif

#ifndef NH_LUGDOWN_STATUS_START_90VELMAXHP_TEST
#define NH_LUGDOWN_STATUS_START_90VELMAXHP_TEST 0x07 // 检测状态-开始90VELMAXHP过程
#endif

#ifndef NH_LUGDOWN_STATUS_START_80VELMAXHP_TEST
#define NH_LUGDOWN_STATUS_START_80VELMAXHP_TEST 0x08 // 检测状态-开始80VELMAXHP过程
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// LUGDOWN检测流程宏定义end

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ASM检测流程宏定义begin
#ifndef NH_ASM_TEST_ERROR
#define NH_ASM_TEST_ERROR	0xFF // 程序错误
#endif

#ifndef NH_ASM_TEST_FINISH
#define NH_ASM_TEST_FINISH	0x00 // 检测完成
#endif

#ifndef NH_ASM_TEST_EXIT
#define NH_ASM_TEST_EXIT	0x01 // 检测退出
#endif

#ifndef NH_ASM_TEST_PREV
#define NH_ASM_TEST_PREV	0x02 // 上一步
#endif

#ifndef NH_ASM_RESULT_ERROR
#define NH_ASM_RESULT_ERROR		0xFF // 程序错误
#endif

#ifndef NH_ASM_RESULT_CONTINUE
#define NH_ASM_RESULT_CONTINUE	0x00 // 检测完成继续检测
#endif

#ifndef NH_ASM_RESULT_EXIT
#define NH_ASM_RESULT_EXIT		0x01 // 检测完成返回主界面
#endif

#ifndef NH_ASM_STATUS_START_TEST
#define NH_ASM_STATUS_START_TEST 0x01 // 检测状态-开始检测
#endif

#ifndef NH_ASM_STATUS_STOP_TEST
#define NH_ASM_STATUS_STOP_TEST 0x02 // 检测状态-停止检测
#endif

#ifndef NH_ASM_STATUS_FINISH_TEST
#define NH_ASM_STATUS_FINISH_TEST 0x03 // 检测状态-检测结束
#endif

#ifndef NH_ASM_STATUS_REALTIME_DATA
#define NH_ASM_STATUS_REALTIME_DATA 0x04 // 检测状态-实时数据
#endif

#ifndef NH_ASM_STATUS_REALTIME_MESSAGE
#define NH_ASM_STATUS_REALTIME_MESSAGE 0x05 // 检测状态-实时信息
#endif

#ifndef NH_ASM_STATUS_REALTIME_VELOCITY
#define NH_ASM_STATUS_REALTIME_VELOCITY 0x06 // 检测状态-实时速度
#endif

#ifndef NH_ASM_STATUS_START_5025_TEST
#define NH_ASM_STATUS_START_5025_TEST 0x07 // 检测状态-开始5025检测
#endif

#ifndef NH_ASM_STATUS_START_2540_TEST
#define NH_ASM_STATUS_START_2540_TEST 0x08 // 检测状态-开始2540检测
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ASM检测流程宏定义end

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// VMAS检测流程宏定义begin
#ifndef NH_VMAS_TEST_ERROR
#define NH_VMAS_TEST_ERROR	0xFF // 程序错误
#endif

#ifndef NH_VMAS_TEST_FINISH
#define NH_VMAS_TEST_FINISH	0x00 // 检测完成
#endif

#ifndef NH_VMAS_TEST_EXIT
#define NH_VMAS_TEST_EXIT	0x01 // 检测退出
#endif

#ifndef NH_VMAS_TEST_PREV
#define NH_VMAS_TEST_PREV	0x02 // 上一步
#endif

#ifndef NH_VMAS_RESULT_ERROR
#define NH_VMAS_RESULT_ERROR		0xFF // 程序错误
#endif

#ifndef NH_VMAS_RESULT_CONTINUE
#define NH_VMAS_RESULT_CONTINUE	0x00 // 检测完成继续检测
#endif

#ifndef NH_VMAS_RESULT_EXIT
#define NH_VMAS_RESULT_EXIT		0x01 // 检测完成返回主界面
#endif

#ifndef NH_VMAS_STATUS_START_TEST
#define NH_VMAS_STATUS_START_TEST 0x01 // 检测状态-开始
#endif

#ifndef NH_VMAS_STATUS_STOP_TEST
#define NH_VMAS_STATUS_STOP_TEST 0x02 // 检测状态-停止
#endif

#ifndef NH_VMAS_STATUS_FINISH_TEST
#define NH_VMAS_STATUS_FINISH_TEST 0x03 // 检测状态-结束
#endif

#ifndef NH_VMAS_STATUS_REALTIME_DATA
#define NH_VMAS_STATUS_REALTIME_DATA 0x04 // 检测状态-实时数据
#endif

#ifndef NH_VMAS_STATUS_TESTING_15KM
#define NH_VMAS_STATUS_TESTING_15KM 0x05 // 检测状态-等速15公里
#endif

#ifndef NH_VMAS_STATUS_TESTING_32KM
#define NH_VMAS_STATUS_TESTING_32KM 0x06 // 检测状态-实时数据
#endif

#ifndef NH_VMAS_STATUS_TESTING_50KM
#define NH_VMAS_STATUS_TESTING_50KM 0x07 // 检测状态-实时数据
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// VMAS检测流程宏定义end


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 双怠速检测流程宏定义begin
#ifndef NH_DIS_TEST_ERROR
#define NH_DIS_TEST_ERROR	0xFF // 程序错误
#endif

#ifndef NH_DIS_TEST_FINISH
#define NH_DIS_TEST_FINISH	0x00 // 检测完成
#endif

#ifndef NH_DIS_TEST_EXIT
#define NH_DIS_TEST_EXIT	0x01 // 检测退出
#endif

#ifndef NH_DIS_TEST_PREV
#define NH_DIS_TEST_PREV	0x02 // 上一步
#endif

#ifndef NH_DIS_RESULT_ERROR
#define NH_DIS_RESULT_ERROR		0xFF // 程序错误
#endif

#ifndef NH_DIS_RESULT_CONTINUE
#define NH_DIS_RESULT_CONTINUE	0x00 // 检测完成继续检测
#endif

#ifndef NH_DIS_RESULT_EXIT
#define NH_DIS_RESULT_EXIT		0x01 // 检测完成返回主界面
#endif

#ifndef NH_DIS_STATUS_START_TEST
#define NH_DIS_STATUS_START_TEST 0x01 // 检测状态-开始
#endif

#ifndef NH_DIS_STATUS_STOP_TEST
#define NH_DIS_STATUS_STOP_TEST 0x02 // 检测状态-停止
#endif

#ifndef NH_DIS_STATUS_FINISH_TEST
#define NH_DIS_STATUS_FINISH_TEST 0x03 // 检测状态-结束
#endif

#ifndef NH_DIS_STATUS_REALTIME_DATA_HIS
#define NH_DIS_STATUS_REALTIME_DATA_HIS 0x04 // 检测状态-实时数据(高怠速)
#endif

#ifndef NH_DIS_STATUS_REALTIME_DATA_IS
#define NH_DIS_STATUS_REALTIME_DATA_IS 0x05 // 检测状态-实时数据(怠速)
#endif

#ifndef NH_DIS_STATUS_REALTIME_MESSAGE
#define NH_DIS_STATUS_REALTIME_MESSAGE 0x06 // 检测状态-实时信息
#endif

#ifndef NH_DIS_STATUS_REALTIME_REV
#define NH_DIS_STATUS_REALTIME_REV 0x07 // 检测状态-实时转速
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 双怠速检测流程宏定义end

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 自由加速检测流程宏定义begin
#ifndef NH_FSU_TEST_ERROR
#define NH_FSU_TEST_ERROR	0xFF // 程序错误
#endif

#ifndef NH_FSU_TEST_FINISH
#define NH_FSU_TEST_FINISH	0x00 // 检测完成
#endif

#ifndef NH_FSU_TEST_EXIT
#define NH_FSU_TEST_EXIT	0x01 // 检测退出
#endif

#ifndef NH_FSU_TEST_PREV
#define NH_FSU_TEST_PREV	0x02 // 上一步
#endif

#ifndef NH_FSU_RESULT_ERROR
#define NH_FSU_RESULT_ERROR		0xFF // 程序错误
#endif

#ifndef NH_FSU_RESULT_CONTINUE
#define NH_FSU_RESULT_CONTINUE	0x00 // 检测完成继续检测
#endif

#ifndef NH_FSU_RESULT_EXIT
#define NH_FSU_RESULT_EXIT		0x01 // 检测完成返回主界面
#endif

#ifndef NH_FSU_STATUS_START_TEST
#define NH_FSU_STATUS_START_TEST 0x01 // 检测状态-开始
#endif

#ifndef NH_FSU_STATUS_STOP_TEST
#define NH_FSU_STATUS_STOP_TEST 0x02 // 检测状态-停止
#endif

#ifndef NH_FSU_STATUS_FINISH_TEST
#define NH_FSU_STATUS_FINISH_TEST 0x03 // 检测状态-结束
#endif

#ifndef NH_FSU_STATUS_REALTIME_DATA
#define NH_FSU_STATUS_REALTIME_DATA 0x04 // 检测状态-实时数据
#endif

#ifndef NH_FSU_STATUS_REALTIME_MESSAGE
#define NH_FSU_STATUS_REALTIME_MESSAGE 0x05 // 检测状态-实时信息
#endif

#ifndef NH_FSU_STATUS_REALTIME_REV
#define NH_FSU_STATUS_REALTIME_REV 0x06 // 检测状态-实时转速
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 自由加速检测流程宏定义end

///////////////////////////////////////////////// 摩托车怠速检测流程宏定义begin
#ifndef NH_MI_TEST_ERROR
#define NH_MI_TEST_ERROR	0xFF // 程序错误
#endif

#ifndef NH_MI_TEST_FINISH
#define NH_MI_TEST_FINISH	0x00 // 检测完成
#endif

#ifndef NH_MI_TEST_EXIT
#define NH_MI_TEST_EXIT	0x01 // 检测退出
#endif

#ifndef NH_MI_TEST_PREV
#define NH_MI_TEST_PREV	0x02 // 上一步
#endif

#ifndef NH_MI_RESULT_ERROR
#define NH_MI_RESULT_ERROR		0xFF // 程序错误
#endif

#ifndef NH_MI_RESULT_CONTINUE
#define NH_MI_RESULT_CONTINUE	0x00 // 检测完成继续检测
#endif

#ifndef NH_MI_RESULT_EXIT
#define NH_MI_RESULT_EXIT		0x01 // 检测完成返回主界面
#endif

#ifndef NH_MI_STATUS_START_TEST
#define NH_MI_STATUS_START_TEST 0x01 // 检测状态-开始
#endif

#ifndef NH_MI_STATUS_STOP_TEST
#define NH_MI_STATUS_STOP_TEST 0x02 // 检测状态-停止
#endif

#ifndef NH_MI_STATUS_FINISH_TEST
#define NH_MI_STATUS_FINISH_TEST 0x03 // 检测状态-结束
#endif

#ifndef NH_MI_STATUS_REALTIME_DATA_IS1
#define NH_MI_STATUS_REALTIME_DATA_IS1 0x04 // 检测状态-实时数据(怠速1)
#endif

#ifndef NH_MI_STATUS_REALTIME_DATA_IS2
#define NH_MI_STATUS_REALTIME_DATA_IS2 0x05 // 检测状态-实时数据(怠速2)
#endif

#ifndef NH_MI_STATUS_REALTIME_MESSAGE
#define NH_MI_STATUS_REALTIME_MESSAGE 0x06 // 检测状态-实时信息
#endif

#ifndef NH_MI_STATUS_REALTIME_REV
#define NH_MI_STATUS_REALTIME_REV 0x07 // 检测状态-实时转速
#endif
//////////////////////////////////////////////// 摩托车检测流程宏定义end

//////////////////////////////////////////////// 摩托车双怠速检测流程宏定义begin
#ifndef NH_MDIS_TEST_ERROR
#define NH_MDIS_TEST_ERROR	0xFF // 程序错误
#endif

#ifndef NH_MDIS_TEST_FINISH
#define NH_MDIS_TEST_FINISH	0x00 // 检测完成
#endif

#ifndef NH_MDIS_TEST_EXIT
#define NH_MDIS_TEST_EXIT	0x01 // 检测退出
#endif

#ifndef NH_MDIS_TEST_PREV
#define NH_MDIS_TEST_PREV	0x02 // 上一步
#endif

#ifndef NH_MDIS_RESULT_ERROR
#define NH_MDIS_RESULT_ERROR		0xFF // 程序错误
#endif

#ifndef NH_MDIS_RESULT_CONTINUE
#define NH_MDIS_RESULT_CONTINUE	0x00 // 检测完成继续检测
#endif

#ifndef NH_MDIS_RESULT_EXIT
#define NH_MDIS_RESULT_EXIT		0x01 // 检测完成返回主界面
#endif

#ifndef NH_MDIS_STATUS_START_TEST
#define NH_MDIS_STATUS_START_TEST 0x01 // 检测状态-开始
#endif

#ifndef NH_MDIS_STATUS_STOP_TEST
#define NH_MDIS_STATUS_STOP_TEST 0x02 // 检测状态-停止
#endif

#ifndef NH_MDIS_STATUS_FINISH_TEST
#define NH_MDIS_STATUS_FINISH_TEST 0x03 // 检测状态-结束
#endif

#ifndef NH_MDIS_STATUS_REALTIME_DATA_HIS
#define NH_MDIS_STATUS_REALTIME_DATA_HIS 0x04 // 检测状态-实时数据(高怠速)
#endif

#ifndef NH_MDIS_STATUS_REALTIME_DATA_IS
#define NH_MDIS_STATUS_REALTIME_DATA_IS 0x05 // 检测状态-实时数据(怠速)
#endif

#ifndef NH_MDIS_STATUS_REALTIME_MESSAGE
#define NH_MDIS_STATUS_REALTIME_MESSAGE 0x06 // 检测状态-实时信息
#endif

#ifndef NH_MDIS_STATUS_REALTIME_REV
#define NH_MDIS_STATUS_REALTIME_REV 0x07 // 检测状态-实时转速
#endif
//////////////////////////////////////////////// 摩托车双怠速检测流程宏定义end

/////////////////////////////////////////////// 检测准备流程宏定义begin
#ifndef NH_TESTPREPARE_STATUS_REALTIME_MESSAGE
#define NH_TESTPREPARE_STATUS_REALTIME_MESSAGE 0x04  //检测状态-当前信息
#endif
#ifndef NH_TESTPREPARE_STATUS_REALTIME_MESSAGE_DRIVER
#define NH_TESTPREPARE_STATUS_REALTIME_MESSAGE_DRIVER 0x05  //检测状态-当前引车员
#endif
#ifndef NH_TESTPREPARE_STATUS_REALTIME_MESSAGE_RN
#define NH_TESTPREPARE_STATUS_REALTIME_MESSAGE_RN 0x06  //检测状态-当前流水号
#endif
////////////////////////////////////////////// 检测准备流程宏定义end

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 安全检测提示流程宏定义begin
#ifndef NH_SAFE_WARNING_PREV
#define NH_SAFE_WARNING_PREV        0x01     // 上一步
#endif

#ifndef NH_SAFE_WARNING_NEXT
#define NH_SAFE_WARNING_NEXT        0x02     // 下一步
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 安全检测提示流程宏定义end

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 测功机加载滑行流程宏定义begin
#define NH_DYN_CL_START_LOW			0x01	// 加载滑行(低速)-开始
#define NH_DYN_CL_START_HIGH		0x02	// 加载滑行(高速)-开始
#define NH_DYN_CL_STOP				0x03	// 加载滑行-停止
#define NH_DYN_CL_FINISH			0x04	// 加载滑行-完成
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 测功机加载滑行流程宏定义end

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 测功机寄生功率滑行流程宏定义begin
#define NH_DYN_PLHP_START			0x01	// 寄生功率滑行-开始
#define NH_DYN_PLHP_STOP			0x02	// 寄生功率滑行-停止
#define NH_DYN_PLHP_FINISH			0x03	// 寄生功率滑行-完成
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 测功机寄生功率滑行流程宏定义end

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 测功机力校准流程宏定义begin
#define NH_DYN_FORCE_CAL_START		0x01	// 力校准-开始
#define NH_DYN_FORCE_CAL_STOP		0x02	// 力校准-停止
#define NH_DYN_FORCE_CAL_FINISH		0x03	// 力校准-完成
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 测功机力校准流程宏定义end

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 测功机基本惯量流程宏定义begin
#define NH_DYN_DIW_START		0x01	// 基本惯量-开始
#define NH_DYN_DIW_STOP		0x02	// 基本惯量-停止
#define NH_DYN_DIW_FINISH		0x03	// 基本惯量-完成
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 测功机基本惯量流程宏定义end

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 测功机力检查流程宏定义begin
#define NH_DYN_FORCE_CHK_START		0x01	// 力检查-开始
#define NH_DYN_FORCE_CHK_STOP		0x02	// 力检查-停止
#define NH_DYN_FORCE_CHK_FINISH		0x03	// 力检查-完成
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 测功机力检查流程宏定义end

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 分析仪气体校准流程宏定义begin
#define NH_ANA_GAS_CAL_START		0x01	// 气体校准-开始
#define NH_ANA_GAS_CAL_STOP			0x02	// 气体校准-停止
#define NH_ANA_GAS_CAL_FINISH		0x03	// 气体校准-完成
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 分析仪气体校准流程宏定义end

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 分析仪气体检查流程宏定义begin
#define NH_ANA_GAS_CHK_START		0x01	// 气体检查-开始
#define NH_ANA_GAS_CHK_STOP			0x02	// 气体检查-停止
#define NH_ANA_GAS_CHK_FINISH		0x03	// 气体检查-完成
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 分析仪气体检查流程宏定义end

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 不透光校准检查流程宏定义begin
#define NH_OPA_CAL_CHK_START		0x01	// 不透光校准检查-开始
#define NH_OPA_CAL_CHK_STOP			0x02	// 不透光校准检查-停止
#define NH_OPA_CAL_CHK_FINISH		0x03	// 不透光校准检查-完成
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 不透光校准检查流程宏定义end

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 不透光线性检查流程宏定义begin
#define NH_OPA_LINE_CHK_START		0x01	// 不透光线性检查-开始
#define NH_OPA_LINE_CHK_STOP		0x02	// 不透光线性检查-停止
#define NH_OPA_LINE_CHK_FINISH		0x03	// 不透光线性检查-完成
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 不透光线性检查流程宏定义end

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 烟度计检查流程宏定义begin
#define NH_SMO_CHK_START			0x01	// 烟度计检查-开始
#define NH_SMO_CHK_STOP				0x02	// 烟度计检查-停止
#define NH_SMO_CHK_FINISH			0x03	// 烟度计检查-完成
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 烟度计检查流程宏定义end

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 流量计流量校准流程宏定义begin
#define NH_FLO_FLUXCAL_START			0x01	// 流量计流量校准-开始
#define NH_FLO_FLUXCAL_STOP				0x02	// 流量计流量校准-停止
#define NH_FLO_FLUXCAL_FINISH			0x03	// 流量计流量校准-完成
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 流量计流量校准流程宏定义end

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 流量计流量检查流程宏定义begin
#define NH_FLO_FLUXCHK_START			0x01	// 流量计流量检查-开始
#define NH_FLO_FLUXCHK_STOP				0x02	// 流量计流量检查-停止
#define NH_FLO_FLUXCHK_FINISH			0x03	// 流量计流量检查-完成
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 流量计流量检查流程宏定义end

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 流量计稀释氧校准流程宏定义begin
#define NH_FLO_DILO2CAL_START			0x01	// 流量计稀释氧校准-开始
#define NH_FLO_DILO2CAL_STOP			0x02	// 流量计稀释氧校准-停止
#define NH_FLO_DILO2CAL_FINISH			0x03	// 流量计稀释氧校准-完成
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 流量计稀释氧校准流程宏定义end

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 流量计压力校准流程宏定义begin
#define NH_FLO_PRESCAL_START			0x01	// 流量计压力校准-开始
#define NH_FLO_PRESCAL_STOP				0x02	// 流量计压力校准-停止
#define NH_FLO_PRESCAL_FINISH			0x03	// 流量计压力校准-完成
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 流量计压力校准流程宏定义end

#ifndef ERROR_MAX
#define ERROR_MAX 1024 // 出错最大输出字符数
#endif

#ifndef SQLMAX
#define SQLMAX 1024 // sql 语句最大输入字符数
#endif


// 消息定义

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 测功机部分begin
// 规定从WM_USER + 0x0001 到 WM_USER + 0x0100
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////执行测功机异步操作消息begin
#define WM_DYN_GET_REALTIME_DATA				WM_USER + 0x0001  // 取测功机实时数据
#define WM_DYN_GET_STATUS						WM_USER + 0x0002  // 取测功机状态
#define WM_DYN_READ_VAR						    WM_USER + 0x0003  // 读测功机变量
#define WM_DYN_WRITE_VAR						WM_USER + 0x0004  // 写测功机变量
#define WM_DYN_SEND_CONTROL_WORD				WM_USER + 0x0005  // 发送测功机控制字符
#define WM_DYN_START_CONST_LOAD_TEST			WM_USER + 0x0006  // 开始测功机定载荷测试
#define WM_DYN_CONST_LOAD_TEST_FINISHED		    WM_USER + 0x0007  // 测功机定载荷测试完成
#define WM_DYN_START_DIW_TEST					WM_USER + 0x0008  // 开始测功机基本惯量测试
#define WM_DYN_START_PLHP_TEST					WM_USER + 0x0009  // 开始测功机寄生功率测试
#define WM_DYN_PLHP_TEST_FINISHED				WM_USER + 0x000A  // 测功机寄生功率测试完成
#define WM_DYN_PLHP_SECTION_FINISHED			WM_USER + 0x000B  // 寄生功率某一个速度段完成
#define WM_DYN_START_VARLOAD_TEST				WM_USER + 0x000C  // 开始测功机基本惯量测试
#define WM_DYN_VARLOAD_TEST_FINISHED			WM_USER + 0x000E  // 测功机寄生功率测试完成
#define WM_DYN_START_RESPONSE_TIME_TEST			WM_USER + 0x000F  // 开始测功机响应时间测试
#define WM_DYN_START_RESPONSE_TIME_FINISHED		WM_USER + 0x0010  // 开始测功机响应时间完成
#define WM_DYN_SAVE_RESPONSE_TIME_DATA		    WM_USER + 0x0011  // 保存测功机响应时间实时数据

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////执行测功机异步操作消息end
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 测功机部分end


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 分析仪部分begin
// 规定从WM_USER + 0x0101 到 WM_USER + 0x0200
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////执行分析仪异步操作消息begin
#define WM_ANA_GET_MAJOR_DATA                  WM_USER + 0x0101  // 取主数据
#define WM_ANA_GET_MINOR_DATA			       WM_USER + 0x0102  // 取辅助数据
#define WM_ANA_GET_PEF		                   WM_USER + 0x0103  // 取Pef值
#define WM_ANA_GET_STATUS	                   WM_USER + 0x0104  // 取仪器状态
#define WM_ANA_GASCALIBRATION                  WM_USER + 0x0105  // 分析仪校准
#define WM_ANA_GASCHECK                        WM_USER + 0x0106  // 分析仪校准
#define WM_ANA_GET_MAJOR_DATA_N                WM_USER + 0x0107  // 取主数据（支持取水位信息）
#define WM_ANA_GET_NOX_DATA					   WM_USER + 0x0108  // 取NOx数据
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////执行分析仪异步操作消息end
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 分析仪部分end


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 流量计部分begin
// 规定从WM_USER + 0x0201 到 WM_USER + 0x0300
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////执行流量计异步操作消息begin
#define WM_ANA_VMAS_REALTIME_DATA				WM_USER + 0x0201  // Vmas实时数据
#define WM_ANA_VMAS_PDS_WT_DATA					WM_USER + 0x0202  // Vmas195秒质量数据
#define WM_ANA_VMAS_PDS_CT_DATA					WM_USER + 0x0203  // Vmas195秒浓度数据
#define WM_ANA_VMAS_TOTAL_WT_DATA				WM_USER + 0x0204  // Vmas总克数
#define WM_ANA_VMAS_START_COLLECT_DATA			WM_USER + 0x0205  // 开始采集数据
#define WM_ANA_VMAS_STOP_COLLECT_DATA			WM_USER + 0x0206  // 停止采集数据
#define WM_ANA_VMAS_START_GETENVIRONMENT_O2		WM_USER + 0x0207  // 开始采集环境氧
#define WM_ANA_VMAS_GET_ENVIRONMENT_O2			WM_USER + 0x0208  // 获取环境氧
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////执行流量计异步操作消息end
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 流量计部分end


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 不透光度计部分begin
// 规定从WM_USER + 0x0301 到 WM_USER + 0x0400
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////执行不透光度计异步操作消息begin
#define WM_OPA_CALIBRATE			        WM_USER + 0x0301  // 校准
#define WM_OPA_GET_REALTIME_DATA			WM_USER + 0x0302  // 取NHT1L实时数据
#define WM_OPA_GET_MAX_VALUE				WM_USER + 0x0303  // 取NHT1L最大值数据
#define WM_OPA_GET_ENV_REALTIME_DATA		WM_USER + 0x0304  // 取NHT1L实时环境参数
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////执行不透光度计异步操作消息end
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 不透光度计部分end


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 烟度计部分begin
// 规定从WM_USER + 0x0401 到 WM_USER + 0x0500
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////执行烟度计异步操作消息begin
#define WM_SMO_RESET			            WM_USER + 0x0401  // 复位
#define WM_SMO_GET_DATA1234					WM_USER + 0x0402  // 获取实时数据1234
#define WM_SMO_REPEAT_BEGIN                 WM_USER + 0x0403
#define WM_SMO_REAL_BEGIN                   WM_USER + 0x0404
#define WM_SMO_REAL_END                     WM_USER + 0x0405
#define WM_SMO_REPEAT_END                   WM_USER + 0x0406
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////执行烟度计异步操作消息end
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 烟度计部分end


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 转速计部分begin
// 规定从WM_USER + 0x0501 到 WM_USER + 0x0600
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////执行转速计异步操作消息begin
#define WM_TAC_GET_REV			WM_USER + 0x0501
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////执行转速计异步操作消息end
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 转速计部分end


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 其他消息定义begin
// 规定从WM_USER + 0x0601 到 WM_USER + 0x0700(其他消息定义)

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////执行设备准备消息begin
#define WM_GASOLINE_VEHICLE_EQUIPMENT_PREPARE		WM_USER + 0x0601
#define WM_GASOLINE_VEHICLE_EQUIPMENT_STARTAUTOPREPARE		WM_USER + 0x0602
#define WM_DIESEL_VEHICLE_EQUIPMENT_PREPARE			WM_USER + 0x0603
#define WM_DIESEL_VEHICLE_EQUIPMENT_STARTAUTOPREPARE		WM_USER + 0x0604
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////执行设备准备消息end

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////执行系统自检消息begin
#define WM_SYSTEM_CHECK_ANA_WORKING			WM_USER + 0x0603
#define WM_SYSTEM_CHECK_FLO_WORKING			WM_USER + 0x0604
#define WM_SYSTEM_CHECK_DYN_WORKING			WM_USER + 0x0605
#define WM_SYSTEM_CHECK_OPA_WORKING			WM_USER + 0x0606
#define WM_SYSTEM_CHECK_NHV_WORKING			WM_USER + 0x0607
#define WM_SYSTEM_CHECK_RPM_WORKING			WM_USER + 0x0608
#define WM_SYSTEM_CHECK_ANA_TESTCOMMUNICATION			WM_USER + 0x0609
#define WM_SYSTEM_CHECK_NOX_WORKING			WM_USER + 0x060A

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////执行系统自检消息end

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 其他消息定义end

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 键盘映射消息定义begin
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
#define WM_HOTKEY_PRINT_SCREEN		WM_USER+0x0712		// 截屏快捷键消息
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 键盘映射消息定义end

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 参数采集仪部分begin
// 规定从WM_USER + 0x0801 到 WM_USER + 0x0900
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////执行参数采集仪部分异步操作消息begin
#define WM_PARA_GET_ENTIRE_DATA			            WM_USER + 0x0801  // 读全部数据
#define WM_PARA_GET_ENVIRONMENT_PARAMETERS			WM_USER + 0x0802  // 读环境参数
#define WM_PARA_GET_OILTEMP_RPM						WM_USER + 0x0803  // 读转速油温
#define WM_PARA_DT_COMMAND							WM_USER + 0x0804  // DT命令
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////执行参数采集仪部分异步操作消息end
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 参数采集仪部分end

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 条形码扫描仪部分begin
// 规定从WM_USER + 0x0901 到 WM_USER + 0x1000
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////执行条形码扫描仪异步操作消息begin
#define WM_BCS_GET_VALUE			WM_USER + 0x0901
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////执行条形码扫描仪异步操作消息end
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 条形码扫描仪部分end


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 仪器最大范围部分begin
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////废气分析仪begin
#define ANA_CO_MAX			1400		// CO最大值,放大100倍,单位:0.01%
#define ANA_HC_MAX			10000		// HC最大值,单位:10-6
#define ANA_NO_MAX			5000		// NO最大值,单位:10-6
#define ANA_CO2_MAX			1800		// CO2最大值,放大100倍,单位:0.01%
#define ANA_O2_MAX			2500		// O2最大值,放大100倍,单位:0.01%
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////废气分析仪end
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////不透光begin
#define OPA_K_MAX			1606		// K最大值,放大100倍,单位:0.01m-1
#define OPA_N_MAX			9990		// N最大值,放大100倍,单位:0.01%
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////不透光end
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////烟度计begin
#define SMO_RB_MAX			1000		// 烟度值最大值,放大100倍,单位:0.01Rb
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////烟度计end
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 仪器最大范围部分end

// 定义检测类型
enum NHTestType
{
	ASM = 1,		// 稳态						1
	VMAS,			// 简易瞬态					2
	LUGDOWN,		// 加载减速					3
	DIS,			// 双怠速					4
	FSUNHT,			// 不透光自由加速			5
	FSUYD,			// 滤纸式自由加速			6
	AVFSU,			// 农用车自由加速			7
	MI,				// 摩托车怠速				8
	MDIS,			// 摩托车双怠速				9
};

// 定义车辆检测状态
enum NHTestState
{
	WAIT_FOR_TESTING = 0,	// 车辆待检测								0
	TESTING,				// 单燃料车辆正在检测						1
	TESTING_FUEL1,			// 双燃料车辆正在检测第一种燃料				2
	TESTING_FUEL2,			// 双燃料车辆正在检测第二种燃料				3
	WAIT_FOR_TESTING_FUEL2,	// 双燃料车辆等待检测第二种燃料				4
	TESTED,					// 全部完成检测								5
	CANCEL_TEST,			// 车辆取消检测								6
	SELECTED_FUEL1,			// 单燃料车辆选中或双燃料第一种燃料已选中	7
	WAIT_FOR_CHARGING,		// 车辆未收费,待收费						8
	SELECTED_FUEL2,			// 双燃料车辆第二种燃料已选中				9
	READ					// （伺服器）已经读取该车辆信息				10
};

// 定义标定项目
enum NHDemarcationItem
{
	DYN_CL = 1,		// 测功机加载滑行		1
	DYN_PLHP,		// 测功机寄生功率滑行	2
	DYN_FORCE_CAL,	// 测功机力校准		3
	DYN_FORCE_CHK,	// 测功机力检查		4
	ANA_GAS_CAL,	// 分析仪气体校准		5
	ANA_GAS_CHK,	// 分析仪气体检查		6
	OPA_CAL_CHK,	// 不透光校准检查		7
	SMO_CHK,		// 滤纸式烟度计检查	8
	FLO_FLUXCAL,	// 流量计流量校准		9
	FLO_FLUXCHK,	// 流量计流量检查		10
	FLO_DILO2CAL,	// 流量计稀释氧校准	11
	FLO_PRESCAL,	// 流量计压力校准		12
	DYN_DIW,		// 测功机基本惯量		13
	NOX_GAS_CAL,	// 氮氧分析仪气体校准	14
	NOX_GAS_CHK,	// 氮氧分析仪气体检查	15
};

// 定义角色类型
enum NHRoleType
{
	EPB = 0,		// 环保局	0
	STATIONMASTER,	// 站  长	1
	ADMIN,			// 管理员	2
	OPERATOR,		// 操作员	3
	BILLER,			// 登录员	4
	SHROFF,			// 收费员	5
	DRIVER,			// 引车员	6
	ASSESSOR,		// 审核员	7
	INSPECTOR,		// 外检员	8
};

// 定义检测线状态
enum NHLineState
{
	EQU_SHUTDOWN = 0,		// 设备关闭	0
	EQU_FREE,				// 空闲		1
	EQU_TESTING,			// 正在检测	2
	EQU_SYSTEMCHECK,		// 正在自检	3
	EQU_DEMARCATION,		// 正在标定	4
};

// 定义检测性质
enum NHTestCharacter
{
	TC_OffLine = 1,		// 新车下线检验	1
	TC_Import,			// 进口车检验		2
	TC_Register,		// 注册登记检验	3
	TC_InUse,			// 在用车检验		4
};

//精度：3位小数
//HC：86/22.4g/L=3.839
//CO:28/22.4g/L=1.250
//NOx:46/22.4g/L=2.050
//CO2:44/22.4g/L=1.964
//转化为mg/L
const int nHC_Density = 3839;
const int nCO_Density = 1250;
const int nNOx_Density = 2050;
const int nCO2_Density = 1964;

// 转速测量方式
enum RevMeasureMode
{
	// 跳过测量转速
	RMM_FromNone = 0,
	// 从接到串口的转速适配器测量转速
	RMM_FromTachometer,
	// 从分析仪测量转速
	RMM_FromAnalyzer,
	// 从不透光度计测量转速
	RMM_FromOpacimeter,
	// 从参数采集仪(司机助手仪)
	RMM_FromParameter,
	// 从OBD读取
	RMM_FromOBD,
};

// 油温测量方式
enum OilTempMeasureMode
{
	// 跳过油温
	OTMM_FromNone = 0,
	// 从分析仪测量转速
	OTMM_FromAnalyzer,
	// 从不透光测量转速
	OTMM_FromOpacimeter,
	// 从参数采集仪(司机助手仪)
	OTMM_FromParameter,
	// 从OBD读取
	OTMM_FromOBD,
};

// 冲程方式
enum StrokeType
{
	// 2冲程
	ST_TwoStroke = 2,
	// 4冲程
	ST_FourStroke = 4,
};
// 点火方式
enum FireType
{
	// 单次点火
	FT_SingleFire = 1,
	// 二次点火
	FT_DoubleFire = 2,
};

// 过程数的负值处理,负值转零
#define NZ(x)	((x)<0 ? 0 : (x))

#endif
