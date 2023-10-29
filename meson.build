project('tetrisClone', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3'])

ncurses = dependency('ncurses')

exe = executable('tetrisclone', 'tetrisclone.cpp', dependencies: ncurses,
  install : true)

test('basic', exe)
