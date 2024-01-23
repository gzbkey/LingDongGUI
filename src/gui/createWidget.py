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
with open(cSrcPath,'r',encoding='utf8') as f:
    alllines=f.readlines()

# c文件替换字符串
with open(cTargetPath,'w+',encoding='utf8') as f:
    for eachline in alllines:
        lineStr=eachline
        lineStr=re.sub('template',lowerWidgetName,lineStr)
        lineStr=re.sub('Template',upperWidgetName,lineStr)
        f.write(lineStr)
        f.flush()


# c文件路径
hSrcPath=dirPath/'_ldTemplate.h'
targetName='ld'+upperWidgetName+'.h'
hTargetPath=dirPath/targetName

# c文件读取
with open(hSrcPath,'r',encoding='utf8') as f:
    alllines=f.readlines()
    f.close()

# c文件替换字符串
with open(hTargetPath,'w+',encoding='utf8') as f:
    for eachline in alllines:
        lineStr=eachline
        lineStr=re.sub('TEMPLATE',widgetName.upper(),lineStr)
        lineStr=re.sub('Template',upperWidgetName,lineStr)
        f.write(lineStr)
        f.flush()

# ldCommon.h处理
hSrcPath=dirPath/'ldCommon.h'

with open(hSrcPath,'r',encoding='utf8') as f:
    alllines=f.readlines()

newFileStr=''
for eachline in alllines:
    pos=eachline.find('}ldWidgetType_t;')
    newLineStr=eachline
    if pos>=0 :
        newLineStr='    widgetType'+upperWidgetName+',\n'+'}ldWidgetType_t;\n'
    newFileStr+=newLineStr

with open(hSrcPath,'w',encoding='utf8') as f:
    f.write(newFileStr)
    f.flush()

# ldGui.c处理
cSrcPath=dirPath/'ldGui.c'

with open(cSrcPath,'r',encoding='utf8') as f:
    alllines=f.readlines()

newFileStr=''
for eachline in alllines:
    newLineStr=eachline
    
    flagStr='/*============================ auto add include ==============================*/'
    pos=eachline.find(flagStr)
    if pos>=0 :
        newLineStr='#include "ld'+upperWidgetName+'.h"\n'+flagStr+'\n'
    
    # flagStr='/*============================ auto add del ==================================*/'
    # pos=eachline.find(flagStr)
    # if pos>=0 :
    #     newLineStr='    case widgetType'+upperWidgetName+':\n    {\n        ld'+upperWidgetName+'Del((ld'+upperWidgetName+'_t*)pWidget);\n        break;\n    }\n'+flagStr+'\n'
    
    # flagStr='/*============================ auto add loop =================================*/'
    # pos=eachline.find(flagStr)
    # if pos>=0 :
    #     newLineStr='    case widgetType'+upperWidgetName+':\n    {\n        ld'+upperWidgetName+'Loop((ld'+upperWidgetName+'_t*)pWidget,ptParent,bIsNewFrame);\n        break;\n    }\n'+flagStr+'\n'
    
    # flagStr='/*============================ auto add start =================================*/'
    # pos=eachline.find(flagStr)
    # if pos>=0 :
    #     newLineStr='    case widgetType'+upperWidgetName+':\n    {\n        ld'+upperWidgetName+'FrameUpdate((ld'+upperWidgetName+'_t*)pWidget);\n        break;\n    }\n'+flagStr+'\n'
    
    newFileStr+=newLineStr

with open(cSrcPath,'w',encoding='utf8') as f:
    f.write(newFileStr)
    f.flush()

