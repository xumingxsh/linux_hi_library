import os
import sys

local = os.path.dirname(os.path.abspath(__file__))
parent = os.path.dirname(local)
sys.path.append(parent)
import build_common
		
build_common.make_build_run(local)