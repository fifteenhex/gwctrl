project('gwctrl', 'c')

mosquitto_glib = subproject('mosquitto_glib')
nodectrl = subproject('nodectrl')

cc = meson.get_compiler('c')

src = ['gwctrl.c']

deps = [ dependency('glib-2.0'),
         dependency('json-glib-1.0'),
         mosquitto_glib.get_variable('mosquitto_glib_dep'),
         nodectrl.get_variable('nodectrl_dep')]

executable('gwctrl', src, 
           dependencies : deps, 
           install : true,
           install_dir : 'bin')