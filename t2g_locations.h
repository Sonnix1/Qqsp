#include "qsp/declarations.h"

#ifndef QSP_T2G_LOCSDEFINES
	#define QSP_T2G_LOCSDEFINES

	typedef struct
	{
		QSP_CHAR *Name;
		QSP_CHAR *OnVisit;
	} t2g_QSPLocation;

	extern t2g_QSPLocation *t2g_qspLocs;
	extern int t2g_qspLocsCount;

	/* External functions */
	void t2g_qspCreateWorld(int);

#endif
