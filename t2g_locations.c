#include "t2g_locations.h"

t2g_QSPLocation *t2g_qspLocs = 0;
int t2g_qspLocsCount = 0;

void t2g_qspCreateWorld(int t2g_locsCount)
{
	int i;
	for (i = 0; i < t2g_qspLocsCount; ++i)
	{
		free(t2g_qspLocs[i].Name);
		free(t2g_qspLocs[i].OnVisit);
	}
	if (t2g_qspLocsCount != t2g_locsCount)
	{
		t2g_qspLocsCount = t2g_locsCount;
		t2g_qspLocs = (t2g_QSPLocation *)realloc(t2g_qspLocs, t2g_qspLocsCount * sizeof(t2g_QSPLocation));
	}
}
