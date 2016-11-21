//#include <liblustre.h>
//#include <lustre/lustre_idl.h>
#define typeof __typeof__
#include <stdlib.h>                                                                                                                                                                                                                    
#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <mntent.h>
#include <errno.h>
#include <err.h>
#include <pwd.h>
#include <grp.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <time.h>
#include <ctype.h>
#include <unistd.h>
long uid;
#include <lustre/lustreapi.h>
#include <lustre/lustre_user.h>
#include "qsStruct.h"                   
struct qsStruct getQuotaSpace(){
struct if_quotactl quotacmd;
 memset(&quotacmd, 0, sizeof(quotacmd));
quotacmd.qc_cmd=LUSTRE_Q_GETQUOTA;
quotacmd.qc_type=GRPQUOTA;
quotacmd.qc_id= getegid();
llapi_quotactl("/lustre/nyx",&quotacmd);
struct obd_dqblk dqb = quotacmd.qc_dqblk;
struct qsStruct qs;
qs.Curr=lustre_stoqb(dqb.dqb_curspace);
qs.Total=dqb.dqb_bsoftlimit;

return qs;
}
