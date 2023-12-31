import re
import os
from pathlib import Path

# def readAllComment(filePath):
#     comments = []
#     pattern = r"/\*\*(.*?)\*/"
#     exclude_pattern = r"@file"
#     with open(filePath,'r',encoding='utf8') as f:
#         allData=f.read()
#         matches = re.findall(pattern,allData,re.DOTALL)
#         for match in matches:
#             if not re.search(exclude_pattern, match):
#                 comments.append(match)
#     return  comments

def getFunctionCommentName(filePath):
    comments = []
    names = []
    comment = ''
    nameStr = ''
    pattern = r"/\*\*(.*?)\*/"
    exclude_pattern = r"@file"
    isFindCommentStart = False
    isFindCommentEnd = False
    isNameStart = False

    with open(filePath, 'r', encoding='utf8') as f:
        alllines = f.readlines()
        for line in alllines:
            if isNameStart:
                nameStr=line
                names.append(nameStr)
                isNameStart=False

            if isFindCommentStart:
                if '*/' in line:
                    isFindCommentEnd=True
            else:
                if '/**' in line:
                    isFindCommentStart=True

            if isFindCommentStart and not isFindCommentEnd:
                comment += line

            if isFindCommentStart and isFindCommentEnd:
                comment += line #read last line
                pattern = r"@file|www\.apache\.org"
                matches = re.findall(pattern, comment)

                if len(matches) <= 0:
                    comments.append(comment)
                    isNameStart = True

                isFindCommentStart = False
                isFindCommentEnd = False
                comment=''
    return comments,names

def getParam(comment,targetName):
    paramStrList = []
    paramStr = ''
    lines = comment.splitlines()
    itemLenMax = 20
    headSpace = '    '
    for line in lines:
        if targetName in line:
            if len(paramStr) != 0:
                paramStrList.append(paramStr)
                paramStr=''
            list=line.split(targetName)
            tempStr=list[1].strip()
            segments = tempStr.split(" ", 1)
            itemName = segments[0].strip()
            spaceStr = ''
            itemStr = ''
            if len(segments) >1:
                itemStr = segments[1].strip()
                tempLen = itemLenMax-len(itemName)
                if tempLen < 0:
                    tempLen = 1
                i = 0
                while i < tempLen:
                    spaceStr+=' '
                    i += 1
            paramStr += headSpace + itemName + ' ' + spaceStr + itemStr
        else:
            if ' *  ' in line:
                if len(paramStr) != 0:
                    list=line.split(' *  ')
                    spaceStr = ''
                    i = 0
                    while i < itemLenMax:
                        spaceStr+=' '
                        i += 1
                    
                    if '@brief' in targetName:
                        paramStr += '\n' + headSpace + list[1].strip()
                    else:
                        paramStr += '\n' + headSpace + ' ' + spaceStr + list[1].strip()
            else:
                if len(paramStr) != 0:
                    paramStrList.append(paramStr)
                    paramStr=''

    return paramStrList

def getFunctionTinyName(funcName):
    match = re.search(r'([\s*].*?)(?=\()', funcName)
    if match:
        extracted_string = match.group(1).strip()
    else:
        extracted_string = funcName.strip()

    extracted_string=extracted_string.replace(' ','')
    extracted_string=extracted_string.replace('*','')
    return extracted_string

