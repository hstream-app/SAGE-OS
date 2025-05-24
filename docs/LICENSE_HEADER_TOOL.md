<!--
─────────────────────────────────────────────────────────────────────────────
SAGE OS — Copyright (c) 2025 Ashish Vasant Yesale (ashishyesale007@gmail.com)
SPDX-License-Identifier: BSD-3-Clause OR Proprietary
SAGE OS is dual-licensed under the BSD 3-Clause License and a Commercial License.

This file is part of the SAGE OS Project.
─────────────────────────────────────────────────────────────────────────────
-->
# Enhanced License Header Management Tool

## Overview

The Enhanced License Header Management Tool is a comprehensive Python script designed to automatically add license headers to source files across multiple programming languages and file formats used in the SAGE OS project.

## Features

### 🚀 **Comprehensive Language Support**
- **50+ Programming Languages**: C/C++, Python, JavaScript, Rust, Go, Java, C#, Swift, and many more
- **Multiple File Formats**: Source code, configuration files, documentation, scripts, and data files
- **Special File Handling**: Makefiles, Dockerfiles, configuration files without extensions

### 🎨 **Multiple Comment Styles**
- **C-style**: `/* ... */` (C, C++, CSS)
- **C++ style**: `// ...` (JavaScript, TypeScript, Java, C#)
- **Hash style**: `# ...` (Python, Shell, YAML, TOML)
- **HTML style**: `<!-- ... -->` (HTML, XML, Markdown)
- **SQL style**: `-- ...` (SQL, Ada)
- **And many more**: Lua, MATLAB, LaTeX, Vim, Lisp, Haskell, etc.

### 🛡️ **Smart Detection**
- **Existing Headers**: Automatically detects and skips files that already have license headers
- **Binary Files**: Identifies and skips binary files automatically
- **Shebang Preservation**: Maintains shebang lines in scripts
- **Special Declarations**: Preserves XML declarations, DOCTYPE declarations

### ⚙️ **Advanced Configuration**
- **Flexible Exclusions**: Configurable file and directory exclusion patterns
- **Template System**: Customizable license templates for each comment style
- **Dry Run Mode**: Preview changes before applying them
- **Verbose Logging**: Detailed logging for debugging and monitoring

## Supported File Types

### Programming Languages

| Language | Extensions | Comment Style |
|----------|------------|---------------|
| **C/C++** | `.c`, `.h`, `.cpp`, `.hpp`, `.cc`, `.cxx` | C-style block |
| **JavaScript/TypeScript** | `.js`, `.jsx`, `.ts`, `.tsx`, `.mjs` | C++ style line |
| **Python** | `.py`, `.pyx`, `.pyi`, `.pyw` | Hash style |
| **Rust** | `.rs` | C++ style line |
| **Go** | `.go` | C++ style line |
| **Java** | `.java`, `.scala`, `.kt`, `.groovy` | C++ style line |
| **C#/.NET** | `.cs`, `.fs`, `.vb` | C++ style line |
| **Swift** | `.swift` | C++ style line |
| **PHP** | `.php`, `.php3`, `.php4`, `.php5` | C++ style line |
| **Ruby** | `.rb`, `.rake`, `.gemspec` | Hash style |
| **Perl** | `.pl`, `.pm`, `.pod` | Hash style |
| **Shell** | `.sh`, `.bash`, `.zsh`, `.fish` | Hash style |
| **SQL** | `.sql`, `.mysql`, `.pgsql` | SQL style |
| **R** | `.r`, `.R`, `.rmd` | Hash style |
| **MATLAB** | `.m`, `.mat`, `.fig` | MATLAB style |
| **Lua** | `.lua` | Lua style |
| **Haskell** | `.hs`, `.lhs` | Haskell style |
| **Erlang/Elixir** | `.erl`, `.hrl`, `.ex`, `.exs` | Erlang style |
| **Lisp** | `.lisp`, `.cl`, `.el`, `.scm` | Lisp style |
| **Fortran** | `.f`, `.f90`, `.f95`, `.for` | Fortran style |
| **Ada** | `.ada`, `.adb`, `.ads` | Ada style |
| **Pascal** | `.pas`, `.pp` | Pascal style |
| **Assembly** | `.s`, `.S`, `.asm`, `.nasm` | Hash/C++ style |

### Web Technologies

| Technology | Extensions | Comment Style |
|------------|------------|---------------|
| **HTML** | `.html`, `.htm`, `.xhtml` | HTML style |
| **XML** | `.xml`, `.svg`, `.xsl`, `.xaml` | HTML style |
| **CSS** | `.css`, `.scss`, `.sass`, `.less` | C-style/C++ style |
| **Vue/Svelte** | `.vue`, `.svelte` | HTML style |

### Configuration & Data

| Type | Extensions | Comment Style |
|------|------------|---------------|
| **YAML** | `.yaml`, `.yml` | Hash style |
| **TOML** | `.toml` | Hash style |
| **JSON** | `.json`, `.json5`, `.jsonc` | C++ style |
| **INI** | `.ini`, `.conf`, `.cfg` | Hash style |
| **Properties** | `.properties` | Hash style |

### Documentation

| Type | Extensions | Comment Style |
|------|------------|---------------|
| **Markdown** | `.md`, `.markdown`, `.mdown` | Markdown style |
| **reStructuredText** | `.rst` | Hash style |
| **LaTeX** | `.tex`, `.latex`, `.sty`, `.cls` | LaTeX style |
| **Plain Text** | `.txt`, `.text`, `.rtf` | Text style |

### Build Systems

| System | Files | Comment Style |
|--------|-------|---------------|
| **Make** | `Makefile`, `*.mk`, `*.make` | Hash style |
| **CMake** | `CMakeLists.txt`, `*.cmake` | Hash style |
| **Docker** | `Dockerfile`, `*.dockerfile` | Hash style |
| **Autotools** | `*.am`, `*.ac`, `*.in` | Hash style |

### Special Files

| File | Comment Style |
|------|---------------|
| `Dockerfile` | Hash style |
| `Vagrantfile` | Hash style |
| `Jenkinsfile` | Hash style |
| `Gemfile` | Hash style |
| `Rakefile` | Hash style |
| `.gitignore` | Hash style |
| `.eslintrc` | Hash style |
| `.prettierrc` | Hash style |

## Usage

### Basic Usage

```bash
# Add license headers to all files recursively
python3 enhanced_license_headers.py --recursive

# Check for missing headers without modifying files
python3 enhanced_license_headers.py --check-only

# Preview changes without applying them
python3 enhanced_license_headers.py --dry-run --verbose

# Process specific directory
python3 enhanced_license_headers.py --dir src --recursive
```

### Advanced Options

```bash
# Use custom template directory
python3 enhanced_license_headers.py --template-dir custom/templates

# Show detailed statistics
python3 enhanced_license_headers.py --stats --verbose

# List all supported file types
python3 enhanced_license_headers.py --list-supported

# Quiet mode (errors only)
python3 enhanced_license_headers.py --quiet
```

### Command Line Options

| Option | Description |
|--------|-------------|
| `--dir`, `-d` | Directory to process (default: current directory) |
| `--recursive`, `-r` | Process directories recursively (default: True) |
| `--template-dir`, `-t` | Custom template directory |
| `--check-only` | Only check for missing headers |
| `--dry-run` | Preview changes without modifying files |
| `--verbose`, `-v` | Enable verbose output |
| `--quiet`, `-q` | Suppress all output except errors |
| `--stats` | Show detailed statistics |
| `--list-supported` | List all supported file types |

## Template System

### Template Directory Structure

```
.github/license-templates/
├── c-style.txt          # /* ... */ comments
├── cpp-style.txt        # // ... comments
├── hash-style.txt       # # ... comments
├── html-style.txt       # <!-- ... --> comments
├── sql-style.txt        # -- ... comments
├── lua-style.txt        # --[[ ... --]] comments
├── matlab-style.txt     # % ... comments
├── latex-style.txt      # % ... comments
├── vim-style.txt        # " ... comments
├── lisp-style.txt       # ;; ... comments
├── haskell-style.txt    # {- ... -} comments
├── erlang-style.txt     # % ... comments
├── fortran-style.txt    # ! ... comments
├── ada-style.txt        # -- ... comments
├── pascal-style.txt     # (* ... *) comments
├── batch-style.txt      # REM ... comments
├── powershell-style.txt # <# ... #> comments
├── markdown-style.txt   # <!-- ... --> comments
└── text-style.txt      # Plain text
```

### Creating Custom Templates

Each template file contains the license header formatted for that comment style:

**Example: `cpp-style.txt`**
```cpp
// ─────────────────────────────────────────────────────────────────────────────
// SAGE OS — Copyright (c) 2025 Ashish Vasant Yesale (ashishyesale007@gmail.com)
// SPDX-License-Identifier: BSD-3-Clause OR Proprietary
// SAGE OS is dual-licensed under the BSD 3-Clause License and a Commercial License.
// 
// This file is part of the SAGE OS Project.
// ─────────────────────────────────────────────────────────────────────────────
```

**Example: `html-style.txt`**
```html
<!--
─────────────────────────────────────────────────────────────────────────────
SAGE OS — Copyright (c) 2025 Ashish Vasant Yesale (ashishyesale007@gmail.com)
SPDX-License-Identifier: BSD-3-Clause OR Proprietary
SAGE OS is dual-licensed under the BSD 3-Clause License and a Commercial License.

This file is part of the SAGE OS Project.
─────────────────────────────────────────────────────────────────────────────
-->
```

## Configuration

### Exclusion Patterns

The tool automatically excludes:

**Directories:**
- `.git`, `.github`, `.svn`, `.hg`
- `build`, `bin`, `obj`, `out`, `dist`, `target`
- `node_modules`, `vendor`, `third_party`
- `venv`, `env`, `__pycache__`
- IDE directories (`.idea`, `.vscode`, etc.)

**Files:**
- License files (`LICENSE`, `LICENSE.txt`, etc.)
- Documentation (`README.md`, `CHANGELOG.md`, etc.)
- Lock files (`package-lock.json`, `Cargo.lock`, etc.)
- Binary files (automatically detected)
- Minified files (`*.min.js`, `*.min.css`)

### Customization

You can customize the exclusion patterns by modifying the `EXCLUDE_DIRS` and `EXCLUDE_FILES` sets in the script.

## Integration

### GitHub Actions

Create `.github/workflows/license-headers.yml`:

```yaml
name: License Headers

on:
  push:
    branches: [ main, develop ]
  pull_request:
    branches: [ main ]

jobs:
  check-license-headers:
    runs-on: ubuntu-latest
    steps:
    - uses: actions/checkout@v3
    
    - name: Set up Python
      uses: actions/setup-python@v4
      with:
        python-version: '3.9'
    
    - name: Check license headers
      run: |
        python3 enhanced_license_headers.py --check-only --verbose
    
    - name: Add missing license headers
      if: failure()
      run: |
        python3 enhanced_license_headers.py --recursive
        git config --local user.email "action@github.com"
        git config --local user.name "GitHub Action"
        git add .
        git diff --staged --quiet || git commit -m "Add missing license headers [automated]"
        git push
```

### Pre-commit Hook

Add to `.pre-commit-config.yaml`:

```yaml
repos:
  - repo: local
    hooks:
      - id: license-headers
        name: License Headers
        entry: python3 enhanced_license_headers.py --check-only
        language: system
        pass_filenames: false
        always_run: true
```

## Examples

### Example Output

```bash
$ python3 enhanced_license_headers.py --dry-run --stats

2025-05-24 10:30:15 - INFO - Processing files in .
2025-05-24 10:30:15 - INFO - Would add license header to src/main.c (c-style)
2025-05-24 10:30:15 - INFO - Would add license header to scripts/build.sh (hash-style)
2025-05-24 10:30:15 - INFO - Would add license header to docs/api.md (markdown-style)
2025-05-24 10:30:15 - INFO - Would modify 3 files

Processing Statistics:
  Files processed: 45
  Files modified: 0
  Files skipped: 42
  Errors: 0
  By comment style:
    c-style: 1
    hash-style: 1
    markdown-style: 1

Processed files:
  - src/main.c
  - scripts/build.sh
  - docs/api.md
```

### Checking Specific File Types

```bash
# List all supported extensions
python3 enhanced_license_headers.py --list-supported | grep -A5 "C-STYLE"

# Check only Python files
find . -name "*.py" -exec python3 enhanced_license_headers.py --check-only {} \;
```

## Troubleshooting

### Common Issues

1. **Binary File Detection**: If text files are being detected as binary, check for null bytes or encoding issues.

2. **Template Not Found**: Ensure all template files exist in the template directory.

3. **Permission Errors**: Make sure the script has write permissions to the target files.

4. **Encoding Issues**: The script uses UTF-8 encoding by default. Files with different encodings may cause issues.

### Debug Mode

Use `--verbose` flag for detailed debugging information:

```bash
python3 enhanced_license_headers.py --verbose --dry-run
```

## Contributing

To add support for new file types:

1. Add the file extension to `FILE_EXTENSIONS` dictionary
2. Create or reference an appropriate comment style in `COMMENT_STYLES`
3. Add a template file if using a new comment style
4. Test with various file samples

## License

This tool is part of the SAGE OS project and is licensed under the same terms as the main project.