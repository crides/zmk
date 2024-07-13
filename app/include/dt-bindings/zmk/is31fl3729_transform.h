/*
 * Copyright (c) 2020 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

/**
 * Maps a 16x8 matrix cell index to a 1D array matching IS31FL3729's PWM registers order.
 */

#define RGB(com, r, g, b) (com + r) (com + g) (com + b)

#define SW(n) ((n - 1) * 16)
#define CS(n) (n - 1)
