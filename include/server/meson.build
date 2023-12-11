pkgdatadir = get_option('prefix') / 'share' / 'keyboard_suggest'
conf = configuration_data()
conf.set_quoted('PKGDATADIR', pkgdatadir)

configure_file(
  input: 'config.h.in',
  output: 'config.h',
  configuration: conf,
)
