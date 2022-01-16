from shutil import copyfile
from subprocess import check_output, CalledProcessError
import sys
import os
import platform
import subprocess
import time
import gzip
from io import BytesIO
from pathlib import Path
import binascii
import codecs

Import("env")

cImport = "#include <Arduino.h>\n\n"
cProgmem = "const char %sHTML[] PROGMEM = {"
cWrapEnd = "};"
cLengthTemplate = "const uint16_t %sHTML_L = %d;\n"

def get_build_flag_value(flag_name):
    build_flags = env.ParseFlags(env['BUILD_FLAGS'])
    flags_with_value_list = [build_flag for build_flag in build_flags.get('CPPDEFINES') if type(build_flag) == list]
    defines = {k: v for (k, v) in flags_with_value_list}
    return defines.get(flag_name)

def readFile(name):
    with open(name, 'r') as file:
        data = file.read().replace('[%WS_PORT%]', get_build_flag_value('WS_PORT'))
    return data

def zip(data, name):
    out = BytesIO()
    with gzip.GzipFile(fileobj=out, mode="w") as f:
        f.write(data.encode('utf-8'))
    
    global cLength
    cLength = cLengthTemplate % (name, len(out.getvalue()))
    return ",".join(["{0:#0{1}x}".format(b,4) for b in out.getvalue()])

def wrap(data, name):
    return cImport + cLength + (cProgmem % name) + data + cWrapEnd 

def write(data, outFile):
    with open(outFile, "w") as text_file:
        text_file.write(data)


def build_web():
    print("Attempting to build webpage...")
    try:
        if platform.system() == "Windows":
            print("Nope...")
        else:
            inFile = Path("./src/page/index.html");
            outFile = Path("./src/page/index.h");
            if not outFile.exists() or inFile.stat().st_mtime > outFile.stat().st_mtime:
                write(wrap(zip(readFile(inFile), "index"), "index"), outFile)

            inFile = Path("./src/page/captive.html");
            outFile = Path("./src/page/captive.h");
            if not outFile.exists() or inFile.stat().st_mtime > outFile.stat().st_mtime:
                write(wrap(zip(readFile(inFile), "captive"), "captive"), outFile)

            inFile = Path("./src/page/reboot.html");
            outFile = Path("./src/page/reboot.h");
            if not outFile.exists() or inFile.stat().st_mtime > outFile.stat().st_mtime:
                write(wrap(zip(readFile(inFile), "reboot"), "reboot"), outFile)

    except OSError as e:
        print("Encountered error OSError building webpage:", e)
        if e.filename:
            print("Filename is", e.filename)
        print("WARNING: Failed to build web package. Using pre-built page.")
    except CalledProcessError as e:
        print(e.output)
        print("Encountered error CalledProcessError building webpage:", e)
        print("WARNING: Failed to build web package. Using pre-built page.")
    except Exception as e:
        print("Encountered error", type(e).__name__, "building webpage:", e)
        print("WARNING: Failed to build web package. Using pre-built page.")
    # sys.exit(1)

build_web()
