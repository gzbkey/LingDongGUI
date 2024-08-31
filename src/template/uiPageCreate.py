from pathlib import Path
import re
import os


def replaceTemplageName(templateFilePath,outFilePath,lowerName,upperName,fullUpperName):
    # 文件读取
    with open(templateFilePath,'r',encoding='utf8') as f:
        alllines=f.readlines()

    # c文件替换字符串
    with open(outFilePath,'w+',encoding='utf8') as f:
        for eachline in alllines:
            lineStr=eachline
            lineStr=re.sub('template',lowerName,lineStr)
            lineStr=re.sub('Template',upperName,lineStr)
            lineStr=re.sub('TEMPLATE',fullUpperName,lineStr)
            f.write(lineStr)
            f.flush()



print('Auto Create UI Page Files')

# 准备名字
widgetName = input('please input new page name,for example [home]:\n')
lowerName='. '.join(x[0].lower() + x[1:] for x in widgetName.split('. '))
upperName='. '.join(x[0].capitalize() + x[1:] for x in widgetName.split('. '))

fullUpperName = ""
for char in widgetName:
    if char.isupper():
        fullUpperName += '_' + char
    else:
        fullUpperName += char

fullUpperName=fullUpperName.upper().strip()

# 文件路径
dirPath = Path(__file__).parent.absolute()
uiPagePath=dirPath/'uiPageOutput'
if not os.path.exists(uiPagePath):
    os.makedirs(uiPagePath)

srcPath=dirPath/'uiTemplate.c'
targetName='ui'+upperName+'.c'
targetPath=uiPagePath/targetName
replaceTemplageName(srcPath,targetPath,lowerName,upperName,fullUpperName)

srcPath=dirPath/'uiTemplate.h'
targetName='ui'+upperName+'.h'
targetPath=uiPagePath/targetName
replaceTemplageName(srcPath,targetPath,lowerName,upperName,fullUpperName)

srcPath=dirPath/'uiTemplateLogic.c'
targetName='ui'+upperName+'Logic.c'
targetPath=uiPagePath/targetName
replaceTemplageName(srcPath,targetPath,lowerName,upperName,fullUpperName)

srcPath=dirPath/'uiTemplateLogic.h'
targetName='ui'+upperName+'Logic.h'
targetPath=uiPagePath/targetName
replaceTemplageName(srcPath,targetPath,lowerName,upperName,fullUpperName)
