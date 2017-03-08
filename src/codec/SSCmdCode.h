/*************************************************************************
	> File Name: SSCmdCode.h
	> Author:
	> Mail:
	> Created Time: Wed 15 Jul 2015 11:40:26 AM CST
 ************************************************************************/

#ifndef _SSCMDCODE_H
#define _SSCMDCODE_H

//system business
const uint32_t MSG_DS_MS_REGISTER = 0x1001;
const uint32_t MSG_DS_MS_REGISTER_ACK = 0x1002;

const uint32_t MSG_MS_DS_STREAMSTORE_PREPARE = 0x1003;
const uint32_t MSG_MS_DS_STREAMSTORE_PREPARE_ACK = 0x1004;

const uint32_t MSG_DS_MS_INVITE_CAMERA = 0x1005;
const uint32_t MSG_DS_MS_INVITE_CAMERA_ACK = 0x1006;

const uint32_t MSG_MS_DS_READ_STREAM = 0x1007;
const uint32_t MSG_MS_DS_READ_STREAM_ACK = 0x1008;
const uint32_t MSG_DS_MS_READ_STREAM_FINISH = 0x1009;
const uint32_t MSG_DS_MS_READ_STREAM_FINISH_ACK = 0x100A;

const uint32_t MSG_DS_MS_SYS_HEARTBEAT = 0x100B;
const uint32_t MSG_DS_MS_SYS_HEARTBEAT_ACK = 0x100C;

const uint32_t MSG_DS_MS_STREAMMETA_HEARTBEAT = 0x100D;
const uint32_t MSG_DS_MS_STREAMMETA_HEARTBEAT_ACK = 0x100E;

const uint32_t MSG_DS_MS_STORE_UNAVAILABLE = 0x100F;
const uint32_t MSG_DS_MS_STORE_UNAVAILABLE_ACK = 0x1010;

const uint32_t MSG_MS_DS_STOPALL_STREAMSTORE = 0x1011;
const uint32_t MSG_MS_DS_STOPALL_STREAMSTORE_ACK = 0x1012;

const uint32_t MSG_DS_MS_STORE_FINISH = 0x1013;
const uint32_t MSG_DS_MS_STORE_FINISH_ACK = 0x1014;

const uint32_t MSG_MS_DS_SLAVE_TO_MASTER = 0x1101;
const uint32_t MSG_MS_DS_SLAVE_TO_MASTER_ACK = 0x1102;





//new protobuf cmd
//TODO fix difference in DS_MAIN and DS_STORE
const uint32_t MSG_DS_STORE_MAIN_REGISTER = 0x601;
const uint32_t MSG_DS_STORE_MAIN_REGISTER_ACK =0x602;
const uint32_t MSG_DS_MAIN_STORE_PREPARE_STORE = 0x603;
const uint32_t MSG_DS_MAIN_STORE_PREPARE_STORE_ACK = 0x604;
const uint32_t MSG_DS_MAIN_STORE_START_STORE = 0x605;
const uint32_t MSG_DS_MAIN_STORE_START_STORE_ACK = 0x606;
const uint32_t MSG_MS_DS_STOREPLAN = 0x610;
const uint32_t MSG_MS_DS_DSREGISTER = 0x611;
const uint32_t MSG_MS_DS_DSREGISTERACK = 0x612;
const uint32_t MSG_STORE_ADMIN_REGISTER = 0x613;





const uint32_t MSG_PLATFORMMANAGEMENT_WD_START_MAINMS = 0x2001;
const uint32_t MSG_PLATFORMMANAGEMENT_WD_START_MAINMS_ACK = 0x2002;

const uint32_t MSG_PLATFORMMANAGEMENT_WD_START_OTHERMS = 0x2011;//二期实现
const uint32_t MSG_PLATFORMMANAGEMENT_WD_START_OTHERMS_ACK = 0x2012;

const uint32_t MSG_WD_WD_START_MS = 0x2003;//二期实现
const uint32_t MSG_WD_WD_START_MS_ACK = 0x2004;

const uint32_t MSG_PLATFORMMANAGEMENT_WD_START_DS = 0x2005;
const uint32_t MSG_PLATFORMMANAGEMENT_WD_START_DS_ACK = 0x2006;

const uint32_t MSG_WD_WD_START_DS = 0x2007;
const uint32_t MSG_WD_WD_START_DS_ACK = 0x2008;

const uint32_t MSG_PLATFORMMANAGEMENT_WD_CONFIG_MAINMS = 0x2009;
const uint32_t MSG_PLATFORMMANAGEMENT_WD_CONFIG_MAINMS_ACK = 0x200A;

const uint32_t MSG_PLATFORMMANAGEMENT_WD_CONFIG_OTHERMS = 0x2013;//二期实现
const uint32_t MSG_PLATFORMMANAGEMENT_WD_CONFIG_OTHERMS_ACK = 0x2014;

const uint32_t MSG_WD_WD_CONFIG_MS = 0x200B;//二期实现
const uint32_t MSG_WD_WD_CONFIG_MS_ACK = 0x200C;

const uint32_t MSG_PLATFORMMANAGEMENT_WD_CONFIG_DS = 0x200D;
const uint32_t MSG_PLATFORMMANAGEMENT_WD_CONFIG_DS_ACK = 0x200E;

const uint32_t MSG_WD_WD_CONFIG_DS = 0x200F;
const uint32_t MSG_WD_WD_CONFIG_DS_ACK = 0x2010;

const uint32_t MSG_PLATFORMMANAGEMENT_WD_CONFIG_SPECIFY_DS = 0x2015;
const uint32_t MSG_PLATFORMMANAGEMENT_WD_CONFIG_SPECIFY_DS_ACK = 0x2016;

const uint32_t MSG_PLATFORMMANAGEMENT_WD_GET_DSCONFIG = 0x2017;
const uint32_t MSG_PLATFORMMANAGEMENT_WD_GET_DSCONFIG_ACK = 0x2018;

const uint32_t MSG_WD_WD_GET_DSCONFIG = 0x2019;
const uint32_t MSG_WD_WD_GET_DSCONFIG_ACK = 0x201A;

const uint32_t MSG_PLATFORMMANAGEMENT_WD_GET_MSCONFIG = 0x201B;
const uint32_t MSG_PLATFORMMANAGEMENT_WD_GET_MSCONFIG_ACK = 0x201C;

#endif
