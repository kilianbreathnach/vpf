from distutils.core import setup, Extension
import numpy.distutils.misc_util

c_ext = Extension("_vpf", ["_vpf.c", "vpf.c", "mtwist.c"],
                  libraries=['m'], extra_compile_args=["-Wall", "-Werror"])

setup(
    ext_modules=[c_ext],
    include_dirs=numpy.distutils.misc_util.get_numpy_include_dirs(),
    )
