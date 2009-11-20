/**
 * Message Type defintions.
 *
 * Copyright (C) 2009, Bernhard Kauer <bk@vmmon.org>
 *
 * This file is part of Vancouver.
 *
 * Vancouver is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Vancouver is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License version 2 for more details.
 */

#pragma once
#include <cassert>

/****************************************************/
/* IOIO messages                                    */
/****************************************************/
/**
 * An in() from an ioport.
 */
struct MessageIOIn
{
  enum Type {
    TYPE_INB = 0,
    TYPE_INW = 1,
    TYPE_INL = 2,
  } type;
  unsigned short port;
  unsigned count;
  union {
    unsigned value;
    void *ptr;
  };
  MessageIOIn(Type _type, unsigned short _port) : type(_type), port(_port), count(0), value(~0u) {}
  MessageIOIn(Type _type, unsigned short _port, unsigned _count, void *_ptr) : type(_type), port(_port), count(_count), ptr(_ptr) {} 
};


/**
 * An out() to an ioport.
 */
struct MessageIOOut {
  enum Type {
    TYPE_OUTB = 0,
    TYPE_OUTW = 1,
    TYPE_OUTL = 2,
  } type;
  unsigned short port;
  unsigned count;
  union {
    unsigned value;
    void *ptr;
  };
  MessageIOOut(Type _type, unsigned short _port, unsigned _value) : type(_type), port(_port), count(0), value(_value) {}
  MessageIOOut(Type _type, unsigned short _port, unsigned _count, void *_ptr) : type(_type), port(_port), count(_count), ptr(_ptr) {} 
};


/****************************************************/
/* Memory messages                                  */
/****************************************************/

/**
 * Generic memory message.
 */
struct MessageMem
{
  MessageMem(unsigned long _phys, void *_ptr, unsigned _count) : phys(_phys), ptr(_ptr), count(_count) {}
  unsigned long phys;
  void *ptr;
  unsigned count;
};


/**
 * A memory write operation.
 */
struct MessageMemWrite    : public MessageMem 
{
  MessageMemWrite(unsigned long _phys, void *_ptr, unsigned _count) : MessageMem(_phys, _ptr, _count) {}
};


/**
 * A memory read operation.
 */
struct MessageMemRead     : public MessageMem
{
  MessageMemRead(unsigned long _phys, void *_ptr, unsigned _count) : MessageMem(_phys, _ptr, _count) {}
};


/**
 * A mapping directly to the user.
 */
struct MessageMemMap     : public MessageMem 
{
 MessageMemMap(unsigned long _phys, void *_ptr, unsigned _count) : MessageMem(_phys, _ptr, _count) {}
};


/**
 * Get a pointer for up to 2 pages of memory for direct read-write access.
 *
 * Phys2 == ~0ul means only a single page.
 */
struct MessageMemAlloc
{
  void **ptr;
  unsigned long phys1;
  unsigned long phys2;
  MessageMemAlloc(void **_ptr, unsigned long _phys1, unsigned long _phys2=~0ul) : ptr(_ptr), phys1(_phys1), phys2(_phys2) {}
};



/****************************************************/
/* PCI messages                                     */
/****************************************************/

/**
 * A PCI config space transaction.
 */
struct MessagePciCfg
{
  enum Type {
    TYPE_READ,
    TYPE_WRITE
  } type;
  unsigned address;
  unsigned value;
  MessagePciCfg(unsigned _address) : type(TYPE_READ), address(_address), value(0xffffffff) {}
  MessagePciCfg(unsigned _address, unsigned _value) : type(TYPE_WRITE), address(_address), value(_value) {}
};


class Device;
/**
 * Add a device to a  PCI bridge.
 */
typedef bool (*PciCfgFunction) (Device *, MessagePciCfg &);
struct MessagePciBridgeAdd
{
  unsigned devfunc;
  Device *dev;
  PciCfgFunction func;
  MessagePciBridgeAdd(unsigned _devfunc, Device *_dev, PciCfgFunction _func) : devfunc(_devfunc), dev(_dev), func(_func) {}
};


