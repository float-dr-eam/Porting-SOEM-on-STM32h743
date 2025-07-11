/*
 * Licensed under the GNU General Public License version 2 with exceptions. See
 * LICENSE file in the project root for full license information
 */

/** \file
 * \brief
 * Headerfile for ethercatmain.c
 */

#ifndef _ethercatmain_
#define _ethercatmain_


#ifdef __cplusplus
extern "C"
{
#endif

/** max. entries in EtherCAT error list */
#define EC_MAXELIST       64
/** max. length of readable name in slavelist and Object Description List */
#define EC_MAXNAME        40
/** max. number of slaves in array */
#define EC_MAXSLAVE       200
/** max. number of groups */
#define EC_MAXGROUP       2
/** max. number of IO segments per group */
#define EC_MAXIOSEGMENTS  64
/** max. mailbox size */
#define EC_MAXMBX         1486
/** max. eeprom PDO entries */
#define EC_MAXEEPDO       0x200
/** max. SM used */
#define EC_MAXSM          8
/** max. FMMU used */
#define EC_MAXFMMU        4
/** max. Adapter */
#define EC_MAXLEN_ADAPTERNAME    128
/** define maximum number of concurrent threads in mapping */
#define EC_MAX_MAPT           1

typedef struct ec_adapter ec_adaptert;
struct ec_adapter
{
   char   name[EC_MAXLEN_ADAPTERNAME];
   char   desc[EC_MAXLEN_ADAPTERNAME];
   ec_adaptert *next;
};

/** record for FMMU */
PACKED_BEGIN
typedef struct PACKED ec_fmmu
{
   uint32  LogStart;
   uint16  LogLength;
   uint8   LogStartbit;
   uint8   LogEndbit;
   uint16  PhysStart;
   uint8   PhysStartBit;
   uint8   FMMUtype;
   uint8   FMMUactive;
   uint8   unused1;
   uint16  unused2;
}  ec_fmmut;
PACKED_END

/** record for sync manager */
PACKED_BEGIN
typedef struct PACKED ec_sm
{
   uint16  StartAddr;
   uint16  SMlength;
   uint32  SMflags;
} ec_smt;
PACKED_END

PACKED_BEGIN
typedef struct PACKED ec_state_status
{
   uint16  State;
   uint16  Unused;
   uint16  ALstatuscode;
} ec_state_status;
PACKED_END

#define ECT_MBXPROT_AOE      0x0001
#define ECT_MBXPROT_EOE      0x0002
#define ECT_MBXPROT_COE      0x0004
#define ECT_MBXPROT_FOE      0x0008
#define ECT_MBXPROT_SOE      0x0010
#define ECT_MBXPROT_VOE      0x0020

#define ECT_COEDET_SDO       0x01
#define ECT_COEDET_SDOINFO   0x02
#define ECT_COEDET_PDOASSIGN 0x04
#define ECT_COEDET_PDOCONFIG 0x08
#define ECT_COEDET_UPLOAD    0x10
#define ECT_COEDET_SDOCA     0x20

#define EC_SMENABLEMASK      0xfffeffff

typedef struct ecx_context ecx_contextt;

/** for list of ethercat slaves detected */
typedef struct ec_slave
{
   /** state of slave */               // 从站状态
   uint16           state;
   /** AL status code */               // AL状态码
   uint16           ALstatuscode;
   /** Configured address */           // 配置地址
   uint16           configadr;
   /** Alias address */                // 别名地址
   uint16           aliasadr;
   /** Manufacturer from EEprom */     // 制造商
   uint32           eep_man;
   /** ID from EEprom */               // ID号
   uint32           eep_id;
   /** revision from EEprom */         // 版本号
   uint32           eep_rev;
   /** Interface type */               // 接口类型
   uint16           Itype;
   /** Device type */                  // 设备类型
   uint16           Dtype; 
   /** output bits */                  // 输出位数
   uint16           Obits;
   /** output bytes, if Obits < 8 then Obytes = 0 */   // 输出字节数
   uint32           Obytes;
   /** output pointer in IOmap buffer */   // 输出指针在IOmap缓冲区
   uint8            *outputs;
   /** startbit in first output byte */   // 第一个输出字节的起始位
   uint8            Ostartbit;
   /** input bits */                  // 输入位数
   uint16           Ibits;
   /** input bytes, if Ibits < 8 then Ibytes = 0 */   // 输入字节数
   uint32           Ibytes;
   /** input pointer in IOmap buffer */   // 输入指针在IOmap缓冲区
   uint8            *inputs;
   /** startbit in first input byte */  // 第一个输入字节的起始位
   uint8            Istartbit;
   /** SM structure */               // SM结构体
   ec_smt           SM[EC_MAXSM];
   /** SM type 0=unused 1=MbxWr 2=MbxRd 3=Outputs 4=Inputs */   // SM类型
   uint8            SMtype[EC_MAXSM];
   /** FMMU structure */             // FMMU结构体
   ec_fmmut         FMMU[EC_MAXFMMU];
   /** FMMU0 function */             // FMMU0功能
   uint8            FMMU0func;
   /** FMMU1 function */             // FMMU1功能
   uint8            FMMU1func;
   /** FMMU2 function */             // FMMU2功能
   uint8            FMMU2func;
   /** FMMU3 function */             // FMMU3功能
   uint8            FMMU3func;
   /** length of write mailbox in bytes, if no mailbox then 0 */   // 写邮箱长度
   uint16           mbx_l;
   /** mailbox write offset */       // 写邮箱偏移
   uint16           mbx_wo;
   /** length of read mailbox in bytes */   // 读邮箱长度
   uint16           mbx_rl;
   /** mailbox read offset */        // 读邮箱偏移
   uint16           mbx_ro;
   /** mailbox supported protocols */     // 邮箱支持的协议
   uint16           mbx_proto;
   /** Counter value of mailbox link layer protocol 1..7 */  // 邮箱链路层协议计数值
   uint8            mbx_cnt;
   /** has DC capability */          // 是否支持DC
   boolean          hasdc;
   /** Physical type; Ebus, EtherNet combinations */  // 物理类型
   uint8            ptype;
   /** topology: 1 to 3 links */  // 拓扑结构
   uint8            topology;
   /** active ports bitmap : ....3210 , set if respective port is active **/  // 活动端口位图
   uint8            activeports;
   /** consumed ports bitmap : ....3210, used for internal delay measurement **/ // 消耗端口位图
   uint8            consumedports;
   /** slave number for parent, 0=master */  // 父从站编号
   uint16           parent;
   /** port number on parent this slave is connected to **/ // 父从站端口号
   uint8            parentport;  
   /** port number on this slave the parent is connected to **/   // 本从站端口号
   uint8            entryport;
   /** DC receivetimes on port A */   // DC接收次数
   int32            DCrtA;
   /** DC receivetimes on port B */  // DC接收次数
   int32            DCrtB;
   /** DC receivetimes on port C */ // DC接收次数
   int32            DCrtC;
   /** DC receivetimes on port D */ // DC接收次数
   int32            DCrtD;
   /** propagation delay */      // 传播延迟
   int32            pdelay;
   /** next DC slave */          // 下一个DC从站
   uint16           DCnext;
   /** previous DC slave */      // 上一个DC从站
   uint16           DCprevious;
   /** DC cycle time in ns */    // DC周期时间
   int32            DCcycle;
   /** DC shift from clock modulus boundary */  // DC移位
   int32            DCshift;
   /** DC sync activation, 0=off, 1=on */  // DC同步激活
   uint8            DCactive;
   /** link to config table */   // 链接到配置表
   uint16           configindex;
   /** link to SII config */     // 链接到SII配置
   uint16           SIIindex;
   /** 1 = 8 bytes per read, 0 = 4 bytes per read */   // 1=8字节/读，0=4字节/读
   uint8            eep_8byte;
   /** 0 = eeprom to master , 1 = eeprom to PDI */    // 0=eeprom到主站，1=eeprom到PDI
   uint8            eep_pdi;
   /** CoE details */         // CoE细节
   uint8            CoEdetails;
   /** FoE details */       // FoE细节
   uint8            FoEdetails;
   /** EoE details */       // EoE细节
   uint8            EoEdetails;
   /** SoE details */       // SoE细节
   uint8            SoEdetails;
   /** E-bus current */     // E-bus电流
   int16            Ebuscurrent;
   /** if >0 block use of LRW in processdata */   // 如果>0，则阻止在processdata中使用LRW
   uint8            blockLRW;
   /** group */             // 组
   uint8            group;
   /** first unused FMMU */   // 第一个未使用的FMMU
   uint8            FMMUunused;
   /** Boolean for tracking whether the slave is (not) responding, not used/set by the SOEM library */   // 布尔值，用于跟踪从站是否（未）响应，不由SOEM库使用/设置
   boolean          islost;
   /** registered configuration function PO->SO, (DEPRECATED)*/   // 注册的配置函数PO->SO 
   int              (*PO2SOconfig)(uint16 slave);
   /** registered configuration function PO->SO */                // 注册的配置函数PO->SO
   int              (*PO2SOconfigx)(ecx_contextt * context, uint16 slave);
   /** readable name */   // 可读名称
   char             name[EC_MAXNAME + 1];
} ec_slavet;

/** for list of ethercat slave groups */
typedef struct ec_group
{
   /** logical start address for this group */
   uint32           logstartaddr;
   /** output bytes, if Obits < 8 then Obytes = 0 */
   uint32           Obytes;
   /** output pointer in IOmap buffer */
   uint8            *outputs;
   /** input bytes, if Ibits < 8 then Ibytes = 0 */
   uint32           Ibytes;
   /** input pointer in IOmap buffer */
   uint8            *inputs;
   /** has DC capabillity */
   boolean          hasdc;
   /** next DC slave */
   uint16           DCnext;
   /** E-bus current */
   int16            Ebuscurrent;
   /** if >0 block use of LRW in processdata */
   uint8            blockLRW;
   /** IO segments used */
   uint16           nsegments;
   /** 1st input segment */
   uint16           Isegment;
   /** Offset in input segment */
   uint16           Ioffset;
   /** Expected workcounter outputs */
   uint16           outputsWKC;
   /** Expected workcounter inputs */
   uint16           inputsWKC;
   /** check slave states */
   boolean          docheckstate;
   /** IO segmentation list. Datagrams must not break SM in two. */
   uint32           IOsegment[EC_MAXIOSEGMENTS];
} ec_groupt;

/** SII FMMU structure */
typedef struct ec_eepromFMMU
{
   uint16  Startpos;
   uint8   nFMMU;
   uint8   FMMU0;
   uint8   FMMU1;
   uint8   FMMU2;
   uint8   FMMU3;
} ec_eepromFMMUt;

/** SII SM structure */
typedef struct ec_eepromSM
{
   uint16  Startpos;
   uint8   nSM;
   uint16  PhStart;
   uint16  Plength;
   uint8   Creg;
   uint8   Sreg;       /* don't care */
   uint8   Activate;
   uint8   PDIctrl;      /* don't care */
} ec_eepromSMt;

/** record to store rxPDO and txPDO table from eeprom */
typedef struct ec_eepromPDO
{
   uint16  Startpos;
   uint16  Length;
   uint16  nPDO;
   uint16  Index[EC_MAXEEPDO];
   uint16  SyncM[EC_MAXEEPDO];
   uint16  BitSize[EC_MAXEEPDO];
   uint16  SMbitsize[EC_MAXSM];
} ec_eepromPDOt;

/** mailbox buffer array */
typedef uint8 ec_mbxbuft[EC_MAXMBX + 1];

/** standard ethercat mailbox header */
PACKED_BEGIN
typedef struct PACKED ec_mbxheader
{
   uint16  length;
   uint16  address;
   uint8   priority;
   uint8   mbxtype;
} ec_mbxheadert;
PACKED_END

/** ALstatus and ALstatus code */
PACKED_BEGIN
typedef struct PACKED ec_alstatus
{
   uint16  alstatus;
   uint16  unused;
   uint16  alstatuscode;
} ec_alstatust;
PACKED_END

/** stack structure to store segmented LRD/LWR/LRW constructs */
typedef struct ec_idxstack
{
   uint8   pushed;
   uint8   pulled;
   uint8   idx[EC_MAXBUF];
   void    *data[EC_MAXBUF];
   uint16  length[EC_MAXBUF];
} ec_idxstackT;

/** ringbuf for error storage */
typedef struct ec_ering
{
   int16     head;
   int16     tail;
   ec_errort Error[EC_MAXELIST + 1];
} ec_eringt;

/** SyncManager Communication Type structure for CA */
PACKED_BEGIN
typedef struct PACKED ec_SMcommtype
{
   uint8   n;
   uint8   nu1;
   uint8   SMtype[EC_MAXSM];
} ec_SMcommtypet;
PACKED_END

/** SDO assign structure for CA */
PACKED_BEGIN
typedef struct PACKED ec_PDOassign
{
   uint8   n;
   uint8   nu1;
   uint16  index[256];
} ec_PDOassignt;
PACKED_END

/** SDO description structure for CA */
PACKED_BEGIN
typedef struct PACKED ec_PDOdesc
{
   uint8   n;
   uint8   nu1;
   uint32  PDO[256];
} ec_PDOdesct;
PACKED_END

/** Context structure , referenced by all ecx functions*/
struct ecx_context
{
   /** port reference, may include red_port */
   ecx_portt      *port;
   /** slavelist reference */
   ec_slavet      *slavelist;
   /** number of slaves found in configuration */
   int            *slavecount;
   /** maximum number of slaves allowed in slavelist */
   int            maxslave;
   /** grouplist reference */
   ec_groupt      *grouplist;
   /** maximum number of groups allowed in grouplist */
   int            maxgroup;
   /** internal, reference to eeprom cache buffer */
   uint8          *esibuf;
   /** internal, reference to eeprom cache map */
   uint32         *esimap;
   /** internal, current slave for eeprom cache */
   uint16         esislave;
   /** internal, reference to error list */
   ec_eringt      *elist;
   /** internal, reference to processdata stack buffer info */
   ec_idxstackT   *idxstack;
   /** reference to ecaterror state */
   boolean        *ecaterror;
   /** internal, position of DC datagram in process data packet */
   uint16         DCtO;
   /** internal, length of DC datagram */
   uint16         DCl;
   /** reference to last DC time from slaves */
   int64          *DCtime;
   /** internal, SM buffer */
   ec_SMcommtypet *SMcommtype;
   /** internal, PDO assign list */
   ec_PDOassignt  *PDOassign;
   /** internal, PDO description list */
   ec_PDOdesct    *PDOdesc;
   /** internal, SM list from eeprom */
   ec_eepromSMt   *eepSM;
   /** internal, FMMU list from eeprom */
   ec_eepromFMMUt *eepFMMU;
   /** registered FoE hook */
   int            (*FOEhook)(uint16 slave, int packetnumber, int datasize);
   /** registered EoE hook */
   int            (*EOEhook)(ecx_contextt * context, uint16 slave, void * eoembx);
   /** flag to control legacy automatic state change or manual state change */
   int            manualstatechange;
};

#ifdef EC_VER1
/** global struct to hold default master context */
extern ecx_contextt  ecx_context;
/** main slave data structure array */
extern ec_slavet   ec_slave[EC_MAXSLAVE];
/** number of slaves found by configuration function */
extern int         ec_slavecount;
/** slave group structure */
extern ec_groupt   ec_group[EC_MAXGROUP];
extern boolean     EcatError;
extern int64       ec_DCtime;

void ec_pusherror(const ec_errort *Ec);
boolean ec_poperror(ec_errort *Ec);
boolean ec_iserror(void);
void ec_packeterror(uint16 Slave, uint16 Index, uint8 SubIdx, uint16 ErrorCode);
int ec_init(const char * ifname);
int ec_init_redundant(const char *ifname, char *if2name);
void ec_close(void);
uint8 ec_siigetbyte(uint16 slave, uint16 address);
int16 ec_siifind(uint16 slave, uint16 cat);
void ec_siistring(char *str, uint16 slave, uint16 Sn);
uint16 ec_siiFMMU(uint16 slave, ec_eepromFMMUt* FMMU);
uint16 ec_siiSM(uint16 slave, ec_eepromSMt* SM);
uint16 ec_siiSMnext(uint16 slave, ec_eepromSMt* SM, uint16 n);
int ec_siiPDO(uint16 slave, ec_eepromPDOt* PDO, uint8 t);
int ec_readstate(void);
int ec_writestate(uint16 slave);
uint16 ec_statecheck(uint16 slave, uint16 reqstate, int timeout);
int ec_mbxempty(uint16 slave, int timeout);
int ec_mbxsend(uint16 slave,ec_mbxbuft *mbx, int timeout);
int ec_mbxreceive(uint16 slave, ec_mbxbuft *mbx, int timeout);
void ec_esidump(uint16 slave, uint8 *esibuf);
uint32 ec_readeeprom(uint16 slave, uint16 eeproma, int timeout);
int ec_writeeeprom(uint16 slave, uint16 eeproma, uint16 data, int timeout);
int ec_eeprom2master(uint16 slave);
int ec_eeprom2pdi(uint16 slave);
uint64 ec_readeepromAP(uint16 aiadr, uint16 eeproma, int timeout);
int ec_writeeepromAP(uint16 aiadr, uint16 eeproma, uint16 data, int timeout);
uint64 ec_readeepromFP(uint16 configadr, uint16 eeproma, int timeout);
int ec_writeeepromFP(uint16 configadr, uint16 eeproma, uint16 data, int timeout);
void ec_readeeprom1(uint16 slave, uint16 eeproma);
uint32 ec_readeeprom2(uint16 slave, int timeout);
int ec_send_processdata_group(uint8 group);
int ec_send_overlap_processdata_group(uint8 group);
int ec_receive_processdata_group(uint8 group, int timeout);
int ec_send_processdata(void);
int ec_send_overlap_processdata(void);
int ec_receive_processdata(int timeout);
#endif

ec_adaptert * ec_find_adapters(void);
void ec_free_adapters(ec_adaptert * adapter);
uint8 ec_nextmbxcnt(uint8 cnt);
void ec_clearmbx(ec_mbxbuft *Mbx);
void ecx_pusherror(ecx_contextt *context, const ec_errort *Ec);
boolean ecx_poperror(ecx_contextt *context, ec_errort *Ec);
boolean ecx_iserror(ecx_contextt *context);
void ecx_packeterror(ecx_contextt *context, uint16 Slave, uint16 Index, uint8 SubIdx, uint16 ErrorCode);
int ecx_init(ecx_contextt *context, const char * ifname);
int ecx_init_redundant(ecx_contextt *context, ecx_redportt *redport, const char *ifname, char *if2name);
void ecx_close(ecx_contextt *context);
uint8 ecx_siigetbyte(ecx_contextt *context, uint16 slave, uint16 address);
int16 ecx_siifind(ecx_contextt *context, uint16 slave, uint16 cat);
void ecx_siistring(ecx_contextt *context, char *str, uint16 slave, uint16 Sn);
uint16 ecx_siiFMMU(ecx_contextt *context, uint16 slave, ec_eepromFMMUt* FMMU);
uint16 ecx_siiSM(ecx_contextt *context, uint16 slave, ec_eepromSMt* SM);
uint16 ecx_siiSMnext(ecx_contextt *context, uint16 slave, ec_eepromSMt* SM, uint16 n);
int ecx_siiPDO(ecx_contextt *context, uint16 slave, ec_eepromPDOt* PDO, uint8 t);
int ecx_readstate(ecx_contextt *context);
int ecx_writestate(ecx_contextt *context, uint16 slave);
uint16 ecx_statecheck(ecx_contextt *context, uint16 slave, uint16 reqstate, int timeout);
int ecx_mbxempty(ecx_contextt *context, uint16 slave, int timeout);
int ecx_mbxsend(ecx_contextt *context, uint16 slave,ec_mbxbuft *mbx, int timeout);
int ecx_mbxreceive(ecx_contextt *context, uint16 slave, ec_mbxbuft *mbx, int timeout);
void ecx_esidump(ecx_contextt *context, uint16 slave, uint8 *esibuf);
uint32 ecx_readeeprom(ecx_contextt *context, uint16 slave, uint16 eeproma, int timeout);
int ecx_writeeeprom(ecx_contextt *context, uint16 slave, uint16 eeproma, uint16 data, int timeout);
int ecx_eeprom2master(ecx_contextt *context, uint16 slave);
int ecx_eeprom2pdi(ecx_contextt *context, uint16 slave);
uint64 ecx_readeepromAP(ecx_contextt *context, uint16 aiadr, uint16 eeproma, int timeout);
int ecx_writeeepromAP(ecx_contextt *context, uint16 aiadr, uint16 eeproma, uint16 data, int timeout);
uint64 ecx_readeepromFP(ecx_contextt *context, uint16 configadr, uint16 eeproma, int timeout);
int ecx_writeeepromFP(ecx_contextt *context, uint16 configadr, uint16 eeproma, uint16 data, int timeout);
void ecx_readeeprom1(ecx_contextt *context, uint16 slave, uint16 eeproma);
uint32 ecx_readeeprom2(ecx_contextt *context, uint16 slave, int timeout);
int ecx_send_overlap_processdata_group(ecx_contextt *context, uint8 group);
int ecx_receive_processdata_group(ecx_contextt *context, uint8 group, int timeout);
int ecx_send_processdata(ecx_contextt *context);
int ecx_send_overlap_processdata(ecx_contextt *context);
int ecx_receive_processdata(ecx_contextt *context, int timeout);
int ecx_send_processdata_group(ecx_contextt *context, uint8 group);

#ifdef __cplusplus
}
#endif

#endif
