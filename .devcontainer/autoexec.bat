if [ "$TERM_PROGRAM" != "vscode" ]; 
    then cd ~/sicp;
fi
source ~/posh-git-sh/git-prompt.sh
PROMPT_COMMAND='__posh_git_ps1 "\u@\h:\w " "\\\$ ";'$PROMPT_COMMAND