/****************************************************/
/* SATA messages                                    */
/****************************************************/

class FisReceiver;

// XXX Use SATA bus to comunicated FISes
/**
 * Set a drive on a port of an AHCI controller.
 */
struct MessageAhciSetDrive
{
  FisReceiver *drive;
  unsigned port;
  MessageAhciSetDrive(FisReceiver *_drive, unsigned _port) : drive(_drive), port(_port) {}
};


/****************************************************/
/* IRQ messages                                     */
/****************************************************/

/**
 * Raise an IRQ.
 */
struct MessageIrq
{
  enum Vector
    {
      LINT0 = 255,
    };
  enum MessageType
    {
      ASSERT_IRQ,
      ASSERT_NOTIFY,
      DEASSERT_IRQ,
    } type;
  unsigned char line;

  MessageIrq(MessageType _type, unsigned char _line) :  type(_type), line(_line) {}
};


/**
 * Notify that a level-triggered IRQ can be reraised.
 */
struct MessageIrqNotify
{
  unsigned char baseirq;
  unsigned char mask;
  MessageIrqNotify(unsigned char _baseirq, unsigned char _mask) : baseirq(_baseirq), mask(_mask)  {}
};


/**
 * Message on the PIC bus.
 */
struct MessagePic
{
  unsigned char slave;
  unsigned char vector;
  MessagePic(unsigned char _slave) :  slave(_slave) { assert(slave < 8); }
};


/**
 * Message on the APIC bus.
 */
struct MessageApic
{
  unsigned char line;
  unsigned char vector;
  MessageApic(unsigned char _line) :  line(_line) { }
};


/****************************************************/
/* Legacy messages                                  */
/****************************************************/

/**
 * Various messages of the legacy chips such as PIT, PPI...
 */
struct MessageLegacy
{
  enum Type
    {
      GATE_A20,
      RESET,
      FAST_A20,
      INIT,
    } type;
  unsigned value;
  MessageLegacy(Type _type, unsigned _value) : type(_type), value(_value) {}
};

/**
 * Pit messages.
 */
struct MessagePit
{
  enum Type
    {
      GET_OUT,
      SET_GATE,
    } type;
  unsigned pit;
  bool value;
  MessagePit(Type _type, unsigned _pit, bool _value=false) : type(_type), pit(_pit), value(_value) {}
};
  

/****************************************************/
/* Keyboard and Serial messages                     */
/****************************************************/
/**
 * Message on the PS2 bus between a KeyboardController and a connected Keyboard/Mouse
 */
struct MessagePS2
{
  unsigned port;
  enum Type
    {
      NOTIFY,
      READ_KEY,
      SEND_COMMAND,
    }  type;
  unsigned char value;
  MessagePS2(unsigned char _port, Type _type, unsigned char _value) : port(_port), type(_type), value(_value) {}
};


/**
 * A keycode. See keyboard.h for the format.
 */
struct MessageKeycode
{
  unsigned keyboard;
  unsigned keycode;
  MessageKeycode(unsigned char _keyboard=0, unsigned _keycode=0) : keyboard(_keyboard), keycode(_keycode) {}
};


/**
 * A PS2 mouse packet.
 */
struct MessageMouse
{
  unsigned mouse;
  unsigned packet;  
  MessageMouse(unsigned char _mouse, unsigned _packet) : mouse(_mouse), packet(_packet) {}
};


/**
 * An ascii character from the serial port.
 */
struct MessageSerial
{
  unsigned serial;
  unsigned char ch;  
  MessageSerial(unsigned _serial, unsigned char _ch) : serial(_serial), ch(_ch) {}
};


/****************************************************/
/* Console messages                                 */
/****************************************************/

struct VgaRegs
{
  unsigned short offset;
  unsigned short cursor_style;
  unsigned cursor_pos;
  unsigned short mode;
};

struct ConsoleModeInfo
{
  bool           textmode;
  unsigned short vesamode;
  unsigned short resolution[2];
  unsigned short bytes_per_scanline;
  unsigned char  bpp;
  ConsoleModeInfo() : textmode(true), resolution(), bpp(0) {}
};


