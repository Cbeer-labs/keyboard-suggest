file_list = files(
  'messages.cpp',
)
deps = []
inc_dirs = [
  '../include',
]

extraarg = []

keyboard_lib = static_library('keyboard_suggestor',
  file_list,
  install: true,
  dependencies: deps,
  include_directories: inc_dirs,
  override_options: extraarg,
)

keyboard = declare_dependency(
  link_with: keyboard_lib,
  dependencies: deps,
  include_directories: inc_dirs,
)

meson.override_dependency('keyboard',
  keyboard,
  native: true,
  static: true,
)

http = static_library('http_server',
  'server/httpd.cpp',
  install: true,
  dependencies: [],
  include_directories: inc_dirs,
  override_options: extraarg,
)

app = executable('app',
  'main.cpp',
  install: true,
  dependencies: deps,
  link_with: [keyboard_lib, http],
  include_directories: inc_dirs,
  override_options: extraarg,
)
