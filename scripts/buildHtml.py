from shutil import copyfile
from subprocess import check_output, CalledProcessError
import sys
import os
import platform
import subprocess
import time

Import("env")

cImport = "#include <Arduino.h>\n\n"
cProgmem = "const char indexHTML[] PROGMEM = R\"=====("
cWrapEnd = ")=====\";"

def get_build_flag_value(flag_name):
    build_flags = env.ParseFlags(env['BUILD_FLAGS'])
    flags_with_value_list = [build_flag for build_flag in build_flags.get('CPPDEFINES') if type(build_flag) == list]
    defines = {k: v for (k, v) in flags_with_value_list}
    return defines.get(flag_name)

def readFile(name):
    with open(name, 'r') as file:
        data = file.read().replace('[%WS_PORT%]', get_build_flag_value('WS_PORT'))
    return data

def wrap(data):
    return cImport + cProgmem + data + cWrapEnd 

# def zip(data):

def write(data):
    with open("./src/page/index.h", "w") as text_file:
        text_file.write(data)


def build_web():
    print("Attempting to build webpage...")
    try:
        if platform.system() == "Windows":
            print("Nope...")
        else:
            file = readFile("./src/page/index.html");
            write(wrap(file))
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
    #sys.exit(1)

build_web()
