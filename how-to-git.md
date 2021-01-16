
Table of Contents
=================

* [HowTos](#howtos)
  * [Bash](#bash)
    * [Fix horribly slow GitBash](#fix-horribly-slow-gitbash)
    * [Drop history duplicates](#drop-history-duplicates)
    * [Share history among <em>Git Bash Here</em> sessions](#share-history-among-git-bash-here-sessions)
    * [Find files sorted by timestamp](#find-files-sorted-by-timestamp)
    * [Pick pipeline items to operate on](#pick-pipeline-items-to-operate-on)
  * [C and C\+\+](#c-and-c)
    * [List all headers included by code](#list-all-headers-included-by-code)
  * [Git](#git)
    * [Install/Setup Git &amp; Maven, on Windows](#installsetup-git--maven-on-windows)
    * [Git Quick diff in console](#git-quick-diff-in-console)
    * [Git amend the very last commit](#git-amend-the-very-last-commit)
    * [Git discard uncommitted changes](#git-discard-uncommitted-changes)
    * [Git config needing admin rights](#git-config-needing-admin-rights)
    * [Git forgets Github credentials](#git-forgets-github-credentials)
    * [Git Quick diff in console](#git-quick-diff-in-console-1)
    * [Git Branches](#git-branches)
      * [Git list remote branches](#git-list-remote-branches)
      * [Git Show branch history graph](#git-show-branch-history-graph)
      * [Create and checkout feature branch off develop](#create-and-checkout-feature-branch-off-develop)
      * [Git list existing branches](#git-list-existing-branches)
      * [Git delete feature branch locally](#git-delete-feature-branch-locally)
      * [Git delete a remote branch (needs git v1\.7 or more)](#git-delete-a-remote-branch-needs-git-v17-or-more)
  * [Java](#java)
    * [Set log level (<em>tedious</em>)](#set-log-level-tedious)
    * [Set log level (<em>SpringBoot</em>)](#set-log-level-springboot)
    * [swagger url](#swagger-url)
  * [Markdown](#markdown)
    * [line breaks with 2 spaces on end](#line-breaks-with-2-spaces-on-end)
    * [generate table of contents](#generate-table-of-contents)
  * [MingW Bash](#mingw-bash)
    * [Add chmod execute permission](#add-chmod-execute-permission)
  * [Notepad\+\+](#notepad)
    * [Quickly insert date or time in Npp](#quickly-insert-date-or-time-in-npp)
  * [PlantUML](#plantuml)
    * [Markup and multiline notes](#markup-and-multiline-notes)
  * [RabbitMQ](#rabbitmq)
    * [Enable RabbitMQ Tracing](#enable-rabbitmq-tracing)
  * [VSCode](#vscode)
    * [Customise keyboard shortcuts](#customise-keyboard-shortcuts)
    * [Select a block of text](#select-a-block-of-text)
  * [Windows](#windows)
    * [<em>certutil</em> to checksum a downloaded file](#certutil-to-checksum-a-downloaded-file)
    * [<em>win10</em> no <strong>Insert</strong> key on HP Elitebook](#win10-no-insert-key-on-hp-elitebook)
    * [<em>New folder</em> button in <em>Win10</em> File Explorer](#new-folder-button-in-win10-file-explorer)
    * [<em>Open cmd here</em> trick <em>win10</em>](#open-cmd-here-trick-win10)
    * [Bypass hibernate restriction <em>win10</em>](#bypass-hibernate-restriction-win10)
    * [Install node\.js on windows](#install-nodejs-on-windows)
  * [Word](#word)
    * [Quickly insert date or time](#quickly-insert-date-or-time)
    * [Keyboard shortcut for symbol](#keyboard-shortcut-for-symbol)
    * [Keyboard shortcut for command](#keyboard-shortcut-for-command)

Created by [gh-md-toc](https://github.com/ekalinin/github-markdown-toc.go)



# HowTos

## Bash

### Fix horribly slow GitBash
It takes seconds for `vi` to open a file for editing or to `cd` to a directory ?
I fixed this on Windows10 x64 simply by ***running gitbash once*** as administrator.
No idea why it's sticky but it's good that I don't have to run it as admin every time.

### Drop history duplicates
**Temporarily**, in current bash session
```
export HISTCONTROL=erasedups
```
**Permanently**
Add this to ~/.bashrc (credit: [Isaac](https://unix.stackexchange.com/a/452870/226568))
```
# prevents commands starting with space going into session's history
# erase previous duplicates from session history
HISTCONTROL=ignoreboth:erasedups

# write the current session history to the file at each prompt
PROMPT_COMMAND='history -w'

# clean duplicates in the history file at start of session
nl "$HISTFILE"|sort -k2 -k 1,1nr|uniq -f1|sort -n|cut -f2- >"${HISTFILE}.temp" && mv "$HISTFILE"{.temp,}
history -c; history -r
```

### Share history among _Git Bash Here_ sessions
Quick way  (credit: [M. Attia](https://stackoverflow.com/a/60722792/7409029)) ..
```
echo "PROMPT_COMMAND='history -a'" >> ~/.bash_profile
echo "PROMPT_COMMAND='history -a'" >> ~/.bashrc
```

### Find files sorted by timestamp
What files have changed recently? Can use `-printf` action in `find` command then sort the output e.g.
```
$ find . -name \*.json -anewer foo.txt -printf "%Ay%Am%Ad %p\n" | sort -nr | head -20
```
For an accurate timestamp you can use `-printf "%AY%Am%Ad%AH%AM%AS %p\n"` but often you can get away with selecting only _files newer than a reference file_ (here: ```foo.txt```), and then just keeping the month/day parts for sorting with.

Credit: [David Conger-Eldeen](https://serverfault.com/a/803753/575308)

### Pick pipeline items to operate on

Described in [this stackoverflow question](https://unix.stackexchange.com/q/591160/226568)

Adapted from l0bo's answer:

```
#!/usr/bin/env bash

set -o errexit -o nounset -o noclobber
trap 'rm -rf "$WORKDIR"' EXIT
WORKDIR="$(mktemp --directory)"
TMPFILE="${WORKDIR}/input.txt"

cat > "$TMPFILE" # Save STDIN to file

echo 'Lines:' 1>/dev/tty
cat --number "$TMPFILE" 1>/dev/tty

IFS=' ' read -p 'Choices: ' input < /dev/tty
lines=($input)

sedArgs=$(printf '%sp;' "${lines[@]}"|sed 's/,/ /g;s/-/,/g;s/ p/p/g')
sed --quiet "$sedArgs" "$TMPFILE"
```

Handy for plugging into middle of a script that repeats a command in a bunch of directories, where you want to choose the directories at each run, e.g.:

```
#!/bin/bash
for d in `cat subdirs.txt | pick.sh`
do
    echo "------------- $d --------------"
    pushd $d
    git pull && mvn clean
    popd
done
popd
```

## C and C++
### List all headers included by code

The -M switch does this in Gnu:

> g++ -M -I$CPPUTEST_HOME/include IncludesMockSupport.c 2>&1 > MockSupportTransitiveIncludes.txt

## Git

### Install/Setup Git & Maven, on Windows

Summary: 
 - define JAVA_HOME e.g. c:\app\Java\jdk1.8.0_241
 - define MAVEN_HOME and M2_HOME in windows env.
 
 Possibly need in GitBash:
 > export JAVA_HOME=/c/app/Java/jdk1.8.0_241

### Git Quick diff in console
> git diff --word-diff=color <file>

### Git amend the very last commit

Say you goof up the commit message and hit return too soon:
>  git commit -m "Oops bad commit message" file1 file2 ...

Say you also forgot _file3_.. 
- You can add such changes to the last commit.  
- You can also fix the commit message  
Do it like this ***before you push the changes***:
```
 git add file3 #forgotten in last commit
 git commit --amend -m "Corrected commit message"
```
### Git discard uncommitted changes

Trick: _stash_ the changes then _drop_ that stash
> git stash && git stash drop

Canonical way to discard _all_ uncommitted changes in project:
> git reset --hard HEAD

Canonical way to discard changes in _one specific_ file:
> git checkout -- MyGoofedUpFile.java

### Git config needing admin rights
If git complains it's unable to lock a gitconfig file, try running Gitbash as admin

### Git forgets Github credentials
This worked on my win7 + git-for-windows combo:
> git config --global credential.helper wincred
 
### Git Quick diff in console
> git diff --word-diff=color <file>

### Git Branches

#### Git list remote branches
> git remote show origin

#### Git Show branch history graph
> git log --graph --all --oneline

#### Create and checkout feature branch off develop

Safe way:
```
 git checkout dev
 git branch myFeature
 git checkout myFeature
```
One-liner:
```
 git checkout -b myFeature dev 
```

#### Git list existing branches
```
#list which branches exist (locally?)
git branch --list
```

#### Git delete feature branch locally 
Can't delete local branch  while it is checked out, so first checkout a different branch e.g.:
```
git checkout develop
git branch -d feature/SEL-1433
```

#### Git delete a remote branch (needs git v1.7 or more)
> git push --delete origin feature/SEL-1433

## Java
### Set log level (_tedious_)
Can set logging level in package _com.foo.bar_ via a system property: `System.setProperty("logging.level.com.foo.bar", "DEBUG");`  
Doh.. Still don’t know which config file can inject system properties, it’s not application.properties..

### Set log level (_SpringBoot_)
In a SpringBoot application, can set logging preferences in _src/main/resources/application.properties_ e.g.:
```
logging.level.root=info
logging.level.org.springframework=warn
logging.level.com.erni=debug
```
However these settings might only take effect _after_ the SpringBoot app starts (e.g. after call to _SpringApplication.run()_)

### swagger url
Swagger docs will be at `/swagger-ui.html` (_fails_ without **.html** extension)    
It’s very cool now, allows ‘try it out’ etc!

## Markdown

### line breaks with 2 spaces on end
Just end a line with 2+ spaces to force a line break (like a <br> in HTML, but cleaner).
Might need to configure your editor to prevent it pruning the spaces!

### generate table of contents 
In Word the Navigation Map helps to quickly find HOWTO entries. This command in gitbash helps:
```
./gh-md-toc.exe https://raw.githubusercontent.com/fazl/how-to/master/how-to-git.md
```
It's not a collapsible nav tree but it helps a bit.
Have to download [gh-md-toc](https://github.com/ekalinin/github-markdown-toc.go) and unpack it.
> Note: The windows prebuilt executables lack the .exe extension, I just added it.

In the Azure wiki you can simply insert this tag at the top:
```
[[_TOC_]] 
```
That's a single underscore either side of TOC.

## MingW Bash
### Add chmod execute permission 
You can't actually use chmod to add execute permission to a shell script. 

Instead simply put a `shebang` at the start of the script, and it will automatically be treated as executable.

## Notepad++

### Quickly insert date or time in Npp
Try [this recipe](https://stackoverflow.com/a/38465954/7409029)

## PlantUML
### Markup and multiline notes
Try this:
```
note left
  This is **bold**
  This is //italics//
  This is ""monospaced""
  This is --striked out--
  This is __underlined__
  This is ~~waved~~
end note
```

## RabbitMQ
### Enable RabbitMQ Tracing  
Steps I took to enable rabbit mq tracing:  
- open “rabbitmq command prompt (sbin dir)” as admin
- set homedrive=c:
- manually copy .erlang.cookie file from C:\Windows\System32\config\systemprofile to C:\.
- run this in the rabbitmq console:  
```
C:\Program Files\RabbitMQ Server\rabbitmq_server-3.7.7\sbin>rabbitmqctl.bat trace_on
C:\Program Files\RabbitMQ Server\rabbitmq_server-3.7.7\sbin>rabbitmq-plugins.bat enable rabbitmq_tracing
```

## VSCode

### Customise keyboard shortcuts
Via menu ```File -> Preferences -> Keyboard Shortcuts```, I searched for _lower_ and _upper_ to find and map the functions that convert to upper-/lower-case to _CTRL+SHIFT+L_  (and _U_, respectively).

### Select a block of text
Place the cursor at start, hold down ```ALT+SHIFT``` and select to the end of your block (opposite corner).


## Windows

### _certutil_ to checksum a downloaded file
Often need to download some package and an associated checksum file, then verify the checksum.
Can append a freshly generated checksum to the checksum file for comparing with an editor like this:
```
certutil -hashfile C:\Downloads\kibana-6.zip SHA512 >> C:\Downloads\kibana-6.zip.sha512
```
Supports algorithms: MD2 MD4 MD5 SHA1 SHA256 SHA384 SHA512

### _win10_ no **Insert** key on HP Elitebook
HP makes some crappy notebooks that lack an _Insert_ key, so it's frustrating to paste things into GitBash windows, etc.
You can use:
```
SHIFT + Fn + e
```
_Note_: **first** press SHIFT then the Fn combo (or use CAPSLOCK)  
Credit: a reply to [this post](https://h30434.www3.hp.com/t5/Business-Notebooks/There-is-no-INSERT-key-on-the-new-2018-Hp-840-elitebook-G5/td-p/6695864).
Here are a few other hidden keys:
-	SHIFT+fn+R = Break
-	SHIFT+fn+S = Sys Rq
-	SHIFT+fn+C = Scroll Lock
-	SHIFT+fn+E = Insert
-	SHIFT+fn+W = Pause

So e.g. CRTL ALT INSERT would be `CTRL + ALT + FN+E`
### _New folder_ button in _Win10_ File Explorer
Hunt the **tiny** and easily-overlooked _create new folder_ button, hiding at top left corner of the File Explorer title bar.

### _Open cmd here_ trick _win10_
It's actually possible to install the context menu via the registry.  
I was locked out of the registry but I have Git for windows, which provides a _Git Bash Here_ context menu.  
You can fire up a cmd prompt directly from the Git Bash terminal:
```
start cmd .
```
### Bypass hibernate restriction _win10_
Lacking a hibernate option in the power menu, you can try this in an admin prompt:
-	Use `powercfg /hibernate /size 0` to let win10 manage hibernate file size
- Then `shutdown /h` hibernates the machine

I tested that it does hibernate and all my windows are still there on restart.. :-)

### Install node.js on windows
Discovered [nvm-windows](https://github.com/coreybutler/nvm-windows) really fixes installing npm on windows! Thus:
1. Uninstall any old npm / node stuff that was installed (yes)
1. Reboot windows (a _couple times_ for good measure)
1. Download the windows install exe for 1.1.7 and run it (_escalates to admin_)  
   (I put it at _c:\app\nvm_ instead of under admin's _appdata_ folder)
1. Run `nvm install 12.16.1` in an _admin_ powershell
1. Finally activate it: with `nvm use 12.16.1`

## Word

### Quickly insert date or time
Use ALT+i, t combo accessed the date/time entry in the Text group.
Can also assign a shortcut to this, as below

### Keyboard shortcut for symbol
1. From Insert tab, choose Symbol menu on far right hand side (quicker: `ALT+i,s`)
1. Select the symbol to map, and press the button __Shortcut *K*ey__
1. The new dialog lets you remove existing shortcut keys and add new ones

### Keyboard shortcut for command
1. Open the options dialog via `ALT+f,t`)
1. Select _Customize ribbon_ in the left hand list, and press the button __Cus*t*omize__
1. In the _Categories_ list (on left) choose _All Commands_
1. In the _Commands_ list (on right) scroll to find your command e.g. _InsertDateTime_
1. Type a shortcut into the field _Press new shortcut key_ and click the _Assign_ button

