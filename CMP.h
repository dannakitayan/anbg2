#pragma once
#include "Mesh.h"

//Own map of chunks based on QBSP idea;
constexpr byte kCMPTextureNameMax = 64;
constexpr auto kCMP = (('M' << 24) + ('D' << 16) + ('L' << 8) + '0');
constexpr int32 kCMPVersion = 1;

class CMP
{
};

