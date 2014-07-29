#coding:utf8
#!/usr/bin/python

import sys
import os
import getopt
import shutil

def help():
    print '#####################################################'
    print '# Usage of png compress tool'
    print '# pngc [options]'
    print '# Options:'
    print '# [-s|--source] folder'
    print '#     source directory in which all png files in it will be compressed'
    print '# [-o|--output] folder'
    print '#     output root directory, compressed png file will be saved here, keep the same folder'
    print '#     hierarchy with source. If not set, current directory will be used'
    print '# [-f|--force]'
    print '#     if set, same files in output directory will be overridden'
    print '# [-m|--mirror]'
    print '#     if set, non-png file will be copied to output directory'
    print '# [-h|--help]'
    print '#     show command usage, or just don\'t specify any arguments'

def compress(src, out):
    # get output path
    srcBaseName = os.path.basename(src)
    outPath = out + '/' + srcBaseName

    # if src is directory, create same directory in output and enter it
    # if src is file and it is png, compress
    # if src is not png, skip or copy it depends on mirror option
    if os.path.isdir(src):
        # create output dir
        if not os.path.exists(outPath):
            os.makedirs(outPath)

        # recursively visit source
        files = os.listdir(src)
        for f in files:
            compress(os.path.join(src, f), outPath)
    else:
        if os.path.splitext(src)[1] == '.png':
            print 'processing file "%s"...' % src
            cmd = './pngquant %s "%s" --output "%s"' % (arg_force is True and '--force' or '', src, outPath)
            os.system(cmd)
        else:
            if arg_mirror is True and (not os.path.exists(outPath) or arg_force is True):
                shutil.copy(src, outPath)
            else:
                print 'skip non-png file "%s"' % src

# options process
shortOpts = 's:o:mfh'
longOpts = ['source=', 'output=', 'mirror', 'force', 'help']
opts, args = getopt.getopt(sys.argv[1:], shortOpts, longOpts)

# if no argument, help
if len(sys.argv) <= 1:
    help()
    sys.exit()

# parse arguments
arg_source = None
arg_output = None
arg_force = False
arg_mirror = False
for k, v in opts:
    if k in ('-s', '--source'):
        arg_source = v
    elif k in ('-o', '--output'):
        arg_output = v
    elif k in ('-f', '--force'):
        arg_force = True
    elif k in ('-m', '--mirror'):
        arg_mirror = True
    elif k in ('-h', '--help'):
        help()
        sys.exit()

# check source, get source folder name
if arg_source is None:
    print 'must specify source directory'
    sys.exit()
if arg_source[-1] == '/' and len(arg_source) > 1:
    arg_source = arg_source[:-1]
arg_source = os.path.expanduser(arg_source)
sourceBaseName = os.path.basename(arg_source)

# check output
if arg_output is None:
    arg_output = '.'
arg_output = os.path.expanduser(arg_output)

# start compress
print 'source dir: %s' % arg_source
print 'output dir: %s' % arg_output
print 'start compressing...'
compress(arg_source, arg_output)
print 'done'