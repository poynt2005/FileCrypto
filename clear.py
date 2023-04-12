import glob
import shutil
import os

f = None
if not os.path.exists('.gitignore'):
    f = open('.gitignore', 'w')


bin_file_exts = [
    'dll',
    'exp',
    'obj',
    'o',
    'exe',
    'zip',
    'node',
    'key'
]

for ext in bin_file_exts:
    if not f is None:
        f.write('*.%s\n' % ext)
    globbed_files = glob.glob('**/*.%s' % ext, recursive=True)
    for file in globbed_files:
        os.remove(file)

if not f is None:
    f.write('node_modules/\n')
    f.write('cryptocpp/\n')


for node_module in glob.glob('**/*/node_modules', recursive=True):
    shutil.rmtree(node_module)

if not f is None:
    f.close()
