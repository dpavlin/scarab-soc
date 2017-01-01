#!/usr/bin/env python3
from litex.build.xilinx import FpgaProg

prog = FpgaProg()
prog.load_bitstream("build/gateware/top.bit")
