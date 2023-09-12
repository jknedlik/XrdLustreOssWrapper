FROM rockylinux:8.8
MAINTAINER jknedlik <j.knedlik@gsi.de>
WORKDIR /root
RUN dnf update -y
RUN dnf install -y epel-release
RUN dnf install --enablerepo=powertools -y bash git make gcc gcc-c++ rpm-build libyaml-devel cpio libtool kernel-headers kernel-devel

# Get Lustre
RUN curl https://downloads.whamcloud.com/public/lustre/lustre-2.12.9/el8/client/SRPMS/lustre-2.12.9-1.src.rpm > lustre.rpm
RUN rpm2cpio ./lustre.rpm| cpio -idmv 
RUN tar -xf lustre-2.12.9.tar.gz

# Build Lustre
WORKDIR /root/lustre-2.12.9/
RUN sh ./autogen.sh 
RUN ./configure
RUN make -j8
RUN make install

# Install XRootD server + sources
RUN dnf install -y xrootd-server xrootd-server-devel 

# Build XrdLustreOSS
COPY src /root/src
WORKDIR /root/src
RUN CPLUS_INCLUDE_PATH=/usr/include/xrootd:/usr/include/lustre make -j8

# package XrdLustreOss
WORKDIR /root/rpmbuild
RUN mkdir -p BUILD SOURCES SPECS RPMS SRPMS XrdLustreOss-0.9
RUN mv /root/src/LibXrdLustreOss.so XrdLustreOss-0.9
RUN tar -cf SOURCES/XrdLustreOss-0.9.tar.gz XrdLustreOss-0.9
WORKDIR /root/
COPY XrdLustreOss.spec /root/rpmbuild/SPECS/
RUN rpmbuild  -bb rpmbuild/SPECS/XrdLustreOss.spec

