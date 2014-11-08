#!/usr/bin/env python2
# -*- coding:utf-8 -*-
import sys
from cx_Freeze import setup, Executable

# Dependencies are automatically detected, but it might need
# fine tuning.
buildOptions = dict(
    packages=['requests', 'danmaQ'],
    excludes=[],
    include_files=['danmaQ\images'],
)

name = 'danmaQ'

if sys.platform == 'win32':
    name = name + '.exe'

base = None
if sys.platform == "win32":
    base = "Win32GUI"

executables = [
    Executable('danmaQ.py',
               base=base,
               targetName=name)
]

with open("README.md") as f:
    readme = f.read()

setup(
    name="danmaQ",
    version="0.1-dev",
    description="Display danmaku on any screen",
    long_description=readme,
    author="Justin Wong",
    author_email="justin.w.xd@gmail.com",
    url="https://github.com/bigeagle/danmaQ/",
    options=dict(build_exe=buildOptions),
    executables=executables,
)

# vim: ts=4 sw=4 sts=4 expandtab
