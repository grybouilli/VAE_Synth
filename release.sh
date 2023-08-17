#!/bin/bash
rm -rf release
mkdir -p release

cp -rf VAE_synth *.{hpp,cpp,txt,json} LICENSE release/

mv release score-addon-basic-model-addon
7z a score-addon-basic-model-addon.zip score-addon-basic-model-addon
