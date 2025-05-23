#!/usr/bin/env python3
# ─────────────────────────────────────────────────────────────────────────────
# SAGE OS — Copyright (c) 2025 Ashish Vasant Yesale (ashishyesale007@gmail.com)
# SPDX-License-Identifier: BSD-3-Clause OR Proprietary
# SAGE OS is dual-licensed under the BSD 3-Clause License and a Commercial License.
# 
# This file is part of the SAGE OS Project.
# ─────────────────────────────────────────────────────────────────────────────

import os
import re
import sys
import argparse
from pathlib import Path

# File extensions and their comment styles
COMMENT_STYLES = {
    # C-style comments
    '.c': 'c-style',
    '.h': 'c-style',
    '.cpp': 'c-style',
    '.hpp': 'c-style',
    '.cc': 'c-style',
    '.cxx': 'c-style',
    '.rs': 'c-style',
    '.java': 'c-style',
    '.js': 'c-style',
    '.jsx': 'c-style',
    '.ts': 'c-style',
    '.tsx': 'c-style',
    '.css': 'c-style',
    '.scss': 'c-style',
    '.swift': 'c-style',
    '.go': 'c-style',
    '.kt': 'c-style',
    '.kts': 'c-style',
    '.scala': 'c-style',
    '.S': 'c-style',  # Assembly
    '.asm': 'c-style',
    
    # Hash-style comments
    '.py': 'hash-style',
    '.sh': 'hash-style',
    '.bash': 'hash-style',
    '.zsh': 'hash-style',
    '.rb': 'hash-style',
    '.pl': 'hash-style',
    '.pm': 'hash-style',
    '.tcl': 'hash-style',
    '.yaml': 'hash-style',
    '.yml': 'hash-style',
    '.toml': 'hash-style',
    '.mk': 'hash-style',
    '.Makefile': 'hash-style',
    '.makefile': 'hash-style',
    
    # HTML/XML-style comments
    '.html': 'html-style',
    '.htm': 'html-style',
    '.xml': 'html-style',
    '.svg': 'html-style',
    '.md': 'html-style',
    '.markdown': 'html-style',
    '.rst': 'html-style',
}

# Directories to exclude
EXCLUDE_DIRS = [
    '.git',
    '.github',
    'build',
    'bin',
    'obj',
    'node_modules',
    'vendor',
    'third_party',
    'external',
    'deps',
]

# Files to exclude
EXCLUDE_FILES = [
    'LICENSE',
    'README.md',
    'CONTRIBUTING.md',
    'CHANGELOG.md',
    'COMMERCIAL_TERMS.md',
    '.gitignore',
    '.gitattributes',
]

# Patterns to detect existing license headers
LICENSE_PATTERNS = [
    r'Copyright.*Ashish',
    r'SAGE OS',
    r'BSD-3-Clause',
    r'SPDX-License-Identifier',
]

def has_license_header(file_path):
    """Check if the file already has a license header."""
    with open(file_path, 'r', encoding='utf-8', errors='ignore') as f:
        try:
            content = f.read(2000)  # Read first 2000 chars to check for header
            for pattern in LICENSE_PATTERNS:
                if re.search(pattern, content):
                    return True
            return False
        except UnicodeDecodeError:
            # Binary file or non-text file
            return True  # Skip binary files

def get_comment_style(file_path):
    """Determine the comment style based on file extension."""
    ext = os.path.splitext(file_path)[1].lower()
    
    # Special case for Makefile without extension
    if os.path.basename(file_path).lower() in ['makefile']:
        return 'hash-style'
        
    return COMMENT_STYLES.get(ext)

def add_license_header(file_path, template_path):
    """Add license header to the file."""
    try:
        with open(template_path, 'r', encoding='utf-8') as template_file:
            header = template_file.read()
            
        with open(file_path, 'r', encoding='utf-8', errors='ignore') as f:
            content = f.read()
            
        # Check for shebang line
        if content.startswith('#!'):
            shebang_end = content.find('\n') + 1
            new_content = content[:shebang_end] + '\n' + header + content[shebang_end:]
        else:
            new_content = header + content
            
        with open(file_path, 'w', encoding='utf-8') as f:
            f.write(new_content)
            
        print(f"Added license header to {file_path}")
        return True
    except Exception as e:
        print(f"Error adding license to {file_path}: {e}")
        return False

def process_files(directory='.', recursive=True):
    """Process all files in the specified directory."""
    repo_root = os.path.abspath(directory)
    template_dir = os.path.join(repo_root, '.github', 'license-templates')
    
    if not os.path.exists(template_dir):
        print(f"Error: Template directory not found at {template_dir}")
        return []
    
    modified_files = []
    
    if recursive:
        # Process all files recursively
        for root, dirs, files in os.walk(repo_root):
            # Skip excluded directories
            dirs[:] = [d for d in dirs if d not in EXCLUDE_DIRS and not d.startswith('.')]
            
            for file in files:
                file_path = os.path.join(root, file)
                rel_path = os.path.relpath(file_path, repo_root)
                
                # Skip excluded files
                if os.path.basename(file_path) in EXCLUDE_FILES:
                    continue
                    
                # Skip files in excluded directories
                if any(exclude_dir in rel_path.split(os.sep) for exclude_dir in EXCLUDE_DIRS):
                    continue
                    
                # Get comment style
                comment_style = get_comment_style(file_path)
                if not comment_style:
                    continue  # Skip files with unknown comment style
                    
                # Check if file already has a license header
                if has_license_header(file_path):
                    continue
                    
                # Add license header
                template_path = os.path.join(template_dir, f"{comment_style}.txt")
                if add_license_header(file_path, template_path):
                    modified_files.append(rel_path)
    else:
        # Process only files in the specified directory (non-recursive)
        for file in os.listdir(repo_root):
            file_path = os.path.join(repo_root, file)
            
            # Skip directories
            if os.path.isdir(file_path):
                continue
                
            # Skip excluded files
            if os.path.basename(file_path) in EXCLUDE_FILES:
                continue
                
            # Get comment style
            comment_style = get_comment_style(file_path)
            if not comment_style:
                continue  # Skip files with unknown comment style
                
            # Check if file already has a license header
            if has_license_header(file_path):
                continue
                
            # Add license header
            template_path = os.path.join(template_dir, f"{comment_style}.txt")
            if add_license_header(file_path, template_path):
                modified_files.append(os.path.relpath(file_path, repo_root))
    
    return modified_files

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Add license headers to source files')
    parser.add_argument('--dir', '-d', default='.', help='Directory to process')
    parser.add_argument('--recursive', '-r', action='store_true', help='Process directories recursively')
    
    args = parser.parse_args()
    
    modified_files = process_files(args.dir, args.recursive)
    
    print(f"Added license headers to {len(modified_files)} files")
    
    if modified_files:
        print("\nModified files:")
        for file in modified_files:
            print(f"  - {file}")