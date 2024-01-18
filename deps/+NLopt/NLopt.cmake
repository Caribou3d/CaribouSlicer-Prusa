add_cmake_project(NLopt
  URL https://github.com/stevengj/nlopt/archive/refs/tags/v2.7.1.tar.gz
  URL_HASH SHA256=db88232fa5cef0ff6e39943fc63ab6074208831dc0031cf1545f6ecd31ae2a1a
  CMAKE_ARGS
    -DNLOPT_PYTHON:BOOL=OFF
    -DNLOPT_OCTAVE:BOOL=OFF
    -DNLOPT_MATLAB:BOOL=OFF
    -DNLOPT_GUILE:BOOL=OFF
    -DNLOPT_SWIG:BOOL=OFF
    -DNLOPT_TESTS:BOOL=OFF
)
