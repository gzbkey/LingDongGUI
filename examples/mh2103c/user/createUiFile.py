import os
import sys

pageCFileStr='#include "uiUser.h"\n\n\
void uiUserInit(uint8_t page)\n\
{\n\
}\n\n\
void uiUserLoop(uint8_t page)\n\
{\n\
}\n\n\
void uiUserQuit(uint8_t page)\n\
{\n\
}'

pageHFileStr='#ifndef _UI_USER_H_\n\
#define _UI_USER_H_\n\n\
#ifdef __cplusplus\n\
extern "C" {\n\
#endif\n\n\
#include "ldGui.h"\n\
#include "srcUser.h"\n\n\
void uiUserInit(uint8_t page);\n\
void uiUserLoop(uint8_t page);\n\
void uiUserQuit(uint8_t page);\n\n\
#ifdef __cplusplus\n\
}\n\
#endif\n\n\
#endif //_UI_USER_H_\n'

def changeStr(targetName, targetStr):
    full_upper_str = targetName.upper()
    full_lower_str = targetName.lower()
    first_upper_str = targetName[:1].upper() + targetName[1:].lower()
    outStr = targetStr.replace('user', full_lower_str).replace('USER', full_upper_str).replace('User',first_upper_str)
    return outStr

def addNewPage(pageNameList):
    new_name = input("Input new page name:")
    if not new_name == '':
        if any(new_name == x.split('.')[0] for x in pageNameList):
            print("Page name already exists! Please choose another.")
            new_name = ''
    return new_name

# version
print('\n==========   V1.1    ==========\n')

# 预备操作
file_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'pageList.txt')

if not os.path.isfile(file_path):
    with open(file_path, 'w') as f:
        f.close
        print('list file not exist,create list file.')

# 名称展示
with open(file_path, 'r') as f:
    dataLines = f.readlines()
dataLines = [line.strip() for line in dataLines if line.strip() != '']

pageNameList = [data.strip() for data in dataLines]

if pageNameList.count == 0:
    print('==========  No Page  ==========')
else:
    print('========== Page List ==========')
    for name in pageNameList:
        print(name)
print('')

# 添加名称
print('========== CMD List  ==========')
print('1   : Add page')

cmdNum = input('Input CMD number:')

if cmdNum == '1':
    newPageName = addNewPage(pageNameList)
    if not newPageName == '':
        pageNameList.append(newPageName)
    with open(file_path, 'w') as f:
        for pageName in pageNameList:
            f.write(pageName+'\n')
        f.flush
        f.close

# 生成文件
with open(file_path, 'r') as f:
    dataLines = f.readlines()
dataLines = [line.strip() for line in dataLines if line.strip() != '']

pageFileNameList = ['ui'+data.capitalize().strip() for data in dataLines]

for fileName in pageFileNameList:
    filePath=os.path.join(os.path.dirname(os.path.abspath(__file__)), fileName+'.c')
    if not os.path.exists(filePath):
        with open(filePath, 'w') as f:
            f.write(changeStr(fileName.replace('ui',''),pageCFileStr))
            f.flush
            f.close
            print('build file:',fileName+'.c')
            
    filePath=os.path.join(os.path.dirname(os.path.abspath(__file__)), fileName+'.h')
    if not os.path.exists(filePath):
        with open(filePath, 'w') as f:
            f.write(changeStr(fileName.replace('ui',''),pageHFileStr))
            f.flush
            f.close
            print('build file:',fileName+'.h')
