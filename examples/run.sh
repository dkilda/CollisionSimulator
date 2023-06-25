#!/bin/bash


BASEDIR=`pwd`
BINDIR=${BASEDIR}/../src
BIN=${BINDIR}/main


setup_control() {
 
CONTROL=CONTROL
cat <<EOF > ${CONTROL}

$read_file
"$filename"
$tmax
$dt
EOF
}


run_with_control() {

setup_control
$BIN < $CONTROL | tee log.txt
cd ../
}


run() {

cd $BINDIR
./main $read_file "$filename" $tmax $dt
cd $BASEDIR
}




read_file=1
filename="${BASEDIR}/data/diffusion2.txt"
N=10
tmax=10000.0
dt=0.5    # (default timestep)
#dt=0.1   # (use with diffusion3.txt, brownian.txt data)
#dt=0.025 # (use with p1000-2K.txt,   p2000.txt    data)

run

























