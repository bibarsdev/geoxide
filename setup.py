# This script is only responible for downloading third-party dependencies for now.

import sys
import os
import shutil
import subprocess

def status(msg):
	print(msg)
	sys.stdout.flush() # display the message immediately

def git_clone_repo(name, url):
	source_directory = 'thirdparty/' + name
	if not os.path.isdir(source_directory):
		subprocess.call(['git', 'clone', '--quiet', url, source_directory])

status('Downloading third-party dependencies...')

# Clone git repositories
git_clone_repo('SDL2', 'https://github.com/libsdl-org/SDL.git')
git_clone_repo('assimp', 'https://github.com/assimp/assimp.git')

