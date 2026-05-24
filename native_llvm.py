import os

Import("env")


if env["PIOPLATFORM"] == "native" and os.name == "nt":
    msys2_ucrt_bin = "C:/msys64/ucrt64/bin"

    if os.path.exists(msys2_ucrt_bin):
        env.PrependENVPath("PATH", msys2_ucrt_bin)
        os.environ["PATH"] = msys2_ucrt_bin + os.pathsep + os.environ.get("PATH", "")