#! /bin/sh

os=${OSTYPE//[0-9.]/}
echo "OS Type: $os"
if [[ $os == "darwin" ]]; then
    script=`perl -e 'use Cwd "abs_path";print abs_path(shift)' $0`
    scriptpath=`dirname $script`
else
    script=`readlink -f $0`
    scriptpath=`dirname $script`
fi

re2_url="https://re2.googlecode.com/files/re2-20140304.tgz"
re2_archive="re2-20140304.tgz"

# download re2
cd $scriptpath
if [[ $os == "darwin" ]]; then
    curl -L $re2_url -o $re2_archive
else
    wget $re2_url -O $re2_archive
fi

tar -xvzf $re2_archive
rm $re2_archive

# compile
cd re2
if [[ $os == "darwin" ]]; then
    num_jobs=`/usr/sbin/system_profiler -detailLevel full SPHardwareDataType | awk '/Total Number [Oo]f Cores/ {print $5};'`
    make -e CXXFLAGS="-Wall -O3 -g -pthread -arch i386" -e LDFLAGS="-pthread -arch i386" -j$num_jobs
else
    num_jobs=`grep -c 'model name' /proc/cpuinfo`
    make -j$num_jobs
fi
