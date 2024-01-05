# 软件说明

1. ldLatticeBuilder.exe是字体和图片的数据生成软件
2. createUiFile.py是界面c文件的生成脚本，[使用说明在docs/tutorial/05 development.md](../docs/tutorial/05%20development.md)

# ldLatticeBuilder使用方法

1. 解压lib压缩包
2. 将exe文件放到dll文件同一目录即可运行
3. 拖动图片到左上角区域，可自动复制图片，对于png图片，可以点击选项框，只生成mask，即提取8位透明度数据
4. 左下角输入文本
5. 点击build file，生产.c .h .bin文件
6. 复制文件到项目中，无论GUI中素材使用数组模式还是外部数据模式都共用文件
