from distutils.core import setup
from distutils.extension import Extension

setup(name='xmlquery',
      version='0.9alpha',
      py_modules=['xmlquery', 'libxmlquery', 'html2xml'],
      ext_modules=[Extension('_libxmlquery',
                             ['libxmlquery_wrap.c', 'helpers.c'],
                             libraries=['xmlquery'])],
      scripts=['html2xml'],
      )

