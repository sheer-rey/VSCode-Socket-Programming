# Something for Git Need to Configure  
## 1. Content of '.gitignore'  
```
# ignore all binary executive files in directory './bin'
/bin/*

# keep empty folder './bin' tracked by using a empty file '.gitkeep'
!/bin/.gitkeep
```  
## 2. Content of '.gitattributes'  
```
* text=auto eol=lf
*.{cmd,[cC][mM][dD]} text eol=crlf
*.{bat,[bB][aA][tT]} text eol=crlf
```  
