/*
 * sbios.c - SBIOS entrypoint and dispatch table.
 *
 * Copyright (c) 2003 Marcus R. Brown <mrbrown@0xd6.org>
 *
 * See the file LICENSE, located within this directory, for licensing terms.
 */

#include "sbcalls.h"
#include "iopmem.h"
#include "stdio.h"

#define SBCALL_MAX	256

static const char *sbiosDescription[] = {
	"SB_GETVER",
	"SB_HALT",
	"SB_SETDVE",
	"SB_PUTCHAR",
	"SB_GETCHAR",
	"SB_SETGSCRT",
	"SB_SETRGBYC",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"SB_SIFINIT",
	"SB_SIFEXIT",
	"SB_SIFSETDMA",
	"SB_SIFDMASTAT",
	"SB_SIFSETDCHAIN",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"SB_SIFINITCMD",
	"SB_SIFEXITCMD",
	"SB_SIFSENDCMD",
	"SB_SIFCMDINTRHDLR",
	"SB_SIFADDCMDHANDLER",
	"SB_SIFREMOVECMDHANDLER",
	"SB_SIFSETCMDBUFFER",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"SB_SIFINITRPC",
	"SB_SIFEXITRPC",
	"SB_SIFGETOTHERDATA",
	"SB_SIFBINDRPC",
	"SB_SIFCALLRPC",
	"SB_SIFCHECKSTATRPC",
	"SB_SIFSETRPCQUEUE",
	"SB_SIFREGISTERRPC",
	"SB_SIFREMOVERPC",
	"SB_SIFREMOVERPCQUEUE",
	"SB_SIFGETNEXTREQUEST",
	"SB_SIFEXECREQUEST",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"SBR_IOPH_INIT",
	"SBR_IOPH_ALLOC",
	"SBR_IOPH_FREE",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"SBR_PAD_INIT",
	"SBR_PAD_END",
	"SBR_PAD_PORTOPEN",
	"SBR_PAD_PORTCLOSE",
	"SBR_PAD_SETMAINMODE",
	"SBR_PAD_SETACTDIRECT",
	"SBR_PAD_SETACTALIGN",
	"SBR_PAD_INFOPRESSMODE",
	"SBR_PAD_ENTERPRESSMODE",
	"SBR_PAD_EXITPRESSMODE",
	"SB_PAD_READ",
	"SB_PAD_GETSTATE",
	"SB_PAD_GETREQSTATE",
	"SB_PAD_INFOACT",
	"SB_PAD_INFOCOMB",
	"SB_PAD_INFOMODE",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"SBR_SOUND_INIT",
	"SBR_SOUND_END",
	"SB_SOUND_GREG",
	"SB_SOUND_SREG",
	"SBR_SOUND_GCOREATTR",
	"SBR_SOUND_SCOREATTR",
	"SBR_SOUND_TRANS",
	"SBR_SOUND_TRANSSTAT",
	"SBR_SOUND_TRANSCALLBACK",
	"unknown",
	"unknown",
	"SBR_SOUND_REMOTE",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"SBR_MC_INIT",
	"SBR_MC_OPEN",
	"SBR_MC_MKDIR",
	"SBR_MC_CLOSE",
	"SBR_MC_SEEK",
	"SBR_MC_READ",
	"SBR_MC_WRITE",
	"SBR_MC_GETINFO",
	"SBR_MC_GETDIR",
	"SBR_MC_FORMAT",
	"SBR_MC_DELETE",
	"SBR_MC_FLUSH",
	"SBR_MC_SETFILEINFO",
	"SBR_MC_RENAME",
	"SBR_MC_UNFORMAT",
	"SBR_MC_GETENTSPACE",
	"SBR_MC_CALL",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"SBR_CDVD_INIT",
	"SBR_CDVD_RESET",
	"SBR_CDVD_READY",
	"SBR_CDVD_READ",
	"SBR_CDVD_STOP",
	"SBR_CDVD_GETTOC",
	"SBR_CDVD_READRTC",
	"SBR_CDVD_WRITERTC",
	"SBR_CDVD_MMODE",
	"SBR_CDVD_GETERROR",
	"SBR_CDVD_GETTYPE",
	"SBR_CDVD_TRAYREQ",
	"SB_CDVD_POWERHOOK",
	"SBR_CDVD_DASTREAM",
	"SBR_CDVD_READSUBQ",
	"SBR_CDVD_OPENCONFIG",
	"SBR_CDVD_CLOSECONFIG",
	"SBR_CDVD_READCONFIG",
	"SBR_CDVD_WRITECONFIG",
	"SBR_CDVD_RCBYCTL",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"SBR_REMOCON_INIT",
	"SBR_REMOCON_END",
	"SBR_REMOCON_PORTOPEN",
	"SBR_REMOCON_PORTCLOSE",
	"SB_REMOCON_READ",
	"SBR_REMOCON2_INIT",
	"SBR_REMOCON2_END",
	"SBR_REMOCON2_PORTOPEN",
	"SBR_REMOCON2_PORTCLOSE",
	"SB_REMOCON2_READ",
	"SBR_REMOCON2_IRFEATURE"
};

