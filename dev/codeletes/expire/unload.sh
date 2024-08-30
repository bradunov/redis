#!/bin/sh

sudo -E $JANUS_PATH/out/bin/janus_lcm_cli --address=/tmp/janus_lcm_ipc --direction unload --config expire_load.json
