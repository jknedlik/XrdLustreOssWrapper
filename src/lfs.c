//#include <liblustre.h>
//#include <lustre/lustre_idl.h>
#define typeof __typeof__
#include <ctype.h>
#include <dirent.h>
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <grp.h>
#include <mntent.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
long uid;
#include "qsStruct.h"
#include <lustre/lustre_user.h>
#include <lustre/lustreapi.h>
struct qsStruct getQuotaSpace(char* target) {
    struct if_quotactl quotacmd;
    memset(&quotacmd, 0, sizeof(quotacmd));
    quotacmd.qc_cmd = LUSTRE_Q_GETQUOTA;
    quotacmd.qc_type = GRPQUOTA;
    quotacmd.qc_id = getegid();
    llapi_quotactl(target, &quotacmd);
    struct obd_dqblk dqb = quotacmd.qc_dqblk;
    struct qsStruct qs;
    qs.Curr = lustre_stoqb(dqb.dqb_curspace);
    qs.Total = dqb.dqb_bsoftlimit;

    return qs;
}
