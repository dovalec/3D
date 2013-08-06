/*
 **************************************************************************
 *                                                                        *
 *          General Purpose Hash Function Algorithms Library              *
 *                                                                        *
 * Author: Arash Partow - 2002                                            *
 * URL: http://www.partow.net                                             *
 * URL: http://www.partow.net/programming/hashfunctions/index.html        *
 *                                                                        *
 * Copyright notice:                                                      *
 * Free use of the General Purpose Hash Function Algorithms Library is    *
 * permitted under the guidelines and in accordance with the most current *
 * version of the Common Public License.                                  *
 * http://www.opensource.org/licenses/cpl1.0.php                          *
 *                                                                        *
 **************************************************************************
*/



#ifndef INCLUDE_GENERALHASHFUNCTION_C_H
#define INCLUDE_GENERALHASHFUNCTION_C_H


#include <stdio.h>
#include <string.h>


unsigned int RSHash  (char* str);
unsigned int JSHash  (char* str);
unsigned int PJWHash (char* str);
unsigned int ELFHash (const char* str);
unsigned int BKDRHash(char* str);
unsigned int SDBMHash(char* str);
unsigned int DJBHash (char* str);
unsigned int DEKHash (char* str);
unsigned int BPHash  (char* str);
unsigned int FNVHash (char* str);
unsigned int APHash  (char* str);


#endif
