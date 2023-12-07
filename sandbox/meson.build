file_list = files(
  'main.cpp',
)
deps = []
inc_dirs = [
  '../include',
]

sandbox = executable('sandbox',
  file_list,
  install: true,
  dependencies: deps,
  include_directories: inc_dirs,
)
