/***************************************************************************
RespirAVO/UnisciMisure
Copyright 2022 3C A. Avogadro & RespirAVO Contributors

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
***************************************************************************/


#include <siglib.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>

#define SEGFAULT_ECODE -3


static void __SegFaultHandler__(int __signum)
{
    printf("ERRORE: Segmentation Fault - Errore di memoria - SIGNUM %d\n", __signum);
    exit(SEGFAULT_ECODE);
}

void sigSetup()
{
    signal(SIGSEGV, __SegFaultHandler__);
}