/**
 * VGA Console.
 */
struct MessageConsole
{
  enum Type
    {
      // allocate a new client
      TYPE_ALLOC_CLIENT,
      // allocate a new view for a client
      TYPE_ALLOC_VIEW,
      // get information about a mode
      TYPE_GET_MODEINFO,
      // switch to another view
      TYPE_SWITCH_VIEW,
      // the user pressed a key
      TYPE_KEY,
      // the user requests a reset
      TYPE_RESET,
      // the user requests to start a new domain
      TYPE_START,
      // the user requests a debug feature
      TYPE_DEBUG,
    } type;
  unsigned short id;
  unsigned short view;
  union
  {
    struct {
      const char *clientname;
    };
    struct {
      const char *name;
      char    * ptr;
      unsigned size;
      VgaRegs *regs;
    };
    struct {
      unsigned index;
      ConsoleModeInfo *info;
    };
    struct {
      unsigned keycode;
    };
  };
  MessageConsole(Type _type = TYPE_ALLOC_CLIENT, unsigned short _id=0) : type(_type), id(_id), ptr(0) {}
  MessageConsole(unsigned _index, ConsoleModeInfo *_info) : type(TYPE_GET_MODEINFO), index(_index), info(_info) {}
  MessageConsole(const char *_name, char * _ptr, unsigned _size, VgaRegs *_regs) 
    : type(TYPE_ALLOC_VIEW), id(~0), name(_name), ptr(_ptr), size(_size), regs(_regs) {}
  MessageConsole(unsigned short _id, unsigned short _view, unsigned _keycode) : type(TYPE_KEY), id(_id), view(_view), keycode(_keycode) {}
};


/**
 * VESA support.
 */

struct VesaModeInfo
{
  bool           textmode;
  unsigned short mode;
  unsigned char  bpp;
  unsigned short resolution[2];
  unsigned short bytes_per_scanline;
  unsigned long  physbase;
  VesaModeInfo() : textmode(false), mode(0xffff), bpp(0), resolution(), physbase(0) {}
};


struct MessageVesa
{
  enum Type
    {
      // return available modes
      TYPE_GET_MODEINFO,
      // switch mode
      TYPE_SWITCH_MODE,
    } type;
  unsigned index;
  union
  {
    VesaModeInfo *info;
  };
  MessageVesa(unsigned _index, VesaModeInfo *_info) : type(TYPE_GET_MODEINFO), index(_index), info(_info) {}
  MessageVesa(unsigned _index) : type(TYPE_SWITCH_MODE), index(_index) {}
};

/****************************************************/
/* HOST messages                                    */
/****************************************************/

/**
 * Request to the host, such as unmask irq or request IO region.
 */
struct MessageHostOp
{
  enum Type
    {
      OP_UNMASK_IRQ,
      OP_ALLOC_IOIO_REGION,
      OP_ALLOC_IOMEM_REGION,
      OP_ATTACH_HOSTIRQ,
      OP_VIRT_TO_PHYS,
      OP_GET_MODULE,
      OP_GET_UID,
      OP_GUEST_MEM,
    } type;  
  unsigned long value;
  union {
    struct {
      unsigned long phys;
      unsigned long phys_len;
    };
    struct {
      char *ptr;
      unsigned len;
    };
    struct 
    {
      unsigned module;
      char * start;
      unsigned long size;
      char * cmdline;
      unsigned long cmdlen;
    };
  };
  MessageHostOp(unsigned _module, char * _start) : type(OP_GET_MODULE), module(_module), start(_start), size(0), cmdlen(0)  {}
  MessageHostOp(Type _type, unsigned long _value) : type(_type), value(_value), ptr(0) {}
};


/****************************************************/
/* DISK messages                                    */
/****************************************************/

class DmaDescriptor;
class DiskParameter;

/**
 * Request/read from the disk.
 */
