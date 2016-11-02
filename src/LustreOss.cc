#define __METHOD_NAME__ methodName(__PRETTY_FUNCTION__)
#define DEBUG(a) std::cout<<"[DEBUG]"<<a<<std::endl;
#include <fcntl.h>
#include <XrdOuc/XrdOucString.hh>
#include <XrdOuc/XrdOucStream.hh>
#include <xrootd/XrdVersion.hh>
#include <sys/quota.h>
#include "LustreOss.hh"
#include "qsStruct.h"
extern "C"
{
  XrdOss*
  XrdOssGetStorageSystem(XrdOss* native_oss,
                         XrdSysLogger* Logger,
                         const char* config_fn,
                         const char* parms)
  {
    LustreOss* Oss = new LustreOss(native_oss,Logger);
    return (XrdOss*) Oss;
  }
}
extern "C" {
   struct qsStruct getQuotaSpace();
};
LustreOss::LustreOss(XrdOss* native_oss,XrdSysLogger*logger):nativeOss(native_oss),log(logger)
{
DEBUG("TEST")
nativeOss->Init(log,"");
}

LustreOss::~LustreOss()
{
}

int LustreOss::StatVS(XrdOssVSInfo *sP, const char *sname, int updt){
DEBUG("TEST")
    struct qsStruct qs =getQuotaSpace();
    sP->Total=qs.Total;
    sP->Usage=qs.Curr;
    sP->LFree=sP->Free=sP->Total - sP->Usage;
    return XrdOssOK;
}
XrdOssDF* LustreOss::newDir(const char *tident){
    return nativeOss->newDir(tident);
}
XrdOssDF* LustreOss::newFile(const char *tident){
    return nativeOss->newFile(tident);
}
int LustreOss::Chmod(const char *pm, mode_t mode, XrdOucEnv *eP){
    return nativeOss->Chmod(pm,mode,eP);
}
int LustreOss::Create(const char* a, const char* b, mode_t c, XrdOucEnv& d, int e){
    return nativeOss->Create(a,b,c,d,e);
}
int LustreOss::Init(XrdSysLogger *xsl, const char * n){
    return nativeOss->Init(xsl,n);
}
int LustreOss::Mkdir(const char * dir, mode_t mode, int mkpath,XrdOucEnv *eP){
    return nativeOss->Mkdir(dir,mode,mkpath,eP);
}
 int LustreOss::Remdir(const char * dir, int Opts, XrdOucEnv *eP){
    return nativeOss->Remdir(dir,Opts,eP);
}
int LustreOss::Rename(const char * source, const char *dest,XrdOucEnv *eP1, XrdOucEnv *eP2){
    return nativeOss->Rename(source,dest,eP1,eP2);
}
int LustreOss::Stat(const char *dest, struct stat *stats, int opts, XrdOucEnv *eP){
    return nativeOss->Stat(dest,stats,opts,eP);
}
int LustreOss::Truncate(const char *path, unsigned long long b, XrdOucEnv *eP){
    return nativeOss->Truncate(path,b,eP);
}
int LustreOss::Unlink(const char *path, int Opts, XrdOucEnv *eP){
    return nativeOss->Unlink(path,Opts,eP);
}
int LustreOss::StatFS(const char *path, char *buff, int &blen, XrdOucEnv *eP){
	XrdOssVSInfo sP;
	int rc = StatVS(&sP, 0, 0);
  	if (rc) {
    	return rc;
  	}
  	int percentUsedSpace = (sP.Usage*100)/sP.Total;
  	blen = snprintf(buff, blen, "%d %lld %d %d %lld %d",1, sP.Free, percentUsedSpace, 0, 0LL, 0);
	return XrdOssOK;
}
int LustreOss::StatLS(XrdOucEnv &env, const char *path, char *buff, int &blen){
	static const char *Resp="oss.cgroup=%s&oss.space=%lld&oss.free=%lld"
	    "&oss.maxf=%lld&oss.used=%lld&oss.quota=%lld";
	XrdOssVSInfo sP;
	int rc = StatVS(&sP, 0, 0);
    if(rc)return rc;
    

	blen = snprintf(buff,blen,Resp,"public",sP.Total,sP.Free,sP.LFree,sP.Usage,sP.Quota);
    return XrdOssOK;
}
XrdVERSIONINFO(XrdOssGetStorageSystem, LustreOss);
