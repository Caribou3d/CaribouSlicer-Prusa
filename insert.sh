#!/bin/bash

set -e

new_version="2.9.6"
latest_dir="01_Latest_Version_$new_version"

if [ -d "$latest_dir" ]; then
  echo "[ERROR] Directory '$latest_dir' already exists. Aborting."
  exit 1
fi

if [ ! -d "upload" ]; then
  echo "[ERROR] Required directory 'upload' does not exist. Aborting."
  exit 1
fi

# Exclude 00_Offline-Profiles and 00_PreRelease_Version_* and rotate remaining numbered folders
for dir in $(ls -1d [0-9][0-9]_*/ | grep -vE '^00_(Offline-Profiles|PreRelase_Version)' | sort -r); do
  num=$(echo "$dir" | cut -d'_' -f1)
  rest=$(echo "$dir" | cut -d'_' -f2- | sed 's|/$||')
  new_num=$(printf "%02d" $((10#$num + 1)))

  if [[ $rest == Latest_Version_* ]]; then
    version_part=$(echo "$rest" | cut -d'_' -f3)
    mv "$dir" "${new_num}_Previous_Version_$version_part"
  else
    mv "$dir" "${new_num}_$rest"
  fi

  echo "Renamed $dir -> ${new_num}_$rest"
done

# Create new latest version directory
mv upload "$latest_dir"
echo "✅ Created $latest_dir from 'upload'"