def getFunctionDescriptionStr(funcNames,comments):
    objLen=len(funcNames)
    index = 0
    outStr = ''
    while index<objLen:
        outStr+='#### '+getFunctionTinyName(funcNames[index])+'\n'
        
        outStr+='<table>\n'
        outStr+='    <tr>\n'
        outStr+='        <td>函数</td>\n'
        outStr+='        <td colspan="2">\n'
        outStr+='            <pre><code class="language-c">'+funcNames[index].replace('\n','')+';</code></pre>\n'
        outStr+='        </td>\n'
        outStr+='    </tr>\n'
        outStr+='    <tr>\n'
        outStr+='        <td>说明</td>\n'
        outStr+='        <td colspan="2">\n'
        list = getParam(comments[index],'@brief')
        if len(list) > 0:
            for item in list:
                item=item.strip()
                item=item.replace('\n','<br>')
                outStr+='    '+item
        outStr+='        </td>\n'
        outStr+='    </tr>\n'
        list = getParam(comments[index],'@param')
        if len(list) > 0:
            onceFlag=1
            for item in list:
                item=item.strip()
                item=item.replace('\n','<br>')
                name=item.split(' ')
                data=item.lstrip(name[0])
                data=data.strip()
                outStr+='    <tr>\n'
                if onceFlag == 1:
                    outStr+='        <td rowspan="'+str(len(list))+'">参数</td>\n'
                    onceFlag=0
                outStr+='        <td>'+name[0]+'</td>\n'
                outStr+='        <td>'+data+'</td>\n'
                outStr+='    </tr>\n'
        list = getParam(comments[index],'@return')
        if len(list) > 0:
            outStr+='    <tr>\n'
            outStr+='        <td>返回</td>\n'
            for item in list:
                item=item.strip()
                item=item.replace('\n','<br>')
                name=item.split(' ')
                data=item.lstrip(name[0])
                data=data.strip()
                outStr+='        <td>'+name[0]+'</td>\n'
                outStr+='        <td>'+data+'</td>\n'
            outStr+='    </tr>\n'
        outStr+='</table>\n<br>\n\n'
        index += 1

    return outStr


def getFileComment(filePath):
    comments = []
    pattern = r"/\*\*(.*?)\*/"
    include_pattern = r"@file"
    with open(filePath,'r',encoding='utf8') as f:
        allData=f.read()
        matches = re.findall(pattern,allData,re.DOTALL)
        for match in matches:
            if re.search(include_pattern, match):
                comments.append(match)
    return  comments

# CheckBox -> check box
def widgetName2FileName(input_string):
    converted_string = ""
    for char in input_string:
        if char.isupper():
            converted_string += " " + char.lower()
        else:
            converted_string += char.lower()
    return converted_string.strip()













dirPath = Path(__file__).parent.absolute()

fileList = []
for root, dirs, files in os.walk(dirPath/'../../src/gui'):
    for file in files:
        if file.endswith(".c") and file.startswith("ld"):
            file_path = os.path.join(root, file)
            fileList.append(file_path)

currentPyDir = Path(__file__).resolve().parent
outStr = ''

for filePath in fileList:
    
    widgetName = ''
    fileComment = getFileComment(filePath)
    outList=getParam(fileComment[0],'@file')
    widgetName=outList[0].replace(" ", "")
    widgetName = widgetName[2:-2]
    if 'scene' in widgetName.lower() or 'gui' in widgetName.lower():
        continue

    outStr += '# '+widgetName+'\n'

    outStr += '### 简述\n'
    outList=getParam(fileComment[0],'@brief')
    outStr += re.sub(r'\s+', ' ', outList[0]).strip()+'\n'

    comments , names =getFunctionCommentName(filePath)
    outStr+= '### 函数列表\n'
    for name in names:
        outStr+='* '+name.replace('\n','')+';\n'

    outStr+= '### 信号列表\n'
    outList=getParam(fileComment[0],'@signal')
    if len(outList) > 0:
        for item in outList:
            item=item.replace(' ','')
            item=item.replace('\n','\n* ')
            outStr+='* '+item.strip()+'\n'

    outStr+= '### 函数说明\n'
    outStr+=getFunctionDescriptionStr(names,comments)

    outStr+= '---\n'

apiNewFilePath = currentPyDir.joinpath('04 api.md')
with open(apiNewFilePath, 'w', encoding='utf8') as file:
    file.write(outStr)
    file.flush

print('完成')




