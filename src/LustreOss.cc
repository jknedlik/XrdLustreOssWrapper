#define __METHOD_NAME__ methodName(__PRETTY_FUNCTION__)
#define DEBUG(a) std::cerr << "[DEBUG]" << a << std::endl;
#include "LustreOss.hh"
#include <XrdOuc/XrdOucStream.hh>
#include <XrdOuc/XrdOucString.hh>
#include <XrdVersion.hh>
#include <chrono>
#include <fcntl.h>
#include <stdexcept>
#include <sys/quota.h>

extern "C" {
XrdOss* XrdOssGetStorageSystem(XrdOss* native_oss, XrdSysLogger* Logger, const char* config_fn,
                               const char* parms) {
    LustreOss* Oss = new LustreOss(native_oss, Logger, config_fn);
    return (XrdOss*)Oss;
}
}
extern "C" {
struct qsStruct getQuotaSpace(char* target);
};
LustreOss::LustreOss(XrdOss* native_oss, XrdSysLogger* logger, const char* config_fn)
  : nativeOss(native_oss), log(logger) {
    lustremount = "";
    nativeOss->Init(log, config_fn);
    loadConfig(config_fn);
}

LustreOss::~LustreOss() {}
void LustreOss::loadConfig(const char* filename) {

    XrdOucStream Config;
    int cfgFD;
    char *var, *configPath = 0;

    if ((cfgFD = open(filename, O_RDONLY, 0)) < 0) {
        return;
    }

    Config.Attach(cfgFD);
    while ((var = Config.GetMyFirstWord())) {
        if (strcmp(var, "LustreOss.lustremount") == 0) {
            var += 21;
            lustremount = std::string(Config.GetWord());
            break;
        }
        if (strcmp(var, "LustreOss.cachetime") == 0) {
            var += 19;
            cacheTime = std::chrono::seconds(std::atol(Config.GetWord()));
            std::cerr << "LustreOss duration:" << cacheTime.count() << std::endl;
            lastChecked = decltype(lastChecked){};
            break;
        }
    }
    if (lustremount.empty())
        throw std::runtime_error("LustreOss.lustremount not set in configuration file");
    if (cacheTime.count() < 0)
        throw std::invalid_argument("LustreOss.cachetime set incorrectly (0,max<long>)");
    Config.Close();
}

int LustreOss::StatVS(XrdOssVSInfo* sP, const char* sname, int updt) {
    char* buf = strdup(lustremount.c_str());
    if (lastChecked == decltype(lastChecked){} ||
        (cacheTime - (std::chrono::system_clock::now() - lastChecked)).count() < 0) {
        cacheValue = getQuotaSpace(buf);
        lastChecked = std::chrono::system_clock::now();
    } else {
        std::cerr << "Using cached statvs value" << std::endl;
    }
    sP->Total = cacheValue.Total * 1024;
    sP->Usage = cacheValue.Curr * 1024;
    sP->LFree = sP->Free = sP->Total - sP->Usage;
    return XrdOssOK;
}

int LustreOss::StatFS(const char* path, char* buff, int& blen, XrdOucEnv* eP) {
    XrdOssVSInfo sP;
    int rc = StatVS(&sP, 0, 0);
    if (rc) {
        return rc;
    }
    int percentUsedSpace = (sP.Usage * 100) / sP.Total;
    blen = snprintf(buff, blen, "%d %lld %d %d %lld %d", 1, sP.Free, percentUsedSpace, 0, 0LL, 0);
    return XrdOssOK;
}
int LustreOss::StatLS(XrdOucEnv& env, const char* path, char* buff, int& blen) {
    static const char* Resp = "oss.cgroup=%s&oss.space=%lld&oss.free=%lld"
                              "&oss.maxf=%lld&oss.used=%lld&oss.quota=%lld";
    XrdOssVSInfo sP;
    int rc = StatVS(&sP, 0, 0);
    if (rc)
        return rc;

    blen = snprintf(buff, blen, Resp, "public", sP.Total, sP.Free, sP.LFree, sP.Usage, sP.Quota);
    return XrdOssOK;
}
XrdVERSIONINFO(XrdOssGetStorageSystem, LustreOss);
