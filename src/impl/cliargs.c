/***************************************************************************
RespirAVO/unicsv
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


#include <string.h>
#include <assertlib.h>
#include "cliargs.h"


// Argomenti
#define ARG_IGNORA_FINO        "-if"
#define ARG_IGNORA_FINO_INTERO "--ignora-fino"
#define ARG_POSFILE            "-fp"
#define ARG_POSFILE_INTERO     "--file-posizioni"
#define ARG_MISFILE            "-fm"
#define ARG_MISFILE_INTERO     "--file-misure"
#define ARG_OUTFILE            "-fo"
#define ARG_OUTFILE_INTERO     "--file-output"

#define TOO_FEW_ARGS -5
#define INVALID_OPT  -6


static bool __MatchOption__(const char *__long, const char *__short, const char *__arg)
{
    return strcmp(__arg, __long) == 0 || strcmp(__arg, __short) == 0;
}

arginfo_t parse_args(int argc, char **argv)
{
    arginfo_t _info = { 1, NULL, NULL, NULL };

    for (int i = 1; i < argc; i++)
    {
        if (__MatchOption__(ARG_IGNORA_FINO_INTERO, ARG_IGNORA_FINO, argv[i]))
        {
            massert(++i < argc, TOO_FEW_ARGS, "Opzione %s richiede un argomento aggiuntivo. 0 forniti.", argv[i]);
            _info.inogra_fino = atoi(argv[i]);
        }
        else if (__MatchOption__(ARG_POSFILE_INTERO, ARG_POSFILE, argv[i]))
        {
            massert(++i < argc, TOO_FEW_ARGS, "Opzione %s richiede un argomento aggiuntivo. 0 forniti.", argv[i]);
            _info.fp = argv[i];
        }
        else if (__MatchOption__(ARG_MISFILE_INTERO, ARG_MISFILE, argv[i]))
        {
            massert(++i < argc, TOO_FEW_ARGS, "Opzione %s richiede un argomento aggiuntivo. 0 forniti.", argv[i]);
            _info.fm = argv[i];
        }
        else if (__MatchOption__(ARG_OUTFILE_INTERO, ARG_OUTFILE, argv[i]))
        {
            massert(++i < argc, TOO_FEW_ARGS, "Opzione %s richiede un argomento aggiuntivo. 0 forniti.", argv[i]);
            _info.fo = argv[i];
        }
        else mexit(-5, "Opzione %s non riconosciuta.", argv[i]);
    }

    return _info;
}
