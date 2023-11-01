project('tetrisClone', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3'])

ncurses = dependency('ncurses')


exe = executable('tetrisclone', 'tetrisclone.cpp','board.hpp','shape.hpp', dependencies: ncurses,
  install : true)

test('basic', exe)
