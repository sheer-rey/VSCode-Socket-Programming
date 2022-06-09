# 在Git中需要配置的选项  
## 文件 .gitignore 中的内容  
```
# ignore all binary executive files in directory './bin'
/bin/*

# keep empty folder './bin' tracked by using a empty file '.gitkeep'
!/bin/.gitkeep
```  
## 文件 .gitattributes 中的内容  
```
* text=auto eol=lf
*.{cmd,[cC][mM][dD]} text eol=crlf
*.{bat,[bB][aA][tT]} text eol=crlf
```  
