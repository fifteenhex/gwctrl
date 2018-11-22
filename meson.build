project('gwctrl', 'c')

mosquittomainloop = subproject('mosquittomainloop')

cc = meson.get_compiler('c')

src = ['gwctrl.c', 'location.c', 'thermal.c']

deps = [ dependency('glib-2.0'),
         dependency('json-glib-1.0'),
         dependency('libgps'),
         cc.find_library('m'),
         mosquittomainloop.get_variable('mosquittomainloop_dep')]

executable('gwctrl', src, 
           dependencies : deps, 
           install : true,
           install_dir : 'bin')