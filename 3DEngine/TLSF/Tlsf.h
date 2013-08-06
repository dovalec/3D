/*
 * Two Levels Segregate Fit memory allocator (TLSF)
 * Version 2.4.6
 *
 * Written by Miguel Masmano Tello <mimastel@doctor.upv.es>
 *
 * Thanks to Ismael Ripoll for his suggestions and reviews
 *
 * Copyright (C) 2008, 2007, 2006, 2005, 2004
 *
 * This code is released using a dual license strategy: GPL/LGPL
 * You can choose the licence that better fits your requirements.
 *
 * Released under the terms of the GNU General Public License Version 2.0
 * Released under the terms of the GNU Lesser General Public License Version 2.1
 *
 */

#ifndef _TLSF_H_
#define _TLSF_H_

#include "../Config.h"

//NAMESPACE_BEGIN_LUCID

unsigned int init_memory_pool(unsigned int, void *);
unsigned int get_used_size(void *);
unsigned int get_max_size(void *);
void destroy_memory_pool(void *);
unsigned int add_new_area(void *, unsigned int, void *);
void *malloc_ex(unsigned int, void *);
void free_ex(void *, void *);
void *realloc_ex(void *, unsigned int, void *);
void *calloc_ex(unsigned int, unsigned int, void *);

void *tlsf_malloc(unsigned int size);
void tlsf_free(void *ptr);
void *tlsf_realloc(void *ptr, unsigned int size);
void *tlsf_calloc(unsigned int nelem, unsigned int elem_size);

//NAMESPACE_END_LUCID

#endif
