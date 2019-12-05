#pragma once

#include "Pattern.h"

static const uint8_t PROGMEM world_64_32_8bit[2048] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xaa, 0xca, 0xb3, 0xaa, 0xaa, 0xbf, 0xcb, 0xcb, 0xb1, 0x4f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4f, 0x8f, 0x90, 0x99, 0xb4, 0xb8, 0xd3, 0xa2, 0x8c, 0xd1, 0xff, 0xff, 0xff, 0xff, 0xff, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0xb1, 0xd7, 0xcb, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x86, 0xb4, 0xbf, 0xaa, 0x7c, 0x9a, 0x91, 0xdd, 0xf6, 0xd9, 0xb8, 0xbc, 0xf2, 0xef, 0xb8, 0x00, 0x38, 0xfb, 0xff, 0xff, 0xff, 0xad, 0x00, 0x00, 0x00, 0x00, 0x00, 0x90, 0xc2, 0x91, 0x42, 0x00, 0x38, 0x80, 0xc0, 0xda, 0xfb, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdd, 0xd7, 0xe7, 0xd1, 0xc5, 0x90, 0x86, 0x90, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x38, 0xc5, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf6, 0xfd, 0xfc, 0xfb, 0xec, 0x86, 0xcb, 0xe5, 0x4f, 0xaa, 0xff, 0xdd, 0xa2, 0x38, 0xb9, 0x7e, 0x00, 0x00, 0x00, 0xaa, 0xff, 0xec, 0xff, 0xe9, 0xeb, 0xff, 0xff, 0xff, 0xfb, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdd, 0xa2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x38, 0xe8, 0xf6, 0xdd, 0xec, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc8, 0x4f, 0x38, 0xe2, 0xa6, 0x68, 0x00, 0x7a, 0xc9, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xbf, 0xff, 0xbf, 0xf1, 0xfb, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfd, 0xf1, 0xc0, 0xec, 0xb8, 0x0d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x70, 0x7f, 0x22, 0x00, 0x00, 0xed, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xd1, 0x62, 0xbf, 0xff, 0xff, 0x99, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc9, 0x7c, 0x6a, 0xd1, 0x8f, 0xf1, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf1, 0x00, 0x00, 0x7c, 0xec, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x2e, 0xfa, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe4, 0xff, 0xff, 0xff, 0xd1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0xbd, 0xd4, 0xdd, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfb, 0xa7, 0x00, 0xa4, 0x0d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x38, 0xfb, 0xff, 0xff, 0xff, 0xff, 0xfd, 0xe5, 0xe9, 0xff, 0xff, 0xec, 0x4d, 0xc4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x62, 0xfb, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf6, 0xfb, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc8, 0x71, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0xc5, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xed, 0xe3, 0xff, 0xbf, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0xcb, 0xf6, 0xb6, 0xde, 0xc5, 0xff, 0xd8, 0x89, 0xa2, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xec, 0x7c, 0xc5, 0x4f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf7, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xae, 0xff, 0xa7, 0x2e, 0x84, 0xa6, 0xd2, 0xd7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf6, 0xc4, 0xcb, 0x00, 0x9e, 0x5a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0xe7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xb8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x62, 0xda, 0xf6, 0xff, 0xb6, 0x00, 0x45, 0x0d, 0x82, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xbb, 0x89, 0x96, 0xd5, 0x4f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x9a, 0xec, 0xff, 0xff, 0xf2, 0xdf, 0xd1, 0xb1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xdd, 0xff, 0xff, 0xff, 0xff, 0xe7, 0xf6, 0xe2, 0xe2, 0xff, 0xff, 0xf6, 0xf6, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x4f, 0x56, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x70, 0xd1, 0xff, 0xfb, 0x38, 0x00, 0x00, 0x91, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xce, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfb, 0xd7, 0xff, 0xff, 0xb2, 0xd7, 0xe7, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfb, 0x5a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0xf6, 0xe7, 0x00, 0x90, 0x70, 0xa0, 0x4f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x5d, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xcc, 0xfb, 0xff, 0xff, 0xfb, 0x38, 0x38, 0xdd, 0xff, 0xff, 0xec, 0xd1, 0xff, 0xff, 0xf1, 0xcb, 0x77, 0x56, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9a, 0xf1, 0xdf, 0xcb, 0x38, 0x00, 0x86, 0x75, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x62, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf6, 0xb1, 0xff, 0xfb, 0xa2, 0x00, 0x00, 0x62, 0xff, 0xf1, 0x5c, 0x00, 0xea, 0xff, 0xe2, 0x6e, 0x00, 0x94, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0xde, 0xa2, 0x00, 0x68, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7a, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe7, 0xbf, 0x85, 0x00, 0x00, 0x00, 0x00, 0xdb, 0xaf, 0x00, 0x00, 0x00, 0xd6, 0xff, 0x9a, 0x00, 0x86, 0x7a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4f, 0xa0, 0xbe, 0xfb, 0xfb, 0xec, 0x62, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfb, 0xba, 0x00, 0x00, 0x00, 0x00, 0x85, 0xaa, 0x00, 0x00, 0x00, 0x90, 0x9a, 0x38, 0x00, 0x00, 0xcd, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xcb, 0xff, 0xff, 0xff, 0xf6, 0xbf, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0xb7, 0xaf, 0x86, 0xbf, 0xf8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfb, 0x4f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4f, 0x00, 0x00, 0x6e, 0xce, 0x7f, 0x00, 0xc2, 0x90, 0x62, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0xfb, 0xff, 0xff, 0xff, 0xff, 0xff, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf6, 0xff, 0xff, 0xff, 0xf1, 0xfb, 0xf9, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc6, 0xaa, 0xbf, 0xff, 0xb2, 0x89, 0x5a, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9d, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xec, 0xb1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xce, 0xff, 0xff, 0xff, 0xf6, 0xff, 0xa2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x51, 0xd1, 0x70, 0xb0, 0xb5, 0x71, 0x60, 0xbb, 0xf4, 0xbf, 0x5f, 0x76, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4f, 0xf6, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x62, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x85, 0xff, 0xff, 0xff, 0xff, 0xff, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4f, 0xa2, 0x85, 0x7a, 0x6e, 0x00, 0x38, 0xb1, 0xcd, 0xb6, 0x38, 0x4f, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xaf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc5, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x5a, 0xff, 0xff, 0xff, 0xff, 0xff, 0xaa, 0x38, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4f, 0x90, 0xc5, 0x62, 0x77, 0x38, 0x00, 0x00, 0x32, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xbb, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xa3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xaa, 0xff, 0xff, 0xff, 0xff, 0xf6, 0x7c, 0xdd, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0xed, 0xff, 0xf2, 0xd1, 0xcb, 0x00, 0x00, 0x00, 0x4d, 0x3d, 0x4f, 0x53,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe8, 0xff, 0xff, 0xff, 0xff, 0xff, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x69, 0xfb, 0xff, 0xff, 0xff, 0x95, 0x4f, 0xf6, 0x35, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x26, 0xbc, 0xf1, 0xff, 0xff, 0xff, 0xff, 0xfb, 0x42, 0x00, 0x00, 0x51, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe7, 0xff, 0xff, 0xff, 0xe7, 0x4f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xef, 0xff, 0xff, 0xf6, 0x65, 0x4f, 0xaa, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x87, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xbe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe7, 0xff, 0xff, 0xff, 0xc5, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xbe, 0xff, 0xff, 0xb7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x86, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xa2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xcb, 0xff, 0xff, 0xec, 0x4f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x62, 0xd2, 0xb3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x98, 0xd1, 0xa2, 0x4f, 0xca, 0xff, 0xff, 0xcb, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xcf, 0xff, 0xf9, 0x98, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xce, 0xc4, 0x00, 0x00, 0x00, 0x00, 0xb6, 0x38, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x86, 0xff, 0xb8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xae, 0x00, 0x00, 0x00, 0x7c, 0xab, 0x62, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf6, 0xb1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4f, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xbf, 0xb8, 0x0d, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x71, 0x4f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


