from pathlib import Path
import ctypes
import re
from enum import Enum

# TODO: read from rpc_common.hpp

class Action(Enum):
    PRESS = 0
    UP = 1
    DOWN = 2

class RemoteException(Exception):
    pass


class RPC_Client:

    # TODO: use ctypes module export functionality
    # see https://stackoverflow.com/questions/69309123/dll-export-function-to-ctypes

    def __init__(self):

        modulePath = Path(__file__).parent

        # load error codes
        with open(modulePath / "error_codes.hpp", 'r') as fh:
            errorSource = fh.read()

        self.errorDict = dict([(int(errorCode), errorStr) for errorStr, errorCode 
                        in re.findall("#define\s+(\w+)\s+(\d+)", errorSource)])


        # load library
        self.lib = ctypes.CDLL(modulePath / "rpc_client.so")

        with open(modulePath / "rpc_client.hpp", 'r') as fh:
            rpcCliHeader = fh.read()

        funcNames = re.findall("\s*int\s+(\w+)\(", rpcCliHeader)

        for name in funcNames:
            setattr(self, name, 
                        self._wrap_function(self.lib._FuncPtr((name, self.lib)))
                        )


    def _wrap_function(self, func):

        def inner(*args, exceptions = False):

            rc = func(*args)

            if exceptions:
                if not rc: # raise errror if return code is non zero
                    raise RemoteException(rc, self.errorDict.get(rc, "UNKNOWN"))

            else:
                return rc

        return inner