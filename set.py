import os
import subprocess

sb = subprocess.Popen(['npm', 'i', '--ignore-scripts'],
                      shell=True, cwd=os.path.realpath('./node'))
sb.wait()

sb = subprocess.Popen(['npm', 'run', 'build-native'],
                      shell=True, cwd=os.path.realpath('./node'))
sb.wait()

sb = subprocess.Popen(['npm', 'run', 'build-downloader'],
                      shell=True, cwd=os.path.realpath('./node'))
sb.wait()
