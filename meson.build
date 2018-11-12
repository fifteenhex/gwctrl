project('gwctrl', 'c')

mosquittomainloop = subproject('mosquittomainloop')

src = ['gwctrl.c', 'location.c']
deps = [ dependency('glib-2.0'),
         dependency('json-glib-1.0'),
         dependency('libgps'), 
         mosquittomainloop.get_variable('mosquittomainloop_dep')]

executable('gwctrl', src, 
           dependencies : deps, 
           install : true,
           install_dir : 'bin')