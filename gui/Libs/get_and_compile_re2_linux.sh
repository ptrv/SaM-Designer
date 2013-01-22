#! /bin/sh

script=`readlink -f $0`
scriptpath=`dirname $script`

re2_url="https://re2.googlecode.com/files/re2-20121127.tgz"
re2_archive="re2-20121127.tgz"

# download re2
cd $scriptpath
wget $re2_url -O $re2_archive
tar -xvzf $re2_archive
rm $re2_archive

# compile
cd re2
num_jobs=`grep -c 'model name' /proc/cpuinfo`
make -j$num_jobs
