@echo off
setlocal enabledelayedexpansion

pushd %~dp0
set "current_dir=%cd%"
set "src_dir=..\\src\\template"
set "target_dir=!current_dir!\\user"
set "pageTool_dir=!target_dir!\\pageTool"
set "readme_file=!target_dir!\\readme.txt"

if not exist "!target_dir!" (
    md "!target_dir!"
)

pushd "!target_dir!"
md "images"
md "fonts"
md "pageTool"
pushd "images"
md "gif"
popd

pushd "!current_dir!"
copy /y "gif2png.py" "!target_dir!\\images\\gif\\gif2png.py"
copy /y "auto_img2c.py" "!target_dir!\\images\\auto_img2c.py"
copy /y "img2c.py" "!target_dir!\\images\\img2c.py"
copy /y "auto_ttf2c.py" "!target_dir!\\fonts\\auto_ttf2c.py"
copy /y "auto_ttf2c.bat" "!target_dir!\\fonts\\auto_ttf2c.bat"
copy /y "ttf2c.py" "!target_dir!\\fonts\\ttf2c.py"

pushd "!src_dir!"
copy /y "ldConfig.c" "!target_dir!\\ldConfig.c"
copy /y "ldConfig.h" "!target_dir!\\ldConfig.h"
copy /y "Retarget.c" "!target_dir!\\Retarget.c"
copy /y "uiPageCreate.py" "!pageTool_dir!\\uiPageCreate.py"
copy /y "uiTemplate.c" "!pageTool_dir!\\uiTemplate.c"
copy /y "uiTemplate.h" "!pageTool_dir!\\uiTemplate.h"
copy /y "uiTemplateLogic.c" "!pageTool_dir!\\uiTemplateLogic.c"
copy /y "uiTemplateLogic.h" "!pageTool_dir!\\uiTemplateLogic.h"

popd
popd
popd

(
echo.
echo ===================================
echo      ָ   - Operation Guide
echo ===================================
echo.
echo 1.     pageTool ļ   
echo 1. Navigate to the pageTool folder
echo.
echo 2.     uiPageCreate.py ű   python uiPageCreate.py
echo 2. Run the uiPageCreate.py script: python uiPageCreate.py
echo.
echo 3.    ɵ ҳ   ļ    Ƶ userĿ¼  ʹ  
echo 3. The generated page files are copied to the user directory for use
echo.
) > "!readme_file!"
