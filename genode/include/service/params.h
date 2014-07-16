/** @file
 * Parameter handling.
 *
 * Copyright (C) 2009, Bernhard Kauer <bk@vmmon.org>
 * Economic rights: Technische Universitaet Dresden (Germany)
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

#include <nul/compiler.h>
#include <service/logging.h>

#include <util/fifo.h>

class Motherboard;
typedef void (*ParameterFn)(Motherboard &, unsigned long *, const char *, unsigned);

class Parameter : public Genode::Fifo<Parameter>::Element {

 public:

  const char *name;
  ParameterFn func;

  static Genode::Fifo<Parameter> &all_parameters();

  Parameter(const char *name, ParameterFn func) : name(name), func(func)
  {
    all_parameters().enqueue(this);
  }
};

/**
 * Defines strings and functions for a parameter with the given
 * name. The variadic part is used to store a help text.
 *
 * PARAM_HANDLER(example, "example - this is just an example for parameter Passing",
 *                        "Another help line...")
 * { Logging::printf("example parameter function called!\n"); }
 */
#define PARAM_HANDLER(NAME, ...)                                        \
  extern "C" void      __parameter_##NAME##_fn(Motherboard &mb, unsigned long *, const char *, unsigned); \
  static Parameter __parameter_##NAME (#NAME, __parameter_##NAME##_fn);  \
  extern "C" void      __parameter_##NAME##_fn(Motherboard &mb, unsigned long *argv, const char *args, unsigned args_len)


#define PARAM_ITER(p)                                               \
  Parameter ** p; Parameter * pp;                                    \
  for (pp = Parameter::all_parameters().dequeue(), p = &pp; !Parameter::all_parameters().empty(); pp = Parameter::all_parameters().dequeue(), p = &pp)

#define PARAM_DEREF(p) (*(*(p)))
