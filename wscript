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
def configure(ctx):
    ctx.load('compiler_c')
    ctx.load('compiler_cxx')
    ctx.load('clang_compilation_database', tooldir='wafscripts')
    ctx.load('gnu_dirs')
    ctx.check_cc(msg="Checking for C compiler", mandatory=True)
    ctx.check_cfg(package='x11', uselib_store='X11', args=['--cflags', '--libs'])
    ctx.check_cfg(package='xinerama', uselib_store='XINER', args=['--cflags', '--libs'])
    ctx.check_cfg(package='xft', uselib_store='XFT', args=['--cflags', '--libs'])
    ctx.check_cfg(package='fontconfig', uselib_store='FTCFG', args=['--cflags', '--libs'])
    ctx.check_cfg(package='gl', uselib_store='GLEW', args=['--cflags', '--libs'])
    # ctx.recurse('dwm')
    # ctx.recurse('dmenu')
    # ctx.recurse('slstatus/components')
    # ctx.recurse('slstatus')
    # ctx.recurse('fonts')
    # ctx.recurse('nuklear')
    # ctx.recurse('st')
    for target in targets:
        ctx.recurse(target)
        
def options(opt):
    opt.load("compiler_c")
    opt.load('compiler_cxx')
    opt.load('clang_compilation_database', tooldir='wafscripts')
    # opt.check_cc()
def build(ctx):
    # ctx.recurse('dwm')
    # ctx.recurse('dmenu')
    # ctx.recurse('slstatus/components')
    # ctx.recurse('slstatus')
    # ctx.recurse('fonts')
    # ctx.recurse('nuklear')
    for target in targets:
        ctx.recurse(target)
