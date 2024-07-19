from pathlib import Path
import re
import os

print('Auto Create Widget Files')

# 准备名字
widgetName = input('please input new widget name,for example [checkBox]:\n')
lowerWidgetName='. '.join(x[0].lower() + x[1:] for x in widgetName.split('. '))
upperWidgetName='. '.join(x[0].capitalize() + x[1:] for x in widgetName.split('. '))

fullUpperWidgetName = ""
for char in widgetName:
    if char.isupper():
        fullUpperWidgetName += '_' + char
    else:
        fullUpperWidgetName += char

fullUpperWidgetName=fullUpperWidgetName.upper().strip()

# c文件路径
dirPath = Path(__file__).parent.absolute()
cSrcPath=dirPath/'widgetTemplate.c'
targetName='ld'+upperWidgetName+'.c'
cTargetPath=f"{dirPath}/../gui/{targetName}"

# c文件读取
with open(cSrcPath,'r',encoding='utf8') as f:
    alllines=f.readlines()

if os.path.exists(cTargetPath):
    print(cTargetPath,"exist")
else:
    # c文件替换字符串
    with open(cTargetPath,'w+',encoding='utf8') as f:
        for eachline in alllines:
            lineStr=eachline
            lineStr=re.sub('template',lowerWidgetName,lineStr)
            lineStr=re.sub('Template',upperWidgetName,lineStr)
            lineStr=re.sub('TEMPLATE',fullUpperWidgetName,lineStr)
            f.write(lineStr)
            f.flush()


# h文件路径
hSrcPath=dirPath/'widgetTemplate.h'
targetName='ld'+upperWidgetName+'.h'
hTargetPath=f"{dirPath}/../gui/{targetName}"

# h文件读取
with open(hSrcPath,'r',encoding='utf8') as f:
    alllines=f.readlines()
    f.close()

if os.path.exists(hTargetPath):
    print(hTargetPath,"exist")
else:
    # h文件替换字符串
    with open(hTargetPath,'w+',encoding='utf8') as f:
        for eachline in alllines:
            lineStr=eachline
            lineStr=re.sub('Template',upperWidgetName,lineStr)
            lineStr=re.sub('TEMPLATE',fullUpperWidgetName,lineStr)
            f.write(lineStr)
            f.flush()

# ldGui.h处理
hSrcPath=dirPath/'../gui/ldGui.h'

with open(hSrcPath,'r',encoding='utf8') as f:
    alllines=f.readlines()

newFileStr=''
for eachline in alllines:
    pos=eachline.find('#include "ldBase.h"')
    newLineStr=eachline
    if pos>=0 :
        newLineStr+='#include "'+targetName+'"\n'
    newFileStr+=newLineStr

with open(hSrcPath,'w',encoding='utf8') as f:
    f.write(newFileStr)
    f.flush()

