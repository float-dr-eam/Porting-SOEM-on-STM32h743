# Porting SOEM on STM32H743

本仓库用于在 **STM32H743** 平台上移植 **SOEM (Simple Open EtherCAT Master)**，并提供与以太网底层驱动、OS 抽象层相关的适配代码。

## 仓库定位

- 目标：让 SOEM 能在 STM32H743 + HAL Ethernet 环境下运行。
- 主要内容：
  - SOEM 协议栈源码（`soem/`）
  - OS 抽象层（`osal/`）
  - 硬件抽象与网卡驱动层（`oshw/`）
  - 与 STM32H7 HAL 以太网相关的适配与参考实现（根目录若干 `eth*` 文件）

---

## 项目结构（面向新用户）

```text
.
├── .gitignore
├── LICENSE
├── README.md
├── eth.c
├── eth.h
├── stm32h7xx_hal_eth.c
├── stm32h7xx_hal_eth.h
├── osal/
│   ├── osal.c
│   ├── osal.h
│   └── osal_defs.h
├── oshw/
│   ├── nicdrv.c
│   ├── nicdrv.h
│   ├── oshw.c
│   └── oshw.h
└── soem/
    ├── ethercat.h
    ├── ethercatbase.c
    ├── ethercatbase.h
    ├── ethercatcoe.c
    ├── ethercatcoe.h
    ├── ethercatconfig.c
    ├── ethercatconfig.h
    ├── ethercatconfiglist.h
    ├── ethercatdc.c
    ├── ethercatdc.h
    ├── ethercateoe.c
    ├── ethercateoe.h
    ├── ethercatfoe.c
    ├── ethercatfoe.h
    ├── ethercatmain.c
    ├── ethercatmain.h
    ├── ethercatprint.c
    ├── ethercatprint.h
    ├── ethercatsoe.c
    ├── ethercatsoe.h
    └── ethercattype.h
```

---

## 各目录与文件作用说明

### 1) 根目录文件

- **`.gitignore`**  
  Git 忽略规则，避免编译中间文件、IDE 临时文件等进入版本库。

- **`LICENSE`**  
  开源许可证（Apache-2.0）。

- **`README.md`**  
  项目说明文档（即当前文件）。

- **`eth.c` / `eth.h`**  
  与以太网发送/接收、缓冲区管理、与 SOEM/驱动对接相关的适配实现（项目移植重点之一）。

- **`stm32h7xx_hal_eth.c` / `stm32h7xx_hal_eth.h`**  
  STM32H7 HAL 以太网驱动代码（通常来自 STM32 HAL，可能包含本项目所需改动或配套版本）。

---

### 2) `osal/`（Operating System Abstraction Layer）

用于把 SOEM 对“操作系统能力”的依赖统一抽象，便于在不同平台替换。

- **`osal/osal.c`**  
  OS 抽象层实现（如时间、线程/同步相关接口的具体实现）。

- **`osal/osal.h`**  
  OS 抽象层对外接口声明。

- **`osal/osal_defs.h`**  
  OSAL 使用的公共类型、宏、平台相关定义。

---

### 3) `oshw/`（Operating System Hardware / NIC Driver Glue）

这一层连接 **SOEM 上层协议处理** 与 **底层网卡/以太网驱动**，是移植到新 MCU/网卡时最关键的部分之一。

- **`oshw/nicdrv.c`**  
  网卡驱动适配核心实现（报文收发队列、帧处理、缓冲管理等）。

- **`oshw/nicdrv.h`**  
  `nicdrv.c` 的接口声明与相关结构体定义。

- **`oshw/oshw.c`**  
  硬件层初始化/底层辅助功能实现。

- **`oshw/oshw.h`**  
  硬件层接口声明。

---

### 4) `soem/`（SOEM 核心协议栈）

该目录是 SOEM 主体源码，建议尽量保持上游兼容，仅在必要时做平台相关补丁。

- **`soem/ethercat.h`**  
  EtherCAT 相关总入口头文件（聚合常用定义/接口）。

- **`soem/ethercatmain.c` / `soem/ethercatmain.h`**  
  主站核心逻辑：主站初始化、从站扫描、PDO/SDO 基础流程等关键能力。

- **`soem/ethercatbase.c` / `soem/ethercatbase.h`**  
  EtherCAT 基础报���与底层命令封装。

- **`soem/ethercatconfig.c` / `soem/ethercatconfig.h` / `soem/ethercatconfiglist.h`**  
  从站配置、拓扑与配置表相关逻辑。

- **`soem/ethercatcoe.c` / `soem/ethercatcoe.h`**  
  CoE（CANopen over EtherCAT）协议支持。

- **`soem/ethercatsoe.c` / `soem/ethercatsoe.h`**  
  SoE（Servo over EtherCAT）协议支持。

- **`soem/ethercatfoe.c` / `soem/ethercatfoe.h`**  
  FoE（File over EtherCAT）协议支持。

- **`soem/ethercateoe.c` / `soem/ethercateoe.h`**  
  EoE（Ethernet over EtherCAT）协议支持。

- **`soem/ethercatdc.c` / `soem/ethercatdc.h`**  
  分布式时钟（Distributed Clocks）相关实现。

- **`soem/ethercatprint.c` / `soem/ethercatprint.h`**  
  错误码、状态等信息的打印/调试辅助。

- **`soem/ethercattype.h`**  
  SOEM 公共类型定义、基础宏、打包结构等。

---

## 新用户建议阅读顺序

如果你是第一次接触本仓库，建议按以下顺序阅读：

1. `README.md`（整体概念）
2. `oshw/nicdrv.h` → `oshw/nicdrv.c`（理解收发链路）
3. `eth.h` → `eth.c`（看 STM32H743 侧适配细节）
4. `osal/osal.h` → `osal/osal.c`（看 OS 相关接口映射）
5. `soem/ethercatmain.h` → `soem/ethercatmain.c`（回到 SOEM 主流程）

---

## 移植时重点关注

- **网卡 DMA/缓存一致性**（H7 上常见问题）
- **中断优先级与实时性**（影响帧收发稳定性）
- **时钟精度与 DC 同步**（分布式时钟场景）
- **SOEM 上游版本差异**（建议记录本仓库基线版本）

---

## 后续可改进方向

- 增加 `example/` 示例工程或最小主站示例
- 增加“从上电到跑通第一个从站”的步骤文档
- 增加 FAQ（常见错误：链路不上、从站扫描失败、WKC 异常等）

---
