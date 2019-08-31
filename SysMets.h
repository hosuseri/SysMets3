#ifndef __SYSMETS_H__
#define __SYSMETS_H__

struct sysmetrics_t {
    int iIndex ;
    const char *szLabel ;
    const char *szDesc ;
};

extern struct sysmetrics_t sysmetrics[];
extern const int NUMLINES;

#endif  /* __SYSMETS_H__ */
