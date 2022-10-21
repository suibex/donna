import os,io,sys
from ctypes import * 
class DonnaBackendTrigger(object):
    def __init__(self,bank,filename):
        
        self.lib = CDLL("./debug/donna_backend.so")


b = DonnaBackendTrigger("./banks/arm64.bank","test_exec")

