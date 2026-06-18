#!/usr/bin/env python

top = '.'
out = 'build'

targets = [
    'dwm',
    'dmenu',
    'slstatus',
    'slstatus/components',
    'st',
    'nuklear',
    'fonts'
]
packages = [
    ('x11','X11'),
    ('xft','XFT'),
    ('gl','GLEW'),
    ('xinerama','XINER'),
    ('fontconfig','FTCFG'),
    ]
def configure(ctx):
    ctx.load('compiler_c')
    ctx.load('compiler_cxx')
    ctx.load('clang_compilation_database', tooldir='wafscripts')
    ctx.load('gnu_dirs')
    ctx.check_cc(msg="Checking for C compiler", mandatory=True)
    for package in packages: ctx.check_cfg(package=package[0], uselib_store=package[1], args=['--cflags', '--libs'])
    for target in targets: ctx.recurse(target)
        
def options(opt):
    opt.load("compiler_c")
    opt.load('compiler_cxx')
    opt.load('clang_compilation_database', tooldir='wafscripts')
    # opt.check_cc()
def build(ctx):
    for target in targets: ctx.recurse(target)
