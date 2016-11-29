#LustreOssWrapper
An XRootD OSS plug-in to overwrite XRootD's base implementation for space usage statistics with calls to the lustre quota API on a singular lustre mount.
## Warning
The sources include XRootD and lustre header files (especially lustre/lustreapi.h,lustre/lustre_user.h)
and needs to be linked against the lustre api library. (-llustreapi)

## Compiling
To create the plug-in shared library, run make in the "src" directory.
For this to work, you need to set 2 environmental variables:

* Set *XRD_PATH* to the top level of your current xrootd installation.
* Set *LUSTRE_PATH* to the top level of your current lustre installation(with header files). 

This will create the shared library "LibXrdLustreOss.so"

## Configuration
This plug-in is loaded by the XRootD server. In order to accomplish this, you need to indicate the serveer where the plugin lies in the server's configuration file.
```shell
ofs.osslib  /installdir/XrdLustreOssWrapper/src/LibXrdLustreOss.so
LustreOss.lustremount /path/to/lustre/mount

```
You also need to define the mount point of your Lustre FS with LustreOss.lustremount


## Usage
When using this plug-in, all high level XRootD usage statistics calls (xrdfs spaceinfo for example) will be fed lustre quota statistics of the data server's executing user's group.

## License
The XrdLustreOssWrapper plug-in is distributed under the terms of the GNU Lesser Public Licence version 3 (LGPLv3)

##ToDo
* ~~Create a way to configure the lustre mount point in the server's configuration file~~
* Create a simple CMake setup that 

