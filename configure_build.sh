#!/bin/sh

#==============================================================================
usage() {
  cat <<EOF
Usage: $(basename $0) <options> [debug|release]

Configures the build for release or debug

Options:
-h                 Display this message.
EOF
  exit
}

#==============================================================================
# Main
while getopts ":h?" option
do
  case $option in
    h)
      usage
      ;;
    \?)
      echo "Invalid option: -$OPTARG \n"
      usage
      ;;
  esac
done
file_to_copy=""
# should be 1st argument
if [ $1 = "debug" ]
then
    echo "Setting debug configuration"
    file_to_copy="debug_appinfo.json"
elif [ "$1" = "release" ]
then
    echo "Setting release configuration"
    file_to_copy="release_appinfo.json"
else
    echo "$1 incorrect option."    
    exit 1
fi


cp -f common_appinfo.json appinfo.json
cat $file_to_copy >> appinfo.json