#!/usr/bin/env bash

if (( $# != 1 )); then
    echo "Usage: ${0} <sol_name>"
    exit 1
fi

SOL_NAME="${1}"

zip -FSr "${SOL_NAME}" . -i **sol*.cpp -i **sol*.java
