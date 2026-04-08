#!/usr/bin/env python3
# Python 3 compatible version of download-deps.py for cocos2d-x 3.17.2
import os
import json
import shutil
import zipfile
import urllib.request

COCOS_ROOT = os.path.dirname(os.path.abspath(__file__)) + '/cocos2d-x'
CONFIG_PATH = os.path.join(COCOS_ROOT, 'external/config.json')
EXTERNAL_PATH = os.path.join(COCOS_ROOT, 'external')
DOWNLOAD_DIR = COCOS_ROOT  # download zip here

with open(CONFIG_PATH) as f:
    data = json.load(f)

version = data['version']          # e.g. "v3-deps-158"
repo_name = data['repo_name']      # "cocos2d-x-3rd-party-libs-bin"
repo_parent = data['repo_parent']  # "https://github.com/cocos2d/"

filename = version + '.zip'
url = repo_parent + repo_name + '/archive/' + filename
zip_path = os.path.join(DOWNLOAD_DIR, filename)
# GitHub strips the leading 'v' from archive folder names
extracted_name = repo_name + '-' + version[1:]  # e.g. "cocos2d-x-3rd-party-libs-bin-3-deps-158"
extracted_path = os.path.join(DOWNLOAD_DIR, extracted_name)

print(f"==> URL: {url}")
print(f"==> Zip size: ~{int(data['zip_file_size'])//1024//1024} MB")

# Download with progress
if not os.path.isfile(zip_path):
    print("==> Downloading... (this may take several minutes)")
    def reporthook(count, block_size, total_size):
        if total_size > 0:
            percent = min(int(count * block_size * 100 / total_size), 100)
            mb_done = count * block_size / 1024 / 1024
            mb_total = total_size / 1024 / 1024
            print(f"\r    {percent}% ({mb_done:.1f}/{mb_total:.1f} MB)", end='', flush=True)
    urllib.request.urlretrieve(url, zip_path, reporthook)
    print("\n==> Download complete!")
else:
    print(f"==> Zip already exists: {zip_path}")

# Extract
if os.path.exists(extracted_path):
    shutil.rmtree(extracted_path)

print("==> Extracting...")
with zipfile.ZipFile(zip_path) as z:
    z.extractall(DOWNLOAD_DIR)
print("==> Extraction done!")

# Copy to external/ (keep config.json)
print("==> Copying to external/...")
for item in os.listdir(extracted_path):
    src = os.path.join(extracted_path, item)
    dst = os.path.join(EXTERNAL_PATH, item)
    if os.path.isdir(src):
        if os.path.exists(dst):
            shutil.rmtree(dst)
        shutil.copytree(src, dst)
    else:
        shutil.copy2(src, dst)

# Cleanup
shutil.rmtree(extracted_path)
print("==> Done! External dependencies installed.")
