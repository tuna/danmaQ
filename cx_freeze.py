#!/usr/bin/env python2
# -*- coding:utf-8 -*-
import os
import sys
import site
from cx_Freeze import setup, Executable


site_dir = site.getsitepackages()[1]

# Dependencies are automatically detected, but it might need
# fine tuning.
buildOptions = dict(
    packages=['requests', 'danmaQ'],
    excludes=[],
    include_files=[
        'LICENSE',
        'danmaQ\images',
        # (os.path.join(site_dir, 'PyQt5', 'libEGL.dll'), 'libEGL.dll'),
    ],
    include_msvcr=True,
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
               icon=os.path.join("danmaQ", "images", "statusicon.ico"),
               targetName=name)
]

with open("README.md") as f:
    readme = f.read()

setup(
    name="danmaQ",
    version="0.11",
    description="Display danmaku on any screen",
    long_description=readme,
    author="Justin Wong",
    author_email="justin.w.xd@gmail.com",
    url="https://github.com/bigeagle/danmaQ/",
    license="GPLv3",
    options=dict(build_exe=buildOptions),
    executables=executables,
)

# vim: ts=4 sw=4 sts=4 expandtab
