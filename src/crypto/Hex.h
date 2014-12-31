#ifndef __HEX__
#define __HEX__

// Copyright 2014 Stellar Development Foundation and contributors. Licensed
// under the ISC License. See the COPYING file at the top-level directory of
// this distribution or at http://opensource.org/licenses/ISC

#include "generated/StellarXDR.h"

namespace stellar
{

class ByteSlice;

std::string
binToHex(ByteSlice const& bin);

std::vector<uint8_t>
hexToBin(std::string const& hex);

}

#endif