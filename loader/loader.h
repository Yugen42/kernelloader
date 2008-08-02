/* Copyright (c) 2007 Mega Man */
#ifndef _LOADER_H_
#define _LOADER_H_

#include "stdint.h"

#define LOADER_VERSION "1.9"

#ifdef __cplusplus
extern "C" {
#endif

	/** Structure describing module that should be loaded. */
	typedef struct
	{
		/** Path to module file. */
		const char *path;
		/** Buffer used for loading the module. */
		unsigned char *buffer;
		/** Module size. */
		unsigned int size;
		/** True, if module must be buffered and can't be loaded after IOP reset. */
		int buffered;
		/** Parameter length. */
		unsigned int argLen;
		/** Module parameters. */
		const char *args;
		/** Whether module should be loaded. */
		int load;
		/** 0 = no dependency on ps2link. 1 = don't use with ps2link, -1 = use
		 * with ps2link. */
		int ps2link;
		/** Should be used with rte, but not with tge. */
		int rte;
		/** Should be used with tge, but not with rte. */
		int tge;
		/** New rom modules. */
		int newmods;
		/** Old rom modules. */
		int oldmods;
		/** Module only for debug reasons. */
		int debug;
		/** True, if allocated memory is used. */
		int allocated;
		/** True, if ps2smap module. */
		int ps2smap;
		/** True, if eedebug.irx, IOP debug output is required. */
		int eedebug;
		/** True, if required for video DVDs. */
		int dvdv;
		/** -1, if for fat PS2, 1 if for slim PSTwo. */
		int slim;
		/** -1, if non free module, 1 if free module. */
		int free;
	} moduleEntry_t;

	typedef struct {
		int newModules;
		int enableTGE;
		int enablePS2LINK;
		int enableDebug;
		int enableSBIOSTGE;
		int enableDev9;
		int enableEEDebug;
		int newModulesInTGE;
		int slimPSTwo;
		int autoBootTime;
		int free;
	} loader_config_t;

	extern loader_config_t loaderConfig;

	int loader(void *arg);
	int getNumberOfModules(void);
	moduleEntry_t *getModuleEntry(int idx);
	void disableSBIOSCalls(uint32_t *SBIOSCallTable);
	const char *getSBIOSFilename(void);
	const char *getKernelFilename(void);
	const char *getInitRdFilename();
	char *getKernelParameter(void);
	void waitForUser(void);
	const char *getPS2MAPParameter(int *len);
	const char *getGraphicMode(void);
	void printAllModules(void);
	void DelayThread(int delay);
	const char *getPcicType(void);
#ifdef __cplusplus
}
#endif

#endif
