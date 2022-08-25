# -*- coding: utf-8 -*-
from setuptools import setup, Extension

long_description = """
# ptpsync

ptpsync is a module that provides the `clock_to_sys` function, which
synchronizes the System clock with the time read from the provided clock
device (which, in my case, is meant to be KVM's paravirtualized PTP clock).
"""

setup_kwargs = {
    "name": "ptpsync",
    "version": "0.0.2",
    "description": "Synchronize the System clock from a PTP clock device",
    "long_description": long_description,
    "author": "Christos Katsakioris",
    "author_email": "ckatsak@gmail.com",
    "maintainer": None,
    "maintainer_email": None,
    "url": "https://github.com/ckatsak/pyptpsync",
    "python_requires": ">=3.6,<4.0",
    "ext_modules": [Extension(name="ptpsync", sources=["ptpsync/ptp_sync.c"])],
    "license": "GPL-2.0-or-later",
    "keywords": ["ptp", "clock", "sync", "system", "time"],
    "classifiers": [
        "Development Status :: 3 - Alpha",
        "Intended Audience :: Developers",
        "Intended Audience :: System Administrators",
        "License :: OSI Approved :: GNU General Public License v2 or later (GPLv2+)",
        "Operating System :: POSIX :: Linux",
        "Programming Language :: C",
        "Programming Language :: Python :: 3.6",
        "Programming Language :: Python :: 3.7",
        "Programming Language :: Python :: 3.8",
        "Programming Language :: Python :: 3.9",
        "Programming Language :: Python :: 3.10",
        "Programming Language :: Python :: 3.11",
        "Topic :: Software Development :: Libraries :: Python Modules",
        "Topic :: System :: Installation/Setup",
        "Topic :: System :: Networking :: Time Synchronization",
        "Topic :: System :: Operating System",
        "Topic :: System :: Systems Administration",
    ],
}

setup(**setup_kwargs)
