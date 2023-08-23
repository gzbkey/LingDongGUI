from pathlib import Path
import re

print('Auto Create Widget Files')

# 准备名字
widgetName = input('please input new widget name,for example [checkBox]：\n')
lowerWidgetName='. '.join(x[0].lower() + x[1:] for x in widgetName.split('. '))
upperWidgetName='. '.join(x[0].capitalize() + x[1:] for x in widgetName.split('. '))

# c文件路径
dirPath = Path(__file__).parent.absolute()
cSrcPath=dirPath/'_ldTemplate.c'
targetName='ld'+upperWidgetName+'.c'
cTargetPath=dirPath/targetName

# c文件读取
f=open(cSrcPath,'r',encoding='utf8')
alllines=f.readlines()
f.close()

# c文件替换字符串
f=open(cTargetPath,'w+',encoding='utf8')
for eachline in alllines:
    # 替换字符串
    lineStr=eachline
    lineStr=re.sub('template',lowerWidgetName,lineStr)
    lineStr=re.sub('Template',upperWidgetName,lineStr)
    f.write(lineStr)
f.close()


# c文件路径
hSrcPath=dirPath/'_ldTemplate.h'
targetName='ld'+upperWidgetName+'.h'
hTargetPath=dirPath/targetName

# c文件读取
f=open(hSrcPath,'r',encoding='utf8')
alllines=f.readlines()
f.close()

# c文件替换字符串
f=open(hTargetPath,'w+',encoding='utf8')
for eachline in alllines:
    # 替换字符串
    lineStr=eachline
    lineStr=re.sub('TEMPLATE',widgetName.upper(),lineStr)
    lineStr=re.sub('Template',upperWidgetName,lineStr)
    f.write(lineStr)
f.close()