#if !defined(pgm_read_byte)
#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#endif
#if !defined(pgm_read_word)
#define pgm_read_word(addr) (*(const unsigned short *)(addr))
#endif


// grayscale 8 bit image
void drawGrayscaleBitmap(int16_t x, int16_t y,
	const uint8_t bitmap[], int16_t w, int16_t h, CRGB fg, CRGB bg) {
	uint16_t buffidx = 0;
	for (int16_t j = 0; j < h; j++) {
		for (int16_t i = 0; i < w; i++) {
			//          CRGB colour = blend(fg, bg, (uint8_t)pgm_read_byte(&bitmap[j * w + i]));
			CRGB colour = blend(bg, fg, pgm_read_byte(bitmap + buffidx));
			buffidx++;
			gfx.putPixel((x + i + SCREEN_WIDTH) % SCREEN_WIDTH, j + y, colour);
		}
	}
}

// 16bit 565 encoded image
void drawRGBBitmap(int16_t x, int16_t y,
	const uint16_t bitmap[], int16_t w, int16_t h) {
	for (int16_t j = 0; j < h; j++, y++) {
		for (int16_t i = 0; i < w; i++) {
			uint16_t color = pgm_read_word(&bitmap[j * w + i]);
			gfx.putPixel(x + i, y, CRGB(pgm_read_byte(&gamma5[color >> 11]),
				pgm_read_byte(&gamma6[(color >> 5) & 0x3F]),
				pgm_read_byte(&gamma5[color & 0x1F])));
		}
	}
}

class PatternWorld: public _Pattern {
public: 
	PatternWorld() : _Pattern("World") {}

	

	uint8_t drawFrame() {

		CRGB bg = CHSV(157, 255, 255);
		CRGB fg = CHSV(93, 255, 255);
		static uint16_t offset = SCREEN_WIDTH * 6;
//#if defined(ARDUINO)
//		ledControl.drawGrayscaleBitmap(offset / 8, 0, world_64_32_8bit, 64, 32, fg, bg);
//#else 
//		
		uint16_t buffidx = 0;
		for (int16_t j = 0; j < SCREEN_HEIGHT; j++) {
			for (int16_t i = 0; i < SCREEN_WIDTH; i++) {
				CRGB colour = blend(bg, fg, (world_64_32_8bit[buffidx]));
				buffidx++;
				gfx.putPixel(((offset/6) + i + SCREEN_WIDTH) % SCREEN_WIDTH, j + 0, colour);
			}
		}
//#endif
		offset--;
		if (offset < 0)
			offset = SCREEN_WIDTH * 6;
		return returnVal;
	}


};
