project(
  'stdcxx',
  'cpp',
  meson_version : '>= 1.3.0',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++14'],
)

dependencies = [
]

subdir('src')

exe = executable(
  'stdcxx',
  sources,
  dependencies : dependencies,
  install : true,
)

test('basic', exe)
