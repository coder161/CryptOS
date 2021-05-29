#!/bin/sh
set -e
echo "[run.sh] Starting QEMU"
qemu-system-x86_64 CryptOS.img
echo "[run.sh] Done"
