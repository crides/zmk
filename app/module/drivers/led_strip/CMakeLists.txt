# Copyright (c) 2022 The ZMK Contributors
# SPDX-License-Identifier: MIT

zephyr_include_directories(.)

zephyr_library()

zephyr_library_sources_ifdef(CONFIG_IS31FL3729 is31fl3729.c)
zephyr_library_sources_ifndef(CONFIG_IS31FL3729 ${ZEPHYR_BASE}/misc/empty_file.c)
