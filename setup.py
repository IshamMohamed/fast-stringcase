from setuptools import setup, Extension

import sysconfig

include_dirs = [sysconfig.get_paths()["include"], sysconfig.get_paths()["platinclude"]]
module = Extension(
    "fast_stringcase.fast_stringcase",
    sources=["fast_stringcase/fast_stringcase.c"],
    include_dirs=include_dirs,
)

with open("readme.md", "r", encoding="utf-8") as fh:
    long_description = fh.read()

setup(
    name="fast_stringcase",
    version="1.2.0b3",
    description="C extension for string case conversions. The logic is ported from and based on stringcase == 1.2.0",
    long_description=long_description,
    long_description_content_type="text/markdown",
    author="Isham Mohamed",
    author_email="isham.iqbal@icloud.com",
    license='MIT',
    url='https://github.com/IshamMohamed/fast-stringcase/',
    download_url='https://pypi.org/project/fast-stringcase/',
    ext_modules=[module],
    packages=["fast_stringcase"],
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: MIT License",
        "Operating System :: OS Independent",
    ],
    python_requires=">=3.9",
)
