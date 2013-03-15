/*
 * Type definitions
 *
 * Copyright (C) 2008, Udo Steinberg <udo@hypervisor.org>
 * Copyright (C) 2008-2010, Bernhard Kauer <bk@vmmon.org>
 * Economic rights: Technische Universitaet Dresden (Germany)
 *
 * This file is part of NUL.
 *
 * NUL is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * NUL is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public
 * License version 2 for more details.
 */



#pragma once

class Desc
{
protected:
  unsigned _value;
  Desc(unsigned v) : _value(v) {}
public:
  unsigned value() { return _value; }
};


enum {
  DESC_TYPE_MEM  = 1,
  DESC_TYPE_IO   = 2,
  DESC_TYPE_CAP  = 3,
  DESC_RIGHT_R   = 0x4,
  DESC_RIGHT_EC_RECALL = 0x4,
  DESC_RIGHT_PD  = 0x4,
  DESC_RIGHT_EC  = 0x8,
  DESC_RIGHT_SC  = 0x10,
  DESC_RIGHT_PT  = 0x20,
  DESC_RIGHT_SM  = 0x40,
  DESC_RIGHTS_ALL= 0x7c,
  DESC_MEM_ALL   = DESC_TYPE_MEM | DESC_RIGHTS_ALL,
  DESC_IO_ALL    = DESC_TYPE_IO  | DESC_RIGHTS_ALL,
  DESC_CAP_ALL   = DESC_TYPE_CAP | DESC_RIGHTS_ALL,
  MAP_HBIT       = 0x801,
  MAP_EPT        = 0x401,
  MAP_DPT        = 0x201,
  MAP_MAP        =     1,	///< Delegate typed item
};


/**
 * A capability range descriptor;
 */
class Crd : public Desc
{
public:
  unsigned order() { return ((_value >> 7) & 0x1f); }
  unsigned size()  { return 1 << (order() + 12); }
  unsigned base()  { return _value & ~0xfff; }
  unsigned attr()  { return _value & 0x1f; }
  unsigned cap()   { return _value >> 12; }
  explicit Crd(unsigned offset, unsigned order, unsigned attr) : Desc((offset << 12) | (order << 7) | attr) { }
  explicit Crd(unsigned v) : Desc(v) {}
};

/**
 * A quantum+period descriptor.
 */
class Qpd : public Desc
{
public:
  Qpd(unsigned prio, unsigned quantum) : Desc((quantum << 12) | prio) { }
};

