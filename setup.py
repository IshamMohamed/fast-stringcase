from setuptools import setup, Extension

import sysconfig

include_dirs = [sysconfig.get_paths()["include"], sysconfig.get_paths()["platinclude"]]
module = Extension(
    "fast_stringcase.fast_stringcase",
    sources=["fast_stringcase/fast_stringcase.c"],
    include_dirs=include_dirs,
)


setup(
    ext_modules=[module],
)
