(cpplint)[https://github.com/cpplint/cpplint]   

## INDEX
- [CPPLINT](#cpplint)
	- [INSTALL](#install)
	- [HOW TO USE](#how-to-use)
		- [COMMAND](#command)
		- [VSCODE EXTENSION](#vscode-extension)

---


# CPPLINT



Cpplint is a command-line tool to check C/C++ files for style issues according to (Google's C++ style guide)[https://google.github.io/styleguide/cppguide.html].   

## INSTALL   

1. Install brew    

```bash
cd goinfre
mkdir brew
export HOMEBREW_PREFIX=~/goinfre/brew
export HOMEBREW_REPOSITORY=~/goinfre/brew/Homebrew


git clone https://github.com/Homebrew/brew.git ~/goinfre/brew/Homebrew
echo 'export PATH=~/goinfre/brew/Homebrew/bin:$PATH' >> ~/.bash_profile
source ~/.bash_profile
```    

2. Install cpplint by python virtual environmet   

```bash
cd ~/goinfre
python3 -m venv ./venv
. ./venv/bin/activate
pip3 install cpplint
deactivate
./venv/bin/cpplint 
cd ~

echo $SHELL
echo 'alias cpplint="~/goinfre/venv/bin/cpplint"' >> ./.zshrc
```   
   
## HOW TO USE   

### COMMAND     

```bash

```   

### VSCODE EXTENSION   

This [extension](https://marketplace.visualstudio.com/items?itemName=mine.cpplint) check coding style of cpp and c, when open and save source file   









