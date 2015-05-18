#! /bin/bash

if [ "$#" -ne 1 ]
then
  echo "Usage: $0 DIRECTORY_TO_EXPORT_ARTIFACTS_TO"
  exit 1
fi

mkdir -p $1/lib
mkdir -p $1/include
mkdir -p $1/bin

cp -a lib/libmosquitto.* $1/lib/
cp -a lib/mosquitto.h $1/include/
cp -a client/mosquitto_* $1/bin/
cp -a src/mosquitto $1/bin/
cp -a src/mosquitto_passwd $1/bin/