static void *dispatch[SBCALL_MAX] __attribute__((section(".text"))) = {
	/* 0 */
	sbcall_getver,
	/* 1 */
	sbcall_halt,
	/* 2 */
	sbcall_setdve,
	/* 3 */
	sbcall_putc,
	/* 4 */
	sbcall_getc,
	/* 5 */
	sbcall_setgscrt,
	/* 6 */
	sbcall_setrgbyc,
	0, 0, 0, 0, 0, 0, 0, 0, 0,
	/* 16 */
	sbcall_sifinit,
	/* 17 */
	sbcall_sifexit,
	/* 18 */
	sbcall_sifsetdma,
	/* 19 */
	sbcall_sifdmastat,
	/* 20 */
	sbcall_sifsetdchain,
	/* 21 */
	sbcall_sifsetreg,
	/* 22 */
	sbcall_sifgetreg,
	/* 23 */
	sbcall_sifstopdma,
	0, 0, 0, 0, 0, 0, 0, 0,
	/* 32 */
	sbcall_sifinitcmd,
	/* 33 */
	sbcall_sifexitcmd,
	/* 34 */
	sbcall_sifsendcmd,
	/* 35 */
	sbcall_sifcmdintrhdlr,
	/* 36 */
	sbcall_sifaddcmdhandler,
	/* 37 */
	sbcall_sifremovecmdhandler,
	/* 38 */
	sbcall_sifsetcmdbuffer,
	/* 39 */
	sbcall_sifsetsreg,
	/* 40 */
	sbcall_sifgetsreg,
	/* 41 */
	sbcall_sifgetdatatable,
	/* 42 */
	sbcall_sifsetsyscmdbuffer,
	0, 0, 0, 0, 0,
	/* 48 */
	sbcall_sifinitrpc,
	/* 49 */
	0,
	/* 50 */
	0,
	/* 51 */
	sbcall_sifbindrpc,
	/* 52 */
	sbcall_sifcallrpc,
	/* 53 */
	0,
	/* 54 */
	0,
	/* 55 */
	0,
	/* 56 */
	0,
	/* 57 */
	0,
	/* 58 */
	0,
	/* 59 */
	0,
	0, 0, 0, 0,
	/* 64 */
	sbcall_iopheapinit,
	sbcall_iopaheapalloc,
	sbcall_iopheapfree
};

int sbios(tge_sbcall_t sbcall, void *arg)
{
	int ret;
	int (*sbfunc)(void *) = dispatch[sbcall];
#if 0
	const char *description = "unknown";

	if (sbcall < sizeof(sbiosDescription) / sizeof(sbiosDescription[0])) {
		description = sbiosDescription[sbcall];
	}

	printf("sbios call %s %s\n", description, (sbfunc) ? "implemented" : "not implemented");
#endif

	if (!sbfunc)
		return -1;

	ret = sbfunc(arg);

#if 0
	printf("result of sbios call %d\n", ret);
#endif
	return ret;
}