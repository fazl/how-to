
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
    * [Start gdb to debug an exe with arguments](#start-gdb-to-debug-an-exe-with-arguments)
    * [List all headers included by code](#list-all-headers-included-by-code)
  * [Git](#git)
    * [Install/Setup Git &amp; Maven, on Windows](#installsetup-git--maven-on-windows)
    * [Git grep](#git-grep)
    * [Git aliases](#git-aliases)
    * [Git amend the very last commit](#git-amend-the-very-last-commit)
      * [Fix the commit message](#fix-the-commit-message)
      * [Add missing changes to commit](#add-missing-changes-to-commit)
      * [Fix wrong author](#fix-wrong-author)
    * [Git squash local commits together](#git-squash-local-commits-together)
    * [Git diff word\-by\-word](#git-diff-word-by-word)
    * [Git discard uncommitted changes](#git-discard-uncommitted-changes)
    * [Git config needing admin rights](#git-config-needing-admin-rights)
    * [Git forgets Github credentials](#git-forgets-github-credentials)
    * [Git Quick diff in console](#git-quick-diff-in-console)
    * [Git Branches](#git-branches)
      * [Git list remote branches](#git-list-remote-branches)
      * [Git Show branch history graph](#git-show-branch-history-graph)
      * [Create and checkout feature branch off develop](#create-and-checkout-feature-branch-off-develop)
      * [Git list existing branches](#git-list-existing-branches)
      * [Git delete feature branch locally](#git-delete-feature-branch-locally)
      * [Git rename a remote branch](#git-rename-a-remote-branch)
      * [Git delete a remote branch (needs git v1\.7 or more)](#git-delete-a-remote-branch-needs-git-v17-or-more)
      * [Git show entire history of specific file](#git-show-entire-history-of-specific-file)
      * [Git Copy version of a File Using git show](#git-copy-version-of-a-file-using-git-show)
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
  * [Sed](#sed)
    * [Sed multi line editing](#sed-multi-line-editing)
    * [In\-place editing of many files](#in-place-editing-of-many-files)
  * [Selenium example to drive web browser](#selenium-example-to-drive-web-browser)
  * [Vi editor](#vi-editor)
    * [How to redo after undo](#how-to-redo-after-undo)
    * [How to select, copy and paste rectangular blocks](#how-to-select-copy-and-paste-rectangular-blocks)
  * [VSCode](#vscode)
    * [VSCode Fix broken TAB key](#vscode-fix-broken-tab-key)
    * [Customise keyboard shortcuts](#customise-keyboard-shortcuts)
    * [Select rectangular (columnar) block of text](#select-rectangular-columnar-block-of-text)
    * [Regexp search / replace with capture groups](#regexp-search--replace-with-capture-groups)
  * [Windows](#windows)
    * [<em>certutil</em> to checksum a downloaded file](#certutil-to-checksum-a-downloaded-file)
    * [<em>win10</em> no <strong>Insert</strong> key on HP Elitebook](#win10-no-insert-key-on-hp-elitebook)
    * [<em>New folder</em> button in <em>Win10</em> File Explorer](#new-folder-button-in-win10-file-explorer)
    * [<em>Open cmd here</em> trick <em>win10</em>](#open-cmd-here-trick-win10)
    * [Bypass hibernate restriction <em>win10</em>](#bypass-hibernate-restriction-win10)
    * [Install node\.js on windows](#install-nodejs-on-windows)
  * [Word](#word)
    * [Search in hyperlinks in word doc](#search-in-hyperlinks-in-word-doc)
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

### Start gdb to debug an exe with arguments  
Example adapted from [John Cashew's stackoverflow question](https://stackoverflow.com/q/32676007/7409029):  
> gdb --args ./SwSwTest_TfnCore -n 4a

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

### Git grep 
Examples from [this stackoverflow post](https://stackoverflow.com/questions/2928584/how-to-grep-search-committed-code-in-the-git-history):

```
git rev-list --all | xargs git grep <expression>  #Search whole repository
```
or
```
git grep <regexp> $(git rev-list --all -- lib/util) -- lib/util #for searching only in lib/util
git rev-list --all -- lib/util | xargs git grep <regexp> -- lib/util #if commandline overflows
```

### Git aliases
You can list them via.. `git config --list --show-origin | less` and check section `[aliases]`

Useful list:
- git config alias.s 'status -s' # `git s` shows current branch and status
- git config alias.co 'checkout' # `git co -` switches to the previous branch
- git config alias.cob 'checkout -b' # `git cob feature/coolstuff` creates and checks out new branch
- git config alias.last 'log -1 HEAD' # `git last` shows last commit info

Other useful aliases:
- List branches, sort by commit date, most recent first:
> br = branch --format='%(HEAD) %(color:yellow)%(refname:short)%(color:reset) - %(contents:subject) %(color:green)(%(committerdate:relative)) [%(authorname)]' --sort=-committerdate


### Git amend the very last commit

#### Fix the commit message

Say you goof up the commit message and hit return too soon:
>  git commit -m "Oops bad commit message" file1 file2 ...

You can fix that like this:
```
 git commit --amend -m "Corrected commit message"
```
#### Add missing changes to commit

Say you also forgot _file3_.. 
- You can add such changes to the last commit.  
- You can also fix the commit message too, at the same time.
Do it like this ***before you push the changes***:
```
 git add file3 #forgotten in last commit
 git commit --amend -m "Corrected commit message"
```
#### Fix wrong author
Say you accidentally committed using corporate credentials because you had cloned a private repo on a loaned pc.
You can fix the author easily _before_ pushing the change. Thus:
- ``` $ git commit --amend --author="fazl <5173704+fazl@users.noreply.github.com>"```

To fix the problem in future commits

- Edit the file .git/config and add or amend the `[user]` section to something like below:
```
[user]
     name = Fazl
     email = 5173704+fazl@users.noreply.github.com
```

### Git squash local commits together
Say instead, you simply want to combine small changes that you made in separate commits into a single commit before pushing out.
Say, while in an online meeting so you couldn't focus on choosing how to commit stuff together.. 
- You saw a typo and committed the fix
-  you added a missing ```const``` and committed that.. 

So, this is really neat: 
http://bit.ly/howto-git-squash-commits-together-b4-pushing

Note the comment about ```rebase --committer-date-is-author-date``` from user @deadbeef

### Git diff word-by-word
> git diff --word-diff=color <file>

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

#### Git rename a remote branch 
Example using technique from [sschuberth](https://stackoverflow.com/a/21302474/7409029):
```
dev@wsdev58 MINGW64 /c/work/git/PumpUnit (work/IT1928_TherapyMinimalViableProduct)
$ git push origin origin/327_AdaptSlicerAlgo:refs/heads/work/327_AdaptSlicerAlgo :327_AdaptSlicerAlgo
Total 0 (delta 0), reused 0 (delta 0)
To ssh://azdo.corp.itsroot.biz:22/ypsomed/YpsoPumpSystem/_git/PumpUnit
 - [deleted]         327_AdaptSlicerAlgo
 * [new branch]      origin/327_AdaptSlicerAlgo -> work/327_AdaptSlicerAlgo
```

#### Git delete a remote branch (needs git v1.7 or more)
> git push --delete origin feature/SEL-1433

#### Git show entire history of specific file 
Including history beyond renames and with diffs for each change (credit [Dan Moulding](https://stackoverflow.com/a/5493663/7409029) and commenters):
> git log --follow -p -- path-to-file  
Also:
> --stat is also helpful. You can use it together with -p.

#### Git Copy version of a File Using git show
`TL;DR`: Once I needed to just throw away all the changes made in selected files in a work branch so that when the PR was merged into the target branch these files would not be changed... Lacking a funky tool that lets me compare branches in a comfy way, I used a tip from [w3docs](https://www.w3docs.com/snippets/git/how-to-copy-a-version-of-a-single-file-from-one-git-branch-to-another.html):
 
> git show commit-hash:path/to/file > path/to/file

 `Details`
 
List the files to process:  
```
$ find . -name \*Motor\*.[hc]*  
./MotorControl/MotorControl.h
./MotorControl/_Mock/MotorControlMock.cpp
./TfnCore/_Impl/MotorControlFake/MotorControlFake.h
./TfnCore/_Impl/MotorControlFake/_Impl/MotorControlFake.c
./TfnCore/_Impl/MotorControlFake/_Test/UnitTest_MotorControlFake.cpp
```

- Determine the files modified in current branch relative to that branch thus:  
> git diff   work/3652_AddIntrospectApis   work/73_TherapyFunctionCore   ./MotorControl/MotorControl.h

- Copy the hash of the commit (branch) where the needed versions are, using e.g. the Git Graph tool in VSCode.  
- Use the hash with `git show` to replace the contents of the local branch version of selected files thus:   
> git show 006f8107c5fd067fec99d941430c1f8da65a3fc3:./MotorControl/MotorControl.h   >   ./MotorControl/MotorControl.h
> ..ditto using ./TfnCore/_Impl/MotorControlFake/_Impl/MotorControlFake.c, etc
 
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

## Sed
 
### Sed multi line editing

 [Credit](https://stackoverflow.com/a/11234322/7409029)
 
 I wanted a script to activate some IGNORED tests like these:
```
 // FAILS : Report does not inform about all deliveries
IGNORE_TEST(TestScenarios, Scenario4a_Tbf)
```
 to give 
```
 // FAILS : Report does not inform about all deliveries
TEST(TestScenarios, Scenario4a_Tbf)
```

 The trick is to:
 - hunt for the comment marker with `s/^\/\/ FAILS : Report/`
 - join the next line (with newline) using the `N` command
 - replace any IGNORE_TEST with just TEST
 
 Thus:
 
 ```
 sed '/^\/\/ FAILS : Report/ {N; s/IGNORE_TEST/TEST/}' scratch.cpp 
 ```
 
 You can inplace-edit with the `-i` switch .
 
 
### In-place editing of many files
- Replaces a specific text in a bunch of files (listed in a text file). The I switch means Case insensitive search.
```
cat list.txt |  
xargs sed -i 's-Review Status:  <span style="color:green">MIGRATED</span>-Review Status:  <span style="font-weight: bold; color:green">MIGRATED</span>-I
```

- Appends 3 lines of text (after any line containing a specific text) in a bunch of files (here listed by find). Note the two newlines:
```
find . -name TC*md | 
xargs sed -i '/^Review Status: /a <!-- span style="font-weight: bold; color:red">DRAFT</span -->\n<!-- span style="font-weight: bold; color:blue">IN PROGRESS</span -->\n<!-- span style="font-weight: bold; color:green">MIGRATED</span -->
```


- Capture groups to shorten color html by replacing `font-weight: bold` style element with markdown around content text 
```
find . -name TC\*.md |
xargs grep -l font-weight: |
xargs sed -i -E 's/span style="font-weight: bold; color:([^"]*)">([^<]*)</span style="color:\1">**\2**</'
```

_Note:_ `sed -iE` fails with obscure error about the second replacement capture group reference:
```
$ sed -ir 's_\<span style="font-weight: bold; color:([^"]*)">([^<]*)<_<span style="color:\1">**\2**<_' HW/TC103975.md
sed: -e expression #1, char 90: invalid reference \2 on `s' command's RHS
```

This was fixed by separate switches `-i` for inplace and `-E` for modern regexps...

## Selenium example to drive web browser

Prereqs:
-	Install python:  Type `python` in cmd window (win10)
-	Install selenium: Then type `pip install selenium` 
-	Download geckodriver.exe into your home folder (say)
- Run `python C:\Users\<user>\startmytest.py` (i.e. this script):

```
from selenium import webdriver
import time
from selenium.webdriver.support.ui import Select
# FFox: F12 -> console, Pick an element via picker, Rightmouse:Copy->Xpath
xpaths={
    'user' : '//*[@id="j_username"]',
    'pass' : '//*[@id="j_password"]',
    'submit' : '/html/body/div[2]/div[2]/div/div/form/table/tbody/tr[3]/td[2]/button',
    'tab_test' : '//*[@id="Register99Test"]',
    'edit_icon' : '//*[@id="editAction"]', 
    'server_fixture' : '/html/body/div[2]/div[2]/div/div/form[2]/table[2]/tbody/tr[3]/td[3]/select',
    'submit_fixture' : '//*[@id="Finish"]',
    'link_fixture' : '/html/body/div[2]/div[2]/div/table[2]/tbody/tr[11]/td[2]/a',    
}
driver = webdriver.Firefox(executable_path=r'C:\Users\eFR01\geckodriver.exe')

# Login
driver.get("http://red:8280/nil/main.jsp")
driver.find_element_by_xpath(xpaths['user']).send_keys('admlp')
driver.find_element_by_xpath(xpaths['pass']).send_keys('sybasesucks') #keep script in safe place
driver.find_element_by_xpath(xpaths['submit']).click()
time.sleep(0.5) #page load slow

# Load server fixture
driver.find_element_by_xpath(xpaths['tab_test']).click()
driver.find_element_by_xpath(xpaths['edit_icon']).click()
Select(driver.find_element_by_xpath(xpaths['server_fixture'])).select_by_visible_text('NeMitgliedServerFixture')
# OR find_element_by_xpath("/html/body.. ../select/option[text()='NeMitgliedServerFixture']").click() # https://stackoverflow.com/a/7972225/7409029
driver.find_element_by_xpath(xpaths['submit_fixture']).click()
time.sleep(2.8) #page load very slow
driver.find_element_by_xpath(xpaths['link_fixture']).click()

# Bingo! If the fixture was created you should now be looking at it..
```


## Vi editor

### How to redo after undo
It's the ```Ctrl+r``` you're looking for..

### How to select, copy and paste rectangular blocks
The ```Ctrl+v``` command starts visual rectangular select mode, then...
- Usual navigational commands to select block
- d to cut
- y to yank (copy)
- p to paste (after optionally selecting destination block)  
Cool stuff! Textpad is getting stiff competition here..

## VSCode
 
### VSCode Fix broken TAB key
Sometimes you accidentally pressed `Ctrl+M` and changed the mode of TAB to be a focus-changing key.  
Just press `Ctrl+M` again to toggle this.  
If that is not the fix, try going [here](https://duckduckgo.com/?t=ffab&q=stackover+vscode+tab+key) for more ideas..



### Customise keyboard shortcuts
Via menu ```File -> Preferences -> Keyboard Shortcuts```, I searched for _lower_ and _upper_ to find and map the functions that convert to upper-/lower-case to _CTRL+SHIFT+L_  (and _U_, respectively).

### Select rectangular (columnar) block of text
Place the cursor at start, hold down ```ALT+SHIFT``` and select to the end of your block (opposite corner).

### Regexp search / replace with capture groups
VSCode's brain damaged regexp replace feature uses _$1 etc instead of \1 etc_ to reference the captured items.  
_What did you expect from Microflop?!_


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

### Search in hyperlinks in word doc
By default Word doesn't look inside links. You can't even choose an advanced search option to do this.
[Workaround](https://office-watch.com/2020/find-and-replace-links-in-word-documents):
- first _reveal the links_ with `ALT`+`F9` 
- then do a normal search (`CTRL`+`F`)


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

