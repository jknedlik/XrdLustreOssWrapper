/************************************************************************
 * EOS - the CERN Disk Storage System                                   *
 * Copyright © 2013 CERN/Switzerland                                    *
 *                                                                      *
 * Author: Joaquim Rocha <joaquim.rocha@cern.ch>                        *
 *                                                                      *
 * This program is free software: you can redistribute it and/or modify *
 * it under the terms of the GNU General Public License as published by *
 * the Free Software Foundation, either version 3 of the License, or    *
 * (at your option) any later version.                                  *
 *                                                                      *
 * This program is distributed in the hope that it will be useful,      *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
 * GNU General Public License for more details.                         *
 *                                                                      *
 * You should have received a copy of the GNU General Public License    *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.*
 ************************************************************************/

#ifndef __lustre_OSS_HH__
#define __lustre_OSS_HH__

#include <XrdOuc/XrdOucStream.hh>
#include <XrdOuc/XrdOucString.hh>
#include <XrdSys/XrdSysLogger.hh>
#include <stdio.h>
#include <xrootd/XrdOss/XrdOss.hh>
#include <xrootd/XrdVersion.hh>
class LustreOss : public XrdOss {
  public:
    virtual int StatVS(XrdOssVSInfo* sP, const char* sname = 0, int updt = 0);
    LustreOss(XrdOss* native_oss, XrdSysLogger*, const char*);
    virtual ~LustreOss();
    XrdOssDF* newDir(const char* tident);
    XrdOssDF* newFile(const char* tident);
    int StatFS(const char* path, char* buff, int& blen, XrdOucEnv* eP);
    int Chmod(const char* pm, mode_t mode, XrdOucEnv* eP = 0);
    int Create(const char* a, const char* b, mode_t c, XrdOucEnv& d, int e);
    int Init(XrdSysLogger*, const char*);
    int Mkdir(const char* dir, mode_t mode, int mkpath = 0, XrdOucEnv* eP = 0);
    int Remdir(const char* dir, int Opts = 0, XrdOucEnv* eP = 0);
    int Rename(const char* source, const char* dest, XrdOucEnv* eP1 = 0, XrdOucEnv* eP2 = 0);
    int Stat(const char* dest, struct stat*, int opts = 0, XrdOucEnv* eP = 0);
    int Truncate(const char*, unsigned long long, XrdOucEnv* eP = 0);
    int Unlink(const char*, int Opts = 0, XrdOucEnv* eP = 0);
    int StatLS(XrdOucEnv& env, const char* path, char* buff, int& blen);
    void loadConfig(const char* filename);

  private:
    std::string lustremount;
    XrdOss* nativeOss;
    XrdSysLogger* log;
};

#endif /* __lustre_OSS_HH__ */
