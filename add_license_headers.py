#!/usr/bin/env python3
"""
Script to add license headers to source files.
"""

import os
import sys
import argparse

# License header template based on the BSD 3-Clause License
LICENSE_HEADER = """/*
 * BSD 3-Clause License
 *
 * Copyright (c) 2025, Ashish Vasant Yesale
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

"""

def has_license_header(file_path):
    """Check if a file already has a license header."""
    with open(file_path, 'r') as f:
        content = f.read(500)  # Read first 500 chars to check for header
        return "Copyright" in content and "BSD 3-Clause License" in content

def add_license_header(file_path):
    """Add license header to a file if it doesn't have one."""
    if has_license_header(file_path):
        return False
    
    with open(file_path, 'r') as f:
        content = f.read()
    
    with open(file_path, 'w') as f:
        f.write(LICENSE_HEADER + content)
    
    return True

def find_source_files(directory):
    """Find all source files that should have license headers."""
    source_files = []
    for root, _, files in os.walk(directory):
        for file in files:
            if file.endswith(('.c', '.h', '.cpp', '.hpp', '.cc', '.S')):
                source_files.append(os.path.join(root, file))
    return source_files

def main():
    parser = argparse.ArgumentParser(description='Add license headers to source files')
    parser.add_argument('--check-only', action='store_true', help='Only check for missing headers without adding them')
    args = parser.parse_args()
    
    source_files = find_source_files('.')
    missing_headers = []
    
    for file_path in source_files:
        if not has_license_header(file_path):
            missing_headers.append(file_path)
            if not args.check_only:
                add_license_header(file_path)
    
    if missing_headers:
        print(f"Found {len(missing_headers)} files missing license headers:")
        for file in missing_headers:
            print(f"- {file}")
        
        if args.check_only:
            sys.exit(1)
        else:
            print(f"Added license headers to {len(missing_headers)} files.")
    else:
        print("All source files have license headers.")

if __name__ == "__main__":
    main()