#!/usr/bin/env python3
# -*- coding:utf-8 -*-
import os
import re

try:
    from setuptools import setup
except ImportError:
    from distutils.core import setup

packages = ["danmaQ"]
requires = ["requests"]

with open("README.md") as f:
    readme = f.read()

__version__ = re.search(
    "__version__\s*=\s*'(.*)'",
    open('danmaQ/__init__.py').read(), re.M).group(1)
assert __version__

setup(
    name="danmaQ",
    version=__version__,
    description="Display danmaku on any screen",
    long_description=readme,
    author="Justin Wong",
    author_email="justin.w.xd@gmail.com",
    url="https://github.com/bigeagle/danmaQ/",
    packages=packages,
    package_data={'': ['LICENCE', ], 'danmaQ': [os.path.join('images', '*.png'), ]},
    package_dir={'danmaQ': 'danmaQ'},
    include_package_data=True,
    install_requires=requires,
    license="GPLv3",
    entry_points={
        'console_scripts': ['danmaQ = danmaQ.app:main'],
    },
)

# vim: ts=4 sw=4 sts=4 expandtab