struct MessageDisk
{
  enum Type
    {
      DISK_GET_PARAMS,
      DISK_READ,
      DISK_WRITE,
      DISK_FLUSH_CACHE,
    } type;
  unsigned disknr;
  union
  {
    DiskParameter *params;
    struct {
      unsigned long long sector;
      unsigned long usertag;
      unsigned dmacount;
      DmaDescriptor *dma;
      unsigned long physoffset;
      unsigned long physsize;
    };
  };
  enum Status {
    DISK_OK = 0,
    DISK_STATUS_BUSY,
    DISK_STATUS_DEVICE,
    DISK_STATUS_DMA,
    DISK_STATUS_USERTAG,
    DISK_STATUS_SHIFT = 4,
    DISK_STATUS_MASK = (1 << DISK_STATUS_SHIFT) -1,
  } error;
  MessageDisk(unsigned _disknr, DiskParameter *_params) : type(DISK_GET_PARAMS), disknr(_disknr), params(_params) {}
  MessageDisk(Type _type, unsigned _disknr, unsigned long _usertag, unsigned long long _sector, 
	      unsigned _dmacount, DmaDescriptor *_dma, unsigned long _physoffset, unsigned long _physsize) 
    : type(_type), disknr(_disknr), sector(_sector), usertag(_usertag), dmacount(_dmacount), dma(_dma), physoffset(_physoffset), physsize(_physsize) {}
};


/**
 * A disk.request is completed.
 */
struct MessageDiskCommit
{
  unsigned disknr;
  unsigned long usertag;
  MessageDisk::Status status;
  MessageDiskCommit(unsigned _disknr=0, unsigned long _usertag=0, MessageDisk::Status _status=MessageDisk::DISK_OK) : disknr(_disknr), usertag(_usertag), status(_status) {}
};


/****************************************************/
/* Executor messages                                */
/****************************************************/

class CpuState;
class VirtualCpuState;

/**
 * All state needed for instruction emulation.
 */
struct MessageExecutor
{
  CpuState *cpu;
  VirtualCpuState *vcpu;
  MessageExecutor(CpuState *_cpu, VirtualCpuState *_vcpu) : cpu(_cpu), vcpu(_vcpu) {}
};

struct MessageBios : public MessageExecutor
{
  unsigned irq;
  MessageBios(MessageExecutor &msg, unsigned _irq) : MessageExecutor(msg), irq(_irq) {}
};



/****************************************************/
/* Timer messages                                   */
/****************************************************/

typedef unsigned long long timevalue;


/**
 * Timer infrastructure.
 *
 * There is no frequency and clock here, as all is based on the same
 * clocksource.
 */
struct MessageTimer
{
  enum Type
    {
      TIMER_NEW,
      TIMER_CANCEL_TIMEOUT,
      TIMER_REQUEST_TIMEOUT,
    } type;
  unsigned  nr;
  timevalue abstime;
  MessageTimer()              : type(TIMER_NEW) {}
  MessageTimer(unsigned  _nr) : type(TIMER_CANCEL_TIMEOUT), nr(_nr) {}
  MessageTimer(unsigned  _nr, timevalue _abstime) : type(TIMER_REQUEST_TIMEOUT), nr(_nr), abstime(_abstime) {}
};


/**
 * A timeout triggered.
 */
struct MessageTimeout
{
  unsigned  nr;
  MessageTimeout(unsigned  _nr) : nr(_nr) {}
};


/**
 * Returns the wall clock time in microseconds.
 *
 * It also contains a timestamp of the Motherboard clock in
 * microseconds, to be able to adjust to the time allready passed and
 * to detect out-of-date values.
 */
struct MessageTime
{
  enum {
    FREQUENCY = 1000000,
  };
  timevalue wallclocktime;
  timevalue timestamp;
  MessageTime() :  wallclocktime(0), timestamp(0) {}
};


/****************************************************/
/* Network messages                                 */
/****************************************************/

struct MessageNetwork
{
  const unsigned char *buffer;
  unsigned len;
  unsigned client;
  MessageNetwork(const unsigned char *_buffer, unsigned _len, unsigned _client) : buffer(_buffer), len(_len), client(_client) {}
};