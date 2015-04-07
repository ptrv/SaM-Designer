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

cd $scriptpath

if [ ! -d re2 ]; then
    echo "Submodule re2 is not checked out"
    exit
fi

# compile
cd re2
if [[ $os == "darwin" ]]; then
    num_jobs=`/usr/sbin/system_profiler -detailLevel full SPHardwareDataType | awk '/Total Number [Oo]f Cores/ {print $5};'`
else
    num_jobs=`grep -c 'model name' /proc/cpuinfo`
fi
make -j$num_jobs
