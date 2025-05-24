#!/bin/bash

# Create test files for all supported comment styles
mkdir -p test_all_formats

# C-style comments
echo '#include <stdio.h>
int main() { return 0; }' > test_all_formats/test.c

echo '#ifndef TEST_H
#define TEST_H
#endif' > test_all_formats/test.h

# C++ style comments  
echo '#include <iostream>
int main() { return 0; }' > test_all_formats/test.cpp

echo 'function test() { console.log("test"); }' > test_all_formats/test.js

# Hash-style comments
echo 'def hello():
    print("Hello")' > test_all_formats/test.py

echo 'name: test
version: 1.0' > test_all_formats/test.yaml

echo '#!/bin/bash
echo "Hello"' > test_all_formats/test.sh

# HTML-style comments
echo '<!DOCTYPE html>
<html><body>Test</body></html>' > test_all_formats/test.html

echo '<?xml version="1.0"?>
<root>test</root>' > test_all_formats/test.xml

# SQL-style comments
echo 'SELECT * FROM users;' > test_all_formats/test.sql

# Lua-style comments
echo 'print("Hello from Lua")' > test_all_formats/test.lua

# MATLAB-style comments
echo 'disp("Hello MATLAB")' > test_all_formats/test.m

# LaTeX-style comments
echo '\documentclass{article}
\begin{document}
Hello LaTeX
\end{document}' > test_all_formats/test.tex

# Vim-style comments
echo 'set number
syntax on' > test_all_formats/test.vim

# Lisp-style comments
echo '(defun hello ()
  (print "Hello Lisp"))' > test_all_formats/test.lisp

# Haskell-style comments
echo 'main = putStrLn "Hello Haskell"' > test_all_formats/test.hs

# Erlang-style comments
echo '-module(test).
-export([hello/0]).
hello() -> "Hello Erlang".' > test_all_formats/test.erl

# Fortran-style comments
echo 'program hello
  print *, "Hello Fortran"
end program' > test_all_formats/test.f90

# Ada-style comments
echo 'with Ada.Text_IO;
procedure Hello is
begin
  Ada.Text_IO.Put_Line("Hello Ada");
end Hello;' > test_all_formats/test.ada

# Pascal-style comments
echo 'program Hello;
begin
  writeln("Hello Pascal");
end.' > test_all_formats/test.pas

# Batch-style comments
echo '@echo off
echo Hello Batch' > test_all_formats/test.bat

# PowerShell-style comments
echo 'Write-Host "Hello PowerShell"' > test_all_formats/test.ps1

# Markdown-style comments
echo '# Test Markdown
This is a test.' > test_all_formats/test.md

# Text-style (no comments)
echo 'This is plain text
with multiple lines.' > test_all_formats/test.txt

echo "Created test files for all supported comment styles!"