#ifndef __ARCH_RISCV_REGS_RFLOAT_HH__
#define __ARCH_RISCV_REGS_RFLOAT_HH__

#include <softfloat.h>
#include <specialize.h>

#include <cstdint>
#include <string>
#include <vector>



namespace gem5
{

namespace RiscvISA
{

// Generic floating point value type.
using zrfreg_t = float64_t;


const std::vector<std::string> ZrRegNames = {
    "zr0"
};




}
